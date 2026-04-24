#include "UI/Widgets/WG_VitalBars.h"
#include "Math/UnrealMathUtility.h"

void UWG_VitalBars::NativeConstruct()
{
	Super::NativeConstruct();

	// Start all display values at full so first frame doesn't flash empty bars
	CurrentHealthDisplay = 1.f;
	CurrentDelayHealthDisplay = 1.f;
	CurrentManaDisplay = 1.f;
	CurrentStaminaDisplay = 1.f;
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

	// Delay bar only chases downward — if HP goes up, sync instantly
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
}

void UWG_VitalBars::SetManaPercent(float Percent)
{
	ManaPercent = FMath::Clamp(Percent, 0.f, 1.f);
}

void UWG_VitalBars::SetStaminaPercent(float Percent)
{
	StaminaPercent = FMath::Clamp(Percent, 0.f, 1.f);
}
