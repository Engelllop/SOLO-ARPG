#include "AI/BehaviorTree/Tasks/BTTask_ActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_ActivateAbility::UBTTask_ActivateAbility()
{
	NodeName = TEXT("Activate Ability");
	// Latent tasks require bNotifyTick only when polling; we use a delegate instead
	bNotifyTick = false;
	bCreateNodeInstance = true; // each node owns its delegate handle
}

EBTNodeResult::Type UBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)
	{
		return EBTNodeResult::Failed;
	}

	APawn* ControlledPawn = AIC->GetPawn();
	// Enemigos: ASC en el propio ASOLOEnemyCharacter (IAbilitySystemInterface).
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(ControlledPawn);
	if (!ASCInterface)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
	if (!ASC || !AbilityTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);

	const bool bActivated = ASC->TryActivateAbilitiesByTag(TagContainer);
	if (!bActivated)
	{
		return EBTNodeResult::Failed;
	}

	if (!bWaitForAbilityEnd)
	{
		return EBTNodeResult::Succeeded;
	}

	// Register for the end delegate so we can finish the latent task cleanly
	CachedASC = ASC;
	UBehaviorTreeComponent* BTComp = &OwnerComp;

	AbilityEndedHandle = ASC->OnAbilityEnded.AddLambda(
		[this, BTComp](const FAbilityEndedData& EndData)
		{
			const UGameplayAbility* Ended = EndData.AbilityThatEnded;
			if (!Ended || !Ended->AbilityTags.HasTag(AbilityTag))
			{
				return;
			}
			if (CachedASC.IsValid())
			{
				CachedASC->OnAbilityEnded.Remove(AbilityEndedHandle);
			}
			FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
		}
	);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedASC.IsValid())
	{
		CachedASC->OnAbilityEnded.Remove(AbilityEndedHandle);

		// Cancel any spec that carries the ability tag
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(AbilityTag);
		CachedASC->CancelAbilities(&TagContainer);
	}
	return EBTNodeResult::Aborted;
}

FString UBTTask_ActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("Activate: %s"), *AbilityTag.ToString());
}
