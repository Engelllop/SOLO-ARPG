#include "AbilitySystem/Abilities/Mage/GA_Mage_Fireball.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

UGA_Mage_Fireball::UGA_Mage_Fireball()
{
	ManaCost = 25.f;
	SpellMultiplier = 1.5f;
}

void UGA_Mage_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !ProjectileClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector SpawnLoc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 100.f + FVector(0, 0, 50.f);
	FRotator SpawnRot = Caster->GetControlRotation();

	FActorSpawnParameters Params;
	Params.Owner = Caster;
	Params.Instigator = Caster;

	AActor* Proj = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLoc, SpawnRot, Params);
	if (Proj)
	{
		if (auto* PMC = Proj->FindComponentByClass<UProjectileMovementComponent>())
		{
			PMC->InitialSpeed = ProjectileSpeed;
			PMC->MaxSpeed = ProjectileSpeed;
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
