#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EMBERVEILGameplayAbility.h"
#include "GA_BasicAttack.generated.h"

class UAnimMontage;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;
class UGameplayEffect;

UCLASS()
class EMBERVEILCORE_API UGA_BasicAttack : public UEMBERVEILGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BasicAttack();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasicAttack|Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasicAttack|Damage")
	TArray<float> DamageMultipliers = { 1.0f, 1.1f, 1.5f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasicAttack|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BasicAttack|Damage")
	TSubclassOf<UGameplayEffect> StaggerEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> PoisonEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PoisonChanceOnComboFinisher = 0.1f;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	void PlayCurrentComboMontage();
	void OpenComboWindow();
	void CloseComboWindow();
	void OnMontageCompleted();
	void OnMontageBlendOut();
	void OnMontageCancelled();
	void OnHitEventReceived(FGameplayEventData Payload);
	void PerformHitDetection();
	void ApplyDamageToTarget(AActor* Target);
	void ResetCombo();

private:
	int32 ComboStep = 0;
	bool bComboWindowOpen = false;
	bool bComboQueued = false;

	/** Indica si el ataque actual ya ha golpeado a al menos un enemigo (para el combo). */
	bool bHitConfirmed = false;
	FTimerHandle ComboWindowTimer;

	FGameplayTag AttackingTag;
	FGameplayTag WeaponSocketTag;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	TObjectPtr<UAbilityTask_WaitGameplayEvent> HitEventTask;
};
