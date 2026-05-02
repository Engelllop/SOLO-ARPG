#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASOLON6_PTSDComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPTSDChanged, int32, NewPTSD);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPanicAttack);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API UASOLON6_PTSDComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UASOLON6_PTSDComponent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|EPT")
    int32 MaxPTSD = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|EPT")
    int32 CurrentPTSD = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|EPT")
    float RecoveryRate = 15.0f;

    float RecoveryTimer = 0.0f;

public:
    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    void ModifyPTSD(int32 Delta);

    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    int32 GetCurrentPTSD() const { return CurrentPTSD; }

    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    void ResetPTSD() { CurrentPTSD = 0; OnPTSDChanged.Broadcast(0); }

    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    void SetMaxPTSD(int32 NewMax);

    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    bool HasCainEchoResolved() const { return bCainEchoResolved; }

    UFUNCTION(BlueprintCallable, Category = "N6|EPT")
    void ResolveCainEcho() { bCainEchoResolved = true; }

    UPROPERTY(BlueprintAssignable, Category = "N6|EPT")
    FOnPTSDChanged OnPTSDChanged;

    UPROPERTY(BlueprintAssignable, Category = "N6|EPT")
    FOnPanicAttack OnPanicAttack;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|EPT")
    void OnPTSDLevelChanged(int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|EPT")
    void OnPanicAttackEvent();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|EPT")
    void OnCainEchoAppear();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|EPT")
    void OnCainEchoResolved();

protected:
    UPROPERTY()
    bool bCainEchoResolved = false;

    int32 CainEchoCount = 0;

    void CheckCainEcho();
};
