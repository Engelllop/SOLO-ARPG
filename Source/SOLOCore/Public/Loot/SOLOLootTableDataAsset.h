#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Loot/SOLOLootTypes.h"
#include "SOLOLootTableDataAsset.generated.h"

/**
 * DataAsset que define la tabla de drops de un enemigo o contenedor.
 * Contiene una lista de FLootEntry que se evalÃºan al llamar a RollLoot().
 *
 * Crear subactivos en el Content Browser para cada tipo de enemigo.
 * Asignar en el LootComponent del EnemyCharacter correspondiente.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API USOLOLootTableDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // â”€â”€â”€ ENTRADAS DE LOOT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Lista completa de posibles drops. Los garantizados siempre caen; los demÃ¡s, segÃºn probabilidad. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
    TArray<FLootEntry> LootEntries;

    // â”€â”€â”€ MONEDA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** MÃ­nimo de monedas que puede soltar este enemigo (0 = sin monedas). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table|Currency",
        meta = (ClampMin = "0"))
    int32 MinCurrencyDrop = 0;

    /** MÃ¡ximo de monedas que puede soltar este enemigo. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table|Currency",
        meta = (ClampMin = "0"))
    int32 MaxCurrencyDrop = 0;

    // â”€â”€â”€ PARÃMETROS GLOBALES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Multiplicador global de probabilidades de drop para esta tabla.
     * Combinado con el LuckBonus del jugador. 1.0 = normal, 2.0 = doble suerte.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table",
        meta = (ClampMin = "0.1"))
    float LuckMultiplier = 1.0f;

    /**
     * NÃºmero mÃ¡ximo de items distintos que pueden caer en una sola muerte.
     * Los drops garantizados se procesan primero y consumen cupo de este lÃ­mite.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table",
        meta = (ClampMin = "1"))
    int32 MaxDropsPerKill = 5;

    // â”€â”€â”€ FUNCIONES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * EvalÃºa la tabla de loot y devuelve los items que caen en esta tirada.
     *
     * Orden de procesado:
     *   1. Entries garantizadas (bIsGuaranteed == true) â€” siempre caen hasta MaxDropsPerKill.
     *   2. Entries probabilÃ­sticas â€” se evalÃºan con FMath::FRand() vs DropChance * LuckMultiplier * (1+LuckBonus).
     *
     * @param LuckBonus  Bonus de suerte del jugador (0.0 = sin bonus, 0.5 = +50% a todas las chances).
     * @return           Array de FLootResult con los items que cayeron. Puede estar vacÃ­o.
     */
    UFUNCTION(BlueprintCallable, Category = "Loot Table")
    TArray<FLootResult> RollLoot(float LuckBonus = 0.f) const;

    /**
     * Tira la cantidad de moneda que suelta este enemigo.
     *
     * @return  Valor aleatorio entre MinCurrencyDrop y MaxCurrencyDrop inclusive (0 si ambos son 0).
     */
    UFUNCTION(BlueprintCallable, Category = "Loot Table|Currency")
    int32 RollCurrency() const;

    /** FPrimaryAssetId para el AssetManager. */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
