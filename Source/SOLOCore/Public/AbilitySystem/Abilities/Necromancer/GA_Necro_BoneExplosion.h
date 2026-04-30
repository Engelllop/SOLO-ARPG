#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_BoneExplosion.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_BoneExplosion : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_BoneExplosion();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float SearchRadius = 1000.f;
	UPROPERTY(EditDefaultsOnly) float ExplosionRadius = 600.f;
};
