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

	// Floor-specific (N3-N10 choices)
	UPROPERTY() TArray<FName> CollectedFragments; // Ending B/C keys
	UPROPERTY() bool bLookedIntoAbyss = false;
	UPROPERTY() bool bSparedElia = false;
	UPROPERTY() bool bSparedOswin = false;
	UPROPERTY() bool bGaveCriadaName = false;
	UPROPERTY() FString GivenNameCriada;
	UPROPERTY() bool bClonedDefeatedPeacefully = false;

	// v3 — New thematic choice flags
	UPROPERTY() bool bAteHumanFleshN4 = false; // Final C condition
	UPROPERTY() bool bListenedVerdugoN9 = false; // Final B and C condition
	UPROPERTY() bool bBurnedThornwall = false; // (opcional — dark path)
	UPROPERTY() bool bSawN85Subfloor = false; // Found the abandoned floor
	UPROPERTY() bool bReadPlayerCodeN9 = false; // Read the code parchment
	UPROPERTY() TArray<FName> UnlockedCodexEntries; // Codex tracking

	// NG+ data
	UPROPERTY() bool bIsNewGamePlus = false;
	UPROPERTY() int32 NGPlusCount = 0;
	UPROPERTY() TArray<FName> PreviousEndings;

	// Meta
	UPROPERTY() FDateTime SaveTimestamp;
	UPROPERTY() int32 SaveSlot = 0;
};
