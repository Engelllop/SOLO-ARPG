#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON3_LeviathanReflejo.generated.h"

class ASOLON3_LeviathanBoss;

UCLASS()
class SOLOCORE_API ASOLON3_LeviathanReflejo : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ASOLON3_LeviathanReflejo();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    ASOLON3_LeviathanBoss* OwnerLeviathan = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Reflejo")
    float DamageMultiplier = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Reflejo")
    float Lifetime = 20.0f;

    float Age = 0.0f;

public:
    UFUNCTION(BlueprintCallable, Category = "N3|Reflejo")
    void SetOwnerLeviathan(ASOLON3_LeviathanBoss* InOwner) { OwnerLeviathan = InOwner; }

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Reflejo")
    void OnExpire();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
