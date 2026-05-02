#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON5_Carnicero.generated.h"

UCLASS()
class SOLOCORE_API ASOLON5_Carnicero : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON5_Carnicero();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N5|Fases")
    void AdvanceLocalPhase();

    // --- Fase 1: El Verdugo ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Fase1")
    float Phase1HP = 250.0f;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Ataques")
    void AttackAxeSwing();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Ataques")
    void AttackStomp();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Ataques")
    void AttackSummonSlave();

    // --- Fase 2: La Bestia ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Fase2")
    float BestiaAttackSpeedMultiplier = 1.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Fase2")
    bool bIsBeastForm = false;

    UFUNCTION(BlueprintCallable, Category = "N5|Fase2")
    void ToggleForm();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Fase2")
    float FormSwitchCooldown = 10.0f;

    float FormSwitchTimer = 0.0f;

    // --- Diálogos ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Dialogo")
    void OnPhaseTransitionDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Dialogo")
    void OnDeathDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N5|Dialogo")
    void OnAttackLine();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N5|Dialogo")
    bool bIsPlayerFemale = false;

public:
    virtual void OnDefeated();
};


