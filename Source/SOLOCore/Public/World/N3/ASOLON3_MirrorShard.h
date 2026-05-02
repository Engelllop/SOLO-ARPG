#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON3_MirrorShard.generated.h"

class ASOLON3_LeviathanBoss;

UCLASS()
class SOLOCORE_API ASOLON3_MirrorShard : public AActor
{
    GENERATED_BODY()

public:
    ASOLON3_MirrorShard();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    ASOLON3_LeviathanBoss* OwnerLeviathan = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Shard")
    float RotationSpeed = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Shard")
    int32 MaxHits = 5;

    int32 CurrentHits = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Shard")
    float OrbitRadius = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Shard")
    float OrbitSpeed = 0.3f;

    float OrbitAngle = 0.0f;

public:
    UFUNCTION(BlueprintCallable, Category = "N3|Shard")
    void SetOwnerLeviathan(ASOLON3_LeviathanBoss* InOwner) { OwnerLeviathan = InOwner; }

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Shard")
    void OnHit();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Shard")
    void OnDestroyedShard();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
