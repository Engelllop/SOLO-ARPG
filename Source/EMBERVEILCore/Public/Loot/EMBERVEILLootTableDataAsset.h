#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Loot/EMBERVEILLootTypes.h"
#include "EMBERVEILLootTableDataAsset.generated.h"

/**
 * DataAsset que define la tabla de drops de un enemigo o contenedor.
 * Contiene una lista de FLootEntry que se evalúan al llamar a RollLoot().
 *
 * Crear subactivos en el Content Browser para cada tipo de enemigo.
 * Asignar en el LootComponent del EnemyCharacter correspondiente.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UEMBERVEILLootTableDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // ─── ENTRADAS DE LOOT ─────────────────────────────────────────────────────

    /** Lista completa de posibles drops. Los garantizados siempre caen; los demás, según probabilidad. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
    TArray<FLootEntry> LootEntries;

    // ─── MONEDA ───────────────────────────────────────────────────────────────

    /** Mínimo de monedas que puede soltar este enemigo (0 = sin monedas). */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table|Currency",
        meta = (ClampMin = "0"))
    int32 MinCurrencyDrop = 0;

    /** Máximo de monedas que puede soltar este enemigo. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table|Currency",
        meta = (ClampMin = "0"))
    int32 MaxCurrencyDrop = 0;

    // ─── PARÁMETROS GLOBALES ──────────────────────────────────────────────────

    /**
     * Multiplicador global de probabilidades de drop para esta tabla.
     * Combinado con el LuckBonus del jugador. 1.0 = normal, 2.0 = doble suerte.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table",
        meta = (ClampMin = "0.1"))
    float LuckMultiplier = 1.0f;

    /**
     * Número máximo de items distintos que pueden caer en una sola muerte.
     * Los drops garantizados se procesan primero y consumen cupo de este límite.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table",
        meta = (ClampMin = "1"))
    int32 MaxDropsPerKill = 5;

    // ─── FUNCIONES ────────────────────────────────────────────────────────────

    /**
     * Evalúa la tabla de loot y devuelve los items que caen en esta tirada.
     *
     * Orden de procesado:
     *   1. Entries garantizadas (bIsGuaranteed == true) — siempre caen hasta MaxDropsPerKill.
     *   2. Entries probabilísticas — se evalúan con FMath::FRand() vs DropChance * LuckMultiplier * (1+LuckBonus).
     *
     * @param LuckBonus  Bonus de suerte del jugador (0.0 = sin bonus, 0.5 = +50% a todas las chances).
     * @return           Array de FLootResult con los items que cayeron. Puede estar vacío.
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
