#pragma once

#include "CoreMinimal.h"
#include "SOLOTowerTypes.generated.h"

/** Identificador de planta o segmento dentro de la torre (placeholder hasta data assets). */
USTRUCT(BlueprintType)
struct FTowerFloorId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	int32 FloorIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower")
	FName FloorTag = NAME_None;
};
