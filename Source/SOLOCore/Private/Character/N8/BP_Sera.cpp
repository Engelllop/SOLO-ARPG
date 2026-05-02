#include "Character/N8/BP_Sera.h"
#include "GameFramework/CharacterMovementComponent.h"

ABP_Sera::ABP_Sera()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABP_Sera::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = 500.0f;
    }
}
