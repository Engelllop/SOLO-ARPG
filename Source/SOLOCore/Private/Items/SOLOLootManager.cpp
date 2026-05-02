#include "Items/SOLOLootManager.h"
#include "Items/SOLOLootPickup.h"
#include "Data/SOLORarityHelper.h"
#include "Data/SOLOEnums.h"

TArray<FInventorySlot> USOLOLootManager::GenerateDeathLoot(AActor* DeadActor, float LuckBonus)
{
	TArray<FInventorySlot> Result;
	if (!EnemyLootTable) return Result;

	// Roll 1-3 items; each gets a rarity roll
	int32 Count = FMath::RandRange(1, 3);
	TArray<FName> RowNames = EnemyLootTable->GetRowNames();
	if (RowNames.IsEmpty()) return Result;

	for (int32 i = 0; i < Count; i++)
	{
		FName Row = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
		FInventorySlot Slot;
		Slot.ItemID = Row;
		Slot.Rarity = RollRarityForLuck(LuckBonus);
		Slot.Quantity = 1;
		Result.Add(Slot);
	}
	return Result;
}

TArray<FInventorySlot> USOLOLootManager::GenerateChestLoot(ESOLOChestType ChestType, float LuckBonus)
{
	TArray<FInventorySlot> Result;
	if (!ChestLootTable) return Result;

	int32 Count;
	float RarityBonus = LuckBonus;
	switch (ChestType)
	{
	case ESOLOChestType::Wood:   Count = 2; break;
	case ESOLOChestType::Iron:   Count = 4; RarityBonus += 10.f; break;
	case ESOLOChestType::Silver: Count = 6; RarityBonus += 25.f; break;
	case ESOLOChestType::Gold:   Count = 8; RarityBonus += 40.f; break;
	default:                     Count = 2; break;
	}

	TArray<FName> RowNames = ChestLootTable->GetRowNames();
	for (int32 i = 0; i < Count && !RowNames.IsEmpty(); i++)
	{
		FInventorySlot Slot;
		Slot.ItemID = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
		Slot.Rarity = RollRarityForLuck(RarityBonus);
		Slot.Quantity = 1;
		Result.Add(Slot);
	}
	return Result;
}

void USOLOLootManager::SpawnLootPickups(const TArray<FInventorySlot>& Slots, FVector Origin)
{
	if (!LootPickupClass) return;
	UWorld* World = GetWorld();
	if (!World) return;

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		float Angle = (2.f * PI * i) / FMath::Max(1, Slots.Num());
		FVector Offset(FMath::Cos(Angle) * 100.f, FMath::Sin(Angle) * 100.f, 0.f);
		FTransform Transform(Origin + Offset);
		if (ASOLOLootPickup* Pickup = World->SpawnActor<ASOLOLootPickup>(LootPickupClass, Transform))
			Pickup->InitPickup(Slots[i]);
	}
}

ESOLOItemRarity USOLOLootManager::RollRarityForLuck(float LuckBonus) const
{
	return USOLORarityHelper::RollRarity(LuckBonus);
}
