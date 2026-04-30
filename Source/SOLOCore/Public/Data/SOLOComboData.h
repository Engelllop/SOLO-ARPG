#pragma once

#include "CoreMinimal.h"
#include "Data/SOLOEnums.h"
#include "GameplayTagContainer.h"
#include "SOLOComboData.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct SOLOCORE_API FComboStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PostureMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SPCost = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFinale = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUncancellable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttackIndicatorTag;
};

USTRUCT(BlueprintType)
struct SOLOCORE_API FWeaponComboData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboStep> LightComboSteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FComboStep> HeavyComboSteps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ComboInputWindow = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FinaleDamageBonus = 2.0f;
};
