#include "Engine/OverlapResult.h"
#include "Character/N1/ASOLON1_VozReflejo.h"
#include "Components/StaticMeshComponent.h"

ASOLON1_VozReflejo::ASOLON1_VozReflejo()
{
	ReflejoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReflejoMesh"));
	RootComponent = ReflejoMesh;
	ReflejoMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

float ASOLON1_VozReflejo::TakeDamage(float Damage, const FDamageEvent& Event,
	AController* InstigatorActor, AActor* Causer)
{
	if (bDestroyed) return 0.f;

	HitsRemaining = FMath::Max(0, HitsRemaining - 1);
	BP_OnReflejoHit(HitsRemaining);
	OnReflejoHit.Broadcast();

	if (HitsRemaining <= 0)
	{
		bDestroyed = true;
		BP_OnReflejoDestroyed();
		OnReflejoDestroyed.Broadcast();
		ReflejoMesh->SetVisibility(false);
		ReflejoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return 1.f;
}


