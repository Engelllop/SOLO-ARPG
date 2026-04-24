#include "AI/BehaviorTree/Decorators/BTDecorator_CanAttack.h"
#include "AIController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("Can Attack");

	// Cache once at construction — FGameplayTag lookups by name are hash-map gets but still worth caching
	Tag_Stunned   = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Stunned"),   false);
	Tag_Dead      = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Dead"),       false);
	Tag_Attacking = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Attacking"),  false);
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* AIC  = OwnerComp.GetAIOwner();
	const APawn*         Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
	{
		return false;
	}

	const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Pawn);
	if (!ASCInterface)
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	if (!ASC)
	{
		return false;
	}

	// Explicit blocking states — order matters: dead check first so we skip everything else
	if (ASC->HasMatchingGameplayTag(Tag_Dead))      { return false; }
	if (ASC->HasMatchingGameplayTag(Tag_Stunned))   { return false; }
	if (ASC->HasMatchingGameplayTag(Tag_Attacking)) { return false; }

	return true;
}

FString UBTDecorator_CanAttack::GetStaticDescription() const
{
	return TEXT("Can Attack");
}
