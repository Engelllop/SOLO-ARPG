#pragma once

#include "CoreMinimal.h"
#include "Inventory/SOLOItemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOLORarityHelper.generated.h"

UCLASS()
class SOLOCORE_API USOLORarityHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SOLO|Rarity")
	static float GetMultiplier(ESOLOItemRarity Rarity);

	UFUNCTION(BlueprintPure, Category = "SOLO|Rarity")
	static FLinearColor GetColor(ESOLOItemRarity Rarity);

	UFUNCTION(BlueprintPure, Category = "SOLO|Rarity")
	static float GetDropWeight(ESOLOItemRarity Rarity);

	UFUNCTION(BlueprintCallable, Category = "SOLO|Rarity")
	static ESOLOItemRarity RollRarity(float LuckBonus = 0.f);

	UFUNCTION(BlueprintPure, Category = "SOLO|Rarity")
	static FText GetRarityDisplayName(ESOLOItemRarity Rarity);
};
