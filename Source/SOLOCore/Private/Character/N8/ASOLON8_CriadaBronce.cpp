#include "Character/N8/ASOLON8_CriadaBronce.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ASOLON8_CriadaBronce::ASOLON8_CriadaBronce()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON8_CriadaBronce::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(3.0f, 3.0f, 3.0f));
    LocalPhase = 1;
    bArmorBroken = false;
    bHasAskedForName = false;
}

void ASOLON8_CriadaBronce::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON8_CriadaBronce::AdvanceLocalPhase()
{
    LocalPhase++;
    if (LocalPhase == 2)
    {
        bArmorBroken = true;
        OnConfusionDialog();
    }
}

void ASOLON8_CriadaBronce::OnDefeated()
{
    OnDeathDialog();
    Destroy();
}



