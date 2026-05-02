#include "World/N1/ASOLON1_CrystalPillar.h"
#include "Engine/OverlapResult.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Data/N1/SOLON1_Constants.h"

ASOLON1_CrystalPillar::ASOLON1_CrystalPillar()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	RootComponent = PillarMesh;
	PillarMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ASOLON1_CrystalPillar::BeginPlay()
{
	Super::BeginPlay();
	HitsRemaining = MaxHits;
}

float ASOLON1_CrystalPillar::TakeDamage(float Damage, const FDamageEvent& Event,
	AController* InstigatorActor, AActor* Causer)
{
	if (bDestroyed) return 0.f;

	HitsRemaining = FMath::Max(0, HitsRemaining - 1);
	BP_OnHit(HitsRemaining);

	if (HitsRemaining <= 0)
		DestroyPillar(InstigatorActor);

	return 1.f;
}

void ASOLON1_CrystalPillar::DestroyPillar(AController* InstigatorActor)
{
	bDestroyed = true;
	bAOEActive = false;
	SetActorTickEnabled(false);

	if (DestructionVFX)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DestructionVFX, GetActorLocation());

	BP_OnDestroyed();
	OnPillarDestroyed.Broadcast(this, PillarIndex);

	PillarMesh->SetVisibility(false);
	PillarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASOLON1_CrystalPillar::SetAOEDamageActive(bool bActive)
{
	bAOEActive = bActive && !bDestroyed;
	SetActorTickEnabled(bAOEActive);
	AOETickAccumulator = 0.f;
}

void ASOLON1_CrystalPillar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bAOEActive && !bDestroyed)
		TickAOEDamage(DeltaSeconds);
}

void ASOLON1_CrystalPillar::TickAOEDamage(float DeltaSeconds)
{
	AOETickAccumulator += DeltaSeconds;
	if (AOETickAccumulator < 1.f) return;
	AOETickAccumulator -= 1.f;

	if (!AOEDamageGE) return;

	FVector Origin = GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(AOEDamageRadius));

	for (const FOverlapResult& O : Overlaps)
	{
		auto* IASI = Cast<IAbilitySystemInterface>(O.GetActor());
		if (!IASI) continue;
		UAbilitySystemComponent* TargetASC = IASI->GetAbilitySystemComponent();
		if (!TargetASC) continue;

		FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(AOEDamageGE, 1, Ctx);
		if (Spec.IsValid())
			TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}
