#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON1_LaVozBoss.generated.h"
class ASOLON1_CrystalPillar;
class ASOLON1_EchoShadow;
class ASOLON1_WaterArm;
class ASOLON1_VozManifestacion;
class ASOLON1_VozReflejo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLaVozPhaseEntered, int32, PhaseIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaVozDefeated);

// La Voz de Velmar â€” orchestrator boss of N1.
// Phase 1 (Whispers): invisible, spawns EchoShadows, Charm/Whisper mechanic.
//   Objective: destroy 4 of 6 Crystal Pillars.
// Phase 2 (The Crack): Water Arms spawn, remaining 2 pillars emit AOE damage.
//   Objective: destroy the last 2 pillars.
// Phase 3 (The Fall): Crystal Tree shatters, VozManifestacion emerges from water.
//   Objective: hit the VozReflejo to open vulnerability windows on the Manifestation.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_LaVozBoss : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON1_LaVozBoss();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable) FOnLaVozPhaseEntered OnPhaseEntered;
	UPROPERTY(BlueprintAssignable) FOnLaVozDefeated OnLaVozDefeated;

	UFUNCTION(BlueprintPure, Category = "N1|LaVoz")
	int32 GetBossPhase() const { return BossPhaseIndex; }

	UFUNCTION(BlueprintPure, Category = "N1|LaVoz")
	int32 GetDestroyedPillarCount() const { return DestroyedPillarCount; }

protected:
	// â”€â”€â”€ Phase 1 â€” Whispers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	TSubclassOf<ASOLON1_EchoShadow> EchoShadowClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	float EchoSpawnDistance = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	float EchoSpawnCooldown = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	float EchoAttackDelay = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	float WhisperInterval = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1|GAS")
	TSubclassOf<UGameplayEffect> CharmGE;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1|GAS")
	TSubclassOf<UGameplayEffect> InvisibleGE;

	// â”€â”€â”€ Phase 2 â€” The Crack â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	TSubclassOf<ASOLON1_WaterArm> WaterArmClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	int32 WaterArmCount = 3;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	float WaterArmSpawnRadius = 500.f;

	// â”€â”€â”€ Phase 3 â€” The Fall â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase3")
	TSubclassOf<ASOLON1_VozManifestacion> ManifestacionClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase3")
	TSubclassOf<ASOLON1_VozReflejo> ReflejoClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase3")
	float RadialForceStrength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase3")
	float RadialForceRadius = 1200.f;

	// â”€â”€â”€ Pillar references (set in editor) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditInstanceOnly, Category = "LaVoz|Arena")
	TArray<ASOLON1_CrystalPillar*> CrystalPillars;

	// â”€â”€â”€ Crystal Tree (set in editor) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditInstanceOnly, Category = "LaVoz|Arena")
	AActor* CrystalTree;

	UPROPERTY(EditInstanceOnly, Category = "LaVoz|Arena")
	FVector ManifestacionSpawnPoint;

	UPROPERTY(EditInstanceOnly, Category = "LaVoz|Arena")
	FVector ReflejoSpawnPoint;

protected:
	// Blueprint hooks
	UFUNCTION(BlueprintImplementableEvent) void BP_OnPhase1Start();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnPhase2Start();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnPhase3Start();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnCrystalTreeShatter();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnBossDefeated();

private:
	int32 BossPhaseIndex = 0;
	int32 DestroyedPillarCount = 0;

	FTimerHandle EchoSpawnTimer;
	FTimerHandle WhisperTimer;

	FActiveGameplayEffectHandle InvisibleHandle;

	TWeakObjectPtr<ASOLON1_VozManifestacion> SpawnedManifestacion;

	// Phase management
	void EnterPhase1_Whispers();
	void EnterPhase2_Crack();
	void EnterPhase3_Fall();

	// Phase 1 logic
	void StartEchoSpawner();
	void StopEchoSpawner();
	UFUNCTION() void TrySpawnEcho();
	void StartWhisperLoop();
	void StopWhisperLoop();
	UFUNCTION() void ApplyWhisper();

	// Phase 2 logic
	void SpawnWaterArms();
	void ActivateRemainingPillarAOE();

	// Phase 3 logic
	void ShatterCrystalTree();
	void SpawnManifestacionAndReflejo();
	UFUNCTION() void OnManifestacionDefeated();

	// Pillar tracking
	UFUNCTION() void OnPillarDestroyed(ASOLON1_CrystalPillar* Pillar, int32 PillarIdx);

	void ApplyInvisibility();
	void RemoveInvisibility();
	void SetStoryFlag(FName Flag);
};


