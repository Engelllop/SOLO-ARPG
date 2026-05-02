#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON10_Centinela.generated.h"

UCLASS()
class SOLOCORE_API ASOLON10_Centinela : public AActor
{
    GENERATED_BODY()

public:
    ASOLON10_Centinela();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Phase tracking ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela")
    int32 CurrentPhase = 1;

    UFUNCTION(BlueprintCallable, Category = "Centinela")
    void AdvancePhase();

    // --- Phase 1: Scanner ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase1")
    float ScanRadius = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase1")
    float ScanSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase1")
    bool bIsScanning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase1")
    bool bPlayerDetected = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase1")
    void OnScanStart();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase1")
    void OnScanPatternDetected();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase1")
    void OnScanPatternEvaded();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase1")
    void OnScanPhaseComplete();

    // --- Phase 2: Decision Clones ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase2")
    TSubclassOf<AActor> DecisionCloneClass;

    UPROPERTY()
    TArray<AActor*> ActiveClones;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase2")
    bool bAllClonesAbsorbed = false;

    UFUNCTION(BlueprintCallable, Category = "Centinela|Phase2")
    void SpawnDecisionClones();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase2")
    void OnClonesSpawned();

    UFUNCTION(BlueprintCallable, Category = "Centinela|Phase2")
    void OnCloneAbsorbed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase2")
    void OnPhase2Complete();

    // --- Phase 3: Sphere of the Bug ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase3")
    TSubclassOf<AActor> SphereClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Centinela|Phase3")
    bool bSphereRevealed = false;

    UPROPERTY()
    AActor* SphereInstance;

    UFUNCTION(BlueprintCallable, Category = "Centinela|Phase3")
    void RevealSphere();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase3")
    void OnSphereRevealed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Centinela|Phase3")
    void OnCentinelaDefeated();

public:
    UFUNCTION(BlueprintCallable, Category = "Centinela")
    void NotifyPlayerMovement(FVector MovementDelta);

    UFUNCTION(BlueprintCallable, Category = "Centinela")
    void SetPlayerDetected(bool bDetected);

    // Called externally when all phases done
    UFUNCTION(BlueprintCallable, Category = "Centinela")
    void DefeatCentinela();
};
