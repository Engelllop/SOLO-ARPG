#include "UI/SOLOInventoryGridWidget.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void USOLOInventoryGridWidget::RefreshGrid(USOLOInventoryComponent* Inventory)
{
	if (!Inventory || !GridPanel || !SlotWidgetClass) return;
	GridPanel->ClearChildren();

	CachedSlots = Inventory->GetAllSlots();
	for (int32 i = 0; i < CachedSlots.Num(); i++)
	{
		UUserWidget* SlotWidget = CreateWidget<UUserWidget>(this, SlotWidgetClass);
		if (!SlotWidget) continue;
		UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(SlotWidget);
		if (GridSlot)
		{
			GridSlot->SetColumn(i % GridColumns);
			GridSlot->SetRow(i / GridColumns);
		}
		// Slot data passed via Blueprint interface on SlotWidget
	}
}

void USOLOInventoryGridWidget::OnSlotClicked(int32 SlotIndex)
{
	// Item use / context menu handled in Blueprint
}

void USOLOInventoryGridWidget::OnSlotDropped(int32 FromSlot, int32 ToSlot)
{
	// Swap handled in Blueprint via InventoryComponent
}
