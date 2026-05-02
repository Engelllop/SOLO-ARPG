#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON4_CorazonHambre.generated.h"

UCLASS()
class SOLOCORE_API ASOLON4_CorazonHambre : public AActor
{
    GENERATED_BODY()

public:
    ASOLON4_CorazonHambre();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    float HeartbeatInterval = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    float HeartbeatDamage = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    float HeartbeatRadius = 1000.0f;

    float HeartbeatTimer = 0.0f;

    // --- Hijos (fetos) ---
    UPROPERTY()
    TArray<AActor*> FetusChildren;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    int32 MaxChildren = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    TSubclassOf<AActor> FetusClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    int32 HitsToDefeat = 10;

    int32 CurrentHits = 0;

    // --- Visual ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    float FloatAmplitude = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Corazon")
    float PulsateSpeed = 1.0f;

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Corazon")
    void OnHeartbeat();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Corazon")
    void OnChildDestroyed(AActor* Child);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Corazon")
    void OnDefeated();

    UFUNCTION(BlueprintCallable, Category = "N4|Corazon")
    void RegisterChildDestroyed(AActor* Child);

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

