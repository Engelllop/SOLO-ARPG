#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/SOLOEnums.h"
#include "SOLODamageTypes.generated.h"

USTRUCT(BlueprintType)
struct SOLOCORE_API FSOLODamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESOLODamageType DamageType = ESOLODamageType::Slashing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESOLODamageCategory Category = ESOLODamageCategory::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Multiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCrit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritMultiplier = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIgnoresDefense = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AdditionalTags;
};

namespace SOLODamageTags
{
	SOLOCORE_API void InitializeDamageTags();
	SOLOCORE_API FGameplayTag GetDamageTag(ESOLODamageType Type);

	SOLOCORE_API extern FGameplayTag Damage_Slashing;
	SOLOCORE_API extern FGameplayTag Damage_Blunt;
	SOLOCORE_API extern FGameplayTag Damage_Piercing;
	SOLOCORE_API extern FGameplayTag Damage_Fire;
	SOLOCORE_API extern FGameplayTag Damage_Ice;
	SOLOCORE_API extern FGameplayTag Damage_Electric;
	SOLOCORE_API extern FGameplayTag Damage_Holy;
	SOLOCORE_API extern FGameplayTag Damage_Dark;
	SOLOCORE_API extern FGameplayTag Damage_Poison;
	SOLOCORE_API extern FGameplayTag Damage_Weave;
}
