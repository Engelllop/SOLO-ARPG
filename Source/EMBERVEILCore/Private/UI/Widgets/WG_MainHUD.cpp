#include "UI/Widgets/WG_MainHUD.h"
#include "UI/Widgets/WG_VitalBars.h"
#include "UI/Widgets/WG_ExperienceBar.h"
#include "UI/Widgets/WG_Minimap.h"
#include "UI/Widgets/WG_AbilityBar.h"
#include "UI/Widgets/WG_Hotbar.h"
#include "UI/Widgets/WG_PartyFrames.h"
#include "UI/Widgets/WG_ComboCounter.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"

void UWG_MainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	CachedPlayerState = PC->GetPlayerState<AEMBERVEILPlayerState>();
	if (!CachedPlayerState)
	{
		return;
	}

	CachedASC = CachedPlayerState->GetAbilitySystemComponent();
	if (CachedASC)
	{
		BindASCDelegates();
	}

	// Snapshot current values so bars start populated rather than empty
	const UEMBERVEILAttributeSet* AS = CachedPlayerState->GetAttributeSet();
	if (AS && VitalBarsWidget)
	{
		CachedMaxHealth = FMath::Max(AS->GetMaxHealth(), 1.f);
		CachedMaxMana = FMath::Max(AS->GetMaxMana(), 1.f);
		CachedMaxStamina = FMath::Max(AS->GetMaxStamina(), 1.f);

		VitalBarsWidget->SetHealthPercent(AS->GetHealth() / CachedMaxHealth);
		VitalBarsWidget->SetManaPercent(AS->GetMana() / CachedMaxMana);
		VitalBarsWidget->SetStaminaPercent(AS->GetStamina() / CachedMaxStamina);
	}
}

void UWG_MainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// VitalBars and Minimap each drive their own Tick — nothing to do here centrally
}

void UWG_MainHUD::ToggleMinimap()
{
	if (MinimapWidget)
	{
		MinimapWidget->ToggleMinimapVisibility();
	}
}

void UWG_MainHUD::BindASCDelegates()
{
	const UEMBERVEILAttributeSet* AS = CachedPlayerState->GetAttributeSet();
	if (!AS)
	{
		return;
	}

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute())
		.AddUObject(this, &UWG_MainHUD::OnHealthChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute())
		.AddUObject(this, &UWG_MainHUD::OnMaxHealthChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute())
		.AddUObject(this, &UWG_MainHUD::OnManaChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute())
		.AddUObject(this, &UWG_MainHUD::OnMaxManaChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute())
		.AddUObject(this, &UWG_MainHUD::OnStaminaChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxStaminaAttribute())
		.AddUObject(this, &UWG_MainHUD::OnMaxStaminaChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetExperienceAttribute())
		.AddUObject(this, &UWG_MainHUD::OnExperienceChanged);

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetLevelAttribute())
		.AddUObject(this, &UWG_MainHUD::OnLevelChanged);
}

void UWG_MainHUD::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (VitalBarsWidget)
	{
		VitalBarsWidget->SetHealthPercent(Data.NewValue / CachedMaxHealth);
	}
}

void UWG_MainHUD::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxHealth = FMath::Max(Data.NewValue, 1.f);
	// Re-push health percent so bars scale correctly when MaxHealth changes mid-session
	if (CachedASC && VitalBarsWidget)
	{
		const UEMBERVEILAttributeSet* AS = CachedPlayerState->GetAttributeSet();
		if (AS)
		{
			VitalBarsWidget->SetHealthPercent(AS->GetHealth() / CachedMaxHealth);
		}
	}
}

void UWG_MainHUD::OnManaChanged(const FOnAttributeChangeData& Data)
{
	if (VitalBarsWidget)
	{
		VitalBarsWidget->SetManaPercent(Data.NewValue / CachedMaxMana);
	}
}

void UWG_MainHUD::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxMana = FMath::Max(Data.NewValue, 1.f);
	if (CachedASC && VitalBarsWidget)
	{
		const UEMBERVEILAttributeSet* AS = CachedPlayerState->GetAttributeSet();
		if (AS)
		{
			VitalBarsWidget->SetManaPercent(AS->GetMana() / CachedMaxMana);
		}
	}
}

void UWG_MainHUD::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (VitalBarsWidget)
	{
		VitalBarsWidget->SetStaminaPercent(Data.NewValue / CachedMaxStamina);
	}
}

void UWG_MainHUD::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxStamina = FMath::Max(Data.NewValue, 1.f);
	if (CachedASC && VitalBarsWidget)
	{
		const UEMBERVEILAttributeSet* AS = CachedPlayerState->GetAttributeSet();
		if (AS)
		{
			VitalBarsWidget->SetStaminaPercent(AS->GetStamina() / CachedMaxStamina);
		}
	}
}

void UWG_MainHUD::OnExperienceChanged(const FOnAttributeChangeData& Data)
{
	const UEMBERVEILAttributeSet* AS = CachedPlayerState ? CachedPlayerState->GetAttributeSet() : nullptr;
	if (!AS)
	{
		return;
	}

	CachedRequiredXP = FMath::Max(AS->GetExperienceToNextLevel(), 1.f);

	if (ExperienceBarWidget)
	{
		ExperienceBarWidget->UpdateExperience(Data.NewValue, CachedRequiredXP);
	}
}

void UWG_MainHUD::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	const int32 NewLevel = FMath::FloorToInt(Data.NewValue);

	if (ExperienceBarWidget)
	{
		ExperienceBarWidget->TriggerLevelUp(NewLevel);
	}

	if (AbilityBarWidget)
	{
		AbilityBarWidget->CheckLevelUnlocks(NewLevel);
	}
}
