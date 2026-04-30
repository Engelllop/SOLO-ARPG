#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GA_Weave_WeaveEcho.generated.h"

// Sera passive: every HitsPerCycle-th attack becomes unblockable
UCLASS()
class SOLOCORE_API UGA_Weave_WeaveEcho : public USOLOGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Weave_WeaveEcho();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION() void OnAttackHit(FGameplayEventData Payload);
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> UnblockableEffect;
	UPROPERTY(EditDefaultsOnly) int32 HitsPerCycle = 4;
private:
	int32 HitCounter = 0;
};
