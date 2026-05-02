#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "Data/N10/SOLON10_Constants.h"
#include "ASOLON10_Arquitecto.generated.h"

UCLASS()
class SOLOCORE_API ASOLON10_Arquitecto : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON10_Arquitecto();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases (v3) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N10|Fases")
    void AdvanceLocalPhase();

    // ========== FASE 1: EL ENCUENTRO (dialogo) ==========
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase1")
    void OnFirstMeeting();

    // ========== FASE 2: ARCHIVOS CORPORATIVOS ==========
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowCorpEmails();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowWorldRevelation(); // "La Torre consume el mundo real"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase2")
    bool bHasShownCorpEmails = false;

    // ========== FASE 3: EL ARCHIVO DE LOS COMPANEROS ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bHasShownArchive = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowArchive();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowCainFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowMiraFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowVassenFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowGarrikFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowSeraFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void ShowPlayerFile();

    // ========== FASE 4: CENTINELA DE GRANITO (debug fight) ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase4")
    TSubclassOf<AActor> CentinelaClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase4")
    TSubclassOf<AActor> DecisionCloneClass;

    UPROPERTY()
    TArray<AActor*> ActiveCentinelaParts;

    UFUNCTION(BlueprintCallable, Category = "N10|Fase4")
    void SpawnCentinela(); // Entra el protocolo de limpieza

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase4")
    void OnCentinelaPhase1(); // Escaneo de patrones

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase4")
    void OnCentinelaPhase2(); // Fragmentos de decisiones (clones)

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase4")
    void OnCentinelaPhase3(); // Esfera del bug aparece

    UFUNCTION(BlueprintCallable, Category = "N10|Fase4")
    void SpawnDecisionClones(); // Clones que representan decisiones pasadas

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase4")
    bool bCentinelaDefeated = false;

    // ========== FASE 5: LA ESFERA DEL BUG (eleccion final) ==========
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bIsChoiceMade = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bChoseEscape = false; // Soltar la esfera -> Final A

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bChoseStay = false; // Proteger la esfera -> Final B

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bChoseDestroy = false; // Destruir la esfera -> Final C

    // --- Condiciones para Final B (v3) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bHasFragment = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bHasListenedVerdugo = false;

    // --- Additional tracking (read from save) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bLookedIntoAbyss = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bHasAllAbilities = false;

    // --- Condiciones para Final C (v3 — comprension tematica) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bAteInN4 = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bDidNotNameCriada = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase5")
    bool bListenedVerdugo = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnSphereAppears();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnSphereChoiceDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnEndingA(); // Escape / Olvido

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnEndingB(); // Stay / Nuevo Arquitecto

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnEndingC(); // Destroy / Compilacion

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnEndingBRequirementCheck();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase5")
    void OnDebugLine(const FString& DebugText);

public:
    // Opciones de eleccion publicas para Blueprint
    UFUNCTION(BlueprintCallable, Category = "N10|Eleccion")
    void ChooseEscape();

    UFUNCTION(BlueprintCallable, Category = "N10|Eleccion")
    void ChooseStay();

    UFUNCTION(BlueprintCallable, Category = "N10|Eleccion")
    void ChooseDestroy();

    // Sistema de condiciones v3
    UFUNCTION(BlueprintCallable, Category = "N10|Checks")
    bool CheckFinalBConditions() const;

    UFUNCTION(BlueprintCallable, Category = "N10|Checks")
    bool CheckFinalCConditions() const;

    UFUNCTION(BlueprintCallable, Category = "N10|Debug")
    void SetCentinelaDefeated();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void OnDefeated();
};
