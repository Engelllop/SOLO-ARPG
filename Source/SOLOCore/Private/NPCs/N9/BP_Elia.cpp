#include "NPCs/N9/BP_Elia.h"

ABP_Elia::ABP_Elia()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABP_Elia::BeginPlay()
{
    Super::BeginPlay();
    bHasGivenFragment = false;
    bHasBeenConfronted = false;
}

void ABP_Elia::ConverseWithPlayer()
{
    if (!bHasBeenConfronted)
    {
        bHasBeenConfronted = true;
        OnConfrontation();
    }
}

void ABP_Elia::GiveFragment()
{
    if (!bHasGivenFragment)
    {
        bHasGivenFragment = true;
        OnGiveFragment();
    }
}
