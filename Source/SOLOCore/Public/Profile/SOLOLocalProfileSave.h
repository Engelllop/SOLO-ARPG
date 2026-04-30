#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/SOLOGameInstance.h"
#include "SOLOLocalProfileSave.generated.h"

USTRUCT()
struct FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FPrimaryAssetId ItemId;

	UPROPERTY(SaveGame)
	int32 Quantity = 0;
};

USTRUCT()
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName QuestId;

	UPROPERTY(SaveGame)
	TArray<int32> ObjectiveProgress;
};

/**
 * Guardado local (single-player / sin EOS).
 * Perfil ligero (DisplayName) y datos de partida ampliables (nivel, posiciÃ³n, quests, etc.).
 */
UCLASS()
class SOLOCORE_API USOLOLocalProfileSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, VisibleAnywhere, Category = "Profile")
	FString DisplayName;

	UPROPERTY(SaveGame)
	int32 SavedLevel = 1;

	UPROPERTY(SaveGame)
	float SavedExperience = 0.f;

	UPROPERTY(SaveGame)
	FVector LastSaveLocation = FVector::ZeroVector;

	UPROPERTY(SaveGame)
	FRotator LastSaveRotation = FRotator::ZeroRotator;

	UPROPERTY(SaveGame)
	int32 SavedCurrency = 0;

	UPROPERTY(SaveGame)
	TArray<FName> CompletedQuestIDs;

	UPROPERTY(SaveGame)
	TArray<FName> UnlockedSkillIDs;

	UPROPERTY(SaveGame)
	ESOLOClass SavedClass = ESOLOClass::Warrior;

	UPROPERTY(SaveGame)
	ESOLORace SavedRace = ESOLORace::Human;

	UPROPERTY(SaveGame)
	TArray<FInventorySaveData> InventoryItems;

	UPROPERTY(SaveGame)
	TArray<FQuestSaveData> ActiveQuests;

	UPROPERTY(SaveGame)
	TArray<FName> AbilityBarSlots;

	UPROPERTY(SaveGame)
	TArray<FName> UnlockedTalentNodes;
};
