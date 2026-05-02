#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON6_CoroDeJade.generated.h"

class AASOLON6_ChildSinger;

UCLASS()
class SOLOCORE_API ASOLON6_CoroDeJade : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON6_CoroDeJade();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Fases ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Fases")
    int32 LocalPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "N6|Fases")
    void AdvanceLocalPhase();

    // --- Fase 1: Cantores ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Fase1")
    TSubclassOf<AASOLON6_ChildSinger> SingerClass;

    UPROPERTY()
    TArray<AASOLON6_ChildSinger*> Singers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Fase1")
    int32 NumSingers = 6;

    UFUNCTION(BlueprintCallable, Category = "N6|Fase1")
    void SpawnSingers();

    UFUNCTION(BlueprintCallable, Category = "N6|Fase1")
    void OnSingerFreed(AASOLON6_ChildSinger* FreedSinger);

    // --- Fase 2: Capataz ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Fase2")
    float Phase2HP = 100.0f;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Ataques")
    void AttackWhip();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Ataques")
    void AttackStomp();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Ataques")
    void AttackSummonZombieChildren();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Dialogo")
    void OnPhase2Dialog();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Dialogo")
    void OnDeathDialog();

public:
    virtual void OnDefeated();
};


