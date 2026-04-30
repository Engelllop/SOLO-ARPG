#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_WeaveSymphony.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_WeaveSymphony : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_WeaveSymphony();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> BaseSymphonyBuff;
	UPROPERTY(EditDefaultsOnly) float AlliesRadius = 1500.f;
};
