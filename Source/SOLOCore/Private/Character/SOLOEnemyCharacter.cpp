#include "Character/SOLOEnemyCharacter.h"

#include "Loot/SOLOLootComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayEffects/GE_GrantExperience.h"
#include "AI/SOLOAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/SOLOPlayerCharacter.h"
#include "GameplayEffect.h"
#include "Player/SOLOPlayerState.h"
#include "UI/HUD/SOLOHUDBase.h"
#include "Components/WidgetComponent.h"
#include "GameplayEffectTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"

ASOLOEnemyCharacter::ASOLOEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// El ASC vive en el propio actor â€” creado aquÃ­, no en PlayerState
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<USOLOAttributeSet>(TEXT("AttributeSet"));

	// Widget flotante de HP â€” oculto por defecto hasta que el jugador se acerque
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(200.f, 20.f));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	HealthBarWidget->SetVisibility(false);

	LootComponent = CreateDefaultSubobject<USOLOLootComponent>(TEXT("LootComponent"));
}

// â”€â”€â”€ BeginPlay â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOEnemyCharacter::BeginPlay()
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

void ASOLOEnemyCharacter::SetCombatFocus(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	bIsAggro      = (NewTarget != nullptr);
}

// â”€â”€â”€ ICombatInterface â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

int32 ASOLOEnemyCharacter::GetCharacterLevel_Implementation() const
{
	return EnemyLevel;
}

FText ASOLOEnemyCharacter::GetBossDisplayName() const
{
	if (!BossDisplayName.IsEmpty())
	{
		return BossDisplayName;
	}
	return FText::FromString(GetName());
}

int32 ASOLOEnemyCharacter::GetBossHealthBarPhaseCount() const
{
	if (PhaseThresholds.Num() == 0)
	{
		return 1;
	}
	return PhaseThresholds.Num() + 1;
}

void ASOLOEnemyCharacter::NotifyLocalPlayersShowBossHealthBar()
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
			if (ASOLOHUDBase* HUD = PC->GetHUD<ASOLOHUDBase>())
			{
				HUD->ShowBossHealthBar(this);
			}
		}
	}
}

void ASOLOEnemyCharacter::ScheduleHideBossHealthBarDelayed()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			BossHealthBarHideTimer,
			FTimerDelegate::CreateUObject(this, &ASOLOEnemyCharacter::ExecuteHideBossHealthBarOnLocalHud),
			3.f,
			false
		);
	}
}

void ASOLOEnemyCharacter::ExecuteHideBossHealthBarOnLocalHud()
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
			if (ASOLOHUDBase* HUD = PC->GetHUD<ASOLOHUDBase>())
			{
				HUD->HideBossHealthBar();
			}
		}
	}
}

void ASOLOEnemyCharacter::RememberKillInstigator(AActor* InstigatorAvatar)
{
	PendingKillInstigator = InstigatorAvatar;
}

void ASOLOEnemyCharacter::Die_Implementation(AActor* InstigatorActor)
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

// â”€â”€â”€ Healthbar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOEnemyCharacter::ShowHealthBar()
{
	if (!HealthBarWidget) return;

	GetWorldTimerManager().ClearTimer(HealthBarFadeTimer);
	HealthBarWidget->SetVisibility(true);

	// Interpolamos el alpha del widget a travÃ©s de un timer de tick fino.
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

void ASOLOEnemyCharacter::HideHealthBar()
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

// â”€â”€â”€ Fases de jefe â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOEnemyCharacter::CheckPhaseTransition()
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

void ASOLOEnemyCharacter::CheckBossPhase(float HPPercent)
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

void ASOLOEnemyCharacter::ApplyBossPhaseAttackSpeedEffect()
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

void ASOLOEnemyCharacter::SpawnBossReinforcements()
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

		World->SpawnActor<ASOLOEnemyCharacter>(ReinforcementEnemyClass, SpawnLoc, FRotator::ZeroRotator, Params);
	}
}

void ASOLOEnemyCharacter::PlayBossPhaseTransitionVFX()
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

void ASOLOEnemyCharacter::SyncBossPhaseToBlackboard()
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
		{
			BB->SetValueAsInt(ASOLOAIController::BB_CurrentPhase, CurrentPhase);
		}
	}
}

void ASOLOEnemyCharacter::OnPhaseChanged_Implementation(int32 NewPhase)
{
	(void)NewPhase;
	// ImplementaciÃ³n base vacÃ­a â€” los Blueprints de cada boss concreto
	// sobreescriben esto para cambiar animsets, activar habilidades, etc.
}

// â”€â”€â”€ Experiencia â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOEnemyCharacter::GrantExperienceToKiller(AActor* Killer)
{
	if (!IsValid(Killer))
	{
		return;
	}

	ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(Killer);
	if (!PlayerChar)
	{
		if (AController* C = Cast<AController>(Killer))
		{
			PlayerChar = Cast<ASOLOPlayerCharacter>(C->GetPawn());
		}
	}
	if (!PlayerChar)
	{
		return;
	}

	ASOLOPlayerState* PS = PlayerChar->GetPlayerState<ASOLOPlayerState>();
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
		: TSubclassOf<UGameplayEffect>(USOLO_GE_GrantExperience::StaticClass());
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
