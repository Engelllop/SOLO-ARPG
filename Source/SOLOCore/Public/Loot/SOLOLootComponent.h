#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLOLootComponent.generated.h"

class USOLOLootTableDataAsset;
class ASOLOWorldItem;

/**
 * Componente que gestiona el loot de un actor (generalmente un EnemyCharacter).
 * Al morir el owner, evalÃºa la LootTable y spawnea ASOLOWorldItem en el mundo.
 *
 * Colocar en EnemyCharacter. Asignar la LootTable en el Blueprint correspondiente.
 * Solo ejecuta lÃ³gica de spawn en el servidor; los WorldItems se replican automÃ¡ticamente.
 */
UCLASS(ClassGroup = (SOLO), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOLootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USOLOLootComponent();

    // â”€â”€â”€ CONFIGURACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Tabla de drops que se evalÃºa al morir el owner. Nulo = sin drops de items. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TObjectPtr<USOLOLootTableDataAsset> LootTable;

    /**
     * Radio en unidades de mundo dentro del cual se dispersan los WorldItems al spawnearse.
     * Los items se distribuyen aleatoriamente en un cÃ­rculo de este radio alrededor de SpawnLocation.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot",
        meta = (ClampMin = "10.0"))
    float LootRadius = 150.f;

    /**
     * Si true, el componente escucha el tag State.Dead del ASC del owner y llama
     * SpawnLoot automÃ¡ticamente al morir. Si false, SpawnLoot debe llamarse a mano.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    bool bAutoSpawnOnDeath = true;

    /**
     * Tiempo en segundos antes de que los ASOLOWorldItem spawneados desaparezcan.
     * Evita que el mundo se llene de items si nadie los recoge.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot",
        meta = (ClampMin = "1.0"))
    float ItemLifetime = 60.f;

    // â”€â”€â”€ FUNCIONES PÃšBLICAS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * EvalÃºa la LootTable y spawnea los ASOLOWorldItem resultantes en el servidor.
     * Cada item se spawnea con un offset aleatorio dentro de LootRadius.
     * Si hay moneda, spawnea un WorldItem especial con bIsCurrency = true.
     *
     * Solo tiene efecto en el servidor (HasAuthority()).
     *
     * @param SpawnLocation  PosiciÃ³n central desde la que se dispersan los drops.
     */
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void SpawnLoot(FVector SpawnLocation);

    /**
     * Llamado cuando el owner muere. Si bAutoSpawnOnDeath es true, invoca SpawnLoot
     * con la ubicaciÃ³n actual del owner.
     *
     * Enlazado automÃ¡ticamente al tag State.Dead del ASC en BeginPlay si bAutoSpawnOnDeath.
     * TambiÃ©n puede llamarse directamente desde Die_Implementation de EnemyCharacter.
     *
     * @param Killer  Actor que matÃ³ al owner (puede ser nulo).
     */
    UFUNCTION(BlueprintCallable, Category = "Loot")
    void OnOwnerDied(AActor* Killer);

protected:
    virtual void BeginPlay() override;

private:
    /**
     * Genera un offset aleatorio 2D dentro de un cÃ­rculo de radio LootRadius.
     * Se usa para distribuir los items en el suelo alrededor del cadÃ¡ver.
     */
    FVector GetRandomOffsetInRadius() const;

    /** Handle para el binding del tag State.Dead en el ASC del owner. */
    FDelegateHandle DeathTagHandle;
};
