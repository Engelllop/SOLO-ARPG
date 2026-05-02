#include "World/N6/ASOLON6_PTSDComponent.h"
#include "Kismet/GameplayStatics.h"

UASOLON6_PTSDComponent::UASOLON6_PTSDComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UASOLON6_PTSDComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentPTSD = 0;
}

void UASOLON6_PTSDComponent::ModifyPTSD(int32 Delta)
{
    int32 OldPTSD = CurrentPTSD;
    CurrentPTSD = FMath::Clamp(CurrentPTSD + Delta, 0, MaxPTSD);

    if (CurrentPTSD != OldPTSD)
    {
        OnPTSDChanged.Broadcast(CurrentPTSD);
        OnPTSDLevelChanged(CurrentPTSD);

        if (CurrentPTSD >= 5)
        {
            OnPanicAttack.Broadcast();
            OnPanicAttackEvent();
            CheckCainEcho();
            CurrentPTSD = 3; // Reset after panic to 3
            OnPTSDChanged.Broadcast(CurrentPTSD);
        }
    }
}

void UASOLON6_PTSDComponent::SetMaxPTSD(int32 NewMax)
{
    MaxPTSD = FMath::Max(1, NewMax);
    CurrentPTSD = FMath::Min(CurrentPTSD, MaxPTSD);
}

void UASOLON6_PTSDComponent::CheckCainEcho()
{
    if (bCainEchoResolved) return;

    CainEchoCount++;
    OnCainEchoAppear();

    if (CainEchoCount >= 3)
    {
        bCainEchoResolved = true;
        OnCainEchoResolved();
    }
}
