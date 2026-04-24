#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILAIController.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup = (EMBERVEIL), meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API AEMBERVEILAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEMBERVEILAIController();

	// Blackboard key names — used by all BT nodes to avoid magic strings
	static const FName BB_TargetActor;
	static const FName BB_bIsAggro;
	static const FName BB_PatrolOrigin;
	static const FName BB_AttackRange;
	/** Distancia al TargetActor en cm (-1 si no hay objetivo). Actualizado por BTService_CombatUpdate. */
	static const FName BB_DistanceToTarget;
	static const FName BB_bCanAttack;
	static const FName BB_CurrentPhase;

	void SetTarget(AActor* NewTarget);
	void ClearTarget();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	void OnSearchTimerExpired();

	void BindStunnedTagDelegate(APawn* InControlledPawn);
	void UnbindStunnedTagDelegate();
	void OnStunnedTagChanged(FGameplayTag Tag, int32 NewCount);

	FGameplayTag StunnedTag;
	TWeakObjectPtr<UAbilitySystemComponent> StunnedTagASC;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SearchDuration = 5.f;

	FTimerHandle SearchTimer;
};
