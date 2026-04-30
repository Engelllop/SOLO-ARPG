#include "AI/Tasks/BTTask_FollowTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_FollowTarget::UBTTask_FollowTarget()
{
	NodeName = TEXT("Follow Target");
}

EBTNodeResult::Type UBTTask_FollowTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BB) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	FAIMoveRequest MoveReq(Target);
	MoveReq.SetAcceptanceRadius(AcceptableRadius);
	MoveReq.SetUsePathfinding(true);

	FPathFollowingRequestResult Result = Controller->MoveTo(MoveReq);
	return Result.Code == EPathFollowingRequestResult::RequestSuccessful
		? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}
