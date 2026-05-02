#include "World/N6/ASOLON6_ChildSinger.h"
#include "Character/N6/ASOLON6_CoroDeJade.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

AASOLON6_ChildSinger::AASOLON6_ChildSinger()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AASOLON6_ChildSinger::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(0.6f, 0.6f, 0.6f));
}

void AASOLON6_ChildSinger::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsSinging) return;

    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(SingingRadius);
    GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, Sphere);

    for (const FOverlapResult& Hit : Overlaps)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->IsA(APawn::StaticClass()))
        {
            FHitResult HitResult;
            FPointDamageEvent DmgEvent(SingingDPS * DeltaTime, HitResult, FVector::UpVector, nullptr);
            HitActor->TakeDamage(SingingDPS * DeltaTime, DmgEvent, nullptr, this);
        }
    }
}

void AASOLON6_ChildSinger::StopSinging()
{
    bIsSinging = false;
    OnFreed();
}

float AASOLON6_ChildSinger::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    CurrentHits++;
    OnHitByPlayer();

    if (CurrentHits >= MaxHitsToFree)
    {
        StopSinging();
    }

    return 0.0f; // No real damage to child
}
