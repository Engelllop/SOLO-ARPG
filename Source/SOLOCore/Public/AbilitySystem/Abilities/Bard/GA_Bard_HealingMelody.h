#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_HealingMelody.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_HealingMelody : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_HealingMelody();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float ManaCostPerSecond = 5.f;
	UPROPERTY(EditDefaultsOnly) float HealRadius = 1000.f;
	UPROPERTY(EditDefaultsOnly) float MaxDuration = 8.f;
private:
	FTimerHandle MelodyHandle;
	float Elapsed = 0.f;
	void OnMelodyTick();
};
