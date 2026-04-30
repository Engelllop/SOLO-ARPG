#include "AI/SOLONoiseAggroComponent.h"
#include "Character/SOLOEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

USOLONoiseAggroComponent::USOLONoiseAggroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLONoiseAggroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USOLONoiseAggroComponent::StartListening()
{
	if (bListening) return;
	bListening = true;
	GetWorld()->GetTimerManager().SetTimer(CheckTimer, this,
		&USOLONoiseAggroComponent::CheckForNoise, CheckInterval, true);
}

void USOLONoiseAggroComponent::StopListening()
{
	bListening = false;
	GetWorld()->GetTimerManager().ClearTimer(CheckTimer);
}

void USOLONoiseAggroComponent::CheckForNoise()
{
	ASOLOEnemyCharacter* Owner = Cast<ASOLOEnemyCharacter>(GetOwner());
	if (!Owner || Owner->IsAggro()) { StopListening(); return; }

	FVector OwnerLoc = Owner->GetActorLocation();
	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		ACharacter* Pawn = *It;
		if (!Pawn || Pawn == Owner) continue;
		if (FVector::DistSquared(OwnerLoc, Pawn->GetActorLocation()) > FMath::Square(NoiseRadius)) continue;
		if (Pawn->GetVelocity().SizeSquared() >= FMath::Square(MinNoiseSpeed))
		{
			Owner->SetCombatFocus(Pawn);
			StopListening();
			return;
		}
	}
}
