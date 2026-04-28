#include "UI/Widgets/WG_BattleLogWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

#define LOCTEXT_NAMESPACE "EMBERVEILBattleLog"

void UWG_BattleLogWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (HintText)
	{
		HintText->SetText(LOCTEXT("HintCollapsed", "[Tab] ver log completo"));
		FSlateFontInfo Font = HintText->GetFont();
		Font.Size = 9;
		HintText->SetFont(Font);
		HintText->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.18f));
	}
}

void UWG_BattleLogWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bExpanded = true;
	RebuildVisible();

	if (HintText)
	{
		HintText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWG_BattleLogWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bExpanded = false;
	RebuildVisible();

	if (HintText)
	{
		HintText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UWG_BattleLogWidget::AddEntry(const FBattleLogEntry& Entry)
{
	Entries.Add(Entry);

	if (Entries.Num() > MaxEntries)
	{
		Entries.RemoveAt(0, Entries.Num() - MaxEntries);
	}

	RebuildVisible();
}

void UWG_BattleLogWidget::ClearLog()
{
	Entries.Empty();
	if (LogContainer)
	{
		LogContainer->ClearChildren();
	}
}

void UWG_BattleLogWidget::RebuildVisible()
{
	if (!LogContainer) return;

	LogContainer->ClearChildren();

	if (Entries.Num() == 0) return;

	int32 StartIndex = bExpanded ? 0 : FMath::Max(0, Entries.Num() - CollapsedVisibleCount);
	int32 Count = bExpanded ? Entries.Num() : FMath::Min(CollapsedVisibleCount, Entries.Num());

	for (int32 i = StartIndex; i < StartIndex + Count && i < Entries.Num(); ++i)
	{
		const FBattleLogEntry& Entry = Entries[i];

		UTextBlock* LogLine = NewObject<UTextBlock>(this);
		if (!LogLine) continue;

		LogLine->SetText(Entry.Message);
		LogLine->SetColorAndOpacity(GetColorForType(Entry.Type));

		FSlateFontInfo Font = LogLine->GetFont();
		Font.Size = 10;
		LogLine->SetFont(Font);

		float Opacity = bExpanded ? 1.f : GetOpacityForIndex(i);
		LogLine->SetRenderOpacity(Opacity);

		LogContainer->AddChild(LogLine);
	}

	// Hint si colapsado y hay más entradas
	if (!bExpanded && HintText && Entries.Num() > CollapsedVisibleCount)
	{
		HintText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

FLinearColor UWG_BattleLogWidget::GetColorForType(EBattleLogEntryType Type) const
{
	switch (Type)
	{
	case EBattleLogEntryType::Damage: return DamageColor;
	case EBattleLogEntryType::Heal:   return HealColor;
	case EBattleLogEntryType::Buff:   return BuffColor;
	case EBattleLogEntryType::System: return SystemColor;
	case EBattleLogEntryType::Miss:   return MissColor;
	default: return FLinearColor::White;
	}
}

float UWG_BattleLogWidget::GetOpacityForIndex(int32 Index) const
{
	// Las entradas más recientes tienen más opacidad
	int32 DistanceFromEnd = (Entries.Num() - 1) - Index;
	if (DistanceFromEnd == 0) return 0.7f;
	if (DistanceFromEnd == 1) return 0.35f;
	return 0.2f;
}

#undef LOCTEXT_NAMESPACE
