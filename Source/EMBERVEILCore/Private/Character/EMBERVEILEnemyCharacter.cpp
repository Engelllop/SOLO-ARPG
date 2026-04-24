#include "Character/EMBERVEILEnemyCharacter.h"

#include "Loot/EMBERVEILLootComponent.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffects/GE_GrantExperience.h"
#include "AI/EMBERVEILAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "GameplayEffect.h"
#include "Player/EMBERVEILPlayerState.h"
#include "UI/HUD/EMBERVEILHUDBase.h"
#include "Components/WidgetComponent.h"
#include "GameplayEffectTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"

AEMBERVEILEnemyCharacter::AEMBERVEILEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// El ASC vive en el propio actor — creado aquí, no en PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEMBERVEILAttributeSet>(TEXT("AttributeSet"));

	// Widget flotante de HP — oculto por defecto hasta que el jugador se acerque
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(200.f, 20.f));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthBarWidget->SetVisibility(false);

	LootComponent = CreateDefaultSubobject<UEMBERVEILLootComponent>(TEXT("LootComponent"));
}

// ─── BeginPlay ────────────────────────────────────────────────────────────────

void AEMBERVEILEnemyCharacter::BeginPlay()
{
	// El ASC de enemies usa al propio actor tanto como Owner como Avatar
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	Super::BeginPlay();

	ApplyDefaultAttributes();
	GrantDefaultAbilities();

	if (EnemyType == EEnemyType::Boss || EnemyType == EEnemyType::WorldBoss)
	{
		NotifyLocalPlayersShowBossHealthBar();
	}
}

void AEMBERVEILEnemyCharacter::SetCombatFocus(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	bIsAggro      = (NewTarget != nullptr);
}

// ─── ICombatInterface ─────────────────────────────────────────────────────────

int32 AEMBERVEILEnemyCharacter::GetCharacterLevel_Implementation() const
{
	return EnemyLevel;
}

FText AEMBERVEILEnemyCharacter::GetBossDisplayName() const
{
	if (!BossDisplayName.IsEmpty())
	{
		return BossDisplayName;
	}
	return FText::FromString(GetName());
}

int32 AEMBERVEILEnemyCharacter::GetBossHealthBarPhaseCount() const
{
	if (PhaseThresholds.Num() == 0)
	{
		return 1;
	}
	return PhaseThresholds.Num() + 1;
}

void AEMBERVEILEnemyCharacter::NotifyLocalPlayersShowBossHealthBar()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->IsLocalController())
		{
			if (AEMBERVEILHUDBase* HUD = PC->GetHUD<AEMBERVEILHUDBase>())
			{
				HUD->ShowBossHealthBar(this);
			}
		}
	}
}

void AEMBERVEILEnemyCharacter::ScheduleHideBossHealthBarDelayed()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			BossHealthBarHideTimer,
			FTimerDelegate::CreateUObject(this, &AEMBERVEILEnemyCharacter::ExecuteHideBossHealthBarOnLocalHud),
			3.f,
			false
		);
	}
}

void AEMBERVEILEnemyCharacter::ExecuteHideBossHealthBarOnLocalHud()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->IsLocalController())
		{
			if (AEMBERVEILHUDBase* HUD = PC->GetHUD<AEMBERVEILHUDBase>())
			{
				HUD->HideBossHealthBar();
			}
		}
	}
}

void AEMBERVEILEnemyCharacter::RememberKillInstigator(AActor* InstigatorAvatar)
{
	PendingKillInstigator = InstigatorAvatar;
}

void AEMBERVEILEnemyCharacter::Die_Implementation(AActor* InstigatorActor)
{
	const bool bFirstDeath = !bDead;

	Super::Die_Implementation(InstigatorActor);

	if (bFirstDeath)
	{
		OnEnemyDied.Broadcast(this);
	}

	if (bFirstDeath && (EnemyType == EEnemyType::Boss || EnemyType == EEnemyType::WorldBoss))
	{
		ScheduleHideBossHealthBarDelayed();
	}

	if (!HasAuthority() || !bFirstDeath)
	{
		return;
	}

	// Tras marcar muerte en la base, aplicar State.Dead al ASC del enemigo sin re-entrar en Die
	if (AbilitySystemComponent && DeadTag.IsValid() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		FGameplayTagContainer DeadGameplayTags;
		DeadGameplayTags.AddTag(DeadTag);
		AbilitySystemComponent->AddLooseGameplayTags(DeadGameplayTags);
	}

	AActor* Killer = InstigatorActor;
	if (!IsValid(Killer))
	{
		Killer = PendingKillInstigator.Get();
	}
	GrantExperienceToKiller(Killer);
	PendingKillInstigator.Reset();
}

// ─── Healthbar ────────────────────────────────────────────────────────────────

void AEMBERVEILEnemyCharacter::ShowHealthBar()
{
	if (!HealthBarWidget) return;

	GetWorldTimerManager().ClearTimer(HealthBarFadeTimer);
	HealthBarWidget->SetVisibility(true);

	// Interpolamos el alpha del widget a través de un timer de tick fino.
	// La referencia real al widget UMG se obtiene via HealthBarWidget->GetUserWidgetObject()
	// para que los Blueprints apliquen el valor de HealthBarAlpha al fade de su material.
	HealthBarAlpha = 0.f;
	const float FadeDuration  = 0.25f;
	const float TickRate      = 0.016f;   // ~60 fps
	const float AlphaPerTick  = TickRate / FadeDuration;

	GetWorldTimerManager().SetTimer(
		HealthBarFadeTimer,
		FTimerDelegate::CreateLambda([this, AlphaPerTick]()
		{
			HealthBarAlpha = FMath::Min(HealthBarAlpha + AlphaPerTick, 1.f);
			if (HealthBarAlpha >= 1.f)
				GetWorldTimerManager().ClearTimer(HealthBarFadeTimer);
		}),
		TickRate,
		true
	);
}

void AEMBERVEILEnemyCharacter::HideHealthBar()
{
	if (!HealthBarWidget) return;

	GetWorldTimerManager().ClearTimer(HealthBarFadeTimer);

	const float FadeDuration  = 0.25f;
	const float TickRate      = 0.016f;
	const float AlphaPerTick  = TickRate / FadeDuration;

	GetWorldTimerManager().SetTimer(
		HealthBarFadeTimer,
		FTimerDelegate::CreateLambda([this, AlphaPerTick]()
		{
			HealthBarAlpha = FMath::Max(HealthBarAlpha - AlphaPerTick, 0.f);
			if (HealthBarAlpha <= 0.f)
			{
				HealthBarWidget->SetVisibility(false);
				GetWorldTimerManager().ClearTimer(HealthBarFadeTimer);
			}
		}),
		TickRate,
		true
	);
}

// ─── Fases de jefe ────────────────────────────────────────────────────────────

void AEMBERVEILEnemyCharacter::CheckPhaseTransition()
{
	if (!bIsBoss || PhaseThresholds.IsEmpty() || !AttributeSet)
	{
		return;
	}

	const float MaxHP = AttributeSet->GetMaxHealth();
	if (MaxHP <= 0.f)
	{
		return;
	}

	CheckBossPhase(AttributeSet->GetHealth() / MaxHP);
}

void AEMBERVEILEnemyCharacter::CheckBossPhase(float HPPercent)
{
	if (!bIsBoss || PhaseThresholds.Num() == 0 || !AttributeSet || !HasAuthority())
	{
		return;
	}

	TArray<float> SortedThresholds = PhaseThresholds;
	SortedThresholds.Sort([](float A, float B) { return A > B; });

	while (CurrentPhase < SortedThresholds.Num() && HPPercent <= SortedThresholds[CurrentPhase])
	{
		CurrentPhase++;
		OnBossPhaseChanged.Broadcast(CurrentPhase);
		OnPhaseChanged(CurrentPhase);
		ApplyBossPhaseAttackSpeedEffect();
		SpawnBossReinforcements();
		PlayBossPhaseTransitionVFX();
		SyncBossPhaseToBlackboard();
	}
}

void AEMBERVEILEnemyCharacter::ApplyBossPhaseAttackSpeedEffect()
{
	if (!BossPhaseAttackSpeedEffect || !AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle Ctx = AbilitySystemComponent->MakeEffectContext();
	Ctx.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec =
		AbilitySystemComponent->MakeOutgoingSpec(BossPhaseAttackSpeedEffect, EnemyLevel, Ctx);
	if (Spec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void AEMBERVEILEnemyCharacter::SpawnBossReinforcements()
{
	if (!ReinforcementEnemyClass || !GetWorld())
	{
		return;
	}

	UWorld* World = GetWorld();
	const FVector Origin = GetActorLocation();

	for (int32 i = 0; i < 2; ++i)
	{
		const float Angle = FMath::FRand() * 6.2831853f;
		const float R = FMath::Sqrt(FMath::FRand()) * ReinforcementSpawnRadius;
		const FVector Offset(R * FMath::Cos(Angle), R * FMath::Sin(Angle), 100.f);
		const FVector SpawnLoc = Origin + Offset;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		World->SpawnActor<AEMBERVEILEnemyCharacter>(ReinforcementEnemyClass, SpawnLoc, FRotator::ZeroRotator, Params);
	}
}

void AEMBERVEILEnemyCharacter::PlayBossPhaseTransitionVFX()
{
	if (!BossPhaseTransitionVFX)
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		BossPhaseTransitionVFX,
		GetActorLocation() + FVector(0.f, 0.f, 80.f),
		FRotator::ZeroRotator,
		FVector(1.f),
		true,
		true,
		ENCPoolMethod::None,
		true
	);
}

void AEMBERVEILEnemyCharacter::SyncBossPhaseToBlackboard()
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsInt(AEMBERVEILAIController::BB_CurrentPhase, CurrentPhase);
		}
	}
}

void AEMBERVEILEnemyCharacter::OnPhaseChanged_Implementation(int32 NewPhase)
{
	(void)NewPhase;
	// Implementación base vacía — los Blueprints de cada boss concreto
	// sobreescriben esto para cambiar animsets, activar habilidades, etc.
}

// ─── Experiencia ──────────────────────────────────────────────────────────────

void AEMBERVEILEnemyCharacter::GrantExperienceToKiller(AActor* Killer)
{
	if (!IsValid(Killer))
	{
		return;
	}

	AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(Killer);
	if (!PlayerChar)
	{
		if (AController* C = Cast<AController>(Killer))
		{
			PlayerChar = Cast<AEMBERVEILPlayerCharacter>(C->GetPawn());
		}
	}
	if (!PlayerChar)
	{
		return;
	}

	AEMBERVEILPlayerState* PS = PlayerChar->GetPlayerState<AEMBERVEILPlayerState>();
	if (!PS)
	{
		return;
	}

	UAbilitySystemComponent* KillerASC = PS->GetAbilitySystemComponent();
	if (!KillerASC)
	{
		return;
	}

	const TSubclassOf<UGameplayEffect> GEClass = ExperienceGrantEffect
		? ExperienceGrantEffect
		: TSubclassOf<UGameplayEffect>(UEMBERVEIL_GE_GrantExperience::StaticClass());
	if (!GEClass)
	{
		return;
	}

	const float TotalXP = FMath::Max(0.f, XPReward * ExperienceMultiplier);

	FGameplayEffectContextHandle Context = KillerASC->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec =
		KillerASC->MakeOutgoingSpec(GEClass, EnemyLevel, Context);

	if (!Spec.IsValid())
	{
		return;
	}

	Spec.Data->SetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.XPReward")),
		TotalXP
	);

	KillerASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}
