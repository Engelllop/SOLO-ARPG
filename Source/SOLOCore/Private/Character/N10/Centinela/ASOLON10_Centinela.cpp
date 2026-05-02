#include "Character/N10/Centinela/ASOLON10_Centinela.h"
#include "Character/N10/ASOLON10_Arquitecto.h"
#include "Kismet/GameplayStatics.h"

ASOLON10_Centinela::ASOLON10_Centinela()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON10_Centinela::BeginPlay()
{
    Super::BeginPlay();
    CurrentPhase = 1;
    bIsScanning = true;
    bPlayerDetected = false;
    bAllClonesAbsorbed = false;
    bSphereRevealed = false;
    SphereInstance = nullptr;

    // Start scanning phase
    OnScanStart();
}

void ASOLON10_Centinela::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentPhase == 1 && bIsScanning && !bPlayerDetected)
    {
        // Scanning logic: pattern detection
        ScanSpeed += DeltaTime * 0.1f;
    }
}

void ASOLON10_Centinela::AdvancePhase()
{
    CurrentPhase++;
    if (CurrentPhase == 2)
    {
        bIsScanning = false;
        OnScanPhaseComplete();
        SpawnDecisionClones();
    }
    else if (CurrentPhase == 3)
    {
        OnPhase2Complete();
        RevealSphere();
    }
    else if (CurrentPhase == 4)
    {
        OnCentinelaDefeated();
    }
}

void ASOLON10_Centinela::SpawnDecisionClones()
{
    if (!DecisionCloneClass) return;

    // Spawn clones based on player decisions
    // The actual decision data comes from the player's save via BP calls
    for (int32 i = 0; i < 3; i++)
    {
        FVector SpawnLoc = GetActorLocation() + FVector(FMath::FRandRange(-400.0f, 400.0f),
                                                         FMath::FRandRange(-400.0f, 400.0f), 0.0f);
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* Clone = GetWorld()->SpawnActor<AActor>(DecisionCloneClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Clone)
        {
            ActiveClones.Add(Clone);
        }
    }

    OnClonesSpawned();
}

void ASOLON10_Centinela::OnCloneAbsorbed()
{
    // Check if all clones are absorbed
    bool bAllAbsorbed = true;
    for (AActor* Clone : ActiveClones)
    {
        if (IsValid(Clone))
        {
            bAllAbsorbed = false;
            break;
        }
    }

    if (bAllAbsorbed)
    {
        bAllClonesAbsorbed = true;
        AdvancePhase();
    }
}

void ASOLON10_Centinela::RevealSphere()
{
    if (!SphereClass) return;

    FVector SpawnLoc = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SphereInstance = GetWorld()->SpawnActor<AActor>(SphereClass, SpawnLoc, FRotator::ZeroRotator, Params);

    if (SphereInstance)
    {
        bSphereRevealed = true;
        OnSphereRevealed();
    }
}

void ASOLON10_Centinela::NotifyPlayerMovement(FVector MovementDelta)
{
    if (CurrentPhase != 1 || !bIsScanning) return;

    // Simplified detection: very repetitive movement triggers detection
    static FVector LastDelta = FVector::ZeroVector;
    static int32 RepetitionCount = 0;

    if (MovementDelta.Equals(LastDelta, 10.0f))
    {
        RepetitionCount++;
        if (RepetitionCount > 20)
        {
            bPlayerDetected = true;
            OnScanPatternDetected();
        }
    }
    else
    {
        RepetitionCount = 0;
        LastDelta = MovementDelta;
    }
}

void ASOLON10_Centinela::SetPlayerDetected(bool bDetected)
{
    bPlayerDetected = bDetected;
    if (bDetected)
    {
        OnScanPatternDetected();
    }
    else
    {
        OnScanPatternEvaded();
    }
}

void ASOLON10_Centinela::DefeatCentinela()
{
    // Clean up remaining clones
    for (AActor* Clone : ActiveClones)
    {
        if (IsValid(Clone))
        {
            Clone->Destroy();
        }
    }
    ActiveClones.Empty();

    CurrentPhase = 4;
    OnCentinelaDefeated();
}
