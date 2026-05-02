#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON4_ColosoHambre.generated.h"

class ASOLON4_CorazonHambre;

UCLASS()
class SOLOCORE_API ASOLON4_ColosoHambre : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON4_ColosoHambre();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N4|Fases")
    void AdvanceLocalPhase();

    // --- Piel de Hueso (Fase 1) ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Fase1")
    float MaxHP = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Fase1")
    int32 HitsToBreakArmor = 15;

    int32 ArmorHits = 0;

    // --- Ataques ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Ataques")
    void AttackStarvationStomp();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Ataques")
    void AttackBoneVomit();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Ataques")
    void AttackMotherCry();

    // --- Succión de Hambre ---
    UFUNCTION(BlueprintCallable, Category = "N4|Mecanicas")
    void SuckHungerFromPlayer(AActor* PlayerActor);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Mecanicas")
    float SelfHealPerHunger = 20.0f;

    // --- Fase 2: Corazón ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Fase2")
    TSubclassOf<ASOLON4_CorazonHambre> CorazonClass;

    UPROPERTY()
    ASOLON4_CorazonHambre* Corazon = nullptr;

    UFUNCTION(BlueprintCallable, Category = "N4|Fase2")
    void SpawnCorazon();

    // --- Diálogos ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Dialogo")
    void OnDeathDialog();

public:
    virtual void OnDefeated();
};


