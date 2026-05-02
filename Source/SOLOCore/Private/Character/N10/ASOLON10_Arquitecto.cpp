#include "Character/N10/ASOLON10_Arquitecto.h"
#include "World/N10/ASOLON10_PlayerClone.h"
#include "Kismet/GameplayStatics.h"

ASOLON10_Arquitecto::ASOLON10_Arquitecto()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON10_Arquitecto::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(1.8f, 1.8f, 1.8f));
    LocalPhase = 1;
    bHasShownArchive = false;
    bIsChoiceMade = false;
    OnFirstMeeting();
}

void ASOLON10_Arquitecto::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON10_Arquitecto::AdvanceLocalPhase()
{
    LocalPhase++;
    if (LocalPhase == 2 && !bHasShownArchive)
    {
        bHasShownArchive = true;
        ShowArchive();
    }
}

void ASOLON10_Arquitecto::SpawnPlayerClones()
{
    if (!PlayerCloneClass) return;

    for (int32 i = 0; i < 10; i++)
    {
        FVector SpawnLoc = GetActorLocation() + FVector(FMath::FRandRange(-600.0f, 600.0f),
                                                         FMath::FRandRange(-600.0f, 600.0f), 0.0f);
        FActorSpawnParameters Params;
        Params.Owner = this;
        Params.Instigator = GetInstigator();
        AActor* Clone = GetWorld()->SpawnActor<AActor>(PlayerCloneClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Clone)
        {
            ActiveClones.Add(Clone);
        }
    }
}

void ASOLON10_Arquitecto::ChooseEscape()
{
    bIsChoiceMade = true;
    bChoseEscape = true;
    OnEndingA();
}

void ASOLON10_Arquitecto::ChooseStay()
{
    bIsChoiceMade = true;
    bChoseStay = true;

    if (bHasFragment && bHasLookedAbyss && bHasAllAbilities)
    {
        OnEndingB();
    }
    else
    {
        OnEndingBRequirementCheck();
        // Fallback to Ending A if requirements not met
        OnEndingA();
    }
}

float ASOLON10_Arquitecto::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (LocalPhase >= 3) return 0.0f; // Combat ends at phase 3
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// OnDefeated handled in Blueprint
void ASOLON10_Arquitecto::OnDefeated()
{
    if (!bIsChoiceMade)
    {
        AdvanceLocalPhase();
        if (LocalPhase >= 3)
        {
            ChooseEscape();
        }
    }
}
