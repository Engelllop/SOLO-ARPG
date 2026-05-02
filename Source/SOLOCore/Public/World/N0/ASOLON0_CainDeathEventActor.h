#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "ASOLON0_CainDeathEventActor.generated.h"
class ASOLOEnemyCharacter;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCainDeathStep, int32, StepIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCainDeathEventFinished);

// Orchestrates the 24-step CaÃ­n death sequence triggered when the Guardian reaches 25% HP.
// C++ manages gameplay logic; Blueprint handles camera, animation montages, and audio.
// Each step fires OnStepReached so Blueprint can bind per-step visuals.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON0_CainDeathEventActor : public AActor
{
	GENERATED_BODY()
public:
	ASOLON0_CainDeathEventActor();

	// Called by ASOLON0_GuardianBoss when Phase 3 begins
	UFUNCTION(BlueprintCallable) void TriggerEvent(ASOLOEnemyCharacter* InGuardian);

	// Delegates for Blueprint to bind per-step visuals / audio
	UPROPERTY(BlueprintAssignable) FOnCainDeathStep OnStepReached;
	UPROPERTY(BlueprintAssignable) FOnCainDeathEventFinished OnEventFinished;

	// Level references â€” assign in editor
	UPROPERTY(EditInstanceOnly, Category = "N0|CainEvent") ACharacter* CainCharacter;
	UPROPERTY(EditInstanceOnly, Category = "N0|CainEvent") ACharacter* MiraCharacter;
	UPROPERTY(EditInstanceOnly, Category = "N0|CainEvent") ACharacter* VassenCharacter;
	UPROPERTY(EditInstanceOnly, Category = "N0|CainEvent") ACharacter* ProtagonistCharacter;

	// Abilities to grant after CaÃ­n dies
	UPROPERTY(EditDefaultsOnly, Category = "N0|CainEvent") TSubclassOf<UGameplayAbility> SimultaneousCutsClass;
	UPROPERTY(EditDefaultsOnly, Category = "N0|CainEvent") TSubclassOf<UGameplayAbility> WeaveArrowClass;

	// GE that applies the Guardian's 5s vulnerability window (notified to boss)
	UPROPERTY(EditDefaultsOnly, Category = "N0|CainEvent") TSubclassOf<UGameplayEffect> GuardianVulnerableGE;

	// GE applied to CaÃ­n at each stab step (non-lethal until step 4)
	UPROPERTY(EditDefaultsOnly, Category = "N0|CainEvent") TSubclassOf<UGameplayEffect> StabDamageGE;

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentStep = 0;
	bool bEventActive = false;
	TWeakObjectPtr<ASOLOEnemyCharacter> GuardianRef;

	FTimerHandle StepTimer;

	void AdvanceToStep(int32 Step);
	void ScheduleNextStep(float Delay);

	// Per-step gameplay logic
	void ExecuteStep(int32 Step);

	void GrantAbilityToCharacter(ACharacter* Character, TSubclassOf<UGameplayAbility> AbilityClass);
	void FreezeCharacterAI(ACharacter* Character, bool bFreeze);
	void ApplyStabToCharacter(ACharacter* Character, bool bLethal);
	void TriggerGuardianVulnerability();
};


