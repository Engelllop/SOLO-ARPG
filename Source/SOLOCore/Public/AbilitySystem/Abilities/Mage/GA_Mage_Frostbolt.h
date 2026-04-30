#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_Frostbolt.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_Frostbolt : public USOLOMageSpellBase
{
	GENERATED_BODY()

public:
	UGA_Mage_Frostbolt();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> SlowEffect;
	UPROPERTY(EditDefaultsOnly) float ProjectileSpeed = 2500.f;
};
