#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLOEnemySpawner.generated.h"

class ASOLOEnemyCharacter;

/**
 * Actor colocable en el nivel: mantiene hasta MaxActiveEnemies enemigos vivos
 * y puede reponerlos tras un delay cuando mueren.
 */
UCLASS()
class SOLOCORE_API ASOLOEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	ASOLOEnemySpawner();

	virtual void BeginPlay() override;

	/** Genera un enemigo si hay cupo y el lÃ­mite total lo permite (solo autoridad). */
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnEnemy();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<ASOLOEnemyCharacter> EnemyClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = "1"))
	int32 MaxEnemiesAlive = 3;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = "0.0"))
	float RespawnDelay = 10.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner", meta = (ClampMin = "0.0"))
	float SpawnRadius = 500.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner")
	bool bActivateOnBeginPlay = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner")
	bool bRespawnInfinitely = true;

	/** -1 = sin lÃ­mite de spawns acumulados. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawner")
	int32 TotalSpawnLimit = -1;

protected:
	UFUNCTION()
	void OnSpawnedEnemyDied(AActor* DeadEnemy);

	void TryScheduleRespawn();

private:
	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	int32 ActiveEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	int32 TotalSpawnedCount = 0;

	FTimerHandle RespawnTimerHandle;
};
