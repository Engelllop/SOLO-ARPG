#include "AbilitySystem/Abilities/Bard/GA_Bard_HealingMelody.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/Character.h"

UGA_Bard_HealingMelody::UGA_Bard_HealingMelody() { ManaCost = 0.f; SpellMultiplier = 0.5f; }

void UGA_Bard_HealingMelody::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	Elapsed = 0.f;
	GetWorld()->GetTimerManager().SetTimer(MelodyHandle, this, &UGA_Bard_HealingMelody::OnMelodyTick, 1.0f, true);
}

void UGA_Bard_HealingMelody::OnMelodyTick()
{
	auto* AI = GetCurrentActorInfo();
	if (!AI || !AI->AbilitySystemComponent.IsValid()) return;

	UAbilitySystemComponent* ASC = AI->AbilitySystemComponent.Get();
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetMana() < ManaCostPerSecond)
	{
		GetWorld()->GetTimerManager().ClearTimer(MelodyHandle);
		auto* Handle = GetCurrentAbilitySpecHandle(); auto* ActivInfo = GetCurrentActivationInfoRef();
		if (Handle && AI && ActivInfo) EndAbility(*Handle, AI, *ActivInfo, true, false);
		return;
	}

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -ManaCostPerSecond);

	ACharacter* Caster = Cast<ACharacter>(AI->AvatarActor.Get());
	if (!Caster) return;

	float HealAmount = Attrs->GetMagicDamage() * SpellMultiplier;
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(HealRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			if (auto* TASI = Cast<IAbilitySystemInterface>(O.GetActor()))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
					TASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealAmount);
			}
		}
	}

	Elapsed += 1.f;
	if (Elapsed >= MaxDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(MelodyHandle);
		auto* Handle = GetCurrentAbilitySpecHandle(); auto* ActivInfo = GetCurrentActivationInfoRef();
		if (Handle && AI && ActivInfo) EndAbility(*Handle, AI, *ActivInfo, true, false);
	}
}
