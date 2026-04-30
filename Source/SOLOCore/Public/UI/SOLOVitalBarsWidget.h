#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOVitalBarsWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOVitalBarsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateBars(float HP, float MaxHP, float MP, float MaxMP, float SP, float MaxSP);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UProgressBar* HPBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UProgressBar* MPBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UProgressBar* SPBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* HPText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* MPText;
};
