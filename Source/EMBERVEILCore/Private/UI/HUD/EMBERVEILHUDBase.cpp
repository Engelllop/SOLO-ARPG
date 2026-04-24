#include "UI/HUD/EMBERVEILHUDBase.h"
#include "UI/HUD/EMBERVEILHUDOverlayWidget.h"
#include "UI/Widgets/WG_MainHUD.h"
#include "UI/Widgets/WG_BossHealthBar.h"
#include "UI/Widgets/WG_DamageNumber.h"
#include "UI/Widgets/WG_ComboCounter.h"
#include "UI/Widgets/WG_Minimap.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Quest/EMBERVEILQuestComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/LogMacros.h"

AEMBERVEILHUDBase::AEMBERVEILHUDBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> OverlayFinder(
		TEXT("/Game/UI/HUD/WBP_HUD_Overlay.WBP_HUD_Overlay_C"));
	if (OverlayFinder.Succeeded())
	{
		OverlayHUDWidgetClass = OverlayFinder.Class;
	}
}

void AEMBERVEILHUDBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();

	if (OverlayHUDWidgetClass && PC)
	{
		OverlayHUDWidget = CreateWidget<UUserWidget>(PC, OverlayHUDWidgetClass);
		if (OverlayHUDWidget)
		{
			OverlayHUDWidget->AddToViewport(100);
			if (UWidget* FoundCombo = OverlayHUDWidget->GetWidgetFromName(TEXT("WG_ComboCounter")))
			{
				ComboCounterWidget = Cast<UWG_ComboCounter>(FoundCombo);
			}
		}
	}

	if (!OverlayHUDWidget)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("EMBERVEILHUDBase: OverlayHUDWidget class not set. Assign WBP_HUD_Overlay in BP_HUD."));
	}

	if (MainHUDWidgetClass && PC)
	{
		MainHUDWidget = CreateWidget<UWG_MainHUD>(PC, MainHUDWidgetClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport(0);
		}
	}

	// Pre-warm the pool so damage numbers never allocate at hit time
	if (DamageNumberWidgetClass && PC)
	{
		DamageNumberPool.Reserve(DamageNumberPoolSize);
		for (int32 i = 0; i < DamageNumberPoolSize; ++i)
		{
			UWG_DamageNumber* Widget = CreateWidget<UWG_DamageNumber>(PC, DamageNumberWidgetClass);
			if (Widget)
			{
				Widget->AddToViewport();
				Widget->SetVisibility(ESlateVisibility::Collapsed);
				DamageNumberPool.Add(Widget);
			}
		}
	}

	BindOverlayReactiveDelegates();
}

void AEMBERVEILHUDBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HideBossHealthBar();
	UnbindOverlayReactiveDelegates();
	Super::EndPlay(EndPlayReason);
}

void AEMBERVEILHUDBase::BindOverlayReactiveDelegates()
{
	APlayerController* PC = GetOwningPlayerController();
	UEMBERVEILHUDOverlayWidget* Overlay = Cast<UEMBERVEILHUDOverlayWidget>(OverlayHUDWidget);
	if (!PC || !Overlay)
	{
		return;
	}

	AEMBERVEILPlayerState* PS = PC->GetPlayerState<AEMBERVEILPlayerState>();
	if (!PS)
	{
		return;
	}

	UnbindOverlayReactiveDelegates();

	if (UEMBERVEILInventoryComponent* Inv = PS->FindComponentByClass<UEMBERVEILInventoryComponent>())
	{
		Inv->OnCurrencyChanged.AddDynamic(Overlay, &UEMBERVEILHUDOverlayWidget::OnCurrencyChanged);
		OverlayReactiveInventory = Inv;
	}

	if (UEMBERVEILQuestComponent* Quest = PS->FindComponentByClass<UEMBERVEILQuestComponent>())
	{
		Quest->OnQuestObjectiveUpdated.AddDynamic(Overlay, &UEMBERVEILHUDOverlayWidget::OnQuestObjectiveProgressUpdated);
		OverlayReactiveQuest = Quest;
		Overlay->RefreshQuestTracker(Quest);
	}
}

void AEMBERVEILHUDBase::UnbindOverlayReactiveDelegates()
{
	UEMBERVEILHUDOverlayWidget* Overlay = Cast<UEMBERVEILHUDOverlayWidget>(OverlayHUDWidget);
	if (OverlayReactiveInventory && Overlay)
	{
		OverlayReactiveInventory->OnCurrencyChanged.RemoveDynamic(Overlay, &UEMBERVEILHUDOverlayWidget::OnCurrencyChanged);
	}
	if (OverlayReactiveQuest && Overlay)
	{
		OverlayReactiveQuest->OnQuestObjectiveUpdated.RemoveDynamic(
			Overlay, &UEMBERVEILHUDOverlayWidget::OnQuestObjectiveProgressUpdated);
	}
	OverlayReactiveInventory = nullptr;
	OverlayReactiveQuest = nullptr;
}

void AEMBERVEILHUDBase::ShowBossHealthBar(AEMBERVEILEnemyCharacter* BossEnemy)
{
	if (!BossEnemy || !BossHealthBarWidgetClass)
	{
		if (BossEnemy && !BossHealthBarWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("EMBERVEILHUDBase::ShowBossHealthBar — Asigna BossHealthBarWidgetClass en el HUD (BP)."));
		}
		return;
	}

	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !PC->IsLocalController())
	{
		return;
	}

	HideBossHealthBar();

	CachedBossEnemy = BossEnemy;
	UAbilitySystemComponent* BossASC = BossEnemy->GetAbilitySystemComponent();
	CachedBossASC = BossASC;
	if (!BossASC)
	{
		CachedBossEnemy = nullptr;
		return;
	}

	BossHealthBarWidget = CreateWidget<UWG_BossHealthBar>(PC, BossHealthBarWidgetClass);
	if (!BossHealthBarWidget)
	{
		CachedBossEnemy = nullptr;
		CachedBossASC = nullptr;
		return;
	}

	BossHealthBarWidget->SetBossReference(BossEnemy);
	BossHealthBarWidget->InitBoss(BossEnemy->GetBossDisplayName(), BossEnemy->GetBossHealthBarPhaseCount());
	BossHealthBarWidget->AddToViewport(50);

	if (UEMBERVEILAttributeSet* AS = BossEnemy->GetAttributeSet())
	{
		const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
		const float Pct = AS->GetHealth() / MaxH;
		BossHealthBarWidget->UpdateHealthPercent(Pct);
	}

	BossASC->GetGameplayAttributeValueChangeDelegate(UEMBERVEILAttributeSet::GetHealthAttribute())
		.AddUObject(this, &AEMBERVEILHUDBase::OnBossEnemyVitalChanged);
	BossASC->GetGameplayAttributeValueChangeDelegate(UEMBERVEILAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &AEMBERVEILHUDBase::OnBossEnemyVitalChanged);

	BossHealthBarWidget->SetVisibility(ESlateVisibility::Visible);
}

void AEMBERVEILHUDBase::HideBossHealthBar()
{
	if (CachedBossASC.IsValid())
	{
		if (UAbilitySystemComponent* ASC = CachedBossASC.Get())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UEMBERVEILAttributeSet::GetHealthAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UEMBERVEILAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
		}
	}
	CachedBossASC = nullptr;
	CachedBossEnemy = nullptr;

	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->RemoveFromParent();
		BossHealthBarWidget = nullptr;
	}
}

void AEMBERVEILHUDBase::OnBossEnemyVitalChanged(const FOnAttributeChangeData& Data)
{
	(void)Data;
	if (!CachedBossEnemy.IsValid() || !BossHealthBarWidget)
	{
		return;
	}

	UEMBERVEILAttributeSet* AS = CachedBossEnemy->GetAttributeSet();
	if (!AS)
	{
		return;
	}

	const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
	const float Pct = AS->GetHealth() / MaxH;
	BossHealthBarWidget->UpdateHealthPercent(Pct);
}

void AEMBERVEILHUDBase::SpawnDamageNumber(FVector WorldLocation, float DamageAmount, EDamageNumberType Type, bool bIsCritical)
{
	ShowDamageNumber(DamageAmount, WorldLocation, Type, bIsCritical);
}

void AEMBERVEILHUDBase::ShowDamageNumber(float DamageAmount, FVector WorldLocation, EDamageNumberType Type, bool bIsCritical)
{
	APlayerController* PC = GetOwningPlayerController();
	UWG_DamageNumber* Widget = GetPooledDamageNumber();
	if (!Widget || !PC)
	{
		return;
	}

	FVector2D ScreenPos;
	if (PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPos, true))
	{
		Widget->SetPositionInViewport(ScreenPos, false);
	}

	const float XOffset = NextDamageNumberXOffset;
	NextDamageNumberXOffset = FMath::Fmod(NextDamageNumberXOffset + 20.f, 60.f) - 30.f;

	static constexpr float DamageNumberLifetime = 1.5f;
	Widget->Activate(DamageAmount, Type, bIsCritical, WorldLocation, XOffset, DamageNumberLifetime);
}

void AEMBERVEILHUDBase::UpdateComboCounter(int32 CurrentStep, int32 MaxSteps)
{
	if (!ComboCounterWidget)
	{
		return;
	}

	// Reservados para futura sincronización exacta con el widget (p. ej. mostrar "paso X de Y").
	(void)CurrentStep;
	(void)MaxSteps;

	ComboCounterWidget->IncrementCombo();
}

void AEMBERVEILHUDBase::ResetComboCounter()
{
	if (!ComboCounterWidget)
	{
		return;
	}

	ComboCounterWidget->ResetCombo();
}

void AEMBERVEILHUDBase::ToggleMinimap()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ToggleMinimap();
	}
}

void AEMBERVEILHUDBase::ToggleInventory()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (!InventoryWidget)
	{
		if (!InventoryWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("AEMBERVEILHUDBase::ToggleInventory — assign InventoryWidgetClass in BP_HUD"));
			return;
		}
		InventoryWidget = CreateWidget<UUserWidget>(PC, InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport(10);
		}
	}

	if (!InventoryWidget) return;

	const bool bVisible = InventoryWidget->GetVisibility() == ESlateVisibility::Visible;
	InventoryWidget->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	PC->SetShowMouseCursor(!bVisible);
	if (!bVisible)
	{
		PC->SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

UWG_DamageNumber* AEMBERVEILHUDBase::GetPooledDamageNumber()
{
	// First pass: find idle widget
	for (UWG_DamageNumber* Widget : DamageNumberPool)
	{
		if (Widget && !Widget->bIsActive)
		{
			return Widget;
		}
	}

	// All widgets are active — reuse the oldest (index 0, pool is treated as a ring)
	// This prevents frame spikes from allocating new widgets under heavy combat load
	if (DamageNumberPool.Num() > 0 && DamageNumberPool[0])
	{
		DamageNumberPool[0]->Deactivate();
		return DamageNumberPool[0];
	}

	return nullptr;
}
