#include "AbilitySystem/Abilities/Weave/GA_Weave_WeaveArrow.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

UGA_Weave_WeaveArrow::UGA_Weave_WeaveArrow() {}

void UGA_Weave_WeaveArrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!Caster || !ASC || !ArrowDamageEffect)
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Start = Caster->GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FVector End = Start + Caster->GetActorForwardVector() * ArrowRange;

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Caster);
	GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(ArrowRadius), Params);

	for (const FHitResult& Hit : Hits)
	{
		if (auto* TASI = Cast<IAbilitySystemInterface>(Hit.GetActor()))
		{
			if (auto* TASC = TASI->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ArrowDamageEffect, GetAbilityLevel(), Ctx);
				if (Spec.IsValid())
				{
					Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.DamageMultiplier"), DamageMultiplier);
					ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
