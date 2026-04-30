#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOBossBarWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOBossBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void SetBossHP(float HP, float MaxHP);
	UFUNCTION(BlueprintCallable) void SetBossName(const FText& Name);
	UFUNCTION(BlueprintCallable) void SetPhaseIndicator(int32 CurrentPhase, int32 TotalPhases);
	UFUNCTION(BlueprintCallable) void PlayPhaseTransitionAnim();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UProgressBar* BossHPBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* BossNameText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UHorizontalBox* PhaseIndicators;
};
