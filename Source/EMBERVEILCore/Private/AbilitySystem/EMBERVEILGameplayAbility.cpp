#include "AbilitySystem/EMBERVEILGameplayAbility.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UEMBERVEILGameplayAbility::UEMBERVEILGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Propagate designer-set tag containers into the base GAS block/cancel lists
	ActivationBlockedTags.AppendTags(BlockAbilitiesWithTags);
	CancelAbilitiesWithTag.AppendTags(CancelAbilitiesWithTags);
}

bool UEMBERVEILGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	// Dead characters cannot activate abilities
	if (ActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead"))))
	{
		return false;
	}

	return true;
}

const UEMBERVEILAttributeSet* UEMBERVEILGameplayAbility::GetAttributeSet() const
{
	if (!GetCurrentActorInfo() || !GetCurrentActorInfo()->AbilitySystemComponent.IsValid())
	{
		return nullptr;
	}

	return GetCurrentActorInfo()->AbilitySystemComponent->GetSet<UEMBERVEILAttributeSet>();
}

ACharacter* UEMBERVEILGameplayAbility::GetPlayerCharacter() const
{
	if (!GetCurrentActorInfo())
	{
		return nullptr;
	}

	return Cast<ACharacter>(GetCurrentActorInfo()->AvatarActor.Get());
}
