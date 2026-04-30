#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOEnchantmentObject.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class SOLOCORE_API USOLOEnchantmentObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FName EnchantmentID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText EnchantmentName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTag EnchantmentTag;
	// GE granting the enchantment bonus
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TSubclassOf<class UGameplayEffect> EnchantmentEffect;
	// Which item types this enchantment can be applied to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<ESOLOItemType> AllowedItemTypes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 GoldCost = 500;
};
