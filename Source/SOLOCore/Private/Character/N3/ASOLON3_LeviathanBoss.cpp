#include "Character/N3/ASOLON3_LeviathanBoss.h"
#include "Character/N3/ASOLON3_LeviathanReflejo.h"
#include "World/N3/ASOLON3_MirrorShard.h"
#include "World/N3/ASOLON3_MirrorCore.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

ASOLON3_LeviathanBoss::ASOLON3_LeviathanBoss()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON3_LeviathanBoss::BeginPlay()
{
    Super::BeginPlay();
    LocalPhase = 1;
    SetActorScale3D(FVector(4.0f, 4.0f, 4.0f));
}

void ASOLON3_LeviathanBoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (LocalPhase == 1)
    {
        DuplicationTimer += DeltaTime;
        if (DuplicationTimer >= DuplicationCooldown)
        {
            DuplicationTimer = 0.0f;
            SpawnReflejo();
        }
    }
}

void ASOLON3_LeviathanBoss::AdvanceLocalPhase()
{
    LocalPhase++;
    if (LocalPhase > MaxPhase) LocalPhase = MaxPhase;

    if (LocalPhase == 2)
    {
        if (CurrentReflejo)
        {
            CurrentReflejo->Destroy();
            CurrentReflejo = nullptr;
        }
        SpawnMirrorShards();
    }
    else if (LocalPhase == 3)
    {
        for (auto* Shard : ActiveShards)
        {
            if (Shard) Shard->Destroy();
        }
        ActiveShards.Empty();
        SpawnMirrorCore();
    }
}

void ASOLON3_LeviathanBoss::SpawnReflejo()
{
    if (!ReflejoClass) return;
    if (CurrentReflejo && CurrentReflejo->IsValidLowLevel()) return;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();
    FVector SpawnLoc = GetActorLocation() + FVector(300.0f, 0.0f, 0.0f);
    CurrentReflejo = GetWorld()->SpawnActor<ASOLON3_LeviathanReflejo>(ReflejoClass, SpawnLoc, FRotator::ZeroRotator, Params);

    if (CurrentReflejo)
    {
        CurrentReflejo->SetOwnerLeviathan(this);
        CurrentReflejo->SpawnDefaultController();
    }
}

void ASOLON3_LeviathanBoss::OnMirrorDamageRedirect_Implementation(AActor* DamageInstigatorActor, float Damage)
{
    if (LocalPhase != 1) return;

    TArray<FOverlapResult> Overlaps;
    FCollisionShape QuerySphere = FCollisionShape::MakeSphere(800.0f);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    GetWorld()->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECC_Pawn, QuerySphere, QueryParams);

    for (const FOverlapResult& Hit : Overlaps)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor != this && HitActor->IsA(APawn::StaticClass()))
        {
            FHitResult HitResult;
            FPointDamageEvent DmgEvent(Damage * 0.5f, HitResult, -FVector::UpVector, nullptr);
            HitActor->TakeDamage(Damage * 0.5f, DmgEvent, GetInstigatorController(), this);
        }
    }
}

void ASOLON3_LeviathanBoss::SpawnMirrorShards()
{
    if (!MirrorShardClass) return;

    for (int32 i = 0; i < NumShardsToSpawn; i++)
    {
        float Angle = (360.0f / NumShardsToSpawn) * i;
        float Radius = 600.0f;
        FVector Offset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
                                  FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 0.0f);
        FVector SpawnLoc = GetActorLocation() + Offset;

        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = GetInstigator();
        ASOLON3_MirrorShard* Shard = GetWorld()->SpawnActor<ASOLON3_MirrorShard>(MirrorShardClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Shard)
        {
            Shard->SetOwnerLeviathan(this);
            ActiveShards.Add(Shard);
        }
    }
}

void ASOLON3_LeviathanBoss::OnShardDestroyed(ASOLON3_MirrorShard* DestroyedShard)
{
    ActiveShards.Remove(DestroyedShard);

    if (ActiveShards.Num() <= 3)
    {
        AdvanceLocalPhase();
    }
}

void ASOLON3_LeviathanBoss::SpawnMirrorCore()
{
    if (!MirrorCoreClass) return;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();
    MirrorCore = GetWorld()->SpawnActor<ASOLON3_MirrorCore>(MirrorCoreClass, GetActorLocation(), FRotator::ZeroRotator, Params);

    if (MirrorCore)
    {
        MirrorCore->SetOwnerLeviathan(this);
    }
}

void ASOLON3_LeviathanBoss::RegisterCoreHit()
{
    CurrentHitsOnCore++;
    if (CurrentHitsOnCore >= HitsToDefeat)
    {
        if (MirrorCore)
        {
            MirrorCore->Destroy();
            MirrorCore = nullptr;
        }
        Destroy();
    }
}
