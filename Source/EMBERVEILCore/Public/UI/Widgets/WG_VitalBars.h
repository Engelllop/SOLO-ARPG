#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_VitalBars.generated.h"

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

	// Blueprint handles the actual visual update — C++ drives the data
	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnHealthChanged(float NewPercent, float DelayPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnManaChanged(float NewPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "VitalBars|Events")
	void BP_OnStaminaChanged(float NewPercent);

private:
	// Display values — interpolated each tick toward their targets
	float CurrentHealthDisplay = 1.f;
	float CurrentManaDisplay = 1.f;
	float CurrentStaminaDisplay = 1.f;
	float CurrentDelayHealthDisplay = 1.f;

	static constexpr float BarInterpSpeed = 8.f;
	static constexpr float DelayBarInterpSpeed = 2.f;
};
