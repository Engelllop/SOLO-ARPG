#include "UI/N4/SOLOHungerWidget.h"

void USOLOHungerWidget::UpdateHunger(float CurrentHunger, float MaxHunger)
{
    if (!HungerBar || !HungerText) return;

    float Ratio = MaxHunger > 0.0f ? FMath::Clamp(CurrentHunger / MaxHunger, 0.0f, 1.0f) : 0.0f;
    HungerBar->SetPercent(Ratio);

    FString Text = FString::Printf(TEXT("%.0f / %.0f"), CurrentHunger, MaxHunger);
    HungerText->SetText(FText::FromString(Text));

    if (Ratio <= CriticalThreshold)
    {
        ShowWarning(true);
    }
}

void USOLOHungerWidget::ShowWarning(bool bCritical)
{
    if (!WarningOverlay) return;

    WarningOverlay->SetVisibility(bCritical ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

    if (bCritical)
    {
        OnHungerCritical();
    }
    else
    {
        OnHungerNormalized();
    }
}
