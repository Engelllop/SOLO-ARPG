#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AEMBERVEILFloorManager.generated.h"

class UEMBERVEILFloorDataAsset;
class AEMBERVEILEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorCleared);

/**
 * Actor colocado en mapas de torre: define la lista de pisos (DataAssets) y genera enemigos
 * o carga el mapa del piso indicado.
 */
UCLASS(Blueprintable, BlueprintType)
class EMBERVEILCORE_API AEMBERVEILFloorManager : public AActor
{
	GENERATED_BODY()

public:
	AEMBERVEILFloorManager();

	virtual void BeginPlay() override;

	/** Pisos disponibles (referencias suaves a DataAssets). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Floors")
	TArray<TSoftObjectPtr<UEMBERVEILFloorDataAsset>> Floors;

	/** Radio alrededor del manager donde se intentan colocar enemigos. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Spawn", meta = (ClampMin = "100.0"))
	float SpawnRadius = 800.f;

	/** Altura desde la que se lanza el trace vertical para apoyar en suelo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Spawn")
	float SpawnTraceHeight = 1500.f;

	/** Carga el mapa asociado al piso con el FloorIndex dado (OpenLevel). */
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void LoadFloor(int32 FloorIndex);

	/** Genera enemigos según Min/Max, EnemyTypes y BossClass opcional. */
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void SpawnEnemiesForFloor(UEMBERVEILFloorDataAsset* FloorData);

	/** Se dispara cuando todos los enemigos generados por el último SpawnEnemiesForFloor han muerto o sido destruidos. */
	UPROPERTY(BlueprintAssignable, Category = "Tower|Events")
	FOnFloorCleared OnFloorCleared;

protected:
	/** Resuelve el DataAsset para un FloorIndex (carga síncrona si hace falta). */
	UEMBERVEILFloorDataAsset* FindFloorData(int32 FloorIndex);

	void RegisterSpawnedEnemy(AEMBERVEILEnemyCharacter* Enemy);

	UFUNCTION()
	void OnTrackedEnemyDestroyed(AActor* DestroyedActor);

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AEMBERVEILEnemyCharacter>> TrackedEnemies;

	bool bTrackingFloorEnemies = false;
};
