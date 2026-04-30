#include "AbilitySystem/Abilities/Mage/GA_Mage_Frostbolt.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

UGA_Mage_Frostbolt::UGA_Mage_Frostbolt()
{
	ManaCost = 20.f;
	SpellMultiplier = 1.0f;
}

void UGA_Mage_Frostbolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster || !ProjectileClass) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Loc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 100.f + FVector(0, 0, 50.f);
	FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
	AActor* Proj = GetWorld()->SpawnActor<AActor>(ProjectileClass, Loc, Caster->GetControlRotation(), P);
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
