#include "UI/N8/SOLONameInputWidget.h"

void USOLONameInputWidget::Show(const FText& Prompt)
{
    if (PromptText)
    {
        PromptText->SetText(Prompt);
    }

    if (NameInputBox)
    {
        NameInputBox->SetText(FText::GetEmpty());
    }

    SetVisibility(ESlateVisibility::Visible);
    SetIsEnabled(true);

    if (NameInputBox)
    {
        NameInputBox->SetUserFocus(GetOwningPlayer());
    }
}

void USOLONameInputWidget::Hide()
{
    SetVisibility(ESlateVisibility::Collapsed);
    SetIsEnabled(false);
}

void USOLONameInputWidget::OnSubmitClicked()
{
    if (!NameInputBox) return;

    FString Name = NameInputBox->GetText().ToString().TrimStartAndEnd();

    if (Name.IsEmpty())
    {
        Name = TEXT("Criada");
    }

    OnNameSubmitted.Broadcast(Name);
    Hide();
}
