#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_BloodRitual.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_BloodRitual : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_BloodRitual();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float HPCost = 50.f;
	UPROPERTY(EditDefaultsOnly) float ChannelTime = 2.0f;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> EliteBuffEffect;
};
