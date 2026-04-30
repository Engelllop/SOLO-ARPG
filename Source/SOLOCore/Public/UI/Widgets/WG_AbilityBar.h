#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/Texture2D.h"
#include "WG_AbilityBar.generated.h"

USTRUCT(BlueprintType)
struct SOLOCORE_API FSOLOAbilitySlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	FGameplayTag AssignedAbilityTag;

	// Player level required to unlock this slot
	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	int32 UnlockLevel = 1;

	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	bool bIsUnlocked = false;

	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	float CooldownRemaining = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	float CooldownTotal = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "AbilitySlot")
	TObjectPtr<UTexture2D> AbilityIcon = nullptr;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class SOLOCORE_API UWG_AbilityBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Called whenever the player's level changes â€” unlocks newly eligible slots
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void CheckLevelUnlocks(int32 CurrentLevel);

	// Fired by the ASC cooldown delegate
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void OnAbilityCooldownStart(FGameplayTag AbilityTag, float Duration);

	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void AssignAbilityToSlot(int32 SlotIndex, FGameplayTag AbilityTag, UTexture2D* Icon);

	UPROPERTY(BlueprintReadOnly, Category = "AbilityBar|State")
	TArray<FSOLOAbilitySlot> AbilitySlots;

protected:
	// Blueprint animates the lock-crack effect for the given slot index
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityBar|Events")
	void BP_OnSlotUnlocked(int32 SlotIndex);

	// Blueprint draws the sweep-clock cooldown overlay (Percent = remaining/total)
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityBar|Events")
	void BP_UpdateCooldown(int32 SlotIndex, float Percent);

	// Blueprint refreshes the full visual state of a slot
	UFUNCTION(BlueprintImplementableEvent, Category = "AbilityBar|Events")
	void BP_UpdateSlot(int32 SlotIndex, const FSOLOAbilitySlot& SlotData);

private:
	// Unlock levels mirror the design spec: 1,5,10,20,35,50
	static const TArray<int32> SlotUnlockLevels;
};
