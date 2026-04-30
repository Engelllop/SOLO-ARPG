#include "UI/SOLODialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"

void USOLODialogueWidget::ShowLine(const FSOLODialogueLine& Line)
{
	if (SpeakerText) SpeakerText->SetText(Line.SpeakerName);
	if (PortraitImage && Line.Portrait) PortraitImage->SetBrushFromTexture(Line.Portrait);

	FullLine = Line.Line.ToString();
	TypeIndex = 0;
	bIsTyping = true;

	if (DialogueText) DialogueText->SetText(FText::GetEmpty());

	GetWorld()->GetTimerManager().SetTimer(TypewriterTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			if (TypeIndex < FullLine.Len())
			{
				if (DialogueText)
					DialogueText->SetText(FText::FromString(FullLine.Left(++TypeIndex)));
			}
			else
			{
				bIsTyping = false;
				GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
			}
		}), 0.03f, true);
}

void USOLODialogueWidget::ShowChoices(const TArray<FSOLODialogueChoice>& Choices)
{
	if (!ChoiceBox || !ChoiceButtonClass) return;
	ClearChoices();

	for (const FSOLODialogueChoice& Choice : Choices)
	{
		UUserWidget* Btn = CreateWidget<UUserWidget>(this, ChoiceButtonClass);
		if (Btn) ChoiceBox->AddChild(Btn);
		// Choice text and NextNodeID set via Blueprint interface on Btn
	}
	ChoiceBox->SetVisibility(ESlateVisibility::Visible);
}

void USOLODialogueWidget::ClearChoices()
{
	if (ChoiceBox)
	{
		ChoiceBox->ClearChildren();
		ChoiceBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USOLODialogueWidget::Advance()
{
	if (bIsTyping)
	{
		// Skip typewriter — show full line immediately
		GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
		bIsTyping = false;
		if (DialogueText) DialogueText->SetText(FText::FromString(FullLine));
	}
	else
	{
		OnDialogueFinished.Broadcast();
	}
}
