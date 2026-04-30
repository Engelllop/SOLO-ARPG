#include "AbilitySystem/Abilities/Druid/GA_Druid_SummonWolf.h"
#include "AI/Minions/SOLOWolfMinion.h"
#include "GameFramework/Character.h"

UGA_Druid_SummonWolf::UGA_Druid_SummonWolf() { ManaCost = 35.f; SpellMultiplier = 0.f; }

void UGA_Druid_SummonWolf::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ActiveWolves.RemoveAll([](const TWeakObjectPtr<AActor>& W){ return !W.IsValid(); });
	if (ActiveWolves.Num() >= MaxWolves) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !WolfClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Loc = Caster->GetActorLocation() + Caster->GetActorRightVector() * 200.f;
	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	ASOLOWolfMinion* Wolf = GetWorld()->SpawnActor<ASOLOWolfMinion>(WolfClass, Loc, Caster->GetActorRotation(), P);
	if (Wolf) { Wolf->InitMinion(Caster, AttackScale, HPScale, WolfDuration); ActiveWolves.Add(Wolf); }

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
