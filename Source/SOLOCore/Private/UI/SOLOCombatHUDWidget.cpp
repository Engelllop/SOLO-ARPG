#include "UI/SOLOCombatHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void USOLOCombatHUDWidget::ShowComboCount(int32 Count)
{
	if (ComboCountText)
		ComboCountText->SetText(Count > 1 ? FText::FromString(FString::Printf(TEXT("x%d"), Count)) : FText::GetEmpty());
}

void USOLOCombatHUDWidget::ShowPostureBar(float Posture, float MaxPosture)
{
	if (PostureBar) PostureBar->SetPercent(MaxPosture > 0.f ? Posture / MaxPosture : 1.f);
}

void USOLOCombatHUDWidget::ShowParryIndicator(bool bVisible)
{
	if (ParryIndicator)
		ParryIndicator->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USOLOCombatHUDWidget::ShowDamageNumber(float Damage, FVector WorldPos, bool bCrit)
{
	// Damage numbers spawned as UMG actors; handled in Blueprint using WorldPos projection
}
