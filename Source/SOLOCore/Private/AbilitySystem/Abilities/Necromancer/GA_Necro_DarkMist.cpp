#include "AbilitySystem/Abilities/Necromancer/GA_Necro_DarkMist.h"
#include "GameFramework/Character.h"

UGA_Necro_DarkMist::UGA_Necro_DarkMist() { ManaCost = 35.f; SpellMultiplier = 0.f; }

void UGA_Necro_DarkMist::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !MistFieldClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Loc = Caster->GetActorLocation() + Caster->GetControlRotation().Vector() * 800.f;
	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	AActor* Field = GetWorld()->SpawnActor<AActor>(MistFieldClass, Loc, FRotator::ZeroRotator, P);
	if (Field) Field->SetLifeSpan(FieldDuration);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
