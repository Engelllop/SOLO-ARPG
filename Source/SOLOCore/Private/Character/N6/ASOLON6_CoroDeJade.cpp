#include "Character/N6/ASOLON6_CoroDeJade.h"
#include "World/N6/ASOLON6_ChildSinger.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ASOLON6_CoroDeJade::ASOLON6_CoroDeJade()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON6_CoroDeJade::BeginPlay()
{
    Super::BeginPlay();
    LocalPhase = 1;
    SpawnSingers();
}

void ASOLON6_CoroDeJade::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON6_CoroDeJade::AdvanceLocalPhase()
{
    LocalPhase = 2;
    OnPhase2Dialog();
}

void ASOLON6_CoroDeJade::SpawnSingers()
{
    if (!SingerClass) return;

    for (int32 i = 0; i < NumSingers; i++)
    {
        float Angle = (360.0f / NumSingers) * i;
        float Radius = 400.0f;
        FVector Offset = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
                                  FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 0.0f);
        FVector SpawnLoc = GetActorLocation() + Offset;

        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = GetInstigator();
        AASOLON6_ChildSinger* Singer = GetWorld()->SpawnActor<AASOLON6_ChildSinger>(SingerClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Singer)
        {
            Singer->SetCoroOwner(this);
            Singers.Add(Singer);
        }
    }
}

void ASOLON6_CoroDeJade::OnSingerFreed(AASOLON6_ChildSinger* FreedSinger)
{
    Singers.Remove(FreedSinger);

    if (Singers.Num() <= 0)
    {
        AdvanceLocalPhase();
    }
}

void ASOLON6_CoroDeJade::OnDefeated()
{
    OnDeathDialog();
    Destroy();
}



