#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOEquipmentScreenWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOEquipmentScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void RefreshEquipment(class USOLOInventoryComponent* Inventory);
	UFUNCTION(BlueprintCallable) void OnSlotClicked(ESOLOEquipmentSlot Slot);

protected:
	// One widget per equipment slot; named slots matched by ESOLOEquipmentSlot
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* HeadSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* ChestSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* LegsSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* FeetSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* MainHandSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* OffHandSlot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* Ring1Slot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUserWidget* Ring2Slot;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* StatSummaryText;
};
