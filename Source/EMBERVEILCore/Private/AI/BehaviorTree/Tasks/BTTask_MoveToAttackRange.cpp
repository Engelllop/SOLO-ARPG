#include "AI/BehaviorTree/Tasks/BTTask_MoveToAttackRange.h"
#include "AI/EMBERVEILAIController.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Interfaces/ICombatInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AITypes.h"

UBTTask_MoveToAttackRange::UBTTask_MoveToAttackRange()
{
	NodeName    = TEXT("Move To Attack Range");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(AEMBERVEILAIController::BB_TargetActor));
	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	// Dead targets cannot be chased
	if (Target->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Target))
	{
		return EBTNodeResult::Failed;
	}

	const float AttackRange = BB->GetValueAsFloat(AEMBERVEILAIController::BB_AttackRange);

	AEMBERVEILAIController* AIC = Cast<AEMBERVEILAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	// Already in range — nothing to do
	const float DistSq = FVector::DistSquared(Pawn->GetActorLocation(), Target->GetActorLocation());
	if (DistSq <= FMath::Square(AttackRange))
	{
		return EBTNodeResult::Succeeded;
	}

	FBTMoveToAttackRangeMemory* Mem = reinterpret_cast<FBTMoveToAttackRangeMemory*>(NodeMemory);
	Mem->ElapsedTime             = 0.f;
	Mem->LastKnownTargetLocation = Target->GetActorLocation();
	Mem->bMovementRequested      = false;

	return RequestMove(OwnerComp, NodeMemory, Target, AttackRange);
}

EBTNodeResult::Type UBTTask_MoveToAttackRange::RequestMove(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, AActor* Target, float AcceptanceRadius)
{
	AEMBERVEILAIController* AIC = Cast<AEMBERVEILAIController>(OwnerComp.GetAIOwner());
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	// Apply MoveSpeed from AttributeSet so fast enemies actually chase faster
	if (AEMBERVEILEnemyCharacter* EnemyChar = Cast<AEMBERVEILEnemyCharacter>(AIC->GetPawn()))
	{
		if (UEMBERVEILAttributeSet* AttrSet = EnemyChar->GetAttributeSet())
		{
			EnemyChar->GetCharacterMovement()->MaxWalkSpeed = AttrSet->GetMoveSpeed();
		}
	}

	FAIMoveRequest MoveReq(Target);
	MoveReq.SetAcceptanceRadius(FMath::Max(AcceptanceRadius - 30.f, 10.f));
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(false);

	const FPathFollowingRequestResult Result = AIC->MoveTo(MoveReq);

	FBTMoveToAttackRangeMemory* Mem  = reinterpret_cast<FBTMoveToAttackRangeMemory*>(NodeMemory);
	Mem->bMovementRequested          = (Result.Code != EPathFollowingRequestResult::Failed);
	Mem->LastKnownTargetLocation     = Target->GetActorLocation();

	return (Result.Code == EPathFollowingRequestResult::Failed)
		? EBTNodeResult::Failed
		: EBTNodeResult::InProgress;
}

void UBTTask_MoveToAttackRange::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTMoveToAttackRangeMemory* Mem = reinterpret_cast<FBTMoveToAttackRangeMemory*>(NodeMemory);
	Mem->ElapsedTime += DeltaSeconds;

	if (Mem->ElapsedTime >= MoveTimeout)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(AEMBERVEILAIController::BB_TargetActor));
	if (!Target || (Target->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Target)))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AEMBERVEILAIController* AIC = Cast<AEMBERVEILAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float AttackRange = BB->GetValueAsFloat(AEMBERVEILAIController::BB_AttackRange);
	const float DistSq      = FVector::DistSquared(Pawn->GetActorLocation(), Target->GetActorLocation());

	if (DistSq <= FMath::Square(AttackRange))
	{
		AIC->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// Recalculate path if the target moved significantly — avoids stale paths against moving players
	const float TargetMovedSq = FVector::DistSquared(Mem->LastKnownTargetLocation, Target->GetActorLocation());
	if (TargetMovedSq > FMath::Square(PathRebuildThreshold))
	{
		RequestMove(OwnerComp, NodeMemory, Target, AttackRange);
	}
}

EBTNodeResult::Type UBTTask_MoveToAttackRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEMBERVEILAIController* AIC = Cast<AEMBERVEILAIController>(OwnerComp.GetAIOwner()))
	{
		AIC->StopMovement();
	}
	return EBTNodeResult::Aborted;
}

FString UBTTask_MoveToAttackRange::GetStaticDescription() const
{
	return TEXT("Move To Attack Range");
}
