#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMBERVEILFloorDataAsset.generated.h"

class AEMBERVEILEnemyCharacter;
class UWorld;

/**
 * Definición de un piso de la torre: enemigos posibles, cantidad, jefe opcional y mapa a cargar.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UEMBERVEILFloorDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Índice lógico del piso (debe coincidir con el subsistema de torre / progresión). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Floor")
	int32 FloorIndex = 0;

	/** Nombre mostrado en UI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Floor")
	FText FloorName;

	/** Pool de clases de enemigos normales para este piso (se elige al azar al spawnear). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat")
	TArray<TSubclassOf<AEMBERVEILEnemyCharacter>> EnemyTypes;

	/** Número mínimo de enemigos normales a generar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat", meta = (ClampMin = "0"))
	int32 MinEnemies = 3;

	/** Número máximo de enemigos normales a generar. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat", meta = (ClampMin = "0"))
	int32 MaxEnemies = 6;

	/** Jefe opcional; si está definido, se genera además de los enemigos normales. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Combat")
	TSubclassOf<AEMBERVEILEnemyCharacter> BossClass;

	/** Nivel (.umap) que representa este piso; se usa con OpenLevel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Level")
	TSoftObjectPtr<UWorld> FloorLevel;
};
