#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ComboCounter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API UWG_ComboCounter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Adds one hit to the combo and resets the fade timer
	UFUNCTION(BlueprintCallable, Category = "ComboCounter")
	void IncrementCombo();

	// Immediately resets the counter and starts a 0.5s fade-out
	UFUNCTION(BlueprintCallable, Category = "ComboCounter")
	void ResetCombo();

	UPROPERTY(BlueprintReadOnly, Category = "ComboCounter|State")
	int32 CurrentCombo = 0;

protected:
	// Blueprint animates the scale-punch on the number label
	UFUNCTION(BlueprintImplementableEvent, Category = "ComboCounter|Events")
	void BP_OnComboIncremented(int32 NewCombo);

	// Blueprint fades out the widget over 0.5s
	UFUNCTION(BlueprintImplementableEvent, Category = "ComboCounter|Events")
	void BP_PlayFadeOut();

private:
	// 1.5s window â€” if no hit arrives, the combo resets
	FTimerHandle ComboFadeTimer;

	static constexpr float ComboTimeout = 1.5f;
	static constexpr int32 MinComboToShow = 2;

	void OnComboTimeout();
};
