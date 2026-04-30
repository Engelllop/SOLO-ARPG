#include "AI/Decorators/BTDecorator_HasCommand.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_HasCommand::UBTDecorator_HasCommand()
{
	NodeName = TEXT("Has Command");
}

bool UBTDecorator_HasCommand::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return false;
	uint8 Val = BB->GetValueAsEnum(CommandKey.SelectedKeyName);
	return Val == static_cast<uint8>(RequiredCommand);
}
