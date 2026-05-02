#include "AbilitySystem/Abilities/Bard/GA_Bard_BardLament.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

UGA_Bard_BardLament::UGA_Bard_BardLament() { ManaCost = 50.f; SpellMultiplier = 0.f; }

void UGA_Bard_BardLament::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(LamentRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			AActor* Target = O.GetActor();
			if (!Target || Target == Caster) continue;
			if (auto* TASI = Cast<IAbilitySystemInterface>(Target))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
				{
					FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();

					if (FearEffect)
					{
						FGameplayEffectSpecHandle FearSpec = ASC->MakeOutgoingSpec(FearEffect, GetAbilityLevel(), Ctx);
						if (FearSpec.IsValid()) ASC->ApplyGameplayEffectSpecToTarget(*FearSpec.Data.Get(), TASC);
					}

					// Apply lament debuff after 3s (fear duration)
					FTimerHandle LamentHandle;
					FTimerDelegate Del;
					Del.BindLambda([ASC, TASC, LamentDebuff = this->LamentDebuff, Level = GetAbilityLevel()]()
					{
						if (!TASC || !LamentDebuff) return;
						FGameplayEffectContextHandle LCtx = ASC->MakeEffectContext();
						FGameplayEffectSpecHandle LSpec = ASC->MakeOutgoingSpec(LamentDebuff, Level, LCtx);
						if (LSpec.IsValid()) ASC->ApplyGameplayEffectSpecToTarget(*LSpec.Data.Get(), TASC);
					});
					GetWorld()->GetTimerManager().SetTimer(LamentHandle, Del, 3.0f, false);
				}
			}
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
