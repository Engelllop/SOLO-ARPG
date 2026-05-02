#include "Character/N2/ASOLON2_PastorBoss.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Data/N2/SOLON2_Constants.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASOLON2_PastorBoss::ASOLON2_PastorBoss()
{
	bIsBoss = true;
	PhaseThresholds = { 0.60f, 0.30f };
	BossDisplayName = FText::FromString(TEXT("Pastor de Cenizas"));
	XPReward = 200.f;
}

void ASOLON2_PastorBoss::BeginPlay()
{
	Super::BeginPlay();
	// Phase 0 summon loop starts at normal rate
	StartSummonLoop(SummonInterval);
}

void ASOLON2_PastorBoss::OnPhaseChanged_Implementation(int32 NewPhase)
{
	Super::OnPhaseChanged_Implementation(NewPhase);
	switch (NewPhase)
	{
	case 1: EnterPhase1(); break;
	case 2: EnterPhase2(); break;
	default: break;
	}
}

void ASOLON2_PastorBoss::Die_Implementation(AActor* InstigatorActor)
{
	Super::Die_Implementation(InstigatorActor);

	GetWorld()->GetTimerManager().ClearTimer(SummonTimer);

	if (SpawnedNucleus.IsValid())
		SpawnedNucleus->Destroy();

	BP_OnPastorDeath();
}

// ─── Phase 1 — Frenzy ────────────────────────────────────────────────────────

void ASOLON2_PastorBoss::EnterPhase1()
{
	// Apply frenzy GE (speed + attack rate buff)
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (FrenzyGE)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(FrenzyGE, 1, Ctx);
			if (Spec.IsValid())
				ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	// Restart summon timer at double rate
	GetWorld()->GetTimerManager().ClearTimer(SummonTimer);
	StartSummonLoop(FrenzySummonInterval);

	BP_OnFrenzyEnter();
}

// ─── Phase 2 — Exposed Nucleus ────────────────────────────────────────────────

void ASOLON2_PastorBoss::EnterPhase2()
{
	GetWorld()->GetTimerManager().ClearTimer(SummonTimer);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	// Tag the Pastor as nucleus-exposed (damage GEs can filter on this)
	if (ASC && NucleusExposedGE)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(NucleusExposedGE, 1, Ctx);
		if (Spec.IsValid())
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	// If HombreAtrapado was rescued, apply damage vulnerability multiplier
	// (We apply a second stacked instance of NucleusExposedGE at a higher level,
	//  or a dedicated GE — here we re-apply at level RescuedBonusDamageMultiplier
	//  which damage calculations interpret as a multiplier)
	if (ASC)
	{
		// Check story flag via GameInstance or FloorSubsystem
		// For now, expose the multiplier as a SetByCaller on the GE — Blueprint wires this up
		// by checking FLAG_HombreRescatado from the StorySubsystem before BeginPlay ends.
		// The BP subclass overrides BP_OnNucleusExposed and calls SetByCallerMagnitude.
	}

	// Spawn the nucleus actor at the Pastor's chest socket (Blueprint positions it)
	if (NucleusActorClass)
	{
		FTransform T(GetActorLocation() + FVector(0, 0, 80.f));
		if (AActor* Nucleus = GetWorld()->SpawnActor<AActor>(NucleusActorClass, T))
		{
			Nucleus->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			SpawnedNucleus = Nucleus;
			BP_OnNucleusExposed(Nucleus);
		}
	}
}

// ─── Summon helpers ───────────────────────────────────────────────────────────

void ASOLON2_PastorBoss::StartSummonLoop(float Interval)
{
	if (!AshDogClass) return;
	GetWorld()->GetTimerManager().SetTimer(SummonTimer,
		this, &ASOLON2_PastorBoss::SummonAshDog,
		Interval, true);
}

void ASOLON2_PastorBoss::SummonAshDog()
{
	if (!AshDogClass) return;

	const float Angle = FMath::FRandRange(0.f, 360.f);
	const FVector Offset = FVector(
		FMath::Cos(FMath::DegreesToRadians(Angle)) * SummonRadius,
		FMath::Sin(FMath::DegreesToRadians(Angle)) * SummonRadius,
		0.f);

	FTransform T(GetActorLocation() + Offset);
	GetWorld()->SpawnActor<ASOLOEnemyCharacter>(AshDogClass, T);
}
