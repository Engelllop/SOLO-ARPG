#include "AI/SOLOBossController.h"
#include "AI/SOLOBossPhaseComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ASOLOBossController::BB_CurrentPhase(TEXT("CurrentPhase"));
const FName ASOLOBossController::BB_CombatTarget(TEXT("CombatTarget"));

ASOLOBossController::ASOLOBossController()
{
	bWantsPlayerState = false;
}

void ASOLOBossController::BeginPlay()
{
	Super::BeginPlay();
}

void ASOLOBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (USOLOBossPhaseComponent* PhaseComp = InPawn->FindComponentByClass<USOLOBossPhaseComponent>())
		PhaseComp->OnPhaseChanged.AddDynamic(this, &ASOLOBossController::OnBossPhaseChanged);

	if (BossBehaviorTree)
	{
		RunBehaviorTree(BossBehaviorTree);
		if (Blackboard) Blackboard->SetValueAsInt(BB_CurrentPhase, 0);
	}
}

void ASOLOBossController::OnBossPhaseChanged(int32 NewPhase)
{
	if (Blackboard) Blackboard->SetValueAsInt(BB_CurrentPhase, NewPhase);
}
