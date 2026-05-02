#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASOLON10_PlayerClone.generated.h"

UCLASS()
class SOLOCORE_API AASOLON10_PlayerClone : public ACharacter
{
    GENERATED_BODY()

public:
    AASOLON10_PlayerClone();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Clone")
    float MoveSpeed = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Clone")
    float AttackDamage = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N10|Clone")
    float Lifetime = 15.0f;

    float Age = 0.0f;

    UFUNCTION()
    void ChasePlayer();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Clone")
    void OnSpawn();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N10|Clone")
    void OnExpire();

public:
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
