#pragma once

#include "CoreMinimal.h"
#include "Data/SOLOEnums.h"
#include "SOLOCompanionData.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct SOLOCORE_API FCompanionArchetypeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESOLOCompanionArchetype Archetype = ESOLOCompanionArchetype::RangedDPS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PreferredRangeMin = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PreferredRangeMax = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DodgeChance = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanParry = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Aggressiveness = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> AutoAbilities;
};
