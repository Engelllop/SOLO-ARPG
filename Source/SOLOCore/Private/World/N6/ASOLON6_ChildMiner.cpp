#include "World/N6/ASOLON6_ChildMiner.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AASOLON6_ChildMiner::AASOLON6_ChildMiner()
{
    PrimaryActorTick.bCanEverTick = false;

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCapsuleRadius(22.0f);
        GetCapsuleComponent()->SetCapsuleHalfHeight(44.0f);
    }
}

void AASOLON6_ChildMiner::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(0.6f, 0.6f, 0.6f));

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = 0.0f; // Won't move
    }
}

void AASOLON6_ChildMiner::TrySpeakToPlayer(AActor* PlayerActor)
{
    if (bHasSpoken) return;

    float Dist = FVector::Dist(GetActorLocation(), PlayerActor->GetActorLocation());
    if (Dist <= DialogueRadius)
    {
        bHasSpoken = true;
        OnSpokenTo();
    }
}
