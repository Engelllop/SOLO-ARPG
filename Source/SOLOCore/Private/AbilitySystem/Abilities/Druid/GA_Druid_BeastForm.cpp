#include "AbilitySystem/Abilities/Druid/GA_Druid_BeastForm.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"

UGA_Druid_BeastForm::UGA_Druid_BeastForm() { ManaCost = 50.f; SpellMultiplier = 0.f; }

void UGA_Druid_BeastForm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

	TSubclassOf<UGameplayEffect> FormEffect = (DefaultForm == EBeastFormType::Bear) ? BearFormEffect : WolfFormEffect;
	if (FormEffect)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(FormEffect, GetAbilityLevel(), Ctx);
		if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

	// Revert after Duration
	FTimerHandle RevertHandle;
	FTimerDelegate Del;
	Del.BindLambda([ASC]()
	{
		// Heal 15% MaxHP on revert
		const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
		if (Attrs) ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, Attrs->GetMaxHealth() * 0.15f);
	});
	GetWorld()->GetTimerManager().SetTimer(RevertHandle, Del, Duration, false);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
