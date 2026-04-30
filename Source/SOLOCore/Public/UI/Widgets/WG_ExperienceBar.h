#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_ExperienceBar.generated.h"

class UProgressBar;
class UTextBlock;
struct FOnAttributeChangeData;

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API UWG_ExperienceBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "XPBar")
	void UpdateExperience(float CurrentXP, float RequiredXP);

	UFUNCTION(BlueprintCallable, Category = "XPBar")
	void TriggerLevelUp(int32 NewLevel);

	void OnExperienceChanged(const FOnAttributeChangeData& Data);
	void OnLevelChanged(const FOnAttributeChangeData& Data);
	void RefreshUI();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "XPBar|State")
	float XPPercent = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "XPBar|State")
	float CurrentXPValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "XPBar|State")
	float RequiredXPValue = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "XPBar|State")
	int32 DisplayLevel = 1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_Experience;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TB_LevelInfo;

	// Golden glow pulse, 1.5s â€” Blueprint drives the animation
	UFUNCTION(BlueprintImplementableEvent, Category = "XPBar|Events")
	void TriggerXPGainAnimation();

	// Full level-up fanfare â€” text, pulse, etc.
	UFUNCTION(BlueprintImplementableEvent, Category = "XPBar|Events")
	void TriggerLevelUpAnimation(int32 NewLevel);

private:
	// Hides the XP text label 3s after the last XP change
	FTimerHandle XPTextVisibilityTimer;

	void HideXPText();
};
