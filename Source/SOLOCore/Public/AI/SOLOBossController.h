#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "SOLOBossController.generated.h"

UCLASS()
class SOLOCORE_API ASOLOBossController : public AAIController
{
	GENERATED_BODY()
public:
	ASOLOBossController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) UBehaviorTree* BossBehaviorTree;

private:
	UFUNCTION() void OnBossPhaseChanged(int32 NewPhase);

	static const FName BB_CurrentPhase;
	static const FName BB_CombatTarget;
};
