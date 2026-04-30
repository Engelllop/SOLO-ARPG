#pragma once

#include "CoreMinimal.h"
#include "Data/SOLOEnums.h"
#include "SOLOSkillData.generated.h"

class UGameplayAbility;
class UTexture2D;

USTRUCT(BlueprintType)
struct SOLOCORE_API FSOLOSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FName SkillID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FText SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FName ClassID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	ESOLOSkillType SkillType = ESOLOSkillType::Active;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	ESOLORank RequiredRank = ESOLORank::E;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	int32 SkillPointCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float CooldownSeconds = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float ResourceCost = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<UGameplayAbility> GrantedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	TArray<FName> PrerequisiteSkillIDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	FVector2D TreePosition = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> SkillIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tree")
	TArray<FName> ConnectedToIDs;
};
