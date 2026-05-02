#include "AbilitySystem/Abilities/Bard/GA_Bard_BattleHymn.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

UGA_Bard_BattleHymn::UGA_Bard_BattleHymn() { ManaCost = 40.f; SpellMultiplier = 0.f; }

void UGA_Bard_BattleHymn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !HymnBuff) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(BuffRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			if (auto* TASI = Cast<IAbilitySystemInterface>(O.GetActor()))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
				{
					FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
					FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(HymnBuff, GetAbilityLevel(), Ctx);
					if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
