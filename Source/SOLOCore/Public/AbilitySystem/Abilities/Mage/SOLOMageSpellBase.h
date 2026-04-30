#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "SOLOMageSpellBase.generated.h"

UCLASS(Abstract)
class SOLOCORE_API USOLOMageSpellBase : public USOLOGameplayAbility
{
	GENERATED_BODY()

public:
	USOLOMageSpellBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	float ManaCost = 25.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	float SpellMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<UGameplayEffect> CooldownEffect;

	bool ConsumeManaCost(const FGameplayAbilityActorInfo* ActorInfo);
	void NotifyManaComponent(const FGameplayAbilityActorInfo* ActorInfo);
	FGameplayEffectSpecHandle MakeDamageSpec(const FGameplayAbilityActorInfo* ActorInfo, float CustomMultiplier = -1.f) const;
};
