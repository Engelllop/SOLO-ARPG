#include "NPCs/N9/BP_Oswin.h"
#include "GameFramework/CharacterMovementComponent.h"

ABP_Oswin::ABP_Oswin()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABP_Oswin::BeginPlay()
{
    Super::BeginPlay();
    OnFirstMeeting();
}

void ABP_Oswin::CheckForgeStatus()
{
    if (bHasGolemClaw && bHasJadeEssence && bHasBloodOfExecuted)
    {
        bCanForge = true;
        OnForgeReady();
    }
}

void ABP_Oswin::ForgeWeapon()
{
    if (bCanForge)
    {
        OnForgeComplete();
        bCanForge = false;
    }
}
