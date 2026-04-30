#include "AI/Tasks/BTTask_CombatMove.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTTask_CombatMove::UBTTask_CombatMove()
{
	NodeName = TEXT("Combat Move");
}

EBTNodeResult::Type UBTTask_CombatMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BB) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Pawn || !Target) return EBTNodeResult::Failed;

	FVector ToTarget = (Target->GetActorLocation() - Pawn->GetActorLocation());
	FVector Dir;

	switch (MoveType)
	{
	case ECombatMoveType::Advance:  Dir = ToTarget.GetSafeNormal(); break;
	case ECombatMoveType::Retreat:  Dir = -ToTarget.GetSafeNormal(); break;
	case ECombatMoveType::Strafe:
		Dir = FVector::CrossProduct(ToTarget.GetSafeNormal(), FVector::UpVector);
		if (FMath::RandBool()) Dir = -Dir;
		break;
	}

	FVector Dest = Pawn->GetActorLocation() + Dir * MoveDistance;
	FAIMoveRequest MoveReq(Dest);
	MoveReq.SetAcceptanceRadius(50.f);
	FPathFollowingRequestResult Result = Controller->MoveTo(MoveReq);
	return Result.Code == EPathFollowingRequestResult::RequestSuccessful
		? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}
