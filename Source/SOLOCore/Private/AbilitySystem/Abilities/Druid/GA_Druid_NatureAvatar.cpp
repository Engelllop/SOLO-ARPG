#include "AbilitySystem/Abilities/Druid/GA_Druid_NatureAvatar.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/Character.h"

UGA_Druid_NatureAvatar::UGA_Druid_NatureAvatar() { ManaCost = 100.f; SpellMultiplier = 0.f; }

void UGA_Druid_NatureAvatar::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	Caster->SetActorScale3D(AvatarScale);

	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC) ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Uninterruptible")));

	GetWorld()->GetTimerManager().SetTimer(AuraTickHandle, this, &UGA_Druid_NatureAvatar::OnAuraTick, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &UGA_Druid_NatureAvatar::EndAvatar, AvatarDuration, false);
}

void UGA_Druid_NatureAvatar::OnAuraTick()
{
	auto* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) return;

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) return;

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(HealAuraRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			AActor* A = O.GetActor();
			if (!A || A == Caster) continue;
			if (auto* TASI = Cast<IAbilitySystemInterface>(A))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
				{
					const auto* Attrs = TASC->GetSet<USOLOAttributeSet>();
					if (Attrs) TASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, Attrs->GetMaxHealth() * HealPercentPerSecond);
				}
			}
		}
	}
}

void UGA_Druid_NatureAvatar::EndAvatar()
{
	GetWorld()->GetTimerManager().ClearTimer(AuraTickHandle);

	auto* ActorInfo = GetCurrentActorInfo();
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		ActorInfo->AvatarActor->SetActorScale3D(FVector::OneVector);
		if (auto* ASC = ActorInfo->AbilitySystemComponent.Get())
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Uninterruptible")));
	}

	auto Handle = GetCurrentAbilitySpecHandle();
	auto* AI = GetCurrentActorInfo();
	auto ActivInfo = GetCurrentActivationInfoRef();
	if (Handle.IsValid() && AI) EndAbility(Handle, AI, ActivInfo, true, false);
}

