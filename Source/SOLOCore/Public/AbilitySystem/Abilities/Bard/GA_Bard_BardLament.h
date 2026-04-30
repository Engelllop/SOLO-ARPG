#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_BardLament.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_BardLament : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_BardLament();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> FearEffect;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> LamentDebuff;
	UPROPERTY(EditDefaultsOnly) float LamentRadius = 1200.f;
};
