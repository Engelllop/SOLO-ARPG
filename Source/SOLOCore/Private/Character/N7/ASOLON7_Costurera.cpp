#include "Character/N7/ASOLON7_Costurera.h"
#include "World/N7/ASOLON7_MarionetaComponent.h"
#include "Kismet/GameplayStatics.h"

ASOLON7_Costurera::ASOLON7_Costurera()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON7_Costurera::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
    LocalPhase = 1;
    BrokenThreads = 0;
    OnPreCombatDialog();
}

void ASOLON7_Costurera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (LocalPhase == 3)
    {
        IntangibleTimer += DeltaTime;
        if (IntangibleTimer >= IntangibleCooldown)
        {
            IntangibleTimer = 0.0f;
            ToggleIntangible();
        }

        if (bIsIntangible)
        {
            IntangibleCurrentDuration += DeltaTime;
            if (IntangibleCurrentDuration >= IntangibleDuration)
            {
                bIsIntangible = false;
                IntangibleCurrentDuration = 0.0f;
            }
        }
    }
}

void ASOLON7_Costurera::AdvanceLocalPhase()
{
    LocalPhase++;
    if (LocalPhase == 2)
    {
        OnMiraDeath();
        OnVassenDeath();
        OnCompanionsDeadCG();
    }
    else if (LocalPhase == 3)
    {
        // Start direct combat
    }
}

void ASOLON7_Costurera::RegisterThreadBreak()
{
    BrokenThreads++;
    if (BrokenThreads >= ThreadsToBreak)
    {
        AdvanceLocalPhase();
    }
}

void ASOLON7_Costurera::ToggleIntangible()
{
    bIsIntangible = !bIsIntangible;
    if (bIsIntangible)
    {
        IntangibleCurrentDuration = 0.0f;
    }
}

float ASOLON7_Costurera::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsIntangible)
    {
        return 0.0f;
    }
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASOLON7_Costurera::OnDefeated()
{
    OnDeathDialog();
    Destroy();
}



