#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON6_ChildSinger.generated.h"

class ASOLON6_CoroDeJade;

UCLASS()
class SOLOCORE_API AASOLON6_ChildSinger : public AActor
{
    GENERATED_BODY()

public:
    AASOLON6_ChildSinger();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    ASOLON6_CoroDeJade* CoroOwner = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Singer")
    bool bIsSinging = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Singer")
    float SingingDPS = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Singer")
    float SingingRadius = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Singer")
    int32 MaxHitsToFree = 10;

    int32 CurrentHits = 0;

public:
    UFUNCTION(BlueprintCallable, Category = "N6|Singer")
    void SetCoroOwner(ASOLON6_CoroDeJade* InOwner) { CoroOwner = InOwner; }

    UFUNCTION(BlueprintCallable, Category = "N6|Singer")
    void StopSinging();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Singer")
    void OnFreed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Singer")
    void OnHitByPlayer();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
