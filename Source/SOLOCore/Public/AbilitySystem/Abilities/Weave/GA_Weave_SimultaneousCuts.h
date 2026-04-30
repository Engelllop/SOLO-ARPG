#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "GA_Weave_SimultaneousCuts.generated.h"

UCLASS()
class SOLOCORE_API UGA_Weave_SimultaneousCuts : public USOLOGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Weave_SimultaneousCuts();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> CutDamageEffect;
	UPROPERTY(EditDefaultsOnly) float ConeRange = 400.f;
	UPROPERTY(EditDefaultsOnly) float ConeHalfAngleDeg = 60.f;
	UPROPERTY(EditDefaultsOnly) float DamageMultiplier = 2.5f;
private:
	void PerformConeSweep(UAbilitySystemComponent* ASC, ACharacter* Caster, float AngleDeg);
};
