#include "AI/BehaviorTree/Decorators/BTDecorator_IsInAttackRange.h"
#include "AI/SOLOAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("In Attack Range");
	// Allow the user to configure abort mode (Self / LowerPriority / Both) in the editor
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return false;
	}

	const AActor* Target = Cast<AActor>(BB->GetValueAsObject(ASOLOAIController::BB_TargetActor));
	if (!Target)
	{
		return false;
	}

	const AAIController* AIC = OwnerComp.GetAIOwner();
	const APawn*         Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
	{
		return false;
	}

	const float AttackRange = BB->GetValueAsFloat(ASOLOAIController::BB_AttackRange);
	const float DistSq      = FVector::DistSquared(Pawn->GetActorLocation(), Target->GetActorLocation());

	return DistSq <= FMath::Square(AttackRange);
}

FString UBTDecorator_IsInAttackRange::GetStaticDescription() const
{
	return TEXT("In Attack Range");
}

FName UBTDecorator_IsInAttackRange::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Blackboard.Icon");
}
