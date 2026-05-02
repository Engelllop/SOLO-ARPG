#include "Character/N4/ASOLON4_CorazonHambre.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

ASOLON4_CorazonHambre::ASOLON4_CorazonHambre()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON4_CorazonHambre::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ASOLON4_CorazonHambre::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    HeartbeatTimer += DeltaTime;
    if (HeartbeatTimer >= HeartbeatInterval)
    {
        HeartbeatTimer = 0.0f;

        TArray<FOverlapResult> Overlaps;
        FCollisionShape Sphere = FCollisionShape::MakeSphere(HeartbeatRadius);
        GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere);

        for (const FOverlapResult& Hit : Overlaps)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor != this && HitActor->IsA(APawn::StaticClass()))
            {
                FHitResult HitResult;
                FPointDamageEvent DmgEvent(HeartbeatDamage, HitResult, FVector::UpVector, nullptr);
                HitActor->TakeDamage(HeartbeatDamage, DmgEvent, nullptr, this);
            }
        }

        OnHeartbeat();
    }
}

void ASOLON4_CorazonHambre::RegisterChildDestroyed(AActor* Child)
{
    FetusChildren.Remove(Child);
    OnChildDestroyed(Child);

    if (FetusChildren.Num() <= MaxChildren - 3)
    {
        HitsToDefeat = 5;
    }
}

float ASOLON4_CorazonHambre::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    CurrentHits++;
    if (CurrentHits >= HitsToDefeat)
    {
        OnDefeated();
        Destroy();
    }
    return DamageAmount;
}
