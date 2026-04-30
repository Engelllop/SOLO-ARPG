#include "AbilitySystem/Abilities/Necromancer/GA_Necro_BloodRitual.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "AI/Minions/SOLOSkeletonMinion.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UGA_Necro_BloodRitual::UGA_Necro_BloodRitual() { ManaCost = 30.f; SpellMultiplier = 0.f; }

void UGA_Necro_BloodRitual::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetHealth() <= HPCost + 1.f)
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -HPCost);

	FTimerHandle ChannelHandle;
	FTimerDelegate Del;
	Del.BindLambda([this, Handle, ActorInfo, ActivationInfo, ASC]()
	{
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

		ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
		if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

		ASOLOSkeletonMinion* NearestSkel = nullptr;
		float NearestDist = 1000.f;
		TArray<AActor*> All;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASOLOSkeletonMinion::StaticClass(), All);
		for (AActor* A : All)
		{
			auto* S = Cast<ASOLOSkeletonMinion>(A);
			if (S && S->GetMinionOwner() == Caster)
			{
				float D = FVector::Dist(Caster->GetActorLocation(), S->GetActorLocation());
				if (D < NearestDist) { NearestDist = D; NearestSkel = S; }
			}
		}

		if (NearestSkel)
		{
			NearestSkel->bIsElite = true;
			NearestSkel->ApplyStatBuff(1.5f);
			if (EliteBuffEffect)
			{
				FGameplayEffectContextHandle Ctx = NearestSkel->GetAbilitySystemComponent()->MakeEffectContext();
				auto Spec = NearestSkel->GetAbilitySystemComponent()->MakeOutgoingSpec(EliteBuffEffect, 1.f, Ctx);
				if (Spec.IsValid()) NearestSkel->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	});
	GetWorld()->GetTimerManager().SetTimer(ChannelHandle, Del, ChannelTime, false);
}
