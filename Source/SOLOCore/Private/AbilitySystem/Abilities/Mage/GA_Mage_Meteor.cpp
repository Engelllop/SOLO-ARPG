#include "AbilitySystem/Abilities/Mage/GA_Mage_Meteor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

UGA_Mage_Meteor::UGA_Mage_Meteor()
{
	ManaCost = 100.f;
	SpellMultiplier = 5.0f;
}

void UGA_Mage_Meteor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	auto* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC) ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));

	FVector Target = Caster->GetActorLocation() + Caster->GetControlRotation().Vector() * TargetRange;
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Caster->GetActorLocation(), Target, ECC_Visibility))
	{
		Target = Hit.ImpactPoint;
	}

	if (MeteorClass)
	{
		FVector SpawnLoc = Target + FVector(0, 0, SpawnHeight);
		FActorSpawnParameters P; P.Owner = Caster; P.Instigator = Caster;
		GetWorld()->SpawnActor<AActor>(MeteorClass, SpawnLoc, FRotator::ZeroRotator, P);
	}

	FTimerHandle DelayHandle;
	FTimerDelegate DelayDelegate;
	FVector ImpactLoc = Target;
	DelayDelegate.BindLambda([this, ImpactLoc, Handle, ActorInfo, ActivationInfo, ASC]()
	{
		OnMeteorImpact(ImpactLoc);
		if (ASC) ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.SuperArmor")));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	});
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, DelayDelegate, ImpactDelay, false);
}

void UGA_Mage_Meteor::OnMeteorImpact(FVector ImpactLocation)
{
	auto* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid()) return;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(ImpactRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, ImpactLocation, FQuat::Identity, ECC_Pawn, Shape))
	{
		FGameplayEffectSpecHandle DmgSpec = MakeDamageSpec(ActorInfo);

		for (auto& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (!HitActor || HitActor == ActorInfo->AvatarActor.Get()) continue;

			if (auto* TargetASI = Cast<IAbilitySystemInterface>(HitActor))
			{
				if (auto* TargetASC = TargetASI->GetAbilitySystemComponent())
				{
					if (DmgSpec.IsValid())
						ASC->ApplyGameplayEffectSpecToTarget(*DmgSpec.Data.Get(), TargetASC);
					if (StunEffect)
					{
						FGameplayEffectSpecHandle StunSpec = ASC->MakeOutgoingSpec(StunEffect, GetAbilityLevel(), ASC->MakeEffectContext());
						if (StunSpec.IsValid())
							ASC->ApplyGameplayEffectSpecToTarget(*StunSpec.Data.Get(), TargetASC);
					}
				}
			}
		}
	}

	if (APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get()))
	{
		PC->ClientStartCameraShake(nullptr);
	}
}
