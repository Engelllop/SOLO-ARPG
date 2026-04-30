#include "AbilitySystem/Abilities/Necromancer/GA_Necro_LifeDrain.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"

UGA_Necro_LifeDrain::UGA_Necro_LifeDrain()
{
	ManaCost = 0.f;
	SpellMultiplier = 0.5f;
}

void UGA_Necro_LifeDrain::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	GetWorld()->GetTimerManager().SetTimer(DrainHandle, this, &UGA_Necro_LifeDrain::OnDrainTick, 1.0f, true);
}

void UGA_Necro_LifeDrain::OnDrainTick()
{
	auto* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid()) { StopDrain(); return; }

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetMana() < ManaCostPerSecond) { StopDrain(); return; }

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -ManaCostPerSecond);

	float Damage = Attrs->GetMagicDamage() * DamagePerSecond;
	float Heal = Damage * HealMultiplier;
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, Heal);
}

void UGA_Necro_LifeDrain::StopDrain()
{
	GetWorld()->GetTimerManager().ClearTimer(DrainHandle);
	auto* Handle = GetCurrentAbilitySpecHandle();
	auto* AI = GetCurrentActorInfo();
	auto* ActivInfo = GetCurrentActivationInfoRef();
	if (Handle && AI && ActivInfo) EndAbility(*Handle, AI, *ActivInfo, true, false);
}
