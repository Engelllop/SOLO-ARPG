#include "AI/SOLOCompanionCharacter.h"
#include "AbilitySystemComponent.h"

ASOLOCompanionCharacter::ASOLOCompanionCharacter() {}

void ASOLOCompanionCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitCompanion(CompanionArchetype);
}

void ASOLOCompanionCharacter::InitCompanion(ESOLOCompanionArchetype Archetype)
{
	CompanionArchetype = Archetype;

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : ArchetypeAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec Spec(AbilityClass, 1);
				ASC->GiveAbility(Spec);
			}
		}
	}
}

void ASOLOCompanionCharacter::SetDowned(bool bIsDowned)
{
	if (bDowned == bIsDowned) return;
	bDowned = bIsDowned;
	OnDownedChanged.Broadcast(bIsDowned);

	// Toggle collision and movement
	SetActorEnableCollision(!bIsDowned);
	if (USkeletalMeshComponent* CharMesh = GetMesh())
		CharMesh->SetSimulatePhysics(bIsDowned);
}
