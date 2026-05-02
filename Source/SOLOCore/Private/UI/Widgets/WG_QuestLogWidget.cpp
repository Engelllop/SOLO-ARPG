#include "UI/Widgets/WG_QuestLogWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"

#define LOCTEXT_NAMESPACE "SOLOQuestLog"

void UWG_QuestLogWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ShowHint();

	// Animations start paused; BP subclass handles the actual play rate
	if (ExpandAnimation)
	{
		StopAnimation(ExpandAnimation);
	}
	if (CollapseAnimation)
	{
		StopAnimation(CollapseAnimation);
	}
}

void UWG_QuestLogWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bExpanded = true;

	if (ExpandAnimation)
	{
		PlayAnimationForward(ExpandAnimation);
	}

	if (CollapseAnimation)
	{
		StopAnimation(CollapseAnimation);
	}

	// Mostrar todos los objetivos
	RebuildList();
}

void UWG_QuestLogWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bExpanded = false;

	if (CollapseAnimation)
	{
		PlayAnimationForward(CollapseAnimation);
	}

	if (ExpandAnimation)
	{
		StopAnimation(ExpandAnimation);
	}

	// Volver a estado colapsado (solo tÃ­tulo de misiÃ³n principal)
	RebuildList();
}

void UWG_QuestLogWidget::SetQuestEntries(const TArray<FQuestLogEntry>& Entries)
{
	if (QuestContainer)
	{
		QuestContainer->ClearChildren();
	}

	for (const FQuestLogEntry& Entry : Entries)
	{
		UpdateQuestEntry(Entry);
	}
}

void UWG_QuestLogWidget::UpdateQuestEntry(const FQuestLogEntry& Entry)
{
	if (!QuestContainer) return;

	// Buscar si ya existe un row para esta quest (por nombre)
	UUserWidget* ExistingRow = nullptr;
	for (int32 i = 0; i < QuestContainer->GetChildrenCount(); ++i)
	{
		if (UUserWidget* Child = Cast<UUserWidget>(QuestContainer->GetChildAt(i)))
		{
			if (UTextBlock* NameBlock = Cast<UTextBlock>(Child->GetWidgetFromName(TEXT("QuestNameText"))))
			{
				if (NameBlock->GetText().EqualTo(Entry.QuestName))
				{
					ExistingRow = Child;
					break;
				}
			}
		}
	}

	if (ExistingRow)
	{
		// Actualizar entrada existente
		QuestContainer->RemoveChild(ExistingRow);
		ExistingRow->RemoveFromParent();
	}

	// Crear nuevo row
	UUserWidget* NewRow = CreateQuestRow(Entry, QuestContainer->GetChildrenCount());
	if (NewRow)
	{
		QuestContainer->AddChild(NewRow);
	}
}

void UWG_QuestLogWidget::RebuildList()
{
	// No necesitamos reconstruir aquÃ­ porque los rows manejan su propia visibilidad
	// Este mÃ©todo se llama para refresh si hay cambios de data
}

UUserWidget* UWG_QuestLogWidget::CreateQuestRow(const FQuestLogEntry& Entry, int32 Index)
{
	if (!WidgetTree) return nullptr;

	UUserWidget* Row = CreateDefaultSubobject<UUserWidget>(*FString::Printf(TEXT("QuestRow_%d"), Index));
	if (!Row) return nullptr;

	// Nombre de la misiÃ³n
	UTextBlock* QuestName = NewObject<UTextBlock>(Row, TEXT("QuestNameText"));
	if (QuestName)
	{
		QuestName->SetText(Entry.QuestName);
		QuestName->SetColorAndOpacity(Entry.bIsMainQuest ? MainQuestColor : SideQuestColor);
		FSlateFontInfo Font = QuestName->GetFont();
		Font.Size = 11;
		if (Entry.bIsMainQuest) Font.TypefaceFontName = FName("Bold");
		QuestName->SetFont(Font);
	}

	if (bExpanded)
	{
		// Mostrar objetivos
		for (int32 i = 0; i < Entry.Objectives.Num(); ++i)
		{
			UUserWidget* ObjRow = CreateObjectiveRow(
				Entry.Objectives[i],
				Entry.bObjectivesCompleted.IsValidIndex(i) && Entry.bObjectivesCompleted[i],
				Entry.ObjectiveCurrent.IsValidIndex(i) ? Entry.ObjectiveCurrent[i] : 0,
				Entry.ObjectiveRequired.IsValidIndex(i) ? Entry.ObjectiveRequired[i] : 1
			);
			if (ObjRow)
			{
				// Row layout handled in Blueprint via BindWidget children
			}
		}

		// Hint al final
		UTextBlock* Hint = NewObject<UTextBlock>(Row, TEXT("HintShort"));
		if (Hint)
		{
			Hint->SetText(LOCTEXT("AllQuests", "[M] ver todas las misiones"));
			FSlateFontInfo Font = Hint->GetFont();
			Font.Size = 9;
			Hint->SetFont(Font);
			Hint->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.2f));
		}
	}

	return Row;
}

UUserWidget* UWG_QuestLogWidget::CreateObjectiveRow(const FText& Desc, bool bComplete, int32 Current, int32 Required)
{
	if (!WidgetTree) return nullptr;

	UUserWidget* ObjRow = NewObject<UUserWidget>(this);
	if (!ObjRow) return nullptr;

	UTextBlock* ObjText = NewObject<UTextBlock>(ObjRow);
	if (ObjText)
	{
		const FText Prefix = bComplete
			? LOCTEXT("Complete", "\u2713 ") // checkmark
			: LOCTEXT("Incomplete", "\u25CB "); // circle

		FText FullText;
		if (bComplete)
		{
			FullText = FText::Format(LOCTEXT("ObjCompleteFmt", "{0}{1}"), Prefix, Desc);
		}
		else
		{
			FullText = FText::Format(LOCTEXT("ObjProgressFmt", "{0}{1}  {2}/{3}"),
				Prefix, Desc, FText::AsNumber(Current), FText::AsNumber(Required));
		}

		ObjText->SetText(FullText);
		ObjText->SetColorAndOpacity(bComplete ? CompleteColor : ObjectiveColor);
		FSlateFontInfo Font = ObjText->GetFont();
		Font.Size = 10;
		ObjText->SetFont(Font);
	}

	return ObjRow;
}

void UWG_QuestLogWidget::ShowHint()
{
	if (HintText)
	{
		HintText->SetVisibility(ESlateVisibility::Collapsed);
		HintText->SetText(FText::GetEmpty());
	}
}

#undef LOCTEXT_NAMESPACE
