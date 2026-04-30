#include "AbilitySystem/Abilities/Druid/GA_Druid_Briars.h"
#include "GameFramework/Character.h"

UGA_Druid_Briars::UGA_Druid_Briars() { ManaCost = 15.f; SpellMultiplier = 0.5f; }

void UGA_Druid_Briars::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !BriarsFieldClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Target = Caster->GetActorLocation() + Caster->GetControlRotation().Vector() * SpawnRange;
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Caster->GetActorLocation(), Target, ECC_Visibility))
		Target = Hit.ImpactPoint;

	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	AActor* Field = GetWorld()->SpawnActor<AActor>(BriarsFieldClass, Target, FRotator::ZeroRotator, P);
	if (Field) Field->SetLifeSpan(FieldDuration);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
