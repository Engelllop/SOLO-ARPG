#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOInventoryGridWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void RefreshGrid(class USOLOInventoryComponent* Inventory);
	UFUNCTION(BlueprintCallable) void OnSlotClicked(int32 SlotIndex);
	UFUNCTION(BlueprintCallable) void OnSlotDropped(int32 FromSlot, int32 ToSlot);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UUniformGridPanel* GridPanel;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UUserWidget> SlotWidgetClass;
	UPROPERTY(EditDefaultsOnly) int32 GridColumns = 8;

	UPROPERTY(BlueprintReadOnly) TArray<FInventorySlot> CachedSlots;
};
