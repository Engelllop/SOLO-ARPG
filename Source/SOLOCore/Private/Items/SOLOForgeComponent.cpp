#include "Items/SOLOForgeComponent.h"
#include "Inventory/SOLOInventoryComponent.h"

USOLOForgeComponent::USOLOForgeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

const FUpgradeRecipe* USOLOForgeComponent::FindRecipe(FName ItemID) const
{
	if (!UpgradeRecipeTable) return nullptr;
	return UpgradeRecipeTable->FindRow<FUpgradeRecipe>(ItemID, TEXT("Forge"));
}

bool USOLOForgeComponent::CanUpgrade(FName ItemID, USOLOInventoryComponent* Inventory) const
{
	if (!Inventory) return false;
	const FUpgradeRecipe* Recipe = FindRecipe(ItemID);
	if (!Recipe) return false;
	if (Inventory->Currency < Recipe->GoldCost) return false;
	for (const TPair<FName, int32>& Mat : Recipe->RequiredMaterials)
		if (Inventory->GetItemQuantity(Mat.Key) < Mat.Value) return false;
	return true;
}

ESOLOItemRarity USOLOForgeComponent::UpgradeItem(FName ItemID, USOLOInventoryComponent* Inventory)
{
	if (!CanUpgrade(ItemID, Inventory)) return ESOLOItemRarity::Common;

	const FUpgradeRecipe* Recipe = FindRecipe(ItemID);
	Inventory->SpendCurrency(Recipe->GoldCost);
	for (const TPair<FName, int32>& Mat : Recipe->RequiredMaterials)
		Inventory->RemoveItem(Mat.Key, Mat.Value);

	OnItemUpgraded.Broadcast(ItemID, Recipe->ResultRarity);
	return Recipe->ResultRarity;
}

bool USOLOForgeComponent::AddRuneSlot(FName ItemID, USOLOInventoryComponent* Inventory)
{
	const FUpgradeRecipe* Recipe = FindRecipe(ItemID);
	if (!Recipe || !Recipe->bAddsRuneSlot) return false;
	return CanUpgrade(ItemID, Inventory) && UpgradeItem(ItemID, Inventory) != ESOLOItemRarity::Common;
}

bool USOLOForgeComponent::ApplyEnchantment(FName ItemID, FName EnchantmentID, USOLOInventoryComponent* Inventory)
{
	if (!Inventory || !Inventory->HasItem(ItemID, 1)) return false;
	// Enchantment application is data-driven; tag is stored in the item slot metadata in BP
	return true;
}

bool USOLOForgeComponent::RepairItem(FName ItemID, USOLOInventoryComponent* Inventory)
{
	if (!Inventory) return false;
	// Repair cost deducted from currency; durability restoration handled in BP/item data
	int32 Cost = FMath::RoundToInt(RepairCostPerDurability * 100.f);
	if (Inventory->Currency < Cost) return false;
	Inventory->SpendCurrency(Cost);
	return true;
}
