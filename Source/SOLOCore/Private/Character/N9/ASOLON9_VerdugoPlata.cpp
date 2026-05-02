#include "Character/N9/ASOLON9_VerdugoPlata.h"
#include "Kismet/GameplayStatics.h"

ASOLON9_VerdugoPlata::ASOLON9_VerdugoPlata()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON9_VerdugoPlata::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
    LocalPhase = 1;
}

void ASOLON9_VerdugoPlata::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON9_VerdugoPlata::AdvanceLocalPhase()
{
    LocalPhase++;

    if (LocalPhase == 2)
    {
        bHasBrainExposed = true;
        OnPhase2Dialog();
    }
}

void ASOLON9_VerdugoPlata::OnDefeated()
{
    OnDeathDialog();

    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &ASOLON9_VerdugoPlata::OnSpiritDialog, 2.0f, false);

    Destroy();
}
