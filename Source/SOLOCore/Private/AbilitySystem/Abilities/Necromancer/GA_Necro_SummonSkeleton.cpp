#include "AbilitySystem/Abilities/Necromancer/GA_Necro_SummonSkeleton.h"
#include "AI/Minions/SOLOSkeletonMinion.h"
#include "GameFramework/Character.h"

UGA_Necro_SummonSkeleton::UGA_Necro_SummonSkeleton()
{
	ManaCost = 40.f;
	SpellMultiplier = 0.f;
}

void UGA_Necro_SummonSkeleton::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	CleanupDeadSkeletons();
	if (ActiveSkeletons.Num() >= MaxSkeletons)
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !SkeletonClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector SpawnLoc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 200.f;
	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	ASOLOSkeletonMinion* Skel = GetWorld()->SpawnActor<ASOLOSkeletonMinion>(SkeletonClass, SpawnLoc, Caster->GetActorRotation(), P);
	if (Skel)
	{
		Skel->InitMinion(Caster, AttackScale, HPScale, MinionDuration);
		ActiveSkeletons.Add(Skel);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Necro_SummonSkeleton::CleanupDeadSkeletons()
{
	ActiveSkeletons.RemoveAll([](const TWeakObjectPtr<AActor>& S) { return !S.IsValid(); });
}
