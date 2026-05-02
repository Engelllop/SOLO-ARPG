#include "World/N3/ASOLON3_MirrorShard.h"
#include "Character/N3/ASOLON3_LeviathanBoss.h"

ASOLON3_MirrorShard::ASOLON3_MirrorShard()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON3_MirrorShard::BeginPlay()
{
    Super::BeginPlay();
    OrbitAngle = FMath::FRandRange(0.0f, 360.0f);
}

void ASOLON3_MirrorShard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

    if (OwnerLeviathan && OwnerLeviathan->IsValidLowLevel())
    {
        OrbitAngle += OrbitSpeed * DeltaTime;
        float Rad = FMath::DegreesToRadians(OrbitAngle);
        FVector Offset = FVector(FMath::Cos(Rad) * OrbitRadius, FMath::Sin(Rad) * OrbitRadius, FMath::Sin(Rad * 2.0f) * 100.0f);
        SetActorLocation(OwnerLeviathan->GetActorLocation() + Offset);
    }
}

float ASOLON3_MirrorShard::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    CurrentHits++;
    OnHit();

    if (CurrentHits >= MaxHits)
    {
        OnDestroyedShard();
        if (OwnerLeviathan && OwnerLeviathan->IsValidLowLevel())
        {
            OwnerLeviathan->OnShardDestroyed(this);
        }
        Destroy();
    }

    return DamageAmount;
}
