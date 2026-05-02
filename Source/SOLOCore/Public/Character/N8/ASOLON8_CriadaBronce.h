#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON8_CriadaBronce.generated.h"

UCLASS()
class SOLOCORE_API ASOLON8_CriadaBronce : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON8_CriadaBronce();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N8|Fases")
    void AdvanceLocalPhase();

    // --- Fase 1: Criada mecánica ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Fase1")
    float Phase1HP = 250.0f;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Ataques")
    void AttackPrune();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Ataques")
    void AttackSeeds();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Ataques")
    void AttackClassify();

    // --- Fase 2: Grieta ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Fase2")
    bool bArmorBroken = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Ataques")
    void AttackBiometricCry();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Ataques")
    void AttackTentacles();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Fase2")
    TSubclassOf<AActor> TentacleClass;

    // --- Fase 3: Muerte ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Fase3")
    bool bHasAskedForName = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Dialogo")
    void OnConfusionDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Dialogo")
    void OnDeathDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Dialogo")
    void OnNaming(const FString& GivenName);

public:
    virtual void OnDefeated();
};


