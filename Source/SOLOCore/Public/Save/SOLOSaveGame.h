#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSOLOEquipmentSaveEntry
{
	GENERATED_BODY()
	UPROPERTY() ESOLOEquipmentSlot Slot = ESOLOEquipmentSlot::MainHand;
	UPROPERTY() FName ItemID;
	UPROPERTY() ESOLOItemRarity Rarity = ESOLOItemRarity::Common;
};

UCLASS()
class SOLOCORE_API USOLOSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	// Character progression
	UPROPERTY() int32 CharacterLevel = 1;
	UPROPERTY() float ExperiencePoints = 0.f;
	UPROPERTY() FName SelectedClassID;
	UPROPERTY() TArray<FName> UnlockedSkillIDs;

	// World position
	UPROPERTY() FVector PlayerPosition = FVector::ZeroVector;
	UPROPERTY() FRotator PlayerRotation = FRotator::ZeroRotator;
	UPROPERTY() int32 FloorID = 0;
	UPROPERTY() FName LastCampfireID;

	// Inventory & equipment
	UPROPERTY() TArray<FInventorySlot> InventorySlots;
	UPROPERTY() TArray<FSOLOEquipmentSaveEntry> EquippedItems;
	UPROPERTY() int32 Gold = 0;
	UPROPERTY() TMap<FName, int32> Materials;

	// Quests & story
	UPROPERTY() TArray<FName> ActiveQuestIDs;
	UPROPERTY() TArray<FName> CompletedQuestIDs;
	UPROPERTY() TMap<FName, bool> StoryFlags;

	// Companions
	UPROPERTY() TArray<FName> ActiveCompanionIDs;

	// Meta
	UPROPERTY() FDateTime SaveTimestamp;
	UPROPERTY() int32 SaveSlot = 0;
};
