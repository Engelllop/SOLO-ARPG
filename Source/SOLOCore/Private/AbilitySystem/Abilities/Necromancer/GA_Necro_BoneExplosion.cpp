#include "AbilitySystem/Abilities/Necromancer/GA_Necro_BoneExplosion.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AI/Minions/SOLOSkeletonMinion.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UGA_Necro_BoneExplosion::UGA_Necro_BoneExplosion() { ManaCost = 25.f; SpellMultiplier = 3.0f; }

void UGA_Necro_BoneExplosion::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	// Find nearest skeleton
	ASOLOSkeletonMinion* NearestSkel = nullptr;
	float NearestDist = SearchRadius;
	TArray<AActor*> Skeletons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASOLOSkeletonMinion::StaticClass(), Skeletons);
	for (AActor* S : Skeletons)
	{
		auto* Skel = Cast<ASOLOSkeletonMinion>(S);
		if (!Skel || !Skel->IsAlive() || Skel->GetMinionOwner() != Caster) continue;
		float Dist = FVector::Dist(Caster->GetActorLocation(), Skel->GetActorLocation());
		if (Dist < NearestDist) { NearestDist = Dist; NearestSkel = Skel; }
	}

	if (!NearestSkel) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector ExplosionCenter = NearestSkel->GetActorLocation();
	NearestSkel->Kill();

	// AOE damage
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRadius);
	if (GetWorld()->OverlapMultiByChannel(Overlaps, ExplosionCenter, FQuat::Identity, ECC_Pawn, Shape))
	{
		FGameplayEffectSpecHandle Spec = MakeDamageSpec(ActorInfo);
		for (auto& O : Overlaps)
		{
			AActor* Hit = O.GetActor();
			if (!Hit || Hit == Caster || Cast<ASOLOSkeletonMinion>(Hit)) continue;
			if (auto* TASI = Cast<IAbilitySystemInterface>(Hit))
			{
				if (auto* TASC = TASI->GetAbilitySystemComponent())
				{
					if (Spec.IsValid())
						ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TASC);
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
