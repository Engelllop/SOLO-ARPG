#pragma once
#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON1_LaVozBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLaVozWaveCleared, int32, WaveIndex);

// La Voz — boss of N1, Santuario de Velmar.
// Phase 0 (100–67%): invisible, untargetable, summons 3 waves of 2 AlmaErrante.
// Phase 1 (66–34%): materializes, melee + ranged attacks (light ray cone + tracking orbs).
// Phase 2 (33–0%):  condenses to white nucleus, spawns pulsing AOE shockwaves.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_LaVozBoss : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON1_LaVozBoss();

	virtual void BeginPlay() override;
	virtual void OnPhaseChanged_Implementation(int32 NewPhase) override;

	// Bound to each Alma's OnEnemyDied delegate
	UFUNCTION() void OnAlmaDied(AActor* DeadAlma);

	UPROPERTY(BlueprintAssignable) FOnLaVozWaveCleared OnWaveCleared;

protected:
	// ─── Phase 0 config ─────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase0")
	TSubclassOf<ASOLOEnemyCharacter> AlmaErranteClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase0")
	int32 AlmasPerWave = 2;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase0")
	int32 TotalWaves = 3;

	// Spawn radius around La Voz for Almas
	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase0")
	float AlmaSpawnRadius = 600.f;

	// GE that makes La Voz invisible + untargetable in Phase 0
	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase0")
	TSubclassOf<UGameplayEffect> InvisibleGE;

	// ─── Phase 1 config ─────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	TSubclassOf<AActor> TrackingOrbClass;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	float LightRayConeHalfAngle = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase1")
	TSubclassOf<UGameplayEffect> LightRayDamageGE;

	// ─── Phase 2 config ─────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	float ShockwaveRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	float ShockwaveInterval = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	TSubclassOf<UGameplayEffect> ShockwaveDamageGE;

	UPROPERTY(EditDefaultsOnly, Category = "LaVoz|Phase2")
	TSubclassOf<UGameplayEffect> StunGE;

private:
	int32 AliveAlmaCount = 0;
	int32 CurrentWave    = 0;
	FActiveGameplayEffectHandle InvisibleHandle;
	FTimerHandle ShockwaveTimer;

	void EnterPhase0();
	void EnterPhase1();
	void EnterPhase2();

	void SpawnNextAlmaWave();
	void RemoveInvisibility();

	UFUNCTION() void PulseShockwave();

	// BlueprintImplementableEvents let BP add VFX/audio without touching these
	UFUNCTION(BlueprintImplementableEvent) void BP_OnMaterialize();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnCondense();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnWaveSpawned(int32 WaveIdx);
};
