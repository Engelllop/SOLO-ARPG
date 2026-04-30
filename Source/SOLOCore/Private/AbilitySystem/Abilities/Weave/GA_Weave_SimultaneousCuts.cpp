#include "AbilitySystem/Abilities/Weave/GA_Weave_SimultaneousCuts.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Attributes/SOLOAttributeSet.h"

UGA_Weave_SimultaneousCuts::UGA_Weave_SimultaneousCuts() {}

void UGA_Weave_SimultaneousCuts::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!ASC || !Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	// Require > 50% HP; cost is 50% MaxHP
	const USOLOAttributeSet* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetHealth() <= Attrs->GetMaxHealth() * 0.5f)
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -(Attrs->GetMaxHealth() * 0.5f));

	// Three cones spread at 0°, 120°, 240°
	for (int32 i = 0; i < 3; i++)
		PerformConeSweep(ASC, Caster, i * 120.f);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Weave_SimultaneousCuts::PerformConeSweep(UAbilitySystemComponent* ASC, ACharacter* Caster, float AngleDeg)
{
	if (!CutDamageEffect) return;

	FVector Forward = Caster->GetActorForwardVector().RotateAngleAxis(AngleDeg, FVector::UpVector);
	FVector Origin = Caster->GetActorLocation();
	float CosHalf = FMath::Cos(FMath::DegreesToRadians(ConeHalfAngleDeg));

	TArray<FOverlapResult> Overlaps;
	if (!GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(ConeRange))) return;

	for (auto& O : Overlaps)
	{
		AActor* Target = O.GetActor();
		if (!Target || Target == Caster) continue;
		FVector ToTarget = (Target->GetActorLocation() - Origin).GetSafeNormal();
		if (FVector::DotProduct(Forward, ToTarget) < CosHalf) continue;

		if (auto* TASI = Cast<IAbilitySystemInterface>(Target))
		{
			if (auto* TASC = TASI->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(CutDamageEffect, GetAbilityLevel(), Ctx);
				if (Spec.IsValid())
				{
					Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.DamageMultiplier"), DamageMultiplier);
					// Tag signals execution to bypass armor
					Spec.Data->AddDynamicAssetTag(FGameplayTag::RequestGameplayTag("Damage.IgnoreArmor"));
					ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}
}
