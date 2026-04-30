#include "AbilitySystem/Abilities/Mage/GA_Mage_GravityField.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_Mage_GravityField::UGA_Mage_GravityField()
{
	ManaCost = 50.f;
	SpellMultiplier = 0.f;
}

void UGA_Mage_GravityField::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !GravityFieldClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Target = Caster->GetActorLocation() + Caster->GetControlRotation().Vector() * SpawnRange;
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Caster->GetActorLocation(), Target, ECC_Visibility))
	{
		Target = Hit.ImpactPoint;
	}

	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	AActor* Field = GetWorld()->SpawnActor<AActor>(GravityFieldClass, Target, FRotator::ZeroRotator, P);
	if (Field)
	{
		Field->SetLifeSpan(FieldDuration);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
