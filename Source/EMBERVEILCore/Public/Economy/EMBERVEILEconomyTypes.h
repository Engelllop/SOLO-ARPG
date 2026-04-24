#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "EMBERVEILEconomyTypes.generated.h"

// ─── ENUMERACIONES ────────────────────────────────────────────────────────────

/** Tipo de transacción económica */
UENUM(BlueprintType)
enum class ETransactionType : uint8
{
    Buy     UMETA(DisplayName = "Compra"),
    Sell    UMETA(DisplayName = "Venta"),
    Trade   UMETA(DisplayName = "Intercambio")
};

/** Resultado de una transacción económica */
UENUM(BlueprintType)
enum class ETransactionResult : uint8
{
    Success                     UMETA(DisplayName = "Éxito"),
    Failed_InsufficientFunds    UMETA(DisplayName = "Fondos insuficientes"),
    Failed_ItemNotAvailable     UMETA(DisplayName = "Item no disponible"),
    Failed_InventoryFull        UMETA(DisplayName = "Inventario lleno"),
    Failed_ItemNotOwned         UMETA(DisplayName = "Item no poseído"),
    Failed_InvalidQuantity      UMETA(DisplayName = "Cantidad inválida")
};

// ─── ESTRUCTURAS ──────────────────────────────────────────────────────────────

/**
 * Representa un item listado en la tienda con sus precios y condiciones.
 * Configurado en el editor a través de MarketplaceComponent.
 */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FShopListing
{
    GENERATED_BODY()

    /** DataAsset del item que se vende en este listing */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Listing")
    TSoftObjectPtr<UEMBERVEILItemBase> Item;

    /**
     * Precio que el jugador paga para comprar el item.
     * Si es 0, se calcula como BaseValue * BuyPriceMultiplier del componente.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Listing",
        meta = (ClampMin = "0"))
    int32 BuyPrice = 0;

    /**
     * Precio que el jugador recibe al vender el item.
     * Si es 0, se calcula como BaseValue * SellPriceMultiplier del componente.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Listing",
        meta = (ClampMin = "0"))
    int32 SellPrice = 0;

    /**
     * Stock máximo disponible. -1 = stock infinito.
     * Se copia a CurrentStock al inicializar la tienda.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Listing",
        meta = (ClampMin = "-1"))
    int32 Stock = -1;

    /** Nivel mínimo del jugador requerido para ver y comprar este item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Listing",
        meta = (ClampMin = "1"))
    int32 RequiredLevel = 1;

    /**
     * Stock actual en runtime. Inicializado desde Stock al hacer BeginPlay.
     * No se muestra en el editor (se gestiona en runtime).
     */
    UPROPERTY(BlueprintReadOnly, Category = "Shop|Listing")
    int32 CurrentStock = -1;
};

/**
 * Registro inmutable de una transacción realizada.
 * Almacenado en el historial del MarketplaceComponent (max 50, FIFO).
 */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FTransactionRecord
{
    GENERATED_BODY()

    /** Tipo de operación realizada */
    UPROPERTY(BlueprintReadOnly, Category = "Transaction")
    ETransactionType Type = ETransactionType::Buy;

    /** ID del item involucrado en la transacción */
    UPROPERTY(BlueprintReadOnly, Category = "Transaction")
    FName ItemID;

    /** Cantidad de unidades transaccionadas */
    UPROPERTY(BlueprintReadOnly, Category = "Transaction")
    int32 Quantity = 0;

    /** Coste total de la transacción (en monedas del juego) */
    UPROPERTY(BlueprintReadOnly, Category = "Transaction")
    int32 TotalCost = 0;

    /** Marca de tiempo cuando se ejecutó la transacción */
    UPROPERTY(BlueprintReadOnly, Category = "Transaction")
    FDateTime Timestamp;
};
