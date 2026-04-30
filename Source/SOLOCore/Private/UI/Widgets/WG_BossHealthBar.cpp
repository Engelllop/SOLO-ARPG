#include "UI/Widgets/WG_BossHealthBar.h"
#include "Character/SOLOEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AbilitySystemBlueprintLibrary.h"

void UWG_BossHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_BossHealthBar::SetBossReference(ASOLOEnemyCharacter* Boss)
{
	CachedBoss = Boss;
	if (Boss)
	{
		BossName = Boss->GetBossDisplayName();
		BossNameText = BossName;
	}
}

void UWG_BossHealthBar::UpdateHealthPercent(float Percent)
{
	UpdateHealth(Percent);
	if (PB_BossHealth)
	{
		PB_BossHealth->SetPercent(Percent);
	}
}

void UWG_BossHealthBar::BindToBoss(ASOLOEnemyCharacter* Boss)
{
	if (!Boss) return;

	CachedBoss = Boss;
	UAbilitySystemComponent* ASC = Boss->GetAbilitySystemComponent();
	if (!ASC) return;

	// Set name
	BossName = Boss->GetBossDisplayName();
	BossNameText = BossName;
	if (TB_BossName)
	{
		TB_BossName->SetText(BossName);
	}

	// Initial health
	if (const USOLOAttributeSet* AS = Boss->GetAttributeSet())
	{
		const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
		UpdateHealthPercent(AS->GetHealth() / MaxH);
	}

	// Register attribute change delegate
	ASC->GetGameplayAttributeValueChangeDelegate(USOLOAttributeSet::GetHealthAttribute())
		.AddUObject(this, &UWG_BossHealthBar::OnHealthChanged);

	// Register death event listener
	ASC->GenericGameplayEventCallbacks.FindOrAdd(FGameplayTag::RequestGameplayTag(FName("Event.Character.Death")))
		.AddUObject(this, &UWG_BossHealthBar::OnBossDied);

	SetVisibility(ESlateVisibility::Visible);
}

void UWG_BossHealthBar::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (CachedBoss.IsValid())
	{
		if (const USOLOAttributeSet* AS = CachedBoss->GetAttributeSet())
		{
			const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
			UpdateHealthPercent(AS->GetHealth() / MaxH);
		}
	}
}

void UWG_BossHealthBar::OnBossDied(const FGameplayEventData* Payload)
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UWG_BossHealthBar::InitBoss(const FText& InBossName, int32 InTotalPhases)
{
	BossNameText = InBossName;
	BossName = InBossName;
	TotalPhases = FMath::Max(InTotalPhases, 1);
	CurrentPhase = 1;
	CurrentHealthPercent = 1.f;

	// Build evenly-spaced phase thresholds if none were set externally.
	// E.g. 3 phases â†’ thresholds at 0.667 and 0.333
	PhaseHealthThresholds.Reset();
	for (int32 i = 1; i < TotalPhases; ++i)
	{
		PhaseHealthThresholds.Add(1.f - (float)i / (float)TotalPhases);
	}
	// Sorted descending so we can pop the front threshold when crossed
	PhaseHealthThresholds.Sort([](float A, float B) { return A > B; });
}

void UWG_BossHealthBar::UpdateHealth(float NewHealthPercent)
{
	CurrentHealthPercent = FMath::Clamp(NewHealthPercent, 0.f, 1.f);
	BP_OnHealthChanged(CurrentHealthPercent);

	// Check if we've crossed into the next phase
	if (PhaseHealthThresholds.Num() > 0 && CurrentHealthPercent <= PhaseHealthThresholds[0])
	{
		PhaseHealthThresholds.RemoveAt(0);
		SetPhase(CurrentPhase + 1);
	}
}

void UWG_BossHealthBar::SetPhase(int32 NewPhase)
{
	CurrentPhase = FMath::Clamp(NewPhase, 1, TotalPhases);

	const FText PhaseText = FText::Format(
		NSLOCTEXT("BossBar", "PhaseLabel", "FASE {0}"),
		FText::AsNumber(CurrentPhase)
	);

	BP_OnPhaseChange(CurrentPhase, PhaseText);
}

void UWG_BossHealthBar::SetVisible(bool bVisible, float FadeDuration)
{
	BP_PlayFade(bVisible, FadeDuration);
	// Collapse after fade completes â€” Blueprint calls SetVisibility(Collapsed) at animation end
}
