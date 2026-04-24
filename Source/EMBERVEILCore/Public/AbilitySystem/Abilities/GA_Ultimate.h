#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EMBERVEILGameplayAbility.h"
#include "AbilitySystem/GameplayEffects/GE_DamageBase.h"
#include "GameplayEffectTypes.h"
#include "GA_Ultimate.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class UGameplayEffect;
class UNiagaraSystem;

/**
 * Ultimate base: montaje → evento de impacto → área de daño, stagger, knockback y VFX.
 * Cooldown: Cooldown.Ability.Ultimate (via GetCooldownTags + CooldownEffect).
 */
UCLASS()
class EMBERVEILCORE_API UGA_Ultimate : public UEMBERVEILGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Ultimate();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Cooldown")
	float CooldownDuration = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Cost")
	float ManaCost = 80.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	float UltimateRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	float UltimateDamageMultiplier = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	float KnockbackHorizontalForce = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	float KnockbackUpwardForce = 350.f;

	/** GE con duración y tag Cooldown.Ability.Ultimate (configurar duración en el asset o vía ApplyCooldown). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Cooldown")
	TSubclassOf<UGameplayEffect> CooldownEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Cost")
	TSubclassOf<UGameplayEffect> ManaCostEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Animation")
	TObjectPtr<UAnimMontage> UltimateMontage;

	/** Tag del evento disparado por AnimNotify en el montaje (impacto). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Animation")
	FGameplayTag UltimateImpactEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	TSubclassOf<UGE_DamageBase> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Damage")
	TSubclassOf<UGameplayEffect> StaggerEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|VFX")
	TObjectPtr<UNiagaraSystem> UltimateAreaVFX;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	void OnUltimateImpact(FGameplayEventData Payload);
	void ApplyUltimateToOverlappingEnemies();
	void SpawnUltimateVFX() const;

	UFUNCTION()
	void OnUltimateMontageFinished();

private:
	UPROPERTY()
	FGameplayTagContainer CooldownTagContainer;

	FGameplayTag ManaCostTag;

	bool bUltimateImpactHandled = false;
};
