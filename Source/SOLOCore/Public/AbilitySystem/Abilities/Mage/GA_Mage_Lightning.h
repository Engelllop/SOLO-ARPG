#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_Lightning.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_Lightning : public USOLOMageSpellBase
{
	GENERATED_BODY()

public:
	UGA_Mage_Lightning();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly) float TraceRange = 5000.f;
	UPROPERTY(EditDefaultsOnly) float WetBonusMultiplier = 3.0f;
};
