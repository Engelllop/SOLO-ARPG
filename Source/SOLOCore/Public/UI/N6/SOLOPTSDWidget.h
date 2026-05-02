#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SOLOPTSDWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOPTSDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PTSD")
    void UpdateTrauma(float CurrentTrauma, float MaxTrauma);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PTSD")
    void OnTraumaLevel40(); // Bordes rojos

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PTSD")
    void OnTraumaLevel60(); // Susurros

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PTSD")
    void OnTraumaLevel80(); // Alucinaciones

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PTSD")
    void OnTraumaBreak(); // Break mental

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PTSD")
    void OnTraumaHealed();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> RedVignette;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> HallucinationOverlay;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TraumaText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PTSD")
    float CurrentLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PTSD")
    float MaxLevel = 100.0f;
};
