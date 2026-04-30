#include "AI/Decorators/BTDecorator_EnemyInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_EnemyInRange::UBTDecorator_EnemyInRange()
{
	NodeName = TEXT("Enemy In Range");
}

bool UBTDecorator_EnemyInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!Controller || !BB) return false;

	const APawn* Pawn = Controller->GetPawn();
	const AActor* Enemy = Cast<AActor>(BB->GetValueAsObject(EnemyKey.SelectedKeyName));
	if (!Pawn || !Enemy) return false;

	return FVector::DistSquared(Pawn->GetActorLocation(), Enemy->GetActorLocation()) <= FMath::Square(Range);
}
