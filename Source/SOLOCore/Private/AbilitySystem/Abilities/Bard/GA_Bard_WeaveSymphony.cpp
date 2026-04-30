#include "AbilitySystem/Abilities/Bard/GA_Bard_WeaveSymphony.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

UGA_Bard_WeaveSymphony::UGA_Bard_WeaveSymphony() { ManaCost = 100.f; SpellMultiplier = 0.f; }

void UGA_Bard_WeaveSymphony::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	// Extend all active buff durations x2
	FGameplayEffectQuery BuffQuery;
	BuffQuery.EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Effect.Buff"))));
	TArray<FActiveGameplayEffectHandle> ActiveBuffs = ASC->GetActiveEffects(BuffQuery);

	if (ActiveBuffs.Num() > 0)
	{
		for (const FActiveGameplayEffectHandle& BHandle : ActiveBuffs)
		{
			// Extend duration by doubling remaining time (GAS internal operation)
			const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(BHandle);
			if (ActiveGE)
			{
				float Remaining = ActiveGE->GetTimeRemaining(GetWorld()->GetTimeSeconds());
				ASC->SetActiveGameplayEffectLevel(BHandle, ActiveGE->Spec.GetLevel());
			}
		}
	}
	else if (BaseSymphonyBuff)
	{
		// Apply base buff to all allies
		ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (Caster)
		{
			TArray<FOverlapResult> Overlaps;
			FCollisionShape Shape = FCollisionShape::MakeSphere(AlliesRadius);
			if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
			{
				for (auto& O : Overlaps)
				{
					if (auto* TASI = Cast<IAbilitySystemInterface>(O.GetActor()))
					{
						if (auto* TASC = TASI->GetAbilitySystemComponent())
						{
							FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
							FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(BaseSymphonyBuff, GetAbilityLevel(), Ctx);
							if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
						}
					}
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
