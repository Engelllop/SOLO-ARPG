#include "World/N7/ASOLON7_MarionetaComponent.h"
#include "Character/N7/ASOLON7_Costurera.h"

UASOLON7_MarionetaComponent::UASOLON7_MarionetaComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UASOLON7_MarionetaComponent::BeginPlay()
{
    Super::BeginPlay();
    bIsControlled = true;
    bIsThreadBroken = false;
}

void UASOLON7_MarionetaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsControlled)
    {
        FreeTimer += DeltaTime;
        if (FreeTimer >= FreeDuration)
        {
            ForceControl();
            OnRecontrolled();
        }
    }
}

void UASOLON7_MarionetaComponent::BreakThread()
{
    ThreadHits++;
    OnThreadHit();

    if (ThreadHits >= ThreadMaxHits)
    {
        bIsControlled = false;
        FreeTimer = 0.0f;
        bIsThreadBroken = true;
        OnFreed();

        if (CostureraOwner && CostureraOwner->IsValidLowLevel())
        {
            CostureraOwner->RegisterThreadBreak();
        }
    }
}
