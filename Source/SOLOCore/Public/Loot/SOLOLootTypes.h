#pragma once

#include "CoreMinimal.h"
#include "SOLOLootTypes.generated.h"

class USOLOItemBase;

/**
 * Una entrada en la tabla de loot.
 * Define quÃ© item puede caer, con quÃ© probabilidad y en quÃ© cantidad.
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FLootEntry
{
    GENERATED_BODY()

    /** Item a dropear. Referencia soft para evitar cargar todos los items en memoria. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Entry")
    TSoftObjectPtr<USOLOItemBase> Item;

    /**
     * Probabilidad de que el item caiga (0.0 = nunca, 1.0 = siempre).
     * Ignorado si bIsGuaranteed == true.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Entry",
        meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DropChance = 0.1f;

    /** Cantidad mÃ­nima a dropear cuando el item cae. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Entry",
        meta = (ClampMin = "1"))
    int32 MinQuantity = 1;

    /** Cantidad mÃ¡xima a dropear cuando el item cae. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Entry",
        meta = (ClampMin = "1"))
    int32 MaxQuantity = 1;

    /**
     * Si true, el item siempre cae al morir el enemigo independientemente de DropChance.
     * Ãštil para drops de quest, loot de boss garantizado, etc.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Entry")
    bool bIsGuaranteed = false;
};

/**
 * Resultado de una tirada de loot: un item resuelto y la cantidad final.
 * Devuelto por USOLOLootTableDataAsset::RollLoot().
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FLootResult
{
    GENERATED_BODY()

    /** Puntero al DataAsset del item que cayÃ³. Nunca nulo si el struct es vÃ¡lido. */
    UPROPERTY(BlueprintReadOnly, Category = "Loot Result")
    TObjectPtr<USOLOItemBase> Item;

    /** Cantidad de unidades del item a otorgar al jugador. */
    UPROPERTY(BlueprintReadOnly, Category = "Loot Result")
    int32 Quantity = 0;
};
