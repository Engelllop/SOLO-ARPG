#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Druid_NaturalBond.generated.h"

UCLASS()
class SOLOCORE_API UGA_Druid_NaturalBond : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Druid_NaturalBond();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> HoTEffect;
};
