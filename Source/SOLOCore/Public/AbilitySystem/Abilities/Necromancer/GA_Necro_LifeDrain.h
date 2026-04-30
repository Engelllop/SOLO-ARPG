#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_LifeDrain.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_LifeDrain : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_LifeDrain();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float ManaCostPerSecond = 10.f;
	UPROPERTY(EditDefaultsOnly) float DamagePerSecond = 0.5f;
	UPROPERTY(EditDefaultsOnly) float HealMultiplier = 0.8f;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> SlowEffect;
private:
	FTimerHandle DrainHandle;
	TWeakObjectPtr<AActor> DrainTarget;
	void OnDrainTick();
	void StopDrain();
};
