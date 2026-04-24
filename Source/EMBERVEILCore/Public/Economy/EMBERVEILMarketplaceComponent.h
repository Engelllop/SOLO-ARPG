#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Economy/EMBERVEILEconomyTypes.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "EMBERVEILMarketplaceComponent.generated.h"

class APlayerController;
class UEMBERVEILInventoryComponent;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/** Disparado en el servidor cuando el jugador compra un item */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPurchaseCompleted,
    const FTransactionRecord&, Record);

/** Disparado en el servidor cuando el jugador vende un item */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaleCompleted,
    const FTransactionRecord&, Record);

/** Disparado en el servidor cuando el stock de un listing cambia */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStockChanged,
    int32, ListingIndex,
    int32, NewStock);

/**
 * Componente de marketplace/tienda para EMBERVEIL.
 *
 * Se coloca en el AEMBERVEILShopNPC (o en cualquier Actor que actúe como tienda).
 * Gestiona los listings de items, los precios, el stock, el historial de transacciones
 * y la lógica de compra/venta con el inventario del jugador.
 *
 * Toda la lógica de modificación de estado debe ejecutarse en el servidor.
 */
UCLASS(ClassGroup = (EMBERVEIL), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILMarketplaceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEMBERVEILMarketplaceComponent();

    virtual void BeginPlay() override;

    // ─── DELEGATES ────────────────────────────────────────────────────────────

    /** Notifica cuando una compra se completa con éxito */
    UPROPERTY(BlueprintAssignable, Category = "Marketplace|Delegates")
    FOnPurchaseCompleted OnPurchaseCompleted;

    /** Notifica cuando una venta se completa con éxito */
    UPROPERTY(BlueprintAssignable, Category = "Marketplace|Delegates")
    FOnSaleCompleted OnSaleCompleted;

    /** Notifica cuando el stock de un listing cambia */
    UPROPERTY(BlueprintAssignable, Category = "Marketplace|Delegates")
    FOnStockChanged OnStockChanged;

    // ─── CONFIGURACIÓN ────────────────────────────────────────────────────────

    /** Lista de items disponibles en esta tienda. Configurada en el editor. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marketplace|Config")
    TArray<FShopListing> Listings;

    /**
     * Multiplicador aplicado al BaseValue del item para calcular el precio de venta
     * cuando SellPrice == 0 en el listing.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marketplace|Config",
        meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float SellPriceMultiplier = 0.5f;

    /**
     * Multiplicador aplicado al BaseValue del item para calcular el precio de compra
     * cuando BuyPrice == 0 en el listing.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marketplace|Config",
        meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float BuyPriceMultiplier = 1.0f;

    /**
     * Si true, el stock finito se restablece automáticamente cada RestockIntervalSeconds.
     * Solo activo en el servidor.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marketplace|Restock")
    bool bRestockOnInterval = false;

    /**
     * Intervalo en segundos entre cada restock automático.
     * Ignorado si bRestockOnInterval == false.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Marketplace|Restock",
        meta = (EditCondition = "bRestockOnInterval", ClampMin = "1.0"))
    float RestockIntervalSeconds = 300.f;

    // ─── HISTORIAL (solo servidor) ────────────────────────────────────────────

    /**
     * Últimas 50 transacciones registradas (FIFO).
     * No replicado — solo disponible en el servidor.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Marketplace|History")
    TArray<FTransactionRecord> TransactionHistory;

    // ─── OPERACIONES DE COMPRA/VENTA ──────────────────────────────────────────

    /**
     * El jugador (Buyer) compra Quantity unidades del listing en ListingIndex.
     *
     * Proceso:
     *   1. Valida ListingIndex en rango y Quantity > 0
     *   2. Calcula precio total (GetBuyPrice * Quantity)
     *   3. Obtiene InventoryComponent del Buyer via PlayerState
     *   4. Verifica que hay fondos suficientes (SpendCurrency)
     *   5. Verifica stock disponible (si no es infinito)
     *   6. Verifica nivel del jugador (RequiredLevel)
     *   7. Ejecuta SpendCurrency + AddItem + reduce CurrentStock
     *   8. Registra en TransactionHistory (max 50, FIFO)
     *   9. Broadcast OnPurchaseCompleted
     *
     * @param Buyer         PlayerController del comprador
     * @param ListingIndex  Índice en el array Listings
     * @param Quantity      Unidades a comprar (mínimo 1)
     * @return              Resultado de la operación
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Transactions")
    ETransactionResult BuyItem(APlayerController* Buyer, int32 ListingIndex, int32 Quantity = 1);

    /**
     * El jugador (Seller) vende Quantity unidades del item con ItemID.
     *
     * Proceso:
     *   1. Verifica que el jugador posee el item (HasItem)
     *   2. Calcula precio de venta (SellPrice del listing si existe, sino BaseValue * SellPriceMultiplier)
     *   3. RemoveItem del inventario del vendedor
     *   4. AddCurrency al vendedor
     *   5. Si el listing correspondiente tiene stock finito, repone CurrentStock
     *   6. Broadcast OnSaleCompleted
     *
     * @param Seller    PlayerController del vendedor
     * @param ItemID    ID del item a vender
     * @param Quantity  Unidades a vender (mínimo 1)
     * @return          Resultado de la operación
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Transactions")
    ETransactionResult SellItem(APlayerController* Seller, FName ItemID, int32 Quantity = 1);

    // ─── CONSULTAS ────────────────────────────────────────────────────────────

    /**
     * Devuelve el precio de compra final para el listing dado.
     * Usa BuyPrice si > 0, sino BaseValue * BuyPriceMultiplier.
     * Devuelve 0 si el índice es inválido o el item no está cargado.
     *
     * @param ListingIndex  Índice en el array Listings
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Query")
    int32 GetBuyPrice(int32 ListingIndex) const;

    /**
     * Devuelve el precio de venta final para el item con ItemID.
     * Busca el listing correspondiente: usa SellPrice si > 0, sino BaseValue * SellPriceMultiplier.
     * Devuelve 0 si no hay listing para ese item.
     *
     * @param ItemID  ID del item a consultar
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Query")
    int32 GetSellPrice(FName ItemID) const;

    /**
     * Devuelve los listings disponibles para un jugador con el nivel dado.
     * Filtra por RequiredLevel <= PlayerLevel.
     * Los listings con stock == 0 (agotado) se excluyen.
     *
     * @param PlayerLevel  Nivel del jugador
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Query")
    TArray<FShopListing> GetAvailableListings(int32 PlayerLevel) const;

    // ─── RESTOCK ──────────────────────────────────────────────────────────────

    /**
     * Restablece CurrentStock al valor de Stock para todos los listings con stock finito.
     * Llamado automáticamente si bRestockOnInterval == true.
     * Puede llamarse manualmente desde Blueprint o eventos del juego.
     */
    UFUNCTION(BlueprintCallable, Category = "Marketplace|Restock")
    void RestockAll();

private:
    // ─── HELPERS INTERNOS ─────────────────────────────────────────────────────

    /**
     * Obtiene el InventoryComponent del PlayerController dado.
     * Lo busca en el PlayerState del controller.
     * Devuelve nullptr si el controller o su PlayerState no tienen el componente.
     *
     * @param Controller  PlayerController del que extraer el inventario
     */
    UEMBERVEILInventoryComponent* GetInventoryFromController(APlayerController* Controller) const;

    /**
     * Devuelve el nivel del jugador asociado al PlayerController dado.
     * Lo obtiene desde AEMBERVEILPlayerState::GetPlayerLevel().
     * Devuelve 0 si no puede obtenerse.
     *
     * @param Controller  PlayerController del que obtener el nivel
     */
    int32 GetPlayerLevelFromController(APlayerController* Controller) const;

    /**
     * Añade un registro al historial de transacciones.
     * Si el historial supera 50 entradas, elimina la más antigua (FIFO).
     *
     * @param Record  Registro a añadir
     */
    void AddToHistory(FTransactionRecord Record);

    /** Handle del timer de restock automático */
    FTimerHandle RestockTimerHandle;
};
