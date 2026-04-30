#include "UI/Widgets/WG_AbilityBar.h"

// Mirrors the design spec unlock gates exactly
const TArray<int32> UWG_AbilityBar::SlotUnlockLevels = { 1, 5, 10, 20, 35, 50 };

void UWG_AbilityBar::NativeConstruct()
{
	Super::NativeConstruct();

	AbilitySlots.SetNum(6);
	for (int32 i = 0; i < AbilitySlots.Num(); ++i)
	{
		AbilitySlots[i].UnlockLevel = SlotUnlockLevels[i];
		AbilitySlots[i].bIsUnlocked = (i == 0); // slot 0 always open at level 1
		AbilitySlots[i].CooldownRemaining = 0.f;
		AbilitySlots[i].CooldownTotal = 0.f;
	}
}

void UWG_AbilityBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int32 i = 0; i < AbilitySlots.Num(); ++i)
	{
		FSOLOAbilitySlot& AbilitySlot = AbilitySlots[i];
		if (AbilitySlot.CooldownRemaining > 0.f)
		{
			AbilitySlot.CooldownRemaining = FMath::Max(0.f, AbilitySlot.CooldownRemaining - InDeltaTime);
			const float Percent = (AbilitySlot.CooldownTotal > 0.f)
				? AbilitySlot.CooldownRemaining / AbilitySlot.CooldownTotal
				: 0.f;
			BP_UpdateCooldown(i, Percent);
		}
	}
}

void UWG_AbilityBar::CheckLevelUnlocks(int32 CurrentLevel)
{
	for (int32 i = 0; i < AbilitySlots.Num(); ++i)
	{
		FSOLOAbilitySlot& AbilitySlot = AbilitySlots[i];
		if (!AbilitySlot.bIsUnlocked && CurrentLevel >= AbilitySlot.UnlockLevel)
		{
			AbilitySlot.bIsUnlocked = true;
			BP_OnSlotUnlocked(i);
			BP_UpdateSlot(i, AbilitySlot);
		}
	}
}

void UWG_AbilityBar::OnAbilityCooldownStart(FGameplayTag AbilityTag, float Duration)
{
	for (int32 i = 0; i < AbilitySlots.Num(); ++i)
	{
		if (AbilitySlots[i].AssignedAbilityTag == AbilityTag)
		{
			AbilitySlots[i].CooldownTotal = Duration;
			AbilitySlots[i].CooldownRemaining = Duration;
			BP_UpdateCooldown(i, 1.f);
			break;
		}
	}
}

void UWG_AbilityBar::AssignAbilityToSlot(int32 SlotIndex, FGameplayTag AbilityTag, UTexture2D* Icon)
{
	if (!AbilitySlots.IsValidIndex(SlotIndex) || !AbilitySlots[SlotIndex].bIsUnlocked)
	{
		return;
	}

	AbilitySlots[SlotIndex].AssignedAbilityTag = AbilityTag;
	AbilitySlots[SlotIndex].AbilityIcon = Icon;
	BP_UpdateSlot(SlotIndex, AbilitySlots[SlotIndex]);
}
