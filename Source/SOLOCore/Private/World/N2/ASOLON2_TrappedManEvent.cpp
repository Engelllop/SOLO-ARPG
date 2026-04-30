#include "World/N2/ASOLON2_TrappedManEvent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Character/SOLOEnemyCharacter.h"
#include "Data/N2/SOLON2_Constants.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASOLON2_TrappedManEvent::ASOLON2_TrappedManEvent()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetBoxExtent(FVector(150.f, 150.f, 100.f));
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(InteractionVolume);
}

void ASOLON2_TrappedManEvent::BeginPlay()
{
	Super::BeginPlay();
}

// ─── Public choice entry ──────────────────────────────────────────────────────

void ASOLON2_TrappedManEvent::MakeChoice(ETrappedManChoice Choice)
{
	if (MadeChoice != ETrappedManChoice::None) return; // Already resolved

	MadeChoice = Choice;
	OnChoiceMade.Broadcast(Choice);

	switch (Choice)
	{
	case ETrappedManChoice::Rescue: StartRescue();   break;
	case ETrappedManChoice::Ignore: ExecuteIgnore(); break;
	case ETrappedManChoice::Kill:   ExecuteKill();   break;
	default: break;
	}
}

float ASOLON2_TrappedManEvent::GetRescueTimeRemaining() const
{
	if (!bRescueActive) return 0.f;
	return FMath::Max(0.f,
		RescueDuration - (GetWorld()->GetTimeSeconds() - RescueStartTime));
}

// ─── Rescue flow ──────────────────────────────────────────────────────────────

void ASOLON2_TrappedManEvent::StartRescue()
{
	bRescueActive   = true;
	CurrentWave     = 0;
	RescueStartTime = GetWorld()->GetTimeSeconds();

	// Fail timer — if player doesn't finish in RescueDuration, rescue fails
	GetWorld()->GetTimerManager().SetTimer(RescueTimer,
		this, &ASOLON2_TrappedManEvent::OnRescueTimerExpired,
		RescueDuration, false);

	BP_OnRescueStarted();
	SpawnDogWave();
}

void ASOLON2_TrappedManEvent::SpawnDogWave()
{
	if (!AshDogClass || CurrentWave >= 3) return;

	AliveDogsCount = DogsPerWave;
	for (int32 i = 0; i < DogsPerWave; i++)
	{
		float Angle = (2.f * PI * i) / DogsPerWave;
		FVector Offset(
			FMath::Cos(Angle) * DogSpawnRadius,
			FMath::Sin(Angle) * DogSpawnRadius,
			0.f);
		FTransform T(GetActorLocation() + Offset);

		if (ASOLOEnemyCharacter* Dog = GetWorld()->SpawnActor<ASOLOEnemyCharacter>(AshDogClass, T))
			Dog->OnEnemyDied.AddDynamic(this, &ASOLON2_TrappedManEvent::OnDogKilled);
	}

	BP_OnWaveSpawned(CurrentWave);
	CurrentWave++;
}

void ASOLON2_TrappedManEvent::OnDogKilled(AActor* /*Dog*/)
{
	AliveDogsCount = FMath::Max(0, AliveDogsCount - 1);
	if (AliveDogsCount > 0) return;

	if (CurrentWave < 3)
	{
		// Brief pause between waves before next spawn
		GetWorld()->GetTimerManager().SetTimer(WaveSpawnTimer,
			FTimerDelegate::CreateWeakLambda(this, [this]() { SpawnDogWave(); }),
			3.f, false);
	}
	else
	{
		// All 3 waves cleared — rescue succeeds
		FinalizeRescue();
	}
}

void ASOLON2_TrappedManEvent::OnRescueTimerExpired()
{
	if (!bRescueActive) return;
	bRescueActive = false;
	GetWorld()->GetTimerManager().ClearTimer(WaveSpawnTimer);

	OnRescueFailed.Broadcast();
	BP_OnRescueFailed();
}

void ASOLON2_TrappedManEvent::FinalizeRescue()
{
	if (!bRescueActive) return;
	bRescueActive = false;

	GetWorld()->GetTimerManager().ClearTimer(RescueTimer);
	GetWorld()->GetTimerManager().ClearTimer(WaveSpawnTimer);

	// Set story flag
	// USOLOStorySubsystem (or equivalent) called via GameInstance
	// (wired in Blueprint subclass via BP_OnRescueCompleted event)

	ApplyReputationDelta(RescueReputationDelta);

	OnRescueCompleted.Broadcast();
	BP_OnRescueCompleted();
}

// ─── Ignore flow ─────────────────────────────────────────────────────────────

void ASOLON2_TrappedManEvent::ExecuteIgnore()
{
	// No reputation change, no story flag. Just the BP cosmetic event.
	BP_OnIgnore();
}

// ─── Kill flow ────────────────────────────────────────────────────────────────

void ASOLON2_TrappedManEvent::ExecuteKill()
{
	// Kill the trapped man via lethal GE (target is set in BP as a UPROPERTY ref)
	// The Blueprint subclass stores a reference to the trapped man actor and
	// calls ApplyGameplayEffectToTarget in BP_OnKill, since the C++ layer
	// doesn't hold a direct reference to the scripted NPC actor.

	ApplyReputationDelta(KillReputationDelta);
	GrantItemToPlayer(RewardItemID);

	BP_OnKill();
	BP_OnMiraHorrorReaction();
}

// ─── Helpers ──────────────────────────────────────────────────────────────────

void ASOLON2_TrappedManEvent::ApplyReputationDelta(int32 Delta)
{
	// Forward to the reputation subsystem through the player's ASC or a dedicated subsystem.
	// Stubbed here; Blueprint override or subsystem call completes this.
	(void)Delta;
}

void ASOLON2_TrappedManEvent::GrantItemToPlayer(FName ItemID)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;

	// USOLOInventoryComponent::AddItem wired through player subsystem;
	// Blueprint subclass casts the pawn and calls AddItem directly.
	(void)ItemID;
}
