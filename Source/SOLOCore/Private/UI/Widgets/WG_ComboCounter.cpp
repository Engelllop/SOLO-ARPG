#include "UI/Widgets/WG_ComboCounter.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UWG_ComboCounter::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
}

void UWG_ComboCounter::IncrementCombo()
{
	CurrentCombo++;

	// Only show the widget once there's a real combo (≥2 hits)
	if (CurrentCombo >= MinComboToShow)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	BP_OnComboIncremented(CurrentCombo);

	// Reset the inactivity timer on every new hit
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(
			ComboFadeTimer,
			this,
			&UWG_ComboCounter::OnComboTimeout,
			ComboTimeout,
			false
		);
	}
}

void UWG_ComboCounter::ResetCombo()
{
	CurrentCombo = 0;

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(ComboFadeTimer);
	}

	BP_PlayFadeOut();
	// Blueprint sets Visibility to Collapsed at the end of the 0.5s fade animation
}

void UWG_ComboCounter::OnComboTimeout()
{
	ResetCombo();
}
