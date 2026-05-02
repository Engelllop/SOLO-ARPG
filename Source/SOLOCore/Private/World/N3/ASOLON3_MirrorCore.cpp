#include "World/N3/ASOLON3_MirrorCore.h"
#include "Character/N3/ASOLON3_LeviathanBoss.h"

ASOLON3_MirrorCore::ASOLON3_MirrorCore()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON3_MirrorCore::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
    SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));
}

void ASOLON3_MirrorCore::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float FloatOffset = FMath::Sin(GetGameTimeSinceCreation() * FloatSpeed) * FloatAmplitude;
    SetActorLocation(StartLocation + FVector(0.0f, 0.0f, FloatOffset));

    AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

float ASOLON3_MirrorCore::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    OnHit();

    if (OwnerLeviathan && OwnerLeviathan->IsValidLowLevel())
    {
        OwnerLeviathan->RegisterCoreHit();
    }

    return DamageAmount;
}
