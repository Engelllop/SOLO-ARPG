#include "World/N10/ASOLON10_PlayerClone.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AASOLON10_PlayerClone::AASOLON10_PlayerClone()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AASOLON10_PlayerClone::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = MoveSpeed;
    }

    OnSpawn();
}

void AASOLON10_PlayerClone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Age += DeltaTime;
    if (Age >= Lifetime)
    {
        OnExpire();
        Destroy();
    }

    ChasePlayer();
}

void AASOLON10_PlayerClone::ChasePlayer()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->GetPawn())
    {
        AActor* PlayerActor = PC->GetPawn();
        FVector Direction = (PlayerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        AddMovementInput(Direction, 1.0f);
    }
}

float AASOLON10_PlayerClone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Destroy();
    return DamageAmount;
}
