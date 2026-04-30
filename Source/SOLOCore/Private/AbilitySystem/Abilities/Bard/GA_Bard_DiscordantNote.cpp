#include "AbilitySystem/Abilities/Bard/GA_Bard_DiscordantNote.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

UGA_Bard_DiscordantNote::UGA_Bard_DiscordantNote() { ManaCost = 20.f; SpellMultiplier = 1.5f; }

void UGA_Bard_DiscordantNote::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !NoteProjectileClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Loc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 100.f + FVector(0,0,50.f);
	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	GetWorld()->SpawnActor<AActor>(NoteProjectileClass, Loc, Caster->GetControlRotation(), P);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
