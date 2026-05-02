#include "Character/N5/ASOLON5_Carnicero.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ASOLON5_Carnicero::ASOLON5_Carnicero()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON5_Carnicero::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));
    LocalPhase = 1;
    bIsBeastForm = false;
}

void ASOLON5_Carnicero::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (LocalPhase == 2)
    {
        FormSwitchTimer += DeltaTime;
        if (FormSwitchTimer >= FormSwitchCooldown)
        {
            FormSwitchTimer = 0.0f;
            ToggleForm();
        }
    }
}

void ASOLON5_Carnicero::AdvanceLocalPhase()
{
    LocalPhase = 2;
    bIsBeastForm = true;
    OnPhaseTransitionDialog();

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed *= BestiaAttackSpeedMultiplier;
    }
}

void ASOLON5_Carnicero::ToggleForm()
{
    bIsBeastForm = !bIsBeastForm;
}

void ASOLON5_Carnicero::OnDefeated()
{
    OnDeathDialog();
    Destroy();
}



