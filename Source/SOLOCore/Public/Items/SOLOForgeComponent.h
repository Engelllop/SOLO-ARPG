#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/SOLOItemTypes.h"
#include "Data/SOLOEnums.h"
#include "Data/SOLOUpgradeData.h"
#include "SOLOForgeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUpgraded, FName, ItemID, ESOLOItemRarity, NewRarity);

UCLASS(ClassGroup=(SOLO), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API USOLOForgeComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLOForgeComponent();

	// Returns true if the inventory has all required materials for the upgrade
	UFUNCTION(BlueprintCallable)
	bool CanUpgrade(FName ItemID, class USOLOInventoryComponent* Inventory) const;

	// Consumes materials and elevates item rarity; returns new rarity or None on fail
	UFUNCTION(BlueprintCallable)
	ESOLOItemRarity UpgradeItem(FName ItemID, class USOLOInventoryComponent* Inventory);

	// Adds a rune socket to the item (requires recipe.bAddsRuneSlot)
	UFUNCTION(BlueprintCallable)
	bool AddRuneSlot(FName ItemID, class USOLOInventoryComponent* Inventory);

	// Applies an enchantment GE tag to the item
	UFUNCTION(BlueprintCallable)
	bool ApplyEnchantment(FName ItemID, FName EnchantmentID, class USOLOInventoryComponent* Inventory);

	// Fully repairs item durability at gold cost
	UFUNCTION(BlueprintCallable)
	bool RepairItem(FName ItemID, class USOLOInventoryComponent* Inventory);

	UPROPERTY(BlueprintAssignable) FOnItemUpgraded OnItemUpgraded;

protected:
	UPROPERTY(EditDefaultsOnly) UDataTable* UpgradeRecipeTable;
	UPROPERTY(EditDefaultsOnly) float RepairCostPerDurability = 2.f;

private:
	const FUpgradeRecipe* FindRecipe(FName ItemID) const;
};
