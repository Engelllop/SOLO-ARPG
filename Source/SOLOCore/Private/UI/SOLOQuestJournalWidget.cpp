#include "UI/SOLOQuestJournalWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void USOLOQuestJournalWidget::RefreshJournal(const TArray<FSOLOQuestEntry>& Quests)
{
	if (!QuestList || !QuestEntryWidgetClass) return;
	QuestList->ClearChildren();

	for (const FSOLOQuestEntry& Quest : Quests)
	{
		UUserWidget* Entry = CreateWidget<UUserWidget>(this, QuestEntryWidgetClass);
		if (Entry) QuestList->AddChild(Entry);
		// Quest data passed to entry widget via Blueprint interface
	}
}

void USOLOQuestJournalWidget::SelectQuest(FName QuestID)
{
	// Description panel populated in Blueprint
}
