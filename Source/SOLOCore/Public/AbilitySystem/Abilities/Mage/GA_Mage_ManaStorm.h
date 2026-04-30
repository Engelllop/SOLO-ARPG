#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_ManaStorm.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_ManaStorm : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Mage_ManaStorm();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float ManaCostPerSecond = 20.f;
	UPROPERTY(EditDefaultsOnly) float MaxDuration = 5.f;
	UPROPERTY(EditDefaultsOnly) float TickInterval = 0.5f;
	UPROPERTY(EditDefaultsOnly) float BaseMultiplier = 0.4f;
	UPROPERTY(EditDefaultsOnly) float StackingBonus = 0.1f;
	UPROPERTY(EditDefaultsOnly) float Radius = 800.f;
private:
	FTimerHandle ChannelHandle;
	int32 TickCount = 0;
	float ElapsedTime = 0.f;
	void OnChannelTick();
	void StopChannel();
};
