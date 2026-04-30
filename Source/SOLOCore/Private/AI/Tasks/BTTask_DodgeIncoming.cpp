#include "AI/Tasks/BTTask_DodgeIncoming.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UBTTask_DodgeIncoming::UBTTask_DodgeIncoming()
{
	NodeName = TEXT("Dodge Incoming");
}

EBTNodeResult::Type UBTTask_DodgeIncoming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	if (auto* IASI = Cast<IAbilitySystemInterface>(Controller->GetPawn()))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayTagContainer Tags;
			Tags.AddTag(DodgeAbilityTag);
			return ASC->TryActivateAbilitiesByTag(Tags)
				? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}
