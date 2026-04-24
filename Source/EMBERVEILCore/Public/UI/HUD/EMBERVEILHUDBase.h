#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/HUD.h"
#include "EMBERVEILHUDBase.generated.h"

class UWG_MainHUD;
class UWG_BossHealthBar;
class UWG_DamageNumber;
class UWG_ComboCounter;
class UUserWidget;
class AEMBERVEILEnemyCharacter;
class UAbilitySystemComponent;
class UEMBERVEILInventoryComponent;
class UEMBERVEILQuestComponent;

UENUM(BlueprintType)
enum class EDamageNumberType : uint8
{
	Physical,
	Fire,
	Ice,
	Lightning,
	Poison,
	Dark,
	Holy,
	Healing
};

UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API AEMBERVEILHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	AEMBERVEILHUDBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "HUD|Boss")
	void ShowBossHealthBar(AEMBERVEILEnemyCharacter* Boss);

	UFUNCTION(BlueprintCallable, Category = "HUD|Boss")
	void HideBossHealthBar();

	UFUNCTION(BlueprintCallable, Category = "HUD|Damage")
	void SpawnDamageNumber(FVector WorldLocation, float DamageAmount, EDamageNumberType Type, bool bIsCritical);

	/**
	 * Muestra un número de daño del pool: proyección inicial a pantalla, float-up/fade en el widget (1,5s por defecto).
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD|Damage")
	void ShowDamageNumber(float DamageAmount, FVector WorldLocation, EDamageNumberType Type, bool bIsCritical = false);

	UFUNCTION(BlueprintCallable, Category = "HUD|Combo")
	void UpdateComboCounter(int32 CurrentStep, int32 MaxSteps);

	UFUNCTION(BlueprintCallable, Category = "HUD|Combo")
	void ResetComboCounter();

	UFUNCTION(BlueprintCallable, Category = "HUD|Minimap")
	void ToggleMinimap();

	UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
	void ToggleInventory();

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UWG_MainHUD> MainHUDWidget;

	/** HUD de partida (barras vitales, nivel, monedas). WBP_HUD_Overlay por defecto. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> OverlayHUDWidget;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Inventory")
	TObjectPtr<UUserWidget> InventoryWidget;

	/** Hijo del overlay con nombre de variable `WG_ComboCounter` (Designer). */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UWG_ComboCounter> ComboCounterWidget;

	/** Barra de jefe (viewport); se crea en ShowBossHealthBar. */
	UPROPERTY(BlueprintReadOnly, Category = "HUD|Boss")
	TObjectPtr<UWG_BossHealthBar> BossHealthBarWidget;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD|Classes")
	TSubclassOf<UWG_MainHUD> MainHUDWidgetClass;

	/** Si null, el HUD intenta cargar /Game/UI/HUD/WBP_HUD_Overlay.WBP_HUD_Overlay_C */
	UPROPERTY(EditDefaultsOnly, Category = "HUD|Classes")
	TSubclassOf<UUserWidget> OverlayHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD|Classes")
	TSubclassOf<UWG_DamageNumber> DamageNumberWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD|Classes")
	TSubclassOf<UWG_BossHealthBar> BossHealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD|Classes")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

private:
	void BindOverlayReactiveDelegates();
	void UnbindOverlayReactiveDelegates();

	void OnBossEnemyVitalChanged(const FOnAttributeChangeData& Data);

	TWeakObjectPtr<AEMBERVEILEnemyCharacter> CachedBossEnemy;
	TWeakObjectPtr<UAbilitySystemComponent> CachedBossASC;

	/** Inventario al que se enlazó OnCurrencyChanged hacia el overlay (desenlazar en EndPlay). */
	UPROPERTY()
	TObjectPtr<UEMBERVEILInventoryComponent> OverlayReactiveInventory;

	/** QuestComponent enlazado al overlay para el tracker. */
	UPROPERTY()
	TObjectPtr<UEMBERVEILQuestComponent> OverlayReactiveQuest;

	static constexpr int32 DamageNumberPoolSize = 20;

	UPROPERTY()
	TArray<TObjectPtr<UWG_DamageNumber>> DamageNumberPool;

	// Returns the first inactive widget from the pool, cycling from oldest active if all are busy
	UWG_DamageNumber* GetPooledDamageNumber();

	// Offset in screen X to prevent simultaneous hits from stacking exactly
	float NextDamageNumberXOffset = 0.f;
};
