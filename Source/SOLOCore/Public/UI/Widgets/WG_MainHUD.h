#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "WG_MainHUD.generated.h"

class UWG_VitalBars;
class UWG_ExperienceBar;
class UWG_Minimap;
class UWG_AbilityBar;
class UWG_Hotbar;
class UWG_PartyFrames;
class UWG_ComboCounter;
class UAbilitySystemComponent;
class ASOLOPlayerState;

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class SOLOCORE_API UWG_MainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Called by ASOLOHUDBase to wire up minimap toggle
	void ToggleMinimap();

protected:
	// Sub-widget bindings â€” Blueprint must contain widgets with these exact names
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_VitalBars> VitalBarsWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_ExperienceBar> ExperienceBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_Minimap> MinimapWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_AbilityBar> AbilityBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_Hotbar> HotbarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_PartyFrames> PartyFramesWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWG_ComboCounter> ComboCounterWidget;

private:
	UPROPERTY()
	TObjectPtr<ASOLOPlayerState> CachedPlayerState;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	// Attribute change callbacks â€” bound via ASC delegate system
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnManaChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnMaxStaminaChanged(const FOnAttributeChangeData& Data);
	void OnExperienceChanged(const FOnAttributeChangeData& Data);
	void OnLevelChanged(const FOnAttributeChangeData& Data);

	// Cached max values so percent can be computed on any change
	float CachedMaxHealth = 1.f;
	float CachedMaxMana = 1.f;
	float CachedMaxStamina = 1.f;
	float CachedRequiredXP = 1.f;

	void BindASCDelegates();
};
