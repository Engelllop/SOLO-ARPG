#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
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

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N10|Fases")
    void AdvanceLocalPhase();

    // --- Fase 1: Excepciones ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase1")
    void ThrowNullReference();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase1")
    void CorruptMemory();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase1")
    void StackOverflow();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase1")
    TSubclassOf<AActor> PlayerCloneClass;

    UPROPERTY()
    TArray<AActor*> ActiveClones;

    UFUNCTION(BlueprintCallable, Category = "N10|Fase1")
    void SpawnPlayerClones();

    // --- Fase 2: El Archivo ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase2")
    bool bHasShownArchive = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowArchive();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowCainFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowMiraFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowVassenFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowGarrikFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowSeraFile();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase2")
    void ShowPlayerFile();

    // --- Fase 3: Elección ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bIsChoiceMade = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bChoseEscape = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bChoseStay = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bHasFragment = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bHasLookedAbyss = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Fase3")
    bool bHasAllAbilities = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void OnChoiceDialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void OnEndingA(); // Escape / Olvido

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Fase3")
    void OnEndingB(); // Stay / Nuevo Arquitecto

    // --- Diálogos ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Dialogo")
    void OnFirstMeeting();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Dialogo")
    void OnDebugLine(const FString& DebugText);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Dialogo")
    void OnEndingBRequirementCheck();

public:
    UFUNCTION(BlueprintCallable, Category = "N10|Eleccion")
    void ChooseEscape();

    UFUNCTION(BlueprintCallable, Category = "N10|Eleccion")
    void ChooseStay();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void OnDefeated();
};


