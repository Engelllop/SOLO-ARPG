#include "AbilitySystem/Abilities/Mage/GA_Mage_Teleport.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_Mage_Teleport::UGA_Mage_Teleport()
{
	ManaCost = 40.f;
	SpellMultiplier = 0.f;
}

void UGA_Mage_Teleport::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Origin = Caster->GetActorLocation();
	FVector Dir = Caster->GetLastMovementInputVector();
	if (Dir.IsNearlyZero()) Dir = Caster->GetActorForwardVector();
	Dir.Z = 0.f; Dir.Normalize();

	FVector Dest = Origin + Dir * TeleportDistance;
	FHitResult Hit;
	Caster->SetActorLocation(Dest, true, &Hit);
	if (Hit.bBlockingHit) Caster->SetActorLocation(Hit.Location);

	// Tag for uninterruptible during teleport
	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC) ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Uninterruptible")));

	if (TeleportVFXClass)
	{
		GetWorld()->SpawnActor<AActor>(TeleportVFXClass, Origin, FRotator::ZeroRotator);
		GetWorld()->SpawnActor<AActor>(TeleportVFXClass, Caster->GetActorLocation(), FRotator::ZeroRotator);
	}

	if (ASC) ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Uninterruptible")));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
