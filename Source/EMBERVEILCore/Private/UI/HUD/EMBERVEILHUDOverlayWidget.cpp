#include "UI/HUD/EMBERVEILHUDOverlayWidget.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Quest/EMBERVEILQuestComponent.h"
#include "Quest/EMBERVEILQuestTypes.h"
#include "AbilitySystemComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Text.h"
#include "TimerManager.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "EMBERVEILHUDOverlay"

void UEMBERVEILHUDOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindRetryCount = 0;
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	BuildLayout();
	TryBindToPlayerState();
}

void UEMBERVEILHUDOverlayWidget::NativeDestruct()
{
	if (CachedInventory)
	{
		CachedInventory->OnCurrencyChanged.RemoveDynamic(this, &UEMBERVEILHUDOverlayWidget::OnCurrencyChanged);
	}
	if (CachedQuestComponent)
	{
		CachedQuestComponent->OnQuestObjectiveUpdated.RemoveDynamic(
			this, &UEMBERVEILHUDOverlayWidget::OnQuestObjectiveProgressUpdated);
	}

	UnbindAll();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(BindRetryTimerHandle);
	}
	Super::NativeDestruct();
}

void UEMBERVEILHUDOverlayWidget::BuildLayout()
{
	if (RootCanvas)
	{
		return;
	}

	UWidgetTree* const Tree = WidgetTree.Get();
	check(Tree);

	RootCanvas = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("HUDOverlayCanvas"));
	Tree->RootWidget = RootCanvas;

	// ── Vitales: arriba izquierda ─────────────────────────────────────────
	UVerticalBox* VitalColumn = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VitalColumn"));
	VitalColumn->SetVisibility(ESlateVisibility::HitTestInvisible);

	auto MakeBar = [&](const TCHAR* Name, const FLinearColor& Fill) -> UProgressBar*
	{
		UProgressBar* Bar = Tree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), Name);
		Bar->SetVisibility(ESlateVisibility::HitTestInvisible);
		Bar->SetPercent(1.f);
		Bar->SetFillColorAndOpacity(Fill);
		VitalColumn->AddChildToVerticalBox(Bar);
		return Bar;
	};

	PB_Health = MakeBar(TEXT("PB_Health"), FLinearColor(0.82f, 0.12f, 0.1f, 1.f));
	PB_Mana = MakeBar(TEXT("PB_Mana"), FLinearColor(0.18f, 0.42f, 0.95f, 1.f));
	PB_Stamina = MakeBar(TEXT("PB_Stamina"), FLinearColor(0.95f, 0.82f, 0.12f, 1.f));

	Text_QuestTracker = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Text_QuestTracker"));
	Text_QuestTracker->SetVisibility(ESlateVisibility::HitTestInvisible);
	Text_QuestTracker->SetText(FText::GetEmpty());
	{
		FSlateFontInfo Font = Text_QuestTracker->GetFont();
		Font.Size = 16;
		Text_QuestTracker->SetFont(Font);
	}
	VitalColumn->AddChildToVerticalBox(Text_QuestTracker);

	if (UCanvasPanelSlot* VitSlot = RootCanvas->AddChildToCanvas(VitalColumn))
	{
		VitSlot->SetAnchors(FAnchors(0.f, 0.f, 0.f, 0.f));
		VitSlot->SetAlignment(FVector2D(0.f, 0.f));
		VitSlot->SetOffsets(FMargin(24.f, 24.f, 0.f, 0.f));
		VitSlot->SetAutoSize(true);
	}

	// ── Nivel: esquina superior derecha ─────────────────────────────────────
	TB_Level = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TB_Level"));
	TB_Level->SetVisibility(ESlateVisibility::HitTestInvisible);
	TB_Level->SetJustification(ETextJustify::Right);
	TB_Level->SetText(LOCTEXT("LevelDefault", "Nv. 1"));
	{
		FSlateFontInfo Font = TB_Level->GetFont();
		Font.Size = 22;
		TB_Level->SetFont(Font);
	}

	if (UCanvasPanelSlot* LvSlot = RootCanvas->AddChildToCanvas(TB_Level))
	{
		LvSlot->SetAnchors(FAnchors(1.f, 0.f, 1.f, 0.f));
		LvSlot->SetAlignment(FVector2D(1.f, 0.f));
		LvSlot->SetOffsets(FMargin(0.f, 24.f, 24.f, 0.f));
		LvSlot->SetAutoSize(true);
	}

	// ── Monedas: abajo derecha ──────────────────────────────────────────────
	TB_Gold = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TB_Gold"));
	TB_Gold->SetVisibility(ESlateVisibility::HitTestInvisible);
	TB_Gold->SetJustification(ETextJustify::Right);
	TB_Gold->SetText(LOCTEXT("CoinsDefault", "0 monedas"));
	{
		FSlateFontInfo Font = TB_Gold->GetFont();
		Font.Size = 20;
		TB_Gold->SetFont(Font);
	}

	if (UCanvasPanelSlot* CoinSlot = RootCanvas->AddChildToCanvas(TB_Gold))
	{
		CoinSlot->SetAnchors(FAnchors(1.f, 1.f, 1.f, 1.f));
		CoinSlot->SetAlignment(FVector2D(1.f, 1.f));
		CoinSlot->SetOffsets(FMargin(0.f, 0.f, 32.f, 28.f));
		CoinSlot->SetAutoSize(true);
	}
}

void UEMBERVEILHUDOverlayWidget::ScheduleBindRetry()
{
	if (BindRetryCount >= 60)
	{
		return;
	}
	++BindRetryCount;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			BindRetryTimerHandle,
			this,
			&UEMBERVEILHUDOverlayWidget::TryBindToPlayerState,
			0.1f,
			false);
	}
}

void UEMBERVEILHUDOverlayWidget::TryBindToPlayerState()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		ScheduleBindRetry();
		return;
	}

	AEMBERVEILPlayerState* PS = PC->GetPlayerState<AEMBERVEILPlayerState>();
	if (!PS)
	{
		ScheduleBindRetry();
		return;
	}

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	const UEMBERVEILAttributeSet* AS = PS->GetAttributeSet();
	if (!ASC || !AS)
	{
		ScheduleBindRetry();
		return;
	}

	UnbindAll();

	CachedPlayerState = PS;
	CachedASC = ASC;
	CachedInventory = PS->FindComponentByClass<UEMBERVEILInventoryComponent>();
	CachedQuestComponent = PS->FindComponentByClass<UEMBERVEILQuestComponent>();

	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnHealthChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnMaxHealthChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnManaChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnMaxManaChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnStaminaChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxStaminaAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnMaxStaminaChanged);
	CachedASC->GetGameplayAttributeValueChangeDelegate(AS->GetLevelAttribute())
		.AddUObject(this, &UEMBERVEILHUDOverlayWidget::OnLevelChanged);

	bDelegatesBound = true;

	CachedMaxHealth = FMath::Max(AS->GetMaxHealth(), 1.f);
	CachedMaxMana = FMath::Max(AS->GetMaxMana(), 1.f);
	CachedMaxStamina = FMath::Max(AS->GetMaxStamina(), 1.f);

	UpdateBars();
	RefreshLevelText();
	RefreshCurrencyText();
}

void UEMBERVEILHUDOverlayWidget::UnbindAll()
{
	if (!bDelegatesBound)
	{
		return;
	}

	const UEMBERVEILAttributeSet* AttrSet = CachedPlayerState ? CachedPlayerState->GetAttributeSet() : nullptr;

	if (CachedASC && AttrSet)
	{
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetHealthAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetMaxHealthAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetManaAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetMaxManaAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetStaminaAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetMaxStaminaAttribute()).RemoveAll(this);
		CachedASC->GetGameplayAttributeValueChangeDelegate(AttrSet->GetLevelAttribute()).RemoveAll(this);
	}

	CachedPlayerState = nullptr;
	CachedASC = nullptr;
	CachedInventory = nullptr;
	CachedQuestComponent = nullptr;
	bDelegatesBound = false;
}

void UEMBERVEILHUDOverlayWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxHealth = FMath::Max(Data.NewValue, 1.f);
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnManaChanged(const FOnAttributeChangeData& Data)
{
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxMana = FMath::Max(Data.NewValue, 1.f);
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	CachedMaxStamina = FMath::Max(Data.NewValue, 1.f);
	UpdateBars();
}

void UEMBERVEILHUDOverlayWidget::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	RefreshLevelText();
}

void UEMBERVEILHUDOverlayWidget::OnCurrencyChanged(int32 NewAmount)
{
	if (TB_Gold)
	{
		TB_Gold->SetText(FText::Format(LOCTEXT("CoinsFmt", "{0} monedas"), FText::AsNumber(NewAmount)));
	}
}

void UEMBERVEILHUDOverlayWidget::OnQuestObjectiveProgressUpdated(FName /*QuestID*/, FName /*ObjectiveID*/, int32 /*NewCount*/)
{
	UEMBERVEILQuestComponent* QuestComp = CachedQuestComponent.Get();
	if (!QuestComp && GetOwningPlayer())
	{
		if (AEMBERVEILPlayerState* PS = GetOwningPlayer()->GetPlayerState<AEMBERVEILPlayerState>())
		{
			QuestComp = PS->FindComponentByClass<UEMBERVEILQuestComponent>();
		}
	}
	RefreshQuestTracker(QuestComp);
}

void UEMBERVEILHUDOverlayWidget::RefreshQuestTracker(UEMBERVEILQuestComponent* QuestComp)
{
	if (!Text_QuestTracker)
	{
		return;
	}

	if (!QuestComp)
	{
		Text_QuestTracker->SetVisibility(ESlateVisibility::Collapsed);
		Text_QuestTracker->SetText(FText::GetEmpty());
		return;
	}

	const TArray<FQuestState> MainQuests = QuestComp->GetActiveMainQuests();
	if (MainQuests.Num() == 0)
	{
		Text_QuestTracker->SetVisibility(ESlateVisibility::Collapsed);
		Text_QuestTracker->SetText(FText::GetEmpty());
		return;
	}

	Text_QuestTracker->SetVisibility(ESlateVisibility::HitTestInvisible);

	const FQuestState& State = MainQuests[0];
	for (const FQuestObjective& Obj : State.Objectives)
	{
		if (!Obj.bIsCompleted && Obj.CurrentCount < Obj.RequiredCount)
		{
			Text_QuestTracker->SetText(FText::Format(
				LOCTEXT("QuestTrackerFmt", "{0} ({1}/{2})"),
				Obj.Description,
				FText::AsNumber(Obj.CurrentCount),
				FText::AsNumber(Obj.RequiredCount)));
			return;
		}
	}

	Text_QuestTracker->SetText(LOCTEXT("QuestTrackerDeliver", "Entrega la misión"));
}

void UEMBERVEILHUDOverlayWidget::UpdateBars()
{
	const UEMBERVEILAttributeSet* AS = CachedPlayerState ? CachedPlayerState->GetAttributeSet() : nullptr;
	if (!AS)
	{
		return;
	}

	if (PB_Health)
	{
		PB_Health->SetPercent(AS->GetHealth() / CachedMaxHealth);
	}
	if (PB_Mana)
	{
		PB_Mana->SetPercent(AS->GetMana() / CachedMaxMana);
	}
	if (PB_Stamina)
	{
		PB_Stamina->SetPercent(AS->GetStamina() / CachedMaxStamina);
	}
}

void UEMBERVEILHUDOverlayWidget::RefreshLevelText()
{
	const UEMBERVEILAttributeSet* AS = CachedPlayerState ? CachedPlayerState->GetAttributeSet() : nullptr;
	if (!TB_Level || !AS)
	{
		return;
	}

	const int32 Lvl = FMath::Max(1, FMath::FloorToInt(AS->GetLevel()));
	TB_Level->SetText(FText::Format(LOCTEXT("LevelFmt", "Nv. {0}"), FText::AsNumber(Lvl)));
}

void UEMBERVEILHUDOverlayWidget::RefreshCurrencyText()
{
	if (!TB_Gold)
	{
		return;
	}

	int32 Coins = 0;
	if (CachedInventory)
	{
		Coins = CachedInventory->Currency;
	}
	TB_Gold->SetText(FText::Format(LOCTEXT("CoinsFmt2", "{0} monedas"), FText::AsNumber(Coins)));
}

#undef LOCTEXT_NAMESPACE
