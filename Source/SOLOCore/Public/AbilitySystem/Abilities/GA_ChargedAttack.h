#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SOLOGameplayAbility.h"
#include "GA_ChargedAttack.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitInputRelease;
class UGameplayEffect;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SOLOCORE_API UGA_ChargedAttack : public USOLOGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ChargedAttack();

	// â”€â”€â”€ Charge timing â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Time in seconds to reach a full charge (ChargeRatio == 1.0). */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack")
	float ChargeTime = 1.5f;

	// â”€â”€â”€ Damage â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Flat damage at zero charge. Scales up with ChargeRatio. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack")
	float BaseDamage = 40.f;

	/** Base AOE radius at zero charge. Scales with ChargeRatio. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack")
	float ChargeRadius = 150.f;

	/** ChargeRatio threshold (0â€“1) that counts as a "full charge" for stagger. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack")
	float FullChargeThreshold = 0.8f;

	// â”€â”€â”€ Stamina cost â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Base stamina cost at zero charge. Scales with ChargeRatio. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack")
	float BaseStaminaCost = 30.f;

	/** GE used to apply the stamina cost via SetByCaller. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charged Attack|Cost")
	TSubclassOf<UGameplayEffect> StaminaCostEffect;

	// â”€â”€â”€ Gameplay Effects â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** GE_DamageBase subclass applied to each hit target. Set per-projectile in BP. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charged Attack|Effects")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** GE_Stagger applied to targets when ChargeRatio >= FullChargeThreshold. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack|Effects")
	TSubclassOf<UGameplayEffect> StaggerEffect;

	// â”€â”€â”€ Animation â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charged Attack|Animation")
	TObjectPtr<UAnimMontage> ChargeStartMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charged Attack|Animation")
	TObjectPtr<UAnimMontage> ChargeReleaseMontage;

	// â”€â”€â”€ VFX â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Niagara system spawned and attached to the character while charging. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack|VFX")
	TSoftObjectPtr<UNiagaraSystem> ChargeVFX;

	/** Niagara system spawned at the character location on release. */
	UPROPERTY(EditDefaultsOnly, Category = "Charged Attack|VFX")
	TSoftObjectPtr<UNiagaraSystem> ReleaseVFX;

	// â”€â”€â”€ Overrides â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	/** Called by UAbilityTask_WaitInputRelease when the player releases the button. */
	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	/** Finalises the attack: AOE hit, costs, VFX, montage, EndAbility. */
	void ReleaseCharge();

	void OnReleaseMontageCompleted();
	void OnReleaseMontageInterrupted();

	// â”€â”€â”€ Runtime state â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Current charge progress. Ticks from 0 to 1 over ChargeTime seconds. */
	float ChargeRatio = 0.f;

	/** Fires every 0.05 s to increment ChargeRatio. */
	FTimerHandle ChargeTimerHandle;

	bool bIsCharging = false;

	/** Active charge VFX component so we can deactivate it on release. */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> ActiveChargeVFX;

	FGameplayTag AttackingTag;
	FGameplayTag StaminaCostTag;
};
