#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EMBERVEILHUDOverlayWidget.generated.h"

class UWG_VitalBars;
class UWG_ExperienceBar;
class UWG_Minimap;
class UWG_QuestLogWidget;
class UWG_BattleLogWidget;
class UWG_StatusEffectsWidget;
class UWidgetSwitcher;
class UCanvasPanel;

/**
 * HUD Overlay principal — layout "Islas Flotantes" (V4).
 * Compone todos los widgets del HUD en posiciones absolutas.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UEMBERVEILHUDOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// ─── Widgets V4 ───

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|V4")
	bool bUseV4Layout = true;

	/** Top-left: Avatar + Vital Bars (HP/MP/Stamina) */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<UWG_VitalBars> VitalBars;

	/** Top-center: XP Bar fina */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<UWG_ExperienceBar> ExperienceBar;

	/** Top-right: Minimapa */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<UWG_Minimap> MinimapWidget;

	/** Right side: Quest Log (hover expandible) */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<UWG_QuestLogWidget> QuestLog;

	/** Bottom-left: Battle Log (hover expandible) */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<UWG_BattleLogWidget> BattleLog;

	/** Bottom-right: Hotbar */
	UPROPERTY(meta = (BindWidget, OptionalWidget = false))
	TObjectPtr<class UWG_Hotbar> Hotbar;

	/** Inline status effects (junto a vital bars) */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWG_StatusEffectsWidget> StatusEffects;

	/** Boss health bar (se muestra cuando hay boss activo) */
	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<class UWG_BossHealthBar> BossHealthBar;

	/** Combo counter */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UWG_ComboCounter> ComboCounter;

	/** Party frames (multiplayer) */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UWG_PartyFrames> PartyFrames;

	/** Canvas contenedor del layout V4 */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCanvasPanel> V4Canvas;

	/** Switcher para alternar entre layouts (V4 / classic) */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> LayoutSwitcher;

	// ─── Boss helpers ───
	UFUNCTION(BlueprintCallable, Category = "HUD|Boss")
	void ShowBossHealthBar(class AEMBERVEILEnemyCharacter* Boss);

	UFUNCTION(BlueprintCallable, Category = "HUD|Boss")
	void HideBossHealthBar();
};
