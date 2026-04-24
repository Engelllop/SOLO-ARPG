#include "AI/BehaviorTree/Tasks/BTTask_Patrol.h"
#include "AI/EMBERVEILAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName    = TEXT("Patrol");
	bNotifyTick = true;
}

bool UBTTask_Patrol::TryPickPatrolPoint(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTPatrolMemory* Mem = reinterpret_cast<FBTPatrolMemory*>(NodeMemory);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	const FVector Origin     = BB ? BB->GetValueAsVector(AEMBERVEILAIController::BB_PatrolOrigin) : FVector::ZeroVector;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld());
	if (!NavSys)
	{
		return false;
	}

	FNavLocation NavLoc;
	const bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, PatrolRadius, NavLoc);
	if (!bFound)
	{
		return false;
	}

	Mem->TargetLocation = NavLoc.Location;
	return true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTPatrolMemory* Mem  = reinterpret_cast<FBTPatrolMemory*>(NodeMemory);
	Mem->WaitElapsed      = 0.f;
	Mem->bWaiting         = false;
	Mem->bMovementStarted = false;
	Mem->RetryCount       = 0;

	if (!TryPickPatrolPoint(OwnerComp, NodeMemory))
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	const EPathFollowingRequestResult::Type MoveResult = AIC->MoveToLocation(Mem->TargetLocation, 50.f, true, true);
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		Mem->RetryCount++;
		if (Mem->RetryCount >= MaxRetries)
		{
			return EBTNodeResult::Failed;
		}
		// Dejar que TickTask reintente con un nuevo punto (bMovementStarted == false)
		Mem->bMovementStarted = false;
		return EBTNodeResult::InProgress;
	}

	Mem->bMovementStarted = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTPatrolMemory* Mem = reinterpret_cast<FBTPatrolMemory*>(NodeMemory);
	AAIController*   AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Mem->bWaiting)
	{
		Mem->WaitElapsed += DeltaSeconds;
		if (Mem->WaitElapsed >= WaitTimeAtPoint)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		return;
	}

	// Handle retry case where movement wasn't started yet
	if (!Mem->bMovementStarted)
	{
		if (!TryPickPatrolPoint(OwnerComp, NodeMemory))
		{
			Mem->RetryCount++;
			if (Mem->RetryCount >= MaxRetries)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
			return;
		}
		const EPathFollowingRequestResult::Type RetryMoveResult = AIC->MoveToLocation(Mem->TargetLocation, 50.f, true, true);
		if (RetryMoveResult != EPathFollowingRequestResult::Failed)
		{
			Mem->bMovementStarted = true;
		}
		else
		{
			Mem->RetryCount++;
			if (Mem->RetryCount >= MaxRetries)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
		return;
	}

	// Check if we've arrived
	UPathFollowingComponent* PFC = AIC->GetPathFollowingComponent();
	if (PFC && PFC->GetStatus() == EPathFollowingStatus::Idle)
	{
		// Arrived — start the idle wait before picking the next point
		Mem->bWaiting    = true;
		Mem->WaitElapsed = 0.f;
	}
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIC = OwnerComp.GetAIOwner())
	{
		AIC->StopMovement();
	}
	return EBTNodeResult::Aborted;
}

FString UBTTask_Patrol::GetStaticDescription() const
{
	return FString::Printf(TEXT("Patrol | Radius: %.0f | Wait: %.1fs"), PatrolRadius, WaitTimeAtPoint);
}
