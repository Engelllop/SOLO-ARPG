#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BP_Elia.generated.h"

UCLASS()
class SOLOCORE_API ABP_Elia : public ACharacter
{
    GENERATED_BODY()

public:
    ABP_Elia();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Elia")
    bool bHasGivenFragment = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N9|Elia")
    bool bHasBeenConfronted = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Elia")
    void OnFirstMeeting();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Elia")
    void OnConfrontation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Elia")
    void OnGiveFragment();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N9|Elia")
    void OnDeathDialog();

public:
    UFUNCTION(BlueprintCallable, Category = "N9|Elia")
    void ConverseWithPlayer();

    UFUNCTION(BlueprintCallable, Category = "N9|Elia")
    void GiveFragment();
};
