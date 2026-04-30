#include "AbilitySystem/Abilities/Druid/GA_Druid_NaturalBond.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_Druid_NaturalBond::UGA_Druid_NaturalBond() { ManaCost = 20.f; SpellMultiplier = 1.5f; }

void UGA_Druid_NaturalBond::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	// Apply HoT to self (target selection via Blueprint targeting in full impl)
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC && HoTEffect)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(HoTEffect, GetAbilityLevel(), Ctx);
		if (Spec.IsValid())
		{
			Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.SpellMultiplier")), SpellMultiplier);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
