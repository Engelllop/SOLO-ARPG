#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON3_MirrorCore.generated.h"

class ASOLON3_LeviathanBoss;

UCLASS()
class SOLOCORE_API ASOLON3_MirrorCore : public AActor
{
    GENERATED_BODY()

public:
    ASOLON3_MirrorCore();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    ASOLON3_LeviathanBoss* OwnerLeviathan = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Core")
    float FloatAmplitude = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Core")
    float FloatSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Core")
    float RotationSpeed = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Core")
    float GlowIntensity = 1.0f;

    FVector StartLocation;

public:
    UFUNCTION(BlueprintCallable, Category = "N3|Core")
    void SetOwnerLeviathan(ASOLON3_LeviathanBoss* InOwner) { OwnerLeviathan = InOwner; }

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Core")
    void OnHit();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Core")
    void OnDefeat();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
