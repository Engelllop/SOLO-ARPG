#include "UI/N6/SOLOPTSDWidget.h"

void USOLOPTSDWidget::UpdateTrauma(float CurrentTrauma, float MaxTrauma)
{
    CurrentLevel = CurrentTrauma;
    MaxLevel = MaxTrauma;

    float Ratio = MaxTrauma > 0.0f ? FMath::Clamp(CurrentTrauma / MaxTrauma, 0.0f, 1.0f) : 0.0f;

    if (TraumaText)
    {
        FString Text = FString::Printf(TEXT("Trauma: %.0f%%"), Ratio * 100.0f);
        TraumaText->SetText(FText::FromString(Text));
    }

    if (RedVignette)
    {
        RedVignette->SetOpacity(Ratio * 0.6f);
        RedVignette->SetVisibility(Ratio > 0.0f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    // Threshold events
    if (Ratio >= 0.8f)
    {
        OnTraumaLevel80();
        if (RedVignette)
        {
            RedVignette->SetRenderOpacity(FMath::Sin(GetWorld()->GetTimeSeconds() * 4.0f) * 0.3f + 0.5f);
        }
    }
    else if (Ratio >= 0.6f)
    {
        OnTraumaLevel60();
    }
    else if (Ratio >= 0.4f)
    {
        OnTraumaLevel40();
    }

    if (Ratio <= 0.0f && RedVignette)
    {
        OnTraumaHealed();
        RedVignette->SetVisibility(ESlateVisibility::Hidden);
    }
}
