#include "AbilitySystem/Abilities/Necromancer/GA_Necro_TouchOfDeath.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/Character.h"

UGA_Necro_TouchOfDeath::UGA_Necro_TouchOfDeath()
{
	ManaCost = 15.f;
	SpellMultiplier = 1.2f;
}

void UGA_Necro_TouchOfDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(TouchRange);
	FVector Loc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * TouchRange;

	if (GetWorld()->OverlapMultiByChannel(Overlaps, Loc, FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			AActor* Hit = O.GetActor();
			if (!Hit || Hit == Caster) continue;
			auto* TargetASI = Cast<IAbilitySystemInterface>(Hit);
			if (!TargetASI) continue;
			auto* TargetASC = TargetASI->GetAbilitySystemComponent();
			if (!TargetASC) continue;

			FGameplayEffectSpecHandle Spec = MakeDamageSpec(ActorInfo);
			if (Spec.IsValid())
			{
				ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
			}

			const auto* TargetAttrs = TargetASC->GetSet<USOLOAttributeSet>();
			float Drain = LifeDrainPercent;
			if (TargetAttrs && TargetAttrs->GetHealth() <= 0.f) Drain = KillDrainPercent;

			float HealAmount = SpellMultiplier * Drain * ActorInfo->AbilitySystemComponent->GetSet<USOLOAttributeSet>()->GetMagicDamage();
			ActorInfo->AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, HealAmount);
			break;
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
