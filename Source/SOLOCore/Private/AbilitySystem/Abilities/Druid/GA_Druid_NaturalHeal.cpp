#include "AbilitySystem/Abilities/Druid/GA_Druid_NaturalHeal.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"

UGA_Druid_NaturalHeal::UGA_Druid_NaturalHeal() { ManaCost = 30.f; SpellMultiplier = 3.0f; }

void UGA_Druid_NaturalHeal::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (Attrs)
	{
		float HealAmount = Attrs->GetMagicDamage() * SpellMultiplier;
		ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealAmount);
	}

	// Remove poison and bleed
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Poisoned")));
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Bleeding")));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
