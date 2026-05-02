#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON3_LeviathanBoss.generated.h"

class ASOLON3_LeviathanReflejo;
class ASOLON3_MirrorShard;
class ASOLON3_MirrorCore;

UCLASS()
class SOLOCORE_API ASOLON3_LeviathanBoss : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON3_LeviathanBoss();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fases")
    int32 LocalPhase = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fases")
    int32 MaxPhase = 3;

    void AdvanceLocalPhase();

    // --- Fase 1 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase1")
    TSubclassOf<ASOLON3_LeviathanReflejo> ReflejoClass;

    UPROPERTY()
    ASOLON3_LeviathanReflejo* CurrentReflejo = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase1")
    float DuplicationCooldown = 15.0f;

    float DuplicationTimer = 0.0f;

    void SpawnReflejo();

    UFUNCTION(BlueprintNativeEvent, Category = "N3|Fase1")
    void OnMirrorDamageRedirect(AActor* DamageInstigatorActor, float Damage);

    // --- Fase 2 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase2")
    TSubclassOf<ASOLON3_MirrorShard> MirrorShardClass;

    UPROPERTY()
    TArray<ASOLON3_MirrorShard*> ActiveShards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase2")
    int32 NumShardsToSpawn = 8;

    void SpawnMirrorShards();

    // --- Fase 3 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase3")
    TSubclassOf<ASOLON3_MirrorCore> MirrorCoreClass;

    UPROPERTY()
    ASOLON3_MirrorCore* MirrorCore = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Fase3")
    int32 HitsToDefeat = 10;

    int32 CurrentHitsOnCore = 0;

    void SpawnMirrorCore();

    // --- Ataques ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Ataques")
    void AttackResonanceWave();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Ataques")
    void AttackMirrorBeam();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Ataques")
    void AttackWaterSurge();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Salud")
    float Phase1MaxHP = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Salud")
    float Phase3CoreHP = 200.0f;

public:
    UFUNCTION(BlueprintCallable, Category = "N3|Fase2")
    void OnShardDestroyed(ASOLON3_MirrorShard* DestroyedShard);

    UFUNCTION(BlueprintCallable, Category = "N3|Fase3")
    void RegisterCoreHit();
};
