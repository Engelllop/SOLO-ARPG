#include "UI/Widgets/WG_VitalBars.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Math/UnrealMathUtility.h"

#define LOCTEXT_NAMESPACE "SOLOVitalBars"

void UWG_VitalBars::NativeConstruct()
{
	Super::NativeConstruct();

	// Start all display values at full so first frame doesn't flash empty bars
	CurrentHealthDisplay = 1.f;
	CurrentDelayHealthDisplay = 1.f;
	CurrentManaDisplay = 1.f;
	CurrentStaminaDisplay = 1.f;

	// Default name
	if (CharacterNameText && CharacterNameText->GetText().IsEmpty())
	{
		CharacterNameText->SetText(LOCTEXT("DefaultName", "Aelindra"));
	}

	UpdateValueTexts();
}

void UWG_VitalBars::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	bool bHealthDirty = false;
	bool bManaDirty = false;
	bool bStaminaDirty = false;

	const float NewHealth = FMath::FInterpTo(CurrentHealthDisplay, HealthPercent, InDeltaTime, BarInterpSpeed);
	if (!FMath::IsNearlyEqual(NewHealth, CurrentHealthDisplay, 0.0001f))
	{
		CurrentHealthDisplay = NewHealth;
		bHealthDirty = true;
	}

	// Delay bar only chases downward â€” if HP goes up, sync instantly
	const float DelayTarget = (HealthPercent < CurrentDelayHealthDisplay) ? HealthPercent : CurrentHealthDisplay;
	const float NewDelay = FMath::FInterpTo(CurrentDelayHealthDisplay, DelayTarget, InDeltaTime, DelayBarInterpSpeed);
	if (!FMath::IsNearlyEqual(NewDelay, CurrentDelayHealthDisplay, 0.0001f))
	{
		CurrentDelayHealthDisplay = NewDelay;
		bHealthDirty = true;
	}

	if (bHealthDirty)
	{
		DelayHealthPercent = CurrentDelayHealthDisplay;
		BP_OnHealthChanged(CurrentHealthDisplay, CurrentDelayHealthDisplay);
	}

	const float NewMana = FMath::FInterpTo(CurrentManaDisplay, ManaPercent, InDeltaTime, BarInterpSpeed);
	if (!FMath::IsNearlyEqual(NewMana, CurrentManaDisplay, 0.0001f))
	{
		CurrentManaDisplay = NewMana;
		bManaDirty = true;
	}

	if (bManaDirty)
	{
		BP_OnManaChanged(CurrentManaDisplay);
	}

	const float NewStamina = FMath::FInterpTo(CurrentStaminaDisplay, StaminaPercent, InDeltaTime, BarInterpSpeed);
	if (!FMath::IsNearlyEqual(NewStamina, CurrentStaminaDisplay, 0.0001f))
	{
		CurrentStaminaDisplay = NewStamina;
		bStaminaDirty = true;
	}

	if (bStaminaDirty)
	{
		BP_OnStaminaChanged(CurrentStaminaDisplay);
	}
}

void UWG_VitalBars::SetHealthPercent(float Percent)
{
	HealthPercent = FMath::Clamp(Percent, 0.f, 1.f);
	UpdateValueTexts();
}

void UWG_VitalBars::SetManaPercent(float Percent)
{
	ManaPercent = FMath::Clamp(Percent, 0.f, 1.f);
	UpdateValueTexts();
}

void UWG_VitalBars::SetStaminaPercent(float Percent)
{
	StaminaPercent = FMath::Clamp(Percent, 0.f, 1.f);
	UpdateValueTexts();
}

void UWG_VitalBars::SetCharacterName(const FText& NewName)
{
	if (CharacterNameText)
	{
		CharacterNameText->SetText(NewName);
	}
}

void UWG_VitalBars::SetAvatarIcon(UTexture2D* Icon)
{
	if (AvatarImage && Icon)
	{
		AvatarImage->SetBrushFromTexture(Icon);
	}
}

void UWG_VitalBars::SetVitalValues(float Health, float MaxHealth, float Mana, float MaxMana, float Stamina, float MaxStamina)
{
	CurrentHealth = FMath::RoundToInt(Health);
	CurrentMaxHealth = FMath::RoundToInt(FMath::Max(MaxHealth, 1.f));
	CurrentMana = FMath::RoundToInt(Mana);
	CurrentMaxMana = FMath::RoundToInt(FMath::Max(MaxMana, 1.f));
	CurrentStamina = FMath::RoundToInt(Stamina);
	CurrentMaxStamina = FMath::RoundToInt(FMath::Max(MaxStamina, 1.f));

	SetHealthPercent(MaxHealth > 0.f ? Health / MaxHealth : 0.f);
	SetManaPercent(MaxMana > 0.f ? Mana / MaxMana : 0.f);
	SetStaminaPercent(MaxStamina > 0.f ? Stamina / MaxStamina : 0.f);

	UpdateValueTexts();
	BP_OnVitalValuesUpdated();
}

void UWG_VitalBars::UpdateValueTexts()
{
	if (HealthValueText)
	{
		HealthValueText->SetText(FText::Format(
			LOCTEXT("HPFmt", "{0}/{1}"),
			FText::AsNumber(CurrentHealth),
			FText::AsNumber(CurrentMaxHealth)));
	}

	if (ManaValueText)
	{
		ManaValueText->SetText(FText::Format(
			LOCTEXT("MPFmt", "{0}/{1}"),
			FText::AsNumber(CurrentMana),
			FText::AsNumber(CurrentMaxMana)));
	}

	if (StaminaValueText)
	{
		StaminaValueText->SetText(FText::Format(
			LOCTEXT("STAFmt", "{0}/{1}"),
			FText::AsNumber(CurrentStamina),
			FText::AsNumber(CurrentMaxStamina)));
	}
}

#undef LOCTEXT_NAMESPACE
