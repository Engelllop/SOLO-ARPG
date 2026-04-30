#include "AbilitySystem/Abilities/Mage/GA_Mage_ManaStorm.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

UGA_Mage_ManaStorm::UGA_Mage_ManaStorm()
{
	ManaCost = 0.f;
	SpellMultiplier = 0.4f;
}

void UGA_Mage_ManaStorm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	TickCount = 0;
	ElapsedTime = 0.f;

	GetWorld()->GetTimerManager().SetTimer(ChannelHandle, this, &UGA_Mage_ManaStorm::OnChannelTick, TickInterval, true);
}

void UGA_Mage_ManaStorm::OnChannelTick()
{
	auto* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		StopChannel();
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const USOLOAttributeSet* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs)
	{
		StopChannel();
		return;
	}

	float ManaDrain = ManaCostPerSecond * TickInterval;
	if (Attrs->GetMana() < ManaDrain)
	{
		StopChannel();
		return;
	}

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -ManaDrain);

	float CurrentMult = BaseMultiplier + (TickCount * StackingBonus);
	FGameplayEffectSpecHandle Spec = MakeDamageSpec(ActorInfo, CurrentMult);
	if (!Spec.IsValid()) { StopChannel(); return; }

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { StopChannel(); return; }

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && HitActor != Caster)
			{
				if (auto* TargetASI = Cast<IAbilitySystemInterface>(HitActor))
				{
					if (auto* TargetASC = TargetASI->GetAbilitySystemComponent())
					{
						ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
					}
				}
			}
		}
	}

	TickCount++;
	ElapsedTime += TickInterval;
	if (ElapsedTime >= MaxDuration) StopChannel();
}

void UGA_Mage_ManaStorm::StopChannel()
{
	GetWorld()->GetTimerManager().ClearTimer(ChannelHandle);
	auto* Handle = GetCurrentAbilitySpecHandle();
	auto* ActorInfo = GetCurrentActorInfo();
	auto* ActivationInfo = GetCurrentActivationInfoRef();
	if (Handle && ActorInfo && ActivationInfo)
	{
		EndAbility(*Handle, ActorInfo, *ActivationInfo, true, false);
	}
}
