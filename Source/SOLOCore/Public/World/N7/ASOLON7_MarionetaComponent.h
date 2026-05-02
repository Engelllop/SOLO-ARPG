#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASOLON7_MarionetaComponent.generated.h"

class ASOLON7_Costurera;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API UASOLON7_MarionetaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UASOLON7_MarionetaComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    ASOLON7_Costurera* CostureraOwner = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Marioneta")
    bool bIsControlled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Marioneta")
    int32 ThreadMaxHits = 10;

    int32 ThreadHits = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Marioneta")
    float FreeDuration = 8.0f;

    float FreeTimer = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Marioneta")
    bool bIsThreadBroken = false;

    // --- Visual ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N7|Marioneta")
    int32 ThreadVisualCount = 2;

public:
    UFUNCTION(BlueprintCallable, Category = "N7|Marioneta")
    void SetCostureraOwner(ASOLON7_Costurera* InOwner) { CostureraOwner = InOwner; }

    UFUNCTION(BlueprintCallable, Category = "N7|Marioneta")
    void BreakThread();

    UFUNCTION(BlueprintCallable, Category = "N7|Marioneta")
    bool IsControlled() const { return bIsControlled; }

    UFUNCTION(BlueprintCallable, Category = "N7|Marioneta")
    void ForceControl() { bIsControlled = true; FreeTimer = 0.0f; }

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Marioneta")
    void OnThreadHit();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Marioneta")
    void OnFreed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Marioneta")
    void OnRecontrolled();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N7|Marioneta")
    void OnDeathDialog();
};
