#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "WG_Hotbar.generated.h"

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FHotbarSlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "HotbarSlot")
	TObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "HotbarSlot")
	int32 Quantity = 0;

	UPROPERTY(BlueprintReadWrite, Category = "HotbarSlot")
	float CooldownRemaining = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "HotbarSlot")
	float CooldownTotal = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "HotbarSlot")
	bool bIsEmpty = true;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class EMBERVEILCORE_API UWG_Hotbar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void SetSlotItem(int32 SlotIndex, UTexture2D* Icon, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void ClearSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void UseSlot(int32 SlotIndex, float ItemCooldown = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void SetActiveSlot(int32 SlotIndex);

	UPROPERTY(BlueprintReadOnly, Category = "Hotbar|State")
	TArray<FHotbarSlot> HotbarSlots;

	UPROPERTY(BlueprintReadOnly, Category = "Hotbar|State")
	int32 ActiveSlotIndex = -1;

	static constexpr int32 HotbarSize = 9;

protected:
	// Blueprint plays the flash animation and updates the cooldown overlay
	UFUNCTION(BlueprintImplementableEvent, Category = "Hotbar|Events")
	void BP_OnSlotUsed(int32 SlotIndex);

	// Blueprint refreshes the visual for a single slot (icon, quantity, active border)
	UFUNCTION(BlueprintImplementableEvent, Category = "Hotbar|Events")
	void BP_UpdateSlot(int32 SlotIndex, const FHotbarSlot& SlotData);

	// Blueprint updates the sweep-clock cooldown overlay
	UFUNCTION(BlueprintImplementableEvent, Category = "Hotbar|Events")
	void BP_UpdateCooldown(int32 SlotIndex, float Percent);
};
