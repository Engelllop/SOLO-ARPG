#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BP_Oswin.generated.h"

UCLASS()
class SOLOCORE_API ABP_Oswin : public ACharacter
{
    GENERATED_BODY()

public:
    ABP_Oswin();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Oswin")
    bool bHasGivenQuest = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Oswin")
    bool bHasGolemClaw = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Oswin")
    bool bHasJadeEssence = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Oswin")
    bool bHasBloodOfExecuted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Oswin")
    bool bCanForge = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Oswin")
    void OnFirstMeeting();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Oswin")
    void OnForgeReady();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Oswin")
    void OnForgeComplete();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Oswin")
    void OnConfrontation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Oswin")
    void OnExecutionerDialog();

public:
    UFUNCTION(BlueprintCallable, Category = "N9|Oswin")
    void CheckForgeStatus();

    UFUNCTION(BlueprintCallable, Category = "N9|Oswin")
    void ForgeWeapon();
};
