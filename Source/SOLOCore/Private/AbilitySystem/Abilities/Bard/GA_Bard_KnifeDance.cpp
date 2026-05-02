#include "AbilitySystem/Abilities/Bard/GA_Bard_KnifeDance.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Attributes/SOLOAttributeSet.h"

UGA_Bard_KnifeDance::UGA_Bard_KnifeDance() { ManaCost = 10.f; SpellMultiplier = 1.0f; }

void UGA_Bard_KnifeDance::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	const USOLOAttributeSet* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetStamina() < StaminaCost)
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -StaminaCost);

	FActiveGameplayEffectHandle EvasionHandle;
	if (EvasionEffect)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(EvasionEffect, GetAbilityLevel(), Ctx);
		if (Spec.IsValid()) EvasionHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	TWeakObjectPtr<UAbilitySystemComponent> WeakASC(ASC);
	TWeakObjectPtr<AActor> WeakAvatar(ActorInfo->AvatarActor.Get());

	for (int32 i = 0; i < 3; i++)
	{
		float Delay = 0.4f * (i + 1);
		FTimerHandle HitHandle;
		GetWorld()->GetTimerManager().SetTimer(HitHandle,
			FTimerDelegate::CreateWeakLambda(this, [this, WeakASC, WeakAvatar]()
			{ PerformHit(WeakASC.Get(), WeakAvatar.Get()); }),
			Delay, false);
	}

	FTimerHandle EndHandle;
	GetWorld()->GetTimerManager().SetTimer(EndHandle,
		FTimerDelegate::CreateWeakLambda(this, [this, Handle, ActivationInfo, WeakASC, EvasionHandle]()
		{
			if (UAbilitySystemComponent* LASC = WeakASC.Get())
				if (EvasionHandle.IsValid()) LASC->RemoveActiveGameplayEffect(EvasionHandle);
			if (CurrentActorInfo) EndAbility(Handle, CurrentActorInfo, ActivationInfo, true, false);
		}), 1.6f, false);
}

void UGA_Bard_KnifeDance::PerformHit(UAbilitySystemComponent* ASC, AActor* AvatarActor)
{
	ACharacter* Caster = Cast<ACharacter>(AvatarActor);
	if (!Caster || !ASC || !KnifeDamageEffect) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(HitRadius);
	if (!GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape)) return;

	for (auto& O : Overlaps)
	{
		AActor* Target = O.GetActor();
		if (!Target || Target == Caster) continue;
		if (auto* TASI = Cast<IAbilitySystemInterface>(Target))
		{
			if (auto* TASC = TASI->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(KnifeDamageEffect, GetAbilityLevel(), Ctx);
				if (Spec.IsValid())
				{
					Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.SpellMultiplier"), SpellMultiplier);
					ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}
}
