#include "AbilitySystem/Abilities/Bard/GA_Bard_SongOfValor.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/Character.h"

UGA_Bard_SongOfValor::UGA_Bard_SongOfValor() { ManaCost = 0.f; SpellMultiplier = 0.f; }

void UGA_Bard_SongOfValor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	Elapsed = 0.f;
	GetWorld()->GetTimerManager().SetTimer(SongHandle, this, &UGA_Bard_SongOfValor::OnSongTick, 1.0f, true);
}

void UGA_Bard_SongOfValor::OnSongTick()
{
	auto* AI = GetCurrentActorInfo();
	if (!AI || !AI->AbilitySystemComponent.IsValid()) { StopSong(); return; }

	UAbilitySystemComponent* ASC = AI->AbilitySystemComponent.Get();
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetMana() < ManaCostPerSecond) { StopSong(); return; }

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Additive, -ManaCostPerSecond);

	ACharacter* Caster = Cast<ACharacter>(AI->AvatarActor.Get());
	if (!Caster || !ValorBuff) { StopSong(); return; }

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(AuraRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, Caster->GetActorLocation(), FQuat::Identity, ECC_Pawn, Shape))
	{
		for (auto& O : Overlaps)
		{
			if (auto* TASI = Cast<IAbilitySystemInterface>(O.GetActor()))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
				{
					FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
					FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ValorBuff, GetAbilityLevel(), Ctx);
					if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}

	Elapsed += 1.0f;
	if (Elapsed >= MaxDuration) StopSong();
}

void UGA_Bard_SongOfValor::StopSong()
{
	GetWorld()->GetTimerManager().ClearTimer(SongHandle);
	auto Handle = GetCurrentAbilitySpecHandle();
	auto* AI = GetCurrentActorInfo();
	auto ActivInfo = GetCurrentActivationInfoRef();
	if (Handle.IsValid() && AI) EndAbility(Handle, AI, ActivInfo, true, false);
}

