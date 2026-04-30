#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOHUDWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void UpdateVitals(float HP, float MaxHP, float MP, float MaxMP, float SP, float MaxSP);
	UFUNCTION(BlueprintCallable) void ShowBossBar(bool bVisible);
	UFUNCTION(BlueprintCallable) void UpdateBossHP(float HP, float MaxHP);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOVitalBarsWidget* VitalBars;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOCombatHUDWidget* CombatHUD;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOBossBarWidget* BossBar;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOMapWidget* MinimapWidget;
};
