#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_BossHealthBar.generated.h"

class ASOLOEnemyCharacter;
class UProgressBar;
class UTextBlock;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;
struct FGameplayTag;
struct FGameplayEventData;

// Barra de jefe en pantalla (HUD) o embebida en un WidgetComponent del enemigo.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API UWG_BossHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Called once when the WidgetComponent activates the boss encounter
	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void InitBoss(const FText& InBossName, int32 InTotalPhases);

	/** Referencia opcional al jefe (fases, nombre, etc.). */
	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void SetBossReference(ASOLOEnemyCharacter* Boss);

	// HealthPercent in [0,1]; triggers phase-change logic when crossing PhaseThresholds
	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void UpdateHealth(float NewHealthPercent);

	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void UpdateHealthPercent(float Percent);

	/** Enlaza el widget a un jefe especÃ­fico, registrando delegates de GAS. */
	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void BindToBoss(ASOLOEnemyCharacter* Boss);

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnBossDied(const FGameplayEventData* Payload);

	UPROPERTY(BlueprintReadOnly, Category = "BossBar|State")
	FText BossName;

	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void SetPhase(int32 NewPhase);

	// Fades the widget in or out; 0.5s default matches design spec
	UFUNCTION(BlueprintCallable, Category = "BossBar")
	void SetVisible(bool bVisible, float FadeDuration = 0.5f);

	UPROPERTY(BlueprintReadOnly, Category = "BossBar|State")
	float CurrentHealthPercent = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "BossBar|State")
	int32 CurrentPhase = 1;

	UPROPERTY(BlueprintReadOnly, Category = "BossBar|State")
	int32 TotalPhases = 1;

	UPROPERTY(BlueprintReadOnly, Category = "BossBar|State")
	FText BossNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_BossHealth;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_BossName;

protected:
	// Blueprint plays the flash-red + phase-transition text animation
	UFUNCTION(BlueprintImplementableEvent, Category = "BossBar|Events")
	void BP_OnPhaseChange(int32 NewPhase, const FText& PhaseText);

	// Blueprint drives the bar fill and any HP-loss particle
	UFUNCTION(BlueprintImplementableEvent, Category = "BossBar|Events")
	void BP_OnHealthChanged(float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "BossBar|Events")
	void BP_PlayFade(bool bFadeIn, float Duration);

private:
	TWeakObjectPtr<ASOLOEnemyCharacter> CachedBoss;

	// Thresholds in descending order so we only check the current-phase boundary
	TArray<float> PhaseHealthThresholds;

	bool bPhaseDirty = false;
};
