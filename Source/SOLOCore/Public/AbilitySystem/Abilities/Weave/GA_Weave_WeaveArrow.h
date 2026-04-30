#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "GA_Weave_WeaveArrow.generated.h"

UCLASS()
class SOLOCORE_API UGA_Weave_WeaveArrow : public USOLOGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Weave_WeaveArrow();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> ArrowDamageEffect;
	UPROPERTY(EditDefaultsOnly) float ArrowRange = 5000.f;
	UPROPERTY(EditDefaultsOnly) float ArrowRadius = 30.f;
	UPROPERTY(EditDefaultsOnly) float DamageMultiplier = 2.0f;
};
