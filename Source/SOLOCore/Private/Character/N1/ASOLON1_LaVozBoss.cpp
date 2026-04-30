#include "Character/N1/ASOLON1_LaVozBoss.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

ASOLON1_LaVozBoss::ASOLON1_LaVozBoss()
{
	bIsBoss = true;
	PhaseThresholds = { 0.66f, 0.33f };
	BossDisplayName = FText::FromString(TEXT("La Voz"));
	XPReward = 150.f;
}

void ASOLON1_LaVozBoss::BeginPlay()
{
	Super::BeginPlay();
	EnterPhase0();
}

void ASOLON1_LaVozBoss::OnPhaseChanged_Implementation(int32 NewPhase)
{
	Super::OnPhaseChanged_Implementation(NewPhase);
	switch (NewPhase)
	{
	case 1: EnterPhase1(); break;
	case 2: EnterPhase2(); break;
	default: break;
	}
}

// ─── Phase 0 — Invisible, wave spawner ──────────────────────────────────────

void ASOLON1_LaVozBoss::EnterPhase0()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (InvisibleGE && ASC)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InvisibleGE, 1, Ctx);
		if (Spec.IsValid())
			InvisibleHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
	// Start invisible mesh
	if (USkeletalMeshComponent* Mesh = GetMesh()) Mesh->SetVisibility(false);

	AliveAlmaCount = 0;
	CurrentWave = 0;
	SpawnNextAlmaWave();
}

void ASOLON1_LaVozBoss::SpawnNextAlmaWave()
{
	if (!AlmaErranteClass || CurrentWave >= TotalWaves) return;

	AliveAlmaCount = AlmasPerWave;
	for (int32 i = 0; i < AlmasPerWave; i++)
	{
		float Angle = (2.f * PI * i) / AlmasPerWave;
		FVector Offset(FMath::Cos(Angle) * AlmaSpawnRadius, FMath::Sin(Angle) * AlmaSpawnRadius, 0.f);
		FTransform T(GetActorLocation() + Offset);
		if (ASOLOEnemyCharacter* Alma = GetWorld()->SpawnActor<ASOLOEnemyCharacter>(AlmaErranteClass, T))
			Alma->OnEnemyDied.AddDynamic(this, &ASOLON1_LaVozBoss::OnAlmaDied);
	}

	BP_OnWaveSpawned(CurrentWave);
	CurrentWave++;
}

void ASOLON1_LaVozBoss::OnAlmaDied(AActor* DeadAlma)
{
	AliveAlmaCount = FMath::Max(0, AliveAlmaCount - 1);
	if (AliveAlmaCount > 0) return;

	OnWaveCleared.Broadcast(CurrentWave - 1);

	if (CurrentWave < TotalWaves)
		SpawnNextAlmaWave();
	// If all waves done and still in Phase 0, wait for HP threshold to trigger Phase 1
}

void ASOLON1_LaVozBoss::RemoveInvisibility()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		if (InvisibleHandle.IsValid()) ASC->RemoveActiveGameplayEffect(InvisibleHandle);
	if (USkeletalMeshComponent* Mesh = GetMesh()) Mesh->SetVisibility(true);
}

// ─── Phase 1 — Materializes, ranged attacks ─────────────────────────────────

void ASOLON1_LaVozBoss::EnterPhase1()
{
	RemoveInvisibility();
	BP_OnMaterialize();
}

// ─── Phase 2 — Condenses to nucleus, shockwave pulses ───────────────────────

void ASOLON1_LaVozBoss::EnterPhase2()
{
	BP_OnCondense();
	// Scale down to nucleus (Blueprint handles mesh change / VFX)
	SetActorScale3D(FVector(0.4f));

	GetWorld()->GetTimerManager().SetTimer(ShockwaveTimer,
		this, &ASOLON1_LaVozBoss::PulseShockwave, ShockwaveInterval, true);
}

void ASOLON1_LaVozBoss::PulseShockwave()
{
	if (!ShockwaveDamageGE) return;
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	if (!SourceASC) return;

	FVector Origin = GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(ShockwaveRadius));

	for (const FOverlapResult& O : Overlaps)
	{
		if (O.GetActor() == this) continue;
		if (auto* IASI = Cast<IAbilitySystemInterface>(O.GetActor()))
		{
			if (auto* TargetASC = IASI->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(ShockwaveDamageGE, GetCharacterLevel_Implementation(), Ctx);
				if (Spec.IsValid()) SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);

				if (StunGE)
				{
					FGameplayEffectSpecHandle StunSpec = SourceASC->MakeOutgoingSpec(StunGE, 1, Ctx);
					if (StunSpec.IsValid()) SourceASC->ApplyGameplayEffectSpecToTarget(*StunSpec.Data.Get(), TargetASC);
				}
			}
		}
	}
}
