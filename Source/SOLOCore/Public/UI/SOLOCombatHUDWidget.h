#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOCombatHUDWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOCombatHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void ShowComboCount(int32 Count);
	UFUNCTION(BlueprintCallable) void ShowPostureBar(float Posture, float MaxPosture);
	UFUNCTION(BlueprintCallable) void ShowParryIndicator(bool bVisible);
	UFUNCTION(BlueprintCallable) void ShowDamageNumber(float Damage, FVector WorldPos, bool bCrit);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* ComboCountText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UProgressBar* PostureBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UImage* ParryIndicator;
};
