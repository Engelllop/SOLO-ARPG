#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EMBERVEILHUDOverlayWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UCanvasPanel;
class UVerticalBox;
class UAbilitySystemComponent;
class UEMBERVEILInventoryComponent;
class UEMBERVEILQuestComponent;
class AEMBERVEILPlayerState;

/**
 * HUD de partida: barras vitales (HP/MP/Stamina), nivel y monedas.
 * Construye el layout en código; WBP_HUD_Overlay hereda de esta clase.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILHUDOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Actualiza el texto del tracker de misión principal (llamado desde el HUD vía delegates). */
	void RefreshQuestTracker(UEMBERVEILQuestComponent* QuestComp);

	/** Objetivo del delegate de moneda; enlazado desde AEMBERVEILHUDBase (no desde NativeConstruct). */
	UFUNCTION()
	void OnCurrencyChanged(int32 NewAmount);

	UFUNCTION()
	void OnQuestObjectiveProgressUpdated(FName QuestID, FName ObjectiveID, int32 NewCount);

protected:
	void BuildLayout();
	void TryBindToPlayerState();
	void UnbindAll();

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnMaxStaminaChanged(const FOnAttributeChangeData& Data);
	void OnLevelChanged(const FOnAttributeChangeData& Data);

	void UpdateBars();
	void RefreshLevelText();
	void RefreshCurrencyText();

private:
	UPROPERTY(Transient)
	TObjectPtr<UCanvasPanel> RootCanvas;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Health;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Mana;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Stamina;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_Gold;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_QuestTracker;

	UPROPERTY(Transient)
	TObjectPtr<AEMBERVEILPlayerState> CachedPlayerState;

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	UPROPERTY(Transient)
	TObjectPtr<UEMBERVEILInventoryComponent> CachedInventory;

	UPROPERTY(Transient)
	TObjectPtr<UEMBERVEILQuestComponent> CachedQuestComponent;

	float CachedMaxHealth = 1.f;
	float CachedMaxMana = 1.f;
	float CachedMaxStamina = 1.f;

	bool bDelegatesBound = false;
	int32 BindRetryCount = 0;

	FTimerHandle BindRetryTimerHandle;

	void ScheduleBindRetry();
};
