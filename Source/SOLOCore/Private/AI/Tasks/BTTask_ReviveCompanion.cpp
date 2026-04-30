#include "AI/Tasks/BTTask_ReviveCompanion.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SOLOCompanionCharacter.h"
#include "Combat/SOLORespawnComponent.h"

UBTTask_ReviveCompanion::UBTTask_ReviveCompanion()
{
	NodeName = TEXT("Revive Companion");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ReviveCompanion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ElapsedTime = 0.f;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	ASOLOCompanionCharacter* Target = Cast<ASOLOCompanionCharacter>(
		BB->GetValueAsObject(DownedCompanionKey.SelectedKeyName));
	if (!Target || !Target->IsDowned()) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_ReviveCompanion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ElapsedTime += DeltaSeconds;
	if (ElapsedTime < ReviveDuration) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
		if (ASOLOCompanionCharacter* Target = Cast<ASOLOCompanionCharacter>(
			BB->GetValueAsObject(DownedCompanionKey.SelectedKeyName)))
		{
			if (USOLORespawnComponent* Respawn = Target->FindComponentByClass<USOLORespawnComponent>())
				Respawn->CompleteRevive();
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTTask_ReviveCompanion::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ElapsedTime = 0.f;
	return EBTNodeResult::Aborted;
}
