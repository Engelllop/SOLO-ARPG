#include "UI/HUD/SOLOHUDBase.h"
#include "UI/HUD/SOLOHUDOverlayWidget.h"
#include "UI/Widgets/WG_MainHUD.h"
#include "UI/Widgets/WG_BossHealthBar.h"
#include "UI/Widgets/WG_DamageNumber.h"
#include "UI/Widgets/WG_ComboCounter.h"
#include "UI/Widgets/WG_Minimap.h"
#include "Character/SOLOEnemyCharacter.h"
#include "Attributes/SOLOAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "Player/SOLOPlayerState.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Quest/SOLOQuestComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Logging/LogMacros.h"

ASOLOHUDBase::ASOLOHUDBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> OverlayFinder(
		TEXT("/Game/UI/HUD/WBP_HUD_Overlay.WBP_HUD_Overlay_C"));
	if (OverlayFinder.Succeeded())
	{
		OverlayHUDWidgetClass = OverlayFinder.Class;
	}
}

void ASOLOHUDBase::BeginPlay()
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
			TEXT("SOLOHUDBase: OverlayHUDWidget class not set. Assign WBP_HUD_Overlay in BP_HUD."));
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

void ASOLOHUDBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HideBossHealthBar();
	UnbindOverlayReactiveDelegates();
	Super::EndPlay(EndPlayReason);
}

void ASOLOHUDBase::BindOverlayReactiveDelegates()
{
	APlayerController* PC = GetOwningPlayerController();
	USOLOHUDOverlayWidget* Overlay = Cast<USOLOHUDOverlayWidget>(OverlayHUDWidget);
	if (!PC || !Overlay)
	{
		return;
	}

	ASOLOPlayerState* PS = PC->GetPlayerState<ASOLOPlayerState>();
	if (!PS)
	{
		return;
	}

	UnbindOverlayReactiveDelegates();

	if (USOLOInventoryComponent* Inv = PS->FindComponentByClass<USOLOInventoryComponent>())
	{
		Inv->OnCurrencyChanged.AddDynamic(Overlay, &USOLOHUDOverlayWidget::OnCurrencyChanged);
		OverlayReactiveInventory = Inv;
	}

	if (USOLOQuestComponent* Quest = PS->FindComponentByClass<USOLOQuestComponent>())
	{
		Quest->OnQuestObjectiveUpdated.AddDynamic(Overlay, &USOLOHUDOverlayWidget::OnQuestObjectiveProgressUpdated);
		OverlayReactiveQuest = Quest;
		Overlay->RefreshQuestTracker(Quest);
	}
}

void ASOLOHUDBase::UnbindOverlayReactiveDelegates()
{
	USOLOHUDOverlayWidget* Overlay = Cast<USOLOHUDOverlayWidget>(OverlayHUDWidget);
	if (OverlayReactiveInventory && Overlay)
	{
		OverlayReactiveInventory->OnCurrencyChanged.RemoveDynamic(Overlay, &USOLOHUDOverlayWidget::OnCurrencyChanged);
	}
	if (OverlayReactiveQuest && Overlay)
	{
		OverlayReactiveQuest->OnQuestObjectiveUpdated.RemoveDynamic(
			Overlay, &USOLOHUDOverlayWidget::OnQuestObjectiveProgressUpdated);
	}
	OverlayReactiveInventory = nullptr;
	OverlayReactiveQuest = nullptr;
}

void ASOLOHUDBase::ShowBossHealthBar(ASOLOEnemyCharacter* BossEnemy)
{
	if (!BossEnemy || !BossHealthBarWidgetClass)
	{
		if (BossEnemy && !BossHealthBarWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("SOLOHUDBase::ShowBossHealthBar â€” Asigna BossHealthBarWidgetClass en el HUD (BP)."));
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

	if (USOLOAttributeSet* AS = BossEnemy->GetAttributeSet())
	{
		const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
		const float Pct = AS->GetHealth() / MaxH;
		BossHealthBarWidget->UpdateHealthPercent(Pct);
	}

	BossASC->GetGameplayAttributeValueChangeDelegate(USOLOAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ASOLOHUDBase::OnBossEnemyVitalChanged);
	BossASC->GetGameplayAttributeValueChangeDelegate(USOLOAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &ASOLOHUDBase::OnBossEnemyVitalChanged);

	BossHealthBarWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASOLOHUDBase::HideBossHealthBar()
{
	if (CachedBossASC.IsValid())
	{
		if (UAbilitySystemComponent* ASC = CachedBossASC.Get())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(USOLOAttributeSet::GetHealthAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(USOLOAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
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

void ASOLOHUDBase::OnBossEnemyVitalChanged(const FOnAttributeChangeData& Data)
{
	(void)Data;
	if (!CachedBossEnemy.IsValid() || !BossHealthBarWidget)
	{
		return;
	}

	USOLOAttributeSet* AS = CachedBossEnemy->GetAttributeSet();
	if (!AS)
	{
		return;
	}

	const float MaxH = FMath::Max(AS->GetMaxHealth(), 1.f);
	const float Pct = AS->GetHealth() / MaxH;
	BossHealthBarWidget->UpdateHealthPercent(Pct);
}

void ASOLOHUDBase::SpawnDamageNumber(FVector WorldLocation, float DamageAmount, EDamageNumberType Type, bool bIsCritical)
{
	ShowDamageNumber(DamageAmount, WorldLocation, Type, bIsCritical);
}

void ASOLOHUDBase::ShowDamageNumber(float DamageAmount, FVector WorldLocation, EDamageNumberType Type, bool bIsCritical)
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

void ASOLOHUDBase::UpdateComboCounter(int32 CurrentStep, int32 MaxSteps)
{
	if (!ComboCounterWidget)
	{
		return;
	}

	// Reservados para futura sincronizaciÃ³n exacta con el widget (p. ej. mostrar "paso X de Y").
	(void)CurrentStep;
	(void)MaxSteps;

	ComboCounterWidget->IncrementCombo();
}

void ASOLOHUDBase::ResetComboCounter()
{
	if (!ComboCounterWidget)
	{
		return;
	}

	ComboCounterWidget->ResetCombo();
}

void ASOLOHUDBase::ToggleMinimap()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ToggleMinimap();
	}
}

void ASOLOHUDBase::ToggleInventory()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (!InventoryWidget)
	{
		if (!InventoryWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("ASOLOHUDBase::ToggleInventory â€” assign InventoryWidgetClass in BP_HUD"));
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

UWG_DamageNumber* ASOLOHUDBase::GetPooledDamageNumber()
{
	// First pass: find idle widget
	for (UWG_DamageNumber* Widget : DamageNumberPool)
	{
		if (Widget && !Widget->bIsActive)
		{
			return Widget;
		}
	}

	// All widgets are active â€” reuse the oldest (index 0, pool is treated as a ring)
	// This prevents frame spikes from allocating new widgets under heavy combat load
	if (DamageNumberPool.Num() > 0 && DamageNumberPool[0])
	{
		DamageNumberPool[0]->Deactivate();
		return DamageNumberPool[0];
	}

	return nullptr;
}
