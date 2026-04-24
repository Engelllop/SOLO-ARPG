#include "Profile/EMBERVEILSaveManager.h"
#include "Profile/EMBERVEILLocalProfileSave.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "Player/EMBERVEILPlayerState.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Quest/EMBERVEILQuestComponent.h"
#include "Skills/EMBERVEILSkillMasteryComponent.h"
#include "Skills/EMBERVEILTalentTreeComponent.h"
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

void UEMBERVEILSaveManager::SavePlayerGame(AEMBERVEILPlayerCharacter* Player, FString SlotName)
{
	SanitizeSlotName(SlotName);

	if (!IsValid(Player) || !Player->HasAuthority())
	{
		return;
	}

	AEMBERVEILPlayerState* PS = Player->GetPlayerState<AEMBERVEILPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	UEMBERVEILLocalProfileSave* Save = Cast<UEMBERVEILLocalProfileSave>(
		UGameplayStatics::CreateSaveGameObject(UEMBERVEILLocalProfileSave::StaticClass()));
	if (!Save)
	{
		return;
	}

	if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
	{
		Save->DisplayName = GI->ActivePlayerData.CharacterName;
		Save->SavedClass = GI->ActivePlayerData.Class;
		Save->SavedRace = GI->ActivePlayerData.Race;
	}

	Save->SavedLevel = PS->GetPlayerLevel();

	if (UEMBERVEILAttributeSet* AS = PS->GetAttributeSet())
	{
		Save->SavedExperience = AS->GetExperience();
	}

	Save->LastSaveLocation = Player->GetActorLocation();
	Save->LastSaveRotation = Player->GetActorRotation();

	if (UEMBERVEILInventoryComponent* Inv = PS->FindComponentByClass<UEMBERVEILInventoryComponent>())
	{
		Save->SavedCurrency = Inv->Currency;
	}

	if (UEMBERVEILQuestComponent* QC = PS->FindComponentByClass<UEMBERVEILQuestComponent>())
	{
		Save->CompletedQuestIDs = QC->CompletedQuestIDs;
	}

	if (UEMBERVEILSkillMasteryComponent* SMC = PS->FindComponentByClass<UEMBERVEILSkillMasteryComponent>())
	{
		Save->UnlockedSkillIDs = SMC->GetUnlockedSkillIDs();
	}

	if (UEMBERVEILTalentTreeComponent* TTC = PS->FindComponentByClass<UEMBERVEILTalentTreeComponent>())
	{
		Save->UnlockedTalentNodes = TTC->UnlockedTalentNodes;
	}

	UGameplayStatics::SaveGameToSlot(Save, SlotName, DefaultUserIndex);
}

void UEMBERVEILSaveManager::LoadPlayerGame(AEMBERVEILPlayerCharacter* Player, FString SlotName)
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

	UEMBERVEILLocalProfileSave* Loaded = Cast<UEMBERVEILLocalProfileSave>(
		UGameplayStatics::LoadGameFromSlot(SlotName, DefaultUserIndex));
	if (!IsValid(Loaded))
	{
		return;
	}

	AEMBERVEILPlayerState* PS = Player->GetPlayerState<AEMBERVEILPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.CharacterName = Loaded->DisplayName;
		GI->ActivePlayerData.Class = Loaded->SavedClass;
		GI->ActivePlayerData.Race = Loaded->SavedRace;
		GI->ActivePlayerData.Level = Loaded->SavedLevel;
	}

	if (UEMBERVEILAttributeSet* AS = PS->GetAttributeSet())
	{
		const int32 ClampedLevel = FMath::Max(1, Loaded->SavedLevel);
		AS->SetLevel(static_cast<float>(ClampedLevel));
		AS->SetExperience(Loaded->SavedExperience);
		AS->SetExperienceToNextLevel(UEMBERVEILAttributeSet::GetExperienceRequiredToAdvanceFromLevel(ClampedLevel));
		AS->FlushExperienceLevelUps();
	}

	if (UEMBERVEILInventoryComponent* Inv = PS->FindComponentByClass<UEMBERVEILInventoryComponent>())
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

	if (UEMBERVEILQuestComponent* QC = PS->FindComponentByClass<UEMBERVEILQuestComponent>())
	{
		QC->CompletedQuestIDs = Loaded->CompletedQuestIDs;
		MARK_PROPERTY_DIRTY_FROM_NAME(UEMBERVEILQuestComponent, CompletedQuestIDs, QC);
	}

	if (UEMBERVEILSkillMasteryComponent* SMC = PS->FindComponentByClass<UEMBERVEILSkillMasteryComponent>())
	{
		SMC->RestoreUnlockedSkillsFromSave(Loaded->UnlockedSkillIDs);
	}

	if (UEMBERVEILTalentTreeComponent* TTC = PS->FindComponentByClass<UEMBERVEILTalentTreeComponent>())
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

void UEMBERVEILSaveManager::AutoSave(AEMBERVEILPlayerCharacter* Player)
{
	SavePlayerGame(Player, TEXT("AutoSave"));
}

bool UEMBERVEILSaveManager::DoesSaveExist(const FString& SlotName) const
{
	FString NameCopy = SlotName;
	SanitizeSlotName(NameCopy);
	return UGameplayStatics::DoesSaveGameExist(NameCopy, DefaultUserIndex);
}
