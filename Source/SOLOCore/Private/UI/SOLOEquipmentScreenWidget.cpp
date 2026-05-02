#include "UI/SOLOEquipmentScreenWidget.h"
#include "Inventory/SOLOInventoryComponent.h"

void USOLOEquipmentScreenWidget::RefreshEquipment(USOLOInventoryComponent* Inventory)
{
	if (!Inventory) return;
	// Each slot widget updated via Blueprint interface using Inventory->GetEquippedItem(Slot)
}

void USOLOEquipmentScreenWidget::OnSlotClicked(ESOLOEquipmentSlot TargetSlot)
{
	// Unequip or open swap panel handled in Blueprint
}
