#include "Profile/SOLOSaveManager.h"
#include "Profile/SOLOLocalProfileSave.h"
#include "Character/SOLOPlayerCharacter.h"
#include "Player/SOLOPlayerState.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Quest/SOLOQuestComponent.h"
#include "Skills/SOLOSkillMasteryComponent.h"
#include "Skills/SOLOTalentTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Net/UnrealNetwork.h"

namespace
{
	static void SanitizeSlotName(FString& SlotName)
	{
		SlotName.TrimStartAndEndInline();
		if (SlotName.IsEmpty())
		{
			SlotName = TEXT("Slot1");
		}
	}
}

void USOLOSaveManager::SavePlayerGame(ASOLOPlayerCharacter* Player, FString SlotName)
{
	SanitizeSlotName(SlotName);

	if (!IsValid(Player) || !Player->HasAuthority())
	{
		return;
	}

	ASOLOPlayerState* PS = Player->GetPlayerState<ASOLOPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	USOLOLocalProfileSave* Save = Cast<USOLOLocalProfileSave>(
		UGameplayStatics::CreateSaveGameObject(USOLOLocalProfileSave::StaticClass()));
	if (!Save)
	{
		return;
	}

	if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
	{
		Save->DisplayName = GI->ActivePlayerData.CharacterName;
		Save->SavedClass = GI->ActivePlayerData.Class;
		Save->SavedRace = GI->ActivePlayerData.Race;
	}

	Save->SavedLevel = PS->GetPlayerLevel();

	if (USOLOAttributeSet* AS = PS->GetAttributeSet())
	{
		Save->SavedExperience = AS->GetExperience();
	}

	Save->LastSaveLocation = Player->GetActorLocation();
	Save->LastSaveRotation = Player->GetActorRotation();

	if (USOLOInventoryComponent* Inv = PS->FindComponentByClass<USOLOInventoryComponent>())
	{
		Save->SavedCurrency = Inv->Currency;
	}

	if (USOLOQuestComponent* QC = PS->FindComponentByClass<USOLOQuestComponent>())
	{
		Save->CompletedQuestIDs = QC->CompletedQuestIDs;
	}

	if (USOLOSkillMasteryComponent* SMC = PS->FindComponentByClass<USOLOSkillMasteryComponent>())
	{
		Save->UnlockedSkillIDs = SMC->GetUnlockedSkillIDs();
	}

	if (USOLOTalentTreeComponent* TTC = PS->FindComponentByClass<USOLOTalentTreeComponent>())
	{
		Save->UnlockedTalentNodes = TTC->UnlockedTalentNodes;
	}

	UGameplayStatics::SaveGameToSlot(Save, SlotName, DefaultUserIndex);
}

void USOLOSaveManager::LoadPlayerGame(ASOLOPlayerCharacter* Player, FString SlotName)
{
	SanitizeSlotName(SlotName);

	if (!IsValid(Player) || !Player->HasAuthority())
	{
		return;
	}

	if (!DoesSaveExist(SlotName))
	{
		return;
	}

	USOLOLocalProfileSave* Loaded = Cast<USOLOLocalProfileSave>(
		UGameplayStatics::LoadGameFromSlot(SlotName, DefaultUserIndex));
	if (!IsValid(Loaded))
	{
		return;
	}

	ASOLOPlayerState* PS = Player->GetPlayerState<ASOLOPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.CharacterName = Loaded->DisplayName;
		GI->ActivePlayerData.Class = Loaded->SavedClass;
		GI->ActivePlayerData.Race = Loaded->SavedRace;
		GI->ActivePlayerData.Level = Loaded->SavedLevel;
	}

	if (USOLOAttributeSet* AS = PS->GetAttributeSet())
	{
		const int32 ClampedLevel = FMath::Max(1, Loaded->SavedLevel);
		AS->SetLevel(static_cast<float>(ClampedLevel));
		AS->SetExperience(Loaded->SavedExperience);
		AS->SetExperienceToNextLevel(USOLOAttributeSet::GetExperienceRequiredToAdvanceFromLevel(ClampedLevel));
		AS->FlushExperienceLevelUps();
	}

	if (USOLOInventoryComponent* Inv = PS->FindComponentByClass<USOLOInventoryComponent>())
	{
		const int32 Current = Inv->Currency;
		const int32 Target = Loaded->SavedCurrency;
		if (Current != Target)
		{
			if (Current < Target)
			{
				Inv->AddCurrency(Target - Current);
			}
			else
			{
				Inv->SpendCurrency(Current - Target);
			}
		}
	}

	if (USOLOQuestComponent* QC = PS->FindComponentByClass<USOLOQuestComponent>())
	{
		QC->CompletedQuestIDs = Loaded->CompletedQuestIDs;
		MARK_PROPERTY_DIRTY_FROM_NAME(USOLOQuestComponent, CompletedQuestIDs, QC);
	}

	if (USOLOSkillMasteryComponent* SMC = PS->FindComponentByClass<USOLOSkillMasteryComponent>())
	{
		SMC->RestoreUnlockedSkillsFromSave(Loaded->UnlockedSkillIDs);
	}

	if (USOLOTalentTreeComponent* TTC = PS->FindComponentByClass<USOLOTalentTreeComponent>())
	{
		TTC->RestoreUnlockedTalentsFromSave(Loaded->UnlockedTalentNodes);
	}

	Player->SetActorLocationAndRotation(
		Loaded->LastSaveLocation,
		Loaded->LastSaveRotation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics);
}

void USOLOSaveManager::AutoSave(ASOLOPlayerCharacter* Player)
{
	SavePlayerGame(Player, TEXT("AutoSave"));
}

bool USOLOSaveManager::DoesSaveExist(const FString& SlotName) const
{
	FString NameCopy = SlotName;
	SanitizeSlotName(NameCopy);
	return UGameplayStatics::DoesSaveGameExist(NameCopy, DefaultUserIndex);
}
