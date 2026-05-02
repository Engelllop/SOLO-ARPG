#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "SOLOHungerWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOHungerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Hunger")
    void UpdateHunger(float CurrentHunger, float MaxHunger);

    UFUNCTION(BlueprintCallable, Category = "Hunger")
    void ShowWarning(bool bCritical);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Hunger")
    void OnHungerCritical();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Hunger")
    void OnHungerNormalized();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HungerBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> WarningOverlay;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HungerText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hunger")
    float CriticalThreshold = 0.2f;
};
