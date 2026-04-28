#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_VitalBars.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

/**
 * Vital bars V4 (Islas Flotantes):
 * Avatar circular + nombre + barras HP/MP/Stamina con etiquetas de porcentaje.
 * Incluye slot para status effects inline debajo de las barras.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class EMBERVEILCORE_API UWG_VitalBars : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Sets the target percent; interpolation runs in Tick
	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetHealthPercent(float Percent);

	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetManaPercent(float Percent);

	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetStaminaPercent(float Percent);

	/** Establece el nombre del personaje (ej. "Aelindra") */
	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetCharacterName(const FText& NewName);

	/** Establece el icono del avatar */
	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetAvatarIcon(UTexture2D* Icon);

	/** Establece los valores numéricos para mostrar al lado de las barras */
	UFUNCTION(BlueprintCallable, Category = "VitalBars")
	void SetVitalValues(float Health, float MaxHealth, float Mana, float MaxMana, float Stamina, float MaxStamina);

protected:
	// Exposed to Blueprint so the visual layer (UMG material/progress bars) can read them directly
	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	float HealthPercent = 1.f;

	// Delayed ghost bar — mirrors HealthPercent but lerps at 2.0/s (Dark Souls feel)
	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	float DelayHealthPercent = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	float ManaPercent = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	float StaminaPercent = 1.f;

	// -- Valores numéricos (V4) --
	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentHealth = 0;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentMaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentMana = 0;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentMaxMana = 100;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentStamina = 0;

	UPROPERTY(BlueprintReadOnly, Category = "VitalBars|State")
	int32 CurrentMaxStamina = 100;

	// -- Bindeo widgets V4 --
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> AvatarImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CharacterNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> HealthValueText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ManaValueText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StaminaValueText;

	// Blueprint handles the actual visual update — C++ drives the data
	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnHealthChanged(float NewPercent, float DelayPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnManaChanged(float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnStaminaChanged(float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnVitalValuesUpdated();

private:
	// Display values — interpolated each tick toward their targets
	float CurrentHealthDisplay = 1.f;
	float CurrentManaDisplay = 1.f;
	float CurrentStaminaDisplay = 1.f;
	float CurrentDelayHealthDisplay = 1.f;

	static constexpr float BarInterpSpeed = 8.f;
	static constexpr float DelayBarInterpSpeed = 2.f;

	void UpdateValueTexts();
};
