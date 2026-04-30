#include "UI/SOLOBossBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USOLOBossBarWidget::SetBossHP(float HP, float MaxHP)
{
	if (BossHPBar) BossHPBar->SetPercent(MaxHP > 0.f ? HP / MaxHP : 0.f);
}

void USOLOBossBarWidget::SetBossName(const FText& Name)
{
	if (BossNameText) BossNameText->SetText(Name);
}

void USOLOBossBarWidget::SetPhaseIndicator(int32 CurrentPhase, int32 TotalPhases)
{
	// Phase pip visibility handled in Blueprint
}

void USOLOBossBarWidget::PlayPhaseTransitionAnim()
{
	// Animation played in Blueprint
}
