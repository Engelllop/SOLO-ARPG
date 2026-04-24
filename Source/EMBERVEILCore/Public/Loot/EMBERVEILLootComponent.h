#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMBERVEILLootComponent.generated.h"

class UEMBERVEILLootTableDataAsset;
class AEMBERVEILWorldItem;

/**
 * Componente que gestiona el loot de un actor (generalmente un EnemyCharacter).
 * Al morir el owner, evalúa la LootTable y spawnea AEMBERVEILWorldItem en el mundo.
 *
 * Colocar en EnemyCharacter. Asignar la LootTable en el Blueprint correspondiente.
 * Solo ejecuta lógica de spawn en el servidor; los WorldItems se replican automáticamente.
 */
UCLASS(ClassGroup = (EMBERVEIL), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILLootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEMBERVEILLootComponent();

    // ─── CONFIGURACIÓN ────────────────────────────────────────────────────────

    /** Tabla de drops que se evalúa al morir el owner. Nulo = sin drops de items. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TObjectPtr<UEMBERVEILLootTableDataAsset> LootTable;

    /**
     * Radio en unidades de mundo dentro del cual se dispersan los WorldItems al spawnearse.
     * Los items se distribuyen aleatoriamente en un círculo de este radio alrededor de SpawnLocation.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot",
        meta = (ClampMin = "10.0"))
    float LootRadius = 150.f;

    /**
     * Si true, el componente escucha el tag State.Dead del ASC del owner y llama
     * SpawnLoot automáticamente al morir. Si false, SpawnLoot debe llamarse a mano.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    bool bAutoSpawnOnDeath = true;

    /**
     * Tiempo en segundos antes de que los AEMBERVEILWorldItem spawneados desaparezcan.
     * Evita que el mundo se llene de items si nadie los recoge.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot",
        meta = (ClampMin = "1.0"))
    float ItemLifetime = 60.f;

    // ─── FUNCIONES PÚBLICAS ───────────────────────────────────────────────────

    /**
     * Evalúa la LootTable y spawnea los AEMBERVEILWorldItem resultantes en el servidor.
     * Cada item se spawnea con un offset aleatorio dentro de LootRadius.
     * Si hay moneda, spawnea un WorldItem especial con bIsCurrency = true.
     *
     * Solo tiene efecto en el servidor (HasAuthority()).
     *
     * @param SpawnLocation  Posición central desde la que se dispersan los drops.
     */
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void SpawnLoot(FVector SpawnLocation);

    /**
     * Llamado cuando el owner muere. Si bAutoSpawnOnDeath es true, invoca SpawnLoot
     * con la ubicación actual del owner.
     *
     * Enlazado automáticamente al tag State.Dead del ASC en BeginPlay si bAutoSpawnOnDeath.
     * También puede llamarse directamente desde Die_Implementation de EnemyCharacter.
     *
     * @param Killer  Actor que mató al owner (puede ser nulo).
     */
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void OnOwnerDied(AActor* Killer);

protected:
    virtual void BeginPlay() override;

private:
    /**
     * Genera un offset aleatorio 2D dentro de un círculo de radio LootRadius.
     * Se usa para distribuir los items en el suelo alrededor del cadáver.
     */
    FVector GetRandomOffsetInRadius() const;

    /** Handle para el binding del tag State.Dead en el ASC del owner. */
    FDelegateHandle DeathTagHandle;
};
