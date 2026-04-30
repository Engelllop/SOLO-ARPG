#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SOLOPostureComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostureBroken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPostureChanged, float, NewPercent);

UCLASS(ClassGroup = (SOLO), meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOPostureComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLOPostureComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	void StartBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	void StopBlock();

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	bool AttemptParry();

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	float ApplyBlockedDamage(float IncomingDamage, AActor* Attacker, FGameplayTag DamageTypeTag);

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	void ApplyPostureDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	void BreakPosture();

	UFUNCTION(BlueprintCallable, Category = "Combat|Posture")
	void RecoverPosture(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	bool CanBlock() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	bool CanParry() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	bool IsPostureBroken() const { return bPostureBroken; }

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	bool IsBlocking() const { return bIsBlocking; }

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	bool IsParrying() const { return bIsParrying; }

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	float GetPosturePercent() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Posture")
	float GetBonusDamageMultiplier() const;

	UPROPERTY(BlueprintAssignable)
	FOnPostureBroken OnPostureBroken;

	UPROPERTY(BlueprintAssignable)
	FOnPostureChanged OnPostureChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Posture")
	float MaxPosture = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Posture")
	float PostureRecoveryRate = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Posture")
	float RecoveryDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	float BlockPhysicalReduction = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	float BlockMagicalReduction = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	float SPPerSecondBlockCost = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Parry")
	float ParryWindowSeconds = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Parry")
	float ParryCooldown = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Parry")
	float StunDurationOnParry = 2.0f;

private:
	float CurrentPosture;
	float TimeSinceLastDamage;
	bool bIsBlocking = false;
	bool bPostureBroken = false;
	bool bIsParrying = false;
	bool bParryOnCooldown = false;
	FTimerHandle ParryWindowHandle;
	FTimerHandle ParryCooldownHandle;

	void EndParryWindow();
	void ResetParryCooldown();
	void ConsumeBlockSP(float DeltaTime);
};
