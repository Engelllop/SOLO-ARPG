#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOLOFloorDataAsset.generated.h"

class ASOLOEnemyCharacter;
class UWorld;

/**
 * DefiniciÃ³n de un piso de la torre: enemigos posibles, cantidad, jefe opcional y mapa a cargar.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API USOLOFloorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Ãndice lÃ³gico del piso (debe coincidir con el subsistema de torre / progresiÃ³n). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Floor")
	int32 FloorIndex = 0;

	/** Nombre mostrado en UI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Floor")
	FText FloorName;

	/** Pool de clases de enemigos normales para este piso (se elige al azar al spawnear). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat")
	TArray<TSubclassOf<ASOLOEnemyCharacter>> EnemyTypes;

	/** NÃºmero mÃ­nimo de enemigos normales a generar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat", meta = (ClampMin = "0"))
	int32 MinEnemies = 3;

	/** NÃºmero mÃ¡ximo de enemigos normales a generar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat", meta = (ClampMin = "0"))
	int32 MaxEnemies = 6;

	/** Jefe opcional; si estÃ¡ definido, se genera ademÃ¡s de los enemigos normales. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat")
	TSubclassOf<ASOLOEnemyCharacter> BossClass;

	/** Nivel (.umap) que representa este piso; se usa con OpenLevel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Level")
	TSoftObjectPtr<UWorld> FloorLevel;
};
