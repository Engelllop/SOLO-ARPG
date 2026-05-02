#include "World/N4/ASOLON4_HungerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UASOLON4_HungerComponent::UASOLON4_HungerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UASOLON4_HungerComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHunger = MaxHunger;
    ApplyHungerEffects();
}

void UASOLON4_HungerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    HungerTimer += DeltaTime;
    if (HungerTimer >= HungerDecreaseInterval)
    {
        HungerTimer = 0.0f;
        ModifyHunger(-1);
    }
}

void UASOLON4_HungerComponent::ModifyHunger(int32 Delta)
{
    int32 OldHunger = CurrentHunger;
    CurrentHunger = FMath::Clamp(CurrentHunger + Delta, 0, MaxHunger);

    if (CurrentHunger != OldHunger)
    {
        OnHungerChanged.Broadcast(CurrentHunger);
        OnHungerLevelChanged(CurrentHunger);
        ApplyHungerEffects();

        if (CurrentHunger <= 0)
        {
            OnStarved.Broadcast();
            OnHungerDeath();
        }
    }
}

void UASOLON4_HungerComponent::SetHunger(int32 NewHunger)
{
    CurrentHunger = FMath::Clamp(NewHunger, 0, MaxHunger);
    OnHungerChanged.Broadcast(CurrentHunger);
    OnHungerLevelChanged(CurrentHunger);
    ApplyHungerEffects();
}

void UASOLON4_HungerComponent::SetMaxHunger(int32 NewMax)
{
    MaxHunger = FMath::Max(1, NewMax);
    CurrentHunger = FMath::Min(CurrentHunger, MaxHunger);
    OnHungerChanged.Broadcast(CurrentHunger);
}

void UASOLON4_HungerComponent::ApplyHungerEffects()
{
    switch (CurrentHunger)
    {
    case 5:
        BlurIntensity = 0.0f;
        SpeedReduction = 0.0f;
        DamageReduction = 0.0f;
        break;
    case 4:
        BlurIntensity = 0.2f;
        SpeedReduction = 0.0f;
        DamageReduction = 0.0f;
        break;
    case 3:
        BlurIntensity = 0.3f;
        SpeedReduction = 0.15f;
        DamageReduction = 0.0f;
        break;
    case 2:
        BlurIntensity = 0.4f;
        SpeedReduction = 0.15f;
        DamageReduction = 0.25f;
        break;
    case 1:
        BlurIntensity = 0.5f;
        SpeedReduction = 0.25f;
        DamageReduction = 0.25f;
        break;
    case 0:
        BlurIntensity = 0.8f;
        SpeedReduction = 0.5f;
        DamageReduction = 0.5f;
        break;
    }

    if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
    {
        if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
        {
            MoveComp->MaxWalkSpeed = MoveComp->MaxWalkSpeed * (1.0f - SpeedReduction);
        }
    }
}
