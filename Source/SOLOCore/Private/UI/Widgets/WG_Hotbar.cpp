#include "UI/Widgets/WG_Hotbar.h"

void UWG_Hotbar::NativeConstruct()
{
	Super::NativeConstruct();

	HotbarSlots.SetNum(HotbarSize);
	for (FHotbarSlot& HotbarSlot : HotbarSlots)
	{
		HotbarSlot.bIsEmpty = true;
		HotbarSlot.Quantity = 0;
		HotbarSlot.CooldownRemaining = 0.f;
		HotbarSlot.CooldownTotal = 0.f;
	}
}

void UWG_Hotbar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int32 i = 0; i < HotbarSlots.Num(); ++i)
	{
		FHotbarSlot& HotbarSlot = HotbarSlots[i];
		if (HotbarSlot.CooldownRemaining > 0.f)
		{
			HotbarSlot.CooldownRemaining = FMath::Max(0.f, HotbarSlot.CooldownRemaining - InDeltaTime);
			const float Percent = (HotbarSlot.CooldownTotal > 0.f)
				? HotbarSlot.CooldownRemaining / HotbarSlot.CooldownTotal
				: 0.f;
			BP_UpdateCooldown(i, Percent);
		}
	}
}

void UWG_Hotbar::SetSlotItem(int32 SlotIndex, UTexture2D* Icon, int32 Quantity)
{
	if (!HotbarSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	HotbarSlots[SlotIndex].ItemIcon = Icon;
	HotbarSlots[SlotIndex].Quantity = Quantity;
	HotbarSlots[SlotIndex].bIsEmpty = (Icon == nullptr);
	BP_UpdateSlot(SlotIndex, HotbarSlots[SlotIndex]);
}

void UWG_Hotbar::ClearSlot(int32 SlotIndex)
{
	if (!HotbarSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	HotbarSlots[SlotIndex] = FHotbarSlot();
	BP_UpdateSlot(SlotIndex, HotbarSlots[SlotIndex]);
}

void UWG_Hotbar::UseSlot(int32 SlotIndex, float ItemCooldown)
{
	if (!HotbarSlots.IsValidIndex(SlotIndex) || HotbarSlots[SlotIndex].bIsEmpty)
	{
		return;
	}

	if (ItemCooldown > 0.f)
	{
		HotbarSlots[SlotIndex].CooldownTotal = ItemCooldown;
		HotbarSlots[SlotIndex].CooldownRemaining = ItemCooldown;
	}

	SetActiveSlot(SlotIndex);
	BP_OnSlotUsed(SlotIndex);
}

void UWG_Hotbar::SetActiveSlot(int32 SlotIndex)
{
	ActiveSlotIndex = SlotIndex;

	// Refresh all slots so the active border is applied only to the current one
	for (int32 i = 0; i < HotbarSlots.Num(); ++i)
	{
		BP_UpdateSlot(i, HotbarSlots[i]);
	}
}
