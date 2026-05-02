#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON7_Costurera.generated.h"

class UASOLON7_MarionetaComponent;

UCLASS()
class SOLOCORE_API ASOLON7_Costurera : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON7_Costurera();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N7|Fases")
    void AdvanceLocalPhase();

    // --- Fase 1: Marionetas ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase1")
    AActor* MiraPawn = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase1")
    AActor* VassenPawn = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase1")
    int32 ThreadsToBreak = 6;

    int32 BrokenThreads = 0;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Fase1")
    void OnMiraFreed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Fase1")
    void OnVassenFreed();

    // --- Fase 2: Muerte ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Fase2")
    void OnMiraDeath();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Fase2")
    void OnVassenDeath();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Fase2")
    void OnCompanionsDeadCG();

    // --- Fase 3: Directa ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase3")
    float Phase3HP = 200.0f;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Ataques")
    void AttackNeedle();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Ataques")
    void AttackThread();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Ataques")
    void AttackDanceOfPatches();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Ataques")
    void AttackSummonPainMarionette();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase3")
    bool bIsIntangible = false;

    void ToggleIntangible();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase3")
    float IntangibleCooldown = 15.0f;

    float IntangibleTimer = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Fase3")
    float IntangibleDuration = 2.0f;

    float IntangibleCurrentDuration = 0.0f;

    // --- Diálogos ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Dialogo")
    void OnPreCombatDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Dialogo")
    void OnDeathDialog();

public:
    UFUNCTION(BlueprintCallable, Category = "N7|Fase1")
    void RegisterThreadBreak();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    void OnDefeated();
};
