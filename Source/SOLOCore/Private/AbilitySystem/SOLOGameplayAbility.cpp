#include "AbilitySystem/SOLOGameplayAbility.h"
#include "Attributes/SOLOAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

USOLOGameplayAbility::USOLOGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Propagate designer-set tag containers into the base GAS block/cancel lists
	ActivationBlockedTags.AppendTags(BlockAbilitiesWithTags);
	CancelAbilitiesWithTag.AppendTags(CancelAbilitiesWithTags);
}

bool USOLOGameplayAbility::CanActivateAbility(
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

const USOLOAttributeSet* USOLOGameplayAbility::GetAttributeSet() const
{
	if (!GetCurrentActorInfo() || !GetCurrentActorInfo()->AbilitySystemComponent.IsValid())
	{
		return nullptr;
	}

	return GetCurrentActorInfo()->AbilitySystemComponent->GetSet<USOLOAttributeSet>();
}

ACharacter* USOLOGameplayAbility::GetPlayerCharacter() const
{
	if (!GetCurrentActorInfo())
	{
		return nullptr;
	}

	return Cast<ACharacter>(GetCurrentActorInfo()->AvatarActor.Get());
}
