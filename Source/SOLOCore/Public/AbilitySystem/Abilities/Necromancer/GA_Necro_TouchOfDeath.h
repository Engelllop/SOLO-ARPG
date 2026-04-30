#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_TouchOfDeath.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_TouchOfDeath : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_TouchOfDeath();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float TouchRange = 200.f;
	UPROPERTY(EditDefaultsOnly) float LifeDrainPercent = 0.2f;
	UPROPERTY(EditDefaultsOnly) float KillDrainPercent = 0.4f;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> HealEffect;
};
