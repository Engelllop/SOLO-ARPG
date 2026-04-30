#pragma once

#include "CoreMinimal.h"
#include "Inventory/SOLOItemTypes.h"
#include "Data/SOLOEnums.h"
#include "SOLOUpgradeData.generated.h"

USTRUCT(BlueprintType)
struct SOLOCORE_API FUpgradeRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName UpgradeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESOLOItemRarity RequiredRarity = ESOLOItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESOLOItemRarity ResultRarity = ESOLOItemRarity::Uncommon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GoldCost = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, int32> RequiredMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESOLOStationLevel RequiredStation = ESOLOStationLevel::Basic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ResultEnchantmentID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAddsRuneSlot = false;
};
