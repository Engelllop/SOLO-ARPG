#include "Character/N1/ASOLON1_LaVozBoss.h"
#include "Engine/OverlapResult.h"
#include "World/N1/ASOLON1_CrystalPillar.h"
#include "World/N1/ASOLON1_EchoShadow.h"
#include "World/N1/ASOLON1_WaterArm.h"
#include "Character/N1/ASOLON1_VozManifestacion.h"
#include "Character/N1/ASOLON1_VozReflejo.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Data/N1/SOLON1_Constants.h"

ASOLON1_LaVozBoss::ASOLON1_LaVozBoss()
{
	bIsBoss = true;
	PhaseThresholds.Empty(); // phases are pillar-gated, not HP-gated
	BossDisplayName = FText::FromString(TEXT("La Voz de Velmar"));
	XPReward = 200.f;
}

void ASOLON1_LaVozBoss::BeginPlay()
{
	Super::BeginPlay();

	for (ASOLON1_CrystalPillar* Pillar : CrystalPillars)
	{
		if (Pillar)
			Pillar->OnPillarDestroyed.AddDynamic(this, &ASOLON1_LaVozBoss::OnPillarDestroyed);
	}

	EnterPhase1_Whispers();
}

// â”€â”€â”€ Pillar tracking â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::OnPillarDestroyed(ASOLON1_CrystalPillar* Pillar, int32 PillarIdx)
{
	DestroyedPillarCount++;

	if (BossPhaseIndex == 1 && DestroyedPillarCount >= N1::PILLARS_FOR_PHASE2)
		EnterPhase2_Crack();
	else if (BossPhaseIndex == 2 && DestroyedPillarCount >= N1::TOTAL_PILLARS)
		EnterPhase3_Fall();
}

// â”€â”€â”€ Phase 1 â€” Whispers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::EnterPhase1_Whispers()
{
	BossPhaseIndex = 1;
	ApplyInvisibility();
	if (USkeletalMeshComponent* M = GetMesh()) M->SetVisibility(false);

	StartEchoSpawner();
	StartWhisperLoop();

	BP_OnPhase1Start();
	OnPhaseEntered.Broadcast(1);
}

void ASOLON1_LaVozBoss::StartEchoSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(EchoSpawnTimer, this,
		&ASOLON1_LaVozBoss::TrySpawnEcho, EchoSpawnCooldown, true);
}

void ASOLON1_LaVozBoss::StopEchoSpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(EchoSpawnTimer);
}

void ASOLON1_LaVozBoss::TrySpawnEcho()
{
	if (!EchoShadowClass) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!Player) return;

	FVector PlayerPos = Player->GetActorLocation();
	FVector PlayerVelocity = Player->GetVelocity();

	// Only spawn if player has moved enough
	if (PlayerVelocity.Size2D() < 50.f) return;

	FVector SpawnPos = PlayerPos - PlayerVelocity.GetSafeNormal2D() * EchoSpawnDistance;
	SpawnPos.Z = PlayerPos.Z;

	FTransform T(FRotator::ZeroRotator, SpawnPos);
	if (ASOLON1_EchoShadow* Echo = GetWorld()->SpawnActor<ASOLON1_EchoShadow>(EchoShadowClass, T))
		Echo->InitEcho(Player, EchoAttackDelay);
}

void ASOLON1_LaVozBoss::StartWhisperLoop()
{
	GetWorld()->GetTimerManager().SetTimer(WhisperTimer, this,
		&ASOLON1_LaVozBoss::ApplyWhisper, WhisperInterval, true);
}

void ASOLON1_LaVozBoss::StopWhisperLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(WhisperTimer);
}

void ASOLON1_LaVozBoss::ApplyWhisper()
{
	if (!CharmGE) return;

	// Gather all potential targets: player + companions
	TArray<AActor*> Candidates;
	if (ACharacter* P = UGameplayStatics::GetPlayerCharacter(this, 0))
		Candidates.Add(P);

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(this, ACharacter::StaticClass(), AllActors);
	for (AActor* A : AllActors)
	{
		if (A->ActorHasTag(TEXT("Companion")))
			Candidates.Add(A);
	}

	if (Candidates.Num() == 0) return;

	AActor* Target = Candidates[FMath::RandRange(0, Candidates.Num() - 1)];
	auto* IASI = Cast<IAbilitySystemInterface>(Target);
	if (!IASI) return;

	UAbilitySystemComponent* TargetASC = IASI->GetAbilitySystemComponent();
	if (!TargetASC) return;

	FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(CharmGE, 1, Ctx);
	if (Spec.IsValid())
		TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

// â”€â”€â”€ Phase 2 â€” The Crack â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::EnterPhase2_Crack()
{
	BossPhaseIndex = 2;
	StopEchoSpawner();
	StopWhisperLoop();

	SpawnWaterArms();
	ActivateRemainingPillarAOE();

	BP_OnPhase2Start();
	OnPhaseEntered.Broadcast(2);
}

void ASOLON1_LaVozBoss::SpawnWaterArms()
{
	if (!WaterArmClass) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	FVector Center = Player ? Player->GetActorLocation() : GetActorLocation();

	for (int32 i = 0; i < WaterArmCount; i++)
	{
		float Angle = (2.f * PI * i) / WaterArmCount;
		FVector Offset(FMath::Cos(Angle) * WaterArmSpawnRadius,
		               FMath::Sin(Angle) * WaterArmSpawnRadius, 0.f);
		FTransform T(FRotator::ZeroRotator, Center + Offset);
		GetWorld()->SpawnActor<ASOLON1_WaterArm>(WaterArmClass, T);
	}
}

void ASOLON1_LaVozBoss::ActivateRemainingPillarAOE()
{
	for (ASOLON1_CrystalPillar* Pillar : CrystalPillars)
	{
		if (Pillar && !Pillar->IsDestroyed())
			Pillar->SetAOEDamageActive(true);
	}
}

// â”€â”€â”€ Phase 3 â€” The Fall â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::EnterPhase3_Fall()
{
	BossPhaseIndex = 3;

	ShatterCrystalTree();
	SpawnManifestacionAndReflejo();

	BP_OnPhase3Start();
	OnPhaseEntered.Broadcast(3);
}

void ASOLON1_LaVozBoss::ShatterCrystalTree()
{
	BP_OnCrystalTreeShatter();

	// Radial force knockback
	TArray<FOverlapResult> Overlaps;
	FVector Origin = CrystalTree ? CrystalTree->GetActorLocation() : GetActorLocation();
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(RadialForceRadius));

	for (const FOverlapResult& O : Overlaps)
	{
		if (ACharacter* Ch = Cast<ACharacter>(O.GetActor()))
		{
			FVector Dir = (Ch->GetActorLocation() - Origin).GetSafeNormal();
			Ch->LaunchCharacter(Dir * RadialForceStrength, true, true);
		}
	}

	if (CrystalTree)
	{
		CrystalTree->SetActorHiddenInGame(true);
		CrystalTree->SetActorEnableCollision(false);
	}
}

void ASOLON1_LaVozBoss::SpawnManifestacionAndReflejo()
{
	ASOLON1_VozReflejo* Reflejo = nullptr;
	if (ReflejoClass)
	{
		FTransform RT(FRotator::ZeroRotator, ReflejoSpawnPoint);
		Reflejo = GetWorld()->SpawnActor<ASOLON1_VozReflejo>(ReflejoClass, RT);
	}

	if (ManifestacionClass)
	{
		FTransform MT(FRotator::ZeroRotator, ManifestacionSpawnPoint);
		ASOLON1_VozManifestacion* Manif = GetWorld()->SpawnActor<ASOLON1_VozManifestacion>(ManifestacionClass, MT);
		if (Manif)
		{
			SpawnedManifestacion = Manif;
			if (Reflejo)
				Manif->LinkReflejo(Reflejo);
			Manif->OnDefeated.AddDynamic(this, &ASOLON1_LaVozBoss::OnManifestacionDefeated);
		}
	}
}

void ASOLON1_LaVozBoss::OnManifestacionDefeated()
{
	SetStoryFlag(N1::FLAG_LaVozKilled);
	BP_OnBossDefeated();
	OnLaVozDefeated.Broadcast();
}

// â”€â”€â”€ Invisibility â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::ApplyInvisibility()
{
	if (!InvisibleGE) return;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InvisibleGE, 1, Ctx);
	if (Spec.IsValid())
		InvisibleHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void ASOLON1_LaVozBoss::RemoveInvisibility()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		if (InvisibleHandle.IsValid())
			ASC->RemoveActiveGameplayEffect(InvisibleHandle);
	if (USkeletalMeshComponent* M = GetMesh()) M->SetVisibility(true);
}

// â”€â”€â”€ Helpers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLON1_LaVozBoss::SetStoryFlag(FName Flag)
{
	auto* GI = Cast<USOLOGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI) GI->SetStoryFlag(Flag, true);
}
