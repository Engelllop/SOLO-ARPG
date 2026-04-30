#include "UI/SOLOVitalBarsWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USOLOVitalBarsWidget::UpdateBars(float HP, float MaxHP, float MP, float MaxMP, float SP, float MaxSP)
{
	if (HPBar) HPBar->SetPercent(MaxHP > 0.f ? HP / MaxHP : 0.f);
	if (MPBar) MPBar->SetPercent(MaxMP > 0.f ? MP / MaxMP : 0.f);
	if (SPBar) SPBar->SetPercent(MaxSP > 0.f ? SP / MaxSP : 0.f);
	if (HPText) HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), HP, MaxHP)));
	if (MPText) MPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), MP, MaxMP)));
}
