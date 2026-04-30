#include "AbilitySystem/Abilities/Necromancer/GA_Necro_ArmyOfTheDead.h"
#include "AbilitySystemComponent.h"
#include "AI/Minions/SOLOSkeletonMinion.h"
#include "GameFramework/Character.h"

UGA_Necro_ArmyOfTheDead::UGA_Necro_ArmyOfTheDead() { ManaCost = 100.f; SpellMultiplier = 0.f; }

void UGA_Necro_ArmyOfTheDead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC) ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !SkeletonClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FTimerHandle Ch;
	FTimerDelegate Del;
	Del.BindLambda([this, Caster, Handle, ActorInfo, ActivationInfo, ASC]()
	{
		if (ASC) ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

		for (int32 i = 0; i < SpawnCount; ++i)
		{
			float Angle = (2.0f * PI * i) / SpawnCount;
			FVector Offset(FMath::Cos(Angle) * SpawnRadius, FMath::Sin(Angle) * SpawnRadius, 0.f);
			FVector Loc = Caster->GetActorLocation() + Offset;

			FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
			auto* Skel = GetWorld()->SpawnActor<ASOLOSkeletonMinion>(SkeletonClass, Loc, Caster->GetActorRotation(), P);
			if (Skel)
			{
				Skel->InitMinion(Caster, 0.8f, 0.5f, ArmyDuration);
				Skel->ApplyStatBuff(StatMultiplier);
			}
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	});
	GetWorld()->GetTimerManager().SetTimer(Ch, Del, ChannelTime, false);
}
