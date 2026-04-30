#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUD/SOLOHUDBase.h"
#include "WG_DamageNumber.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class SOLOCORE_API UWG_DamageNumber : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Inicia el nÃºmero de daÃ±o. InLifetime = tiempo hasta Deactivate (pool). */
	void Activate(float DamageAmount, EDamageNumberType Type, bool bIsCritical, FVector WorldLocation, float XScreenOffset = 0.f, float InLifetime = 1.5f);

	// Returns this widget to the pool â€” called at end of lifetime
	void Deactivate();

	UPROPERTY(BlueprintReadOnly, Category = "DamageNumber|State")
	bool bIsActive = false;

protected:
	// Blueprint sets the UTextBlock text, color, and optional scale multiplier
	UFUNCTION(BlueprintImplementableEvent, Category = "DamageNumber|Events")
	void BP_SetDamageText(const FText& Text, FLinearColor Color, float Scale);

	// Blueprint kicks off the pop-in animation
	UFUNCTION(BlueprintImplementableEvent, Category = "DamageNumber|Events")
	void BP_PlayAppearAnimation();

private:
	float Lifetime = 0.8f;
	float ElapsedTime = 0.f;

	FVector CurrentWorldLocation = FVector::ZeroVector;

	// Accumulated vertical rise in screen pixels
	float VerticalRise = 0.f;
	float XOffset = 0.f;

	static constexpr float RiseSpeed = 80.f;   // pixels per second
	static constexpr float FadeStartRatio = 0.7f; // start fade at 70% of lifetime

	static FLinearColor GetColorForType(EDamageNumberType Type);
	static float GetScaleForCritical(bool bIsCritical);
};
