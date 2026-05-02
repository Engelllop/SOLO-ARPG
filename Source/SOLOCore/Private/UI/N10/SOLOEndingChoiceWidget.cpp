#include "UI/N10/SOLOEndingChoiceWidget.h"

void USOLOEndingChoiceWidget::Show(bool bCanExit)
{
    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(true);

    if (ExitButton)
    {
        ExitButton->SetIsEnabled(bCanExit);
        ExitButton->SetVisibility(bCanExit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }

    if (ExitDescription)
    {
        ExitDescription->SetVisibility(bCanExit ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void USOLOEndingChoiceWidget::OnDestroyClicked()
{
    OnEndingChosen.Broadcast(EEndingChoice::Destroy);
    OnChoiceMade(EEndingChoice::Destroy);
}

void USOLOEndingChoiceWidget::OnControlClicked()
{
    OnEndingChosen.Broadcast(EEndingChoice::Control);
    OnChoiceMade(EEndingChoice::Control);
}

void USOLOEndingChoiceWidget::OnExitClicked()
{
    OnEndingChosen.Broadcast(EEndingChoice::Exit);
    OnChoiceMade(EEndingChoice::Exit);
}
