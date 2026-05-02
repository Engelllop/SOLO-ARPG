#include "Character/N3/ASOLON3_LeviathanReflejo.h"
#include "Character/N3/ASOLON3_LeviathanBoss.h"
#include "GameFramework/CharacterMovementComponent.h"

ASOLON3_LeviathanReflejo::ASOLON3_LeviathanReflejo()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON3_LeviathanReflejo::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = 300.0f;
    }
}

void ASOLON3_LeviathanReflejo::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Age += DeltaTime;
    if (Age >= Lifetime)
    {
        OnExpire();
        Destroy();
    }

    if (OwnerLeviathan && OwnerLeviathan->IsValidLowLevel())
    {
        FVector TargetLoc = OwnerLeviathan->GetActorLocation() + FVector(300.0f, FMath::Sin(GetGameTimeSinceCreation()) * 200.0f, 0.0f);
        SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, 2.0f));
    }
}

float ASOLON3_LeviathanReflejo::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = DamageAmount * DamageMultiplier;
    return Super::TakeDamage(FinalDamage, DamageEvent, EventInstigator, DamageCauser);
}
