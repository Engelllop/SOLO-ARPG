#include "Character/N3/BP_Garrik_Aelthar.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABP_Garrik_Aelthar::ABP_Garrik_Aelthar()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABP_Garrik_Aelthar::BeginPlay()
{
    Super::BeginPlay();

    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = 500.0f;
    }
}

void ABP_Garrik_Aelthar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABP_Garrik_Aelthar::TwinBladeAttack(AActor* Target)
{
    if (!Target) return;

    float FinalDamage = BaseDamage;

    if (FMath::FRand() <= CriticalChance)
    {
        FinalDamage *= CriticalMultiplier;
    }

    FHitResult HitResult;
    FPointDamageEvent DmgEvent(FinalDamage, HitResult, FVector::ForwardVector, nullptr);
    Target->TakeDamage(FinalDamage, DmgEvent, GetController(), this);

    OnAttackLanded(Target);
}

void ABP_Garrik_Aelthar::OnAttackLanded_Implementation(AActor* HitTarget)
{
    // Blueprint hook for VFX/SFX
}

void ABP_Garrik_Aelthar::SetStoryFlag(const FString& FlagName)
{
    StoryFlags.Add(FlagName);
}

bool ABP_Garrik_Aelthar::HasStoryFlag(const FString& FlagName) const
{
    return StoryFlags.Contains(FlagName);
}
