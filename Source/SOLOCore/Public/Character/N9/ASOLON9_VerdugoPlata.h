#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON9_VerdugoPlata.generated.h"

UCLASS()
class SOLOCORE_API ASOLON9_VerdugoPlata : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON9_VerdugoPlata();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Fases")
    int32 LocalPhase = 1;

    void AdvanceLocalPhase();

    // --- Ataques Fase 1 ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Ataques")
    void AttackConfessionBlow();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Ataques")
    void AttackTongueCut();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Ataques")
    void AttackTortureWheel();

    // --- Ataques Fase 2 ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Ataques")
    void AttackPainMemory();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Ataques")
    void AttackBrainNeedle();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Fase2")
    bool bHasBrainExposed = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Fase2")
    bool bHasEyeOfSentinel = false;

    // --- Diálogos ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Dialogo")
    void OnAttackLine();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Dialogo")
    void OnPhase2Dialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Dialogo")
    void OnSpiritDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Dialogo")
    void OnDeathDialog();

public:
    void OnDefeated();
};
