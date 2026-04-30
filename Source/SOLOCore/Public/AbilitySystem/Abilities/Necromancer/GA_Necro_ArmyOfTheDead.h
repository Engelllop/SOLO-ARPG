#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_ArmyOfTheDead.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_ArmyOfTheDead : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_ArmyOfTheDead();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ASOLOSkeletonMinion> SkeletonClass;
	UPROPERTY(EditDefaultsOnly) int32 SpawnCount = 5;
	UPROPERTY(EditDefaultsOnly) float SpawnRadius = 500.f;
	UPROPERTY(EditDefaultsOnly) float ArmyDuration = 30.f;
	UPROPERTY(EditDefaultsOnly) float StatMultiplier = 1.25f;
	UPROPERTY(EditDefaultsOnly) float ChannelTime = 1.5f;
};
