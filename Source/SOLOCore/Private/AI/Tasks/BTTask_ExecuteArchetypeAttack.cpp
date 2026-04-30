#include "AI/Tasks/BTTask_ExecuteArchetypeAttack.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UBTTask_ExecuteArchetypeAttack::UBTTask_ExecuteArchetypeAttack()
{
	NodeName = TEXT("Execute Archetype Attack");
}

EBTNodeResult::Type UBTTask_ExecuteArchetypeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	if (auto* IASI = Cast<IAbilitySystemInterface>(Controller->GetPawn()))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayTagContainer Tags;
			Tags.AddTag(AttackAbilityTag);
			if (ASC->TryActivateAbilitiesByTag(Tags))
				return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
