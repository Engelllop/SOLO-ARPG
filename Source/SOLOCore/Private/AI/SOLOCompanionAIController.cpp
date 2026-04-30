#include "AI/SOLOCompanionAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ASOLOCompanionAIController::BB_FollowTarget(TEXT("FollowTarget"));
const FName ASOLOCompanionAIController::BB_AggroTarget(TEXT("AggroTarget"));
const FName ASOLOCompanionAIController::BB_Command(TEXT("Command"));

ASOLOCompanionAIController::ASOLOCompanionAIController()
{
	bWantsPlayerState = false;
}

void ASOLOCompanionAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASOLOCompanionAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (CompanionBehaviorTree)
	{
		RunBehaviorTree(CompanionBehaviorTree);
		if (Blackboard)
			Blackboard->SetValueAsEnum(BB_Command, static_cast<uint8>(CurrentCommand));
	}
}

void ASOLOCompanionAIController::SetCommand(ESOLOCompanionCommand NewCommand)
{
	CurrentCommand = NewCommand;
	if (Blackboard) Blackboard->SetValueAsEnum(BB_Command, static_cast<uint8>(NewCommand));
}

void ASOLOCompanionAIController::SetFollowTarget(AActor* Target)
{
	FollowTarget = Target;
	if (Blackboard) Blackboard->SetValueAsObject(BB_FollowTarget, Target);
}

void ASOLOCompanionAIController::SetAggroTarget(AActor* Target)
{
	AggroTarget = Target;
	if (Blackboard) Blackboard->SetValueAsObject(BB_AggroTarget, Target);
}
