#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Combat/SOLOManaComponent.h"

USOLOMageSpellBase::USOLOMageSpellBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void USOLOMageSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ConsumeManaCost(ActorInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	NotifyManaComponent(ActorInfo);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USOLOMageSpellBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool USOLOMageSpellBase::ConsumeManaCost(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid()) return false;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const USOLOAttributeSet* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs) return false;

	float EffectiveCost = ManaCost;

	if (auto* ManaComp = ActorInfo->AvatarActor->FindComponentByClass<USOLOManaComponent>())
	{
		EffectiveCost *= ManaComp->GetManaCostMultiplier();

		if (ManaComp->ShouldSpellFail())
		{
			return false;
		}

		if (Attrs->GetMana() < EffectiveCost)
		{
			return ManaComp->TryOverload(EffectiveCost);
		}
	}

	if (Attrs->GetMana() < EffectiveCost) return false;

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -EffectiveCost);
	return true;
}

void USOLOMageSpellBase::NotifyManaComponent(const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) return;
	if (auto* ManaComp = ActorInfo->AvatarActor->FindComponentByClass<USOLOManaComponent>())
	{
		ManaComp->OnSpellCast();
	}
}

FGameplayEffectSpecHandle USOLOMageSpellBase::MakeDamageSpec(const FGameplayAbilityActorInfo* ActorInfo, float CustomMultiplier) const
{
	if (!DamageEffect || !ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
		return FGameplayEffectSpecHandle();

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	Ctx.AddSourceObject(ActorInfo->AvatarActor.Get());

	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(DamageEffect, GetAbilityLevel(), Ctx);
	if (Spec.IsValid())
	{
		float Mult = CustomMultiplier > 0.f ? CustomMultiplier : SpellMultiplier;
		Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.SpellMultiplier")), Mult);
	}
	return Spec;
}
