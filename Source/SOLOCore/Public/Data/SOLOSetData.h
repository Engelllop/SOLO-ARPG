#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SOLOSetData.generated.h"

USTRUCT(BlueprintType)
struct SOLOCORE_API FSOLOSetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SetID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SetLore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> PieceIDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> Bonus2Pieces;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> Bonus4Pieces;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Bonus2Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Bonus4Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor SetColor = FLinearColor::White;
};
