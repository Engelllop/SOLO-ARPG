#include "UI/SOLOHUDWidget.h"
#include "UI/SOLOVitalBarsWidget.h"
#include "UI/SOLOBossBarWidget.h"

void USOLOHUDWidget::UpdateVitals(float HP, float MaxHP, float MP, float MaxMP, float SP, float MaxSP)
{
	if (VitalBars) VitalBars->UpdateBars(HP, MaxHP, MP, MaxMP, SP, MaxSP);
}

void USOLOHUDWidget::ShowBossBar(bool bVisible)
{
	if (BossBar) BossBar->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USOLOHUDWidget::UpdateBossHP(float HP, float MaxHP)
{
	if (BossBar) BossBar->SetBossHP(HP, MaxHP);
}
