#include "UI/Widgets/WG_ExperienceBar.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/EMBERVEILPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWG_ExperienceBar::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	AEMBERVEILPlayerState* PS = PC->GetPlayerState<AEMBERVEILPlayerState>();
	if (!PS) return;

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	const UEMBERVEILAttributeSet* AS = PS->GetAttributeSet();
	if (!ASC || !AS) return;

	// Bind to attribute changes
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetExperienceAttribute())
		.AddUObject(this, &UWG_ExperienceBar::OnExperienceChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetExperienceToNextLevelAttribute())
		.AddUObject(this, &UWG_ExperienceBar::OnExperienceChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetLevelAttribute())
		.AddUObject(this, &UWG_ExperienceBar::OnLevelChanged);

	// Initial update
	RefreshUI();
}

void UWG_ExperienceBar::UpdateExperience(float CurrentXP, float RequiredXP)
{
	const float PreviousXP = CurrentXPValue;

	CurrentXPValue = CurrentXP;
	RequiredXPValue = FMath::Max(RequiredXP, 1.f);
	XPPercent = CurrentXPValue / RequiredXPValue;

	// Only animate and show text if XP actually increased
	if (CurrentXP > PreviousXP)
	{
		TriggerXPGainAnimation();

		// Reset the 3s auto-hide timer each time new XP arrives
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(
				XPTextVisibilityTimer,
				this,
				&UWG_ExperienceBar::HideXPText,
				3.f,
				false
			);
		}
	}
}

void UWG_ExperienceBar::TriggerLevelUp(int32 NewLevel)
{
	DisplayLevel = NewLevel;
	TriggerLevelUpAnimation(NewLevel);
}

void UWG_ExperienceBar::HideXPText()
{
	// Blueprint responds by fading out the XP text label
}

void UWG_ExperienceBar::OnExperienceChanged(const FOnAttributeChangeData& Data)
{
	RefreshUI();
}

void UWG_ExperienceBar::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	TriggerLevelUp(FMath::FloorToInt(Data.NewValue));
	RefreshUI();
}

void UWG_ExperienceBar::RefreshUI()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	AEMBERVEILPlayerState* PS = PC->GetPlayerState<AEMBERVEILPlayerState>();
	if (!PS) return;

	const UEMBERVEILAttributeSet* AS = PS->GetAttributeSet();
	if (!AS) return;

	const float CurXP = AS->GetExperience();
	const float ReqXP = FMath::Max(AS->GetExperienceToNextLevel(), 1.f);
	const int32 Lvl = FMath::FloorToInt(AS->GetLevel());

	UpdateExperience(CurXP, ReqXP);

	if (PB_Experience)
	{
		PB_Experience->SetPercent(CurXP / ReqXP);
	}

	if (TB_LevelInfo)
	{
		// Lv. {Level} — {CurrentXP}/{XPNeeded}
		FText InfoText = FText::Format(
			NSLOCTEXT("XPBar", "LevelInfoFmt", "Lv. {0} — {1}/{2}"),
			FText::AsNumber(Lvl),
			FText::AsNumber(FMath::FloorToInt(CurXP)),
			FText::AsNumber(FMath::FloorToInt(ReqXP))
		);
		TB_LevelInfo->SetText(InfoText);
	}
}
