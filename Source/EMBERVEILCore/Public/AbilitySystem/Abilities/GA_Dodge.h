#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EMBERVEILGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "GA_Dodge.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_ApplyRootMotionConstantForce;
class UGameplayEffect;

UCLASS()
class EMBERVEILCORE_API UGA_Dodge : public UEMBERVEILGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Dodge();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeDistance = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeDuration = 0.35f;

	/** Duración del GameplayEffect que aplica State.Invulnerable (i-frames). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge|Defense", meta = (ClampMin = "0.05", ClampMax = "2.0"))
	float DodgeInvulnerabilityDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge|Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge|Cost")
	TSubclassOf<UGameplayEffect> StaminaCostEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge|Cooldown")
	TSubclassOf<UGameplayEffect> CooldownEffect;

	/**
	 * GE con duración, tag State.Invulnerable en InheritableGameplayTagContainer.
	 * Por defecto se usa UEMBERVEIL_GE_DodgeInvulnerability si no se asigna otro.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge|Defense")
	TSubclassOf<UGameplayEffect> DodgeInvulnerabilityEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:
	void ClearInvulnerabilityEffect();
	void OnMontageCompleted();
	void OnMontageCancelled();

	FGameplayTag DodgingTag;
	FGameplayTag StaminaCostTag;

	FActiveGameplayEffectHandle InvulnerabilityEffectHandle;

	// Cache cooldown tags so GetCooldownTags can return a stable pointer
	UPROPERTY()
	FGameplayTagContainer CooldownTagContainer;
};
