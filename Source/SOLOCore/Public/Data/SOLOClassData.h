#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/SOLOEnums.h"
#include "SOLOClassData.generated.h"

class UGameplayAbility;
class UTexture2D;

USTRUCT(BlueprintType)
struct SOLOCORE_API FSOLOClassData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	FName ClassID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	FText ClassName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseStrength = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseDexterity = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseIntelligence = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseVitality = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseAgility = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseMaxHP = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BaseMaxMP = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FGameplayTagContainer AllowedWeaponTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FGameplayTagContainer AllowedArmorTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
	ESOLOResourceType PrimaryResource = ESOLOResourceType::Mana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> ClassIcon;
};
