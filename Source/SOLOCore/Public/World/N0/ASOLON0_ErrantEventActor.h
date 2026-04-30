#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON0_ErrantEventActor.generated.h"

class ASOLOEnemyCharacter;
class ACharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrantStep, int32, StepIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnErrantEventFinished);

// Orchestrates the 9-step Errante attack on Emberveil.
// Trigger: TriggerEvent() called by Level Blueprint (night + rank D OR 2 quests done).
// C++ kills the guards and the Errante at the correct steps; Blueprint adds camera/animation/audio.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON0_ErrantEventActor : public AActor
{
	GENERATED_BODY()
public:
	ASOLON0_ErrantEventActor();

	UFUNCTION(BlueprintCallable) void TriggerEvent();
	UFUNCTION(BlueprintPure) bool HasTriggered() const { return bTriggered; }

	UPROPERTY(BlueprintAssignable) FOnErrantStep OnStepReached;
	UPROPERTY(BlueprintAssignable) FOnErrantEventFinished OnEventFinished;

	// Level references — assign in editor
	UPROPERTY(EditInstanceOnly, Category = "N0|ErrantEvent") TSubclassOf<ASOLOEnemyCharacter> ErrantClass;
	UPROPERTY(EditInstanceOnly, Category = "N0|ErrantEvent") FVector ErrantSpawnLocation;
	UPROPERTY(EditInstanceOnly, Category = "N0|ErrantEvent") ACharacter* Guard1;
	UPROPERTY(EditInstanceOnly, Category = "N0|ErrantEvent") ACharacter* Guard2;
	UPROPERTY(EditInstanceOnly, Category = "N0|ErrantEvent") ACharacter* AldricCharacter;

	// GE to instantly kill the guards (lethal damage)
	UPROPERTY(EditDefaultsOnly, Category = "N0|ErrantEvent") TSubclassOf<UGameplayEffect> LethalDamageGE;

	// Post-event flag name written to the save/story system
	UPROPERTY(EditDefaultsOnly, Category = "N0|ErrantEvent") FName PostEventStoryFlag;

protected:
	virtual void BeginPlay() override;

private:
	bool bTriggered = false;
	int32 CurrentStep = 0;
	FTimerHandle StepTimer;

	TWeakObjectPtr<ASOLOEnemyCharacter> SpawnedErrante;

	void AdvanceToStep(int32 Step);
	void ExecuteStep(int32 Step);
	void KillActor(ACharacter* Target);
	void KillErrante();
};
