#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASOLON4_HungerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHungerChanged, int32, NewHunger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStarved);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API UASOLON4_HungerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UASOLON4_HungerComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre")
    int32 MaxHunger = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre")
    int32 CurrentHunger = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre")
    float HungerDecreaseInterval = 60.0f;

    float HungerTimer = 0.0f;

    // --- Efectos por nivel ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre|Efectos")
    float BlurIntensity = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre|Efectos")
    float SpeedReduction = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N4|Hambre|Efectos")
    float DamageReduction = 0.0f;

public:
    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    void ModifyHunger(int32 Delta);

    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    int32 GetCurrentHunger() const { return CurrentHunger; }

    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    int32 GetMaxHunger() const { return MaxHunger; }

    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    void SetMaxHunger(int32 NewMax);

    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    void SetHunger(int32 NewHunger);

    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    void ResetHunger() { CurrentHunger = MaxHunger; }

    // --- Delegados ---
    UPROPERTY(BlueprintAssignable, Category = "N4|Hambre")
    FOnHungerChanged OnHungerChanged;

    UPROPERTY(BlueprintAssignable, Category = "N4|Hambre")
    FOnStarved OnStarved;

    // --- Aplicar efectos ---
    UFUNCTION(BlueprintCallable, Category = "N4|Hambre")
    void ApplyHungerEffects();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Hambre")
    void OnHungerLevelChanged(int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N4|Hambre")
    void OnHungerDeath();
};
