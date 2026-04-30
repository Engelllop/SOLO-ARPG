#include "AI/Decorators/BTDecorator_CompanionDowned.h"
#include "AI/SOLOCompanionCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CompanionDowned::UBTDecorator_CompanionDowned()
{
	NodeName = TEXT("Companion Downed");
}

bool UBTDecorator_CompanionDowned::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;
	const ASOLOCompanionCharacter* Companion = Cast<ASOLOCompanionCharacter>(
		BB->GetValueAsObject(CompanionKey.SelectedKeyName));
	return Companion && Companion->IsDowned();
}
