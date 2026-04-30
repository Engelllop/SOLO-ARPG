#include "AbilitySystem/Abilities/Druid/GA_Druid_BarkSkin.h"
#include "AbilitySystemComponent.h"

UGA_Druid_BarkSkin::UGA_Druid_BarkSkin() { ManaCost = 25.f; SpellMultiplier = 0.f; }

void UGA_Druid_BarkSkin::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC && BarkSkinEffect)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(BarkSkinEffect, GetAbilityLevel(), Ctx);
		if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
