#include "Economy/EMBERVEILMarketplaceComponent.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "Player/EMBERVEILPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Engine/World.h"

// ─── CONSTANTES ───────────────────────────────────────────────────────────────

static constexpr int32 MaxTransactionHistorySize = 50;

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────

UEMBERVEILMarketplaceComponent::UEMBERVEILMarketplaceComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    // La lógica de compra/venta solo se ejecuta en el servidor
    SetIsReplicatedByDefault(false);
}

// ─── BEGINPLAY ────────────────────────────────────────────────────────────────

void UEMBERVEILMarketplaceComponent::BeginPlay()
{
    Super::BeginPlay();

    // Inicializar CurrentStock desde Stock para todos los listings
    for (FShopListing& Listing : Listings)
    {
        Listing.CurrentStock = Listing.Stock;
    }

    // Activar restock automático solo en el servidor
    if (GetOwner() && GetOwner()->HasAuthority() && bRestockOnInterval)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            World->GetTimerManager().SetTimer(
                RestockTimerHandle,
                this,
                &UEMBERVEILMarketplaceComponent::RestockAll,
                RestockIntervalSeconds,
                true  // bLoop = true
            );
        }
    }
}

// ─── BUYITEM ──────────────────────────────────────────────────────────────────

ETransactionResult UEMBERVEILMarketplaceComponent::BuyItem(
    APlayerController* Buyer,
    int32 ListingIndex,
    int32 Quantity)
{
    // 1. Validar ListingIndex en rango
    if (!Listings.IsValidIndex(ListingIndex))
    {
        return ETransactionResult::Failed_ItemNotAvailable;
    }

    // 2. Validar Quantity > 0
    if (Quantity <= 0)
    {
        return ETransactionResult::Failed_InvalidQuantity;
    }

    FShopListing& Listing = Listings[ListingIndex];

    // Verificar que el item tiene un asset válido cargado
    UEMBERVEILItemBase* ItemAsset = Listing.Item.LoadSynchronous();
    if (!ItemAsset)
    {
        return ETransactionResult::Failed_ItemNotAvailable;
    }

    // 3. Calcular precio total
    const int32 UnitPrice  = GetBuyPrice(ListingIndex);
    const int32 TotalPrice = UnitPrice * Quantity;

    // 4. Obtener InventoryComponent del Buyer via PlayerState
    UEMBERVEILInventoryComponent* InventoryComp = GetInventoryFromController(Buyer);
    if (!InventoryComp)
    {
        return ETransactionResult::Failed_ItemNotAvailable;
    }

    // 5. Verificar fondos (sin gastarlos todavía)
    if (InventoryComp->Currency < TotalPrice)
    {
        return ETransactionResult::Failed_InsufficientFunds;
    }

    // 6. Verificar stock disponible (si no es infinito)
    if (Listing.CurrentStock != -1 && Listing.CurrentStock < Quantity)
    {
        return ETransactionResult::Failed_ItemNotAvailable;
    }

    // 7. Verificar nivel del jugador
    const int32 PlayerLevel = GetPlayerLevelFromController(Buyer);
    if (PlayerLevel < Listing.RequiredLevel)
    {
        return ETransactionResult::Failed_ItemNotAvailable;
    }

    // 8. Ejecutar transacción: gastar moneda + añadir item + reducir stock

    // Gastar moneda
    const bool bSpent = InventoryComp->SpendCurrency(TotalPrice);
    if (!bSpent)
    {
        // Condición de carrera defensiva (el inventario hizo la comprobación real)
        return ETransactionResult::Failed_InsufficientFunds;
    }

    // Añadir el item al inventario
    const bool bAdded = InventoryComp->AddItem(ItemAsset, Quantity);
    if (!bAdded)
    {
        // Revertir el gasto de moneda si el inventario está lleno
        InventoryComp->AddCurrency(TotalPrice);
        return ETransactionResult::Failed_InventoryFull;
    }

    // Reducir stock si es finito
    if (Listing.CurrentStock != -1)
    {
        Listing.CurrentStock -= Quantity;
        OnStockChanged.Broadcast(ListingIndex, Listing.CurrentStock);
    }

    // 9. Registrar en historial (max 50, FIFO)
    FTransactionRecord Record;
    Record.Type      = ETransactionType::Buy;
    Record.ItemID    = ItemAsset->ItemID;
    Record.Quantity  = Quantity;
    Record.TotalCost = TotalPrice;
    Record.Timestamp = FDateTime::UtcNow();

    AddToHistory(Record);

    // 10. Broadcast
    OnPurchaseCompleted.Broadcast(Record);

    return ETransactionResult::Success;
}

// ─── SELLITEM ─────────────────────────────────────────────────────────────────

ETransactionResult UEMBERVEILMarketplaceComponent::SellItem(
    APlayerController* Seller,
    FName ItemID,
    int32 Quantity)
{
    // Validar Quantity > 0
    if (Quantity <= 0)
    {
        return ETransactionResult::Failed_InvalidQuantity;
    }

    // Obtener InventoryComponent del vendedor
    UEMBERVEILInventoryComponent* InventoryComp = GetInventoryFromController(Seller);
    if (!InventoryComp)
    {
        return ETransactionResult::Failed_ItemNotOwned;
    }

    // 1. Verificar que el jugador posee la cantidad requerida del item
    if (!InventoryComp->HasItem(ItemID, Quantity))
    {
        return ETransactionResult::Failed_ItemNotOwned;
    }

    // 2. Calcular precio de venta total
    const int32 UnitSellPrice  = GetSellPrice(ItemID);
    const int32 TotalSellValue = UnitSellPrice * Quantity;

    // 3. Quitar el item del inventario
    const bool bRemoved = InventoryComp->RemoveItem(ItemID, Quantity);
    if (!bRemoved)
    {
        return ETransactionResult::Failed_ItemNotOwned;
    }

    // 4. Añadir moneda al vendedor
    InventoryComp->AddCurrency(TotalSellValue);

    // 5. Si hay un listing correspondiente con stock finito, reponerlo
    for (int32 i = 0; i < Listings.Num(); ++i)
    {
        FShopListing& Listing = Listings[i];
        UEMBERVEILItemBase* ListingItem = Listing.Item.LoadSynchronous();
        if (ListingItem && ListingItem->ItemID == ItemID && Listing.CurrentStock != -1)
        {
            Listing.CurrentStock += Quantity;
            // No sobrepasar el Stock máximo si es finito
            if (Listing.Stock != -1 && Listing.CurrentStock > Listing.Stock)
            {
                Listing.CurrentStock = Listing.Stock;
            }
            OnStockChanged.Broadcast(i, Listing.CurrentStock);
            break;
        }
    }

    // 6. Registrar en historial y hacer broadcast
    FTransactionRecord Record;
    Record.Type      = ETransactionType::Sell;
    Record.ItemID    = ItemID;
    Record.Quantity  = Quantity;
    Record.TotalCost = TotalSellValue;
    Record.Timestamp = FDateTime::UtcNow();

    AddToHistory(Record);

    OnSaleCompleted.Broadcast(Record);

    return ETransactionResult::Success;
}

// ─── GETBUYPRICE ──────────────────────────────────────────────────────────────

int32 UEMBERVEILMarketplaceComponent::GetBuyPrice(int32 ListingIndex) const
{
    if (!Listings.IsValidIndex(ListingIndex))
    {
        return 0;
    }

    const FShopListing& Listing = Listings[ListingIndex];

    // Si tiene precio fijo configurado, usarlo directamente
    if (Listing.BuyPrice > 0)
    {
        return Listing.BuyPrice;
    }

    // Calcular desde BaseValue * multiplicador
    UEMBERVEILItemBase* ItemAsset = Listing.Item.LoadSynchronous();
    if (!ItemAsset)
    {
        return 0;
    }

    return FMath::RoundToInt(static_cast<float>(ItemAsset->BaseValue) * BuyPriceMultiplier);
}

// ─── GETSELLPRICE ─────────────────────────────────────────────────────────────

int32 UEMBERVEILMarketplaceComponent::GetSellPrice(FName ItemID) const
{
    // Buscar el listing que corresponde a este ItemID
    for (const FShopListing& Listing : Listings)
    {
        UEMBERVEILItemBase* ItemAsset = Listing.Item.LoadSynchronous();
        if (!ItemAsset || ItemAsset->ItemID != ItemID)
        {
            continue;
        }

        // Si el listing tiene precio de venta fijo, usarlo
        if (Listing.SellPrice > 0)
        {
            return Listing.SellPrice;
        }

        // Calcular desde BaseValue * multiplicador
        return FMath::RoundToInt(static_cast<float>(ItemAsset->BaseValue) * SellPriceMultiplier);
    }

    return 0;
}

// ─── GETAVAILABLELISTINGS ─────────────────────────────────────────────────────

TArray<FShopListing> UEMBERVEILMarketplaceComponent::GetAvailableListings(int32 PlayerLevel) const
{
    TArray<FShopListing> Available;
    Available.Reserve(Listings.Num());

    for (const FShopListing& Listing : Listings)
    {
        // Filtrar por nivel requerido
        if (PlayerLevel < Listing.RequiredLevel)
        {
            continue;
        }

        // Filtrar listings agotados (stock == 0; -1 es infinito)
        if (Listing.CurrentStock == 0)
        {
            continue;
        }

        Available.Add(Listing);
    }

    return Available;
}

// ─── RESTOCKALL ───────────────────────────────────────────────────────────────

void UEMBERVEILMarketplaceComponent::RestockAll()
{
    for (int32 i = 0; i < Listings.Num(); ++i)
    {
        FShopListing& Listing = Listings[i];

        // Solo restockear listings con stock finito
        if (Listing.Stock == -1)
        {
            continue;
        }

        Listing.CurrentStock = Listing.Stock;
        OnStockChanged.Broadcast(i, Listing.CurrentStock);
    }
}

// ─── HELPERS PRIVADOS ─────────────────────────────────────────────────────────

UEMBERVEILInventoryComponent* UEMBERVEILMarketplaceComponent::GetInventoryFromController(
    APlayerController* Controller) const
{
    if (!Controller)
    {
        return nullptr;
    }

    AEMBERVEILPlayerState* PS = Controller->GetPlayerState<AEMBERVEILPlayerState>();
    if (!PS)
    {
        return nullptr;
    }

    return PS->FindComponentByClass<UEMBERVEILInventoryComponent>();
}

int32 UEMBERVEILMarketplaceComponent::GetPlayerLevelFromController(
    APlayerController* Controller) const
{
    if (!Controller)
    {
        return 0;
    }

    AEMBERVEILPlayerState* PS = Controller->GetPlayerState<AEMBERVEILPlayerState>();
    if (!PS)
    {
        return 0;
    }

    return PS->GetPlayerLevel();
}

void UEMBERVEILMarketplaceComponent::AddToHistory(FTransactionRecord Record)
{
    // Eliminar la entrada más antigua si se supera el límite (FIFO)
    if (TransactionHistory.Num() >= MaxTransactionHistorySize)
    {
        TransactionHistory.RemoveAt(0, 1, EAllowShrinking::No);
    }

    TransactionHistory.Add(MoveTemp(Record));
}
