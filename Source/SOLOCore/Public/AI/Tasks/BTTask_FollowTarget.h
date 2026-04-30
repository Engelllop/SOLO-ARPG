#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FollowTarget.generated.h"

UCLASS()
class SOLOCORE_API UBTTask_FollowTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FollowTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere) FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere) float AcceptableRadius = 150.f;
};
