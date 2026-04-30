#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SOLOComboData.h"
#include "SOLOComboComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComboStep, int32, CurrentStep, int32, MaxSteps);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnComboFinale);

UCLASS(ClassGroup = (SOLO), meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLOComboComponent();

	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	bool StartLightCombo();

	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	bool StartHeavyCombo();

	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void CancelCombo();

	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void OnComboStepCompleted();

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	bool CanContinueCombo() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	bool IsInCombo() const { return bIsInCombo; }

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	bool IsFinaleStep() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	float GetCurrentStepDamage() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	float GetCurrentStepPostureDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void SetWeaponComboData(const FWeaponComboData& InData);

	UPROPERTY(BlueprintAssignable)
	FOnComboStep OnComboStepEvent;

	UPROPERTY(BlueprintAssignable)
	FOnComboReset OnComboResetEvent;

	UPROPERTY(BlueprintAssignable)
	FOnComboFinale OnComboFinaleEvent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	FWeaponComboData CurrentComboData;

private:
	int32 CurrentComboStep = 0;
	ESOLOAttackType CurrentAttackType = ESOLOAttackType::Light;
	bool bIsInCombo = false;
	float LastAttackTime = 0.f;

	bool ExecuteNextStep();
	void ResetCombo();
	const TArray<FComboStep>& GetCurrentSteps() const;
	bool ConsumeSPForStep(const FComboStep& Step);
};
