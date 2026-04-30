#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Inventory/SOLOItemTypes.h"
#include "Data/SOLOEnums.h"
#include "SOLOLootManager.generated.h"

UCLASS()
class SOLOCORE_API USOLOLootManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// Returns a randomized loot table entry for an enemy death
	UFUNCTION(BlueprintCallable)
	TArray<FInventorySlot> GenerateDeathLoot(AActor* DeadActor, float LuckBonus = 0.f);

	// Returns loot appropriate for the given chest type
	UFUNCTION(BlueprintCallable)
	TArray<FInventorySlot> GenerateChestLoot(ESOLOChestType ChestType, float LuckBonus = 0.f);

	// Spawns physical pickup actors for the given slots near Origin
	UFUNCTION(BlueprintCallable)
	void SpawnLootPickups(const TArray<FInventorySlot>& Slots, FVector Origin);

protected:
	UPROPERTY(EditDefaultsOnly) UDataTable* EnemyLootTable;
	UPROPERTY(EditDefaultsOnly) UDataTable* ChestLootTable;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> LootPickupClass;

private:
	ESOLOItemRarity RollRarityForLuck(float LuckBonus) const;
};
