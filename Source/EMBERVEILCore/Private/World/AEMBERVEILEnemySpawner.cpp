#include "World/AEMBERVEILEnemySpawner.h"

#include "Character/EMBERVEILEnemyCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

AEMBERVEILEnemySpawner::AEMBERVEILEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEMBERVEILEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() || !EnemyClass || !bActivateOnBeginPlay)
	{
		return;
	}

	for (int32 i = 0; i < MaxEnemiesAlive; ++i)
	{
		SpawnEnemy();
	}
}

void AEMBERVEILEnemySpawner::SpawnEnemy()
{
	if (!HasAuthority() || !GetWorld() || !EnemyClass)
	{
		return;
	}

	if (ActiveEnemyCount >= MaxEnemiesAlive)
	{
		return;
	}

	if (TotalSpawnLimit >= 0 && TotalSpawnedCount >= TotalSpawnLimit)
	{
		return;
	}

	const float Angle = FMath::FRand() * 6.2831853f;
	const float R = FMath::Sqrt(FMath::FRand()) * SpawnRadius;
	const FVector Offset(R * FMath::Cos(Angle), R * FMath::Sin(Angle), 0.f);
	const FVector SpawnLocation = GetActorLocation() + Offset;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AEMBERVEILEnemyCharacter* Spawned = GetWorld()->SpawnActor<AEMBERVEILEnemyCharacter>(
		EnemyClass,
		SpawnLocation,
		GetActorRotation(),
		Params
	);

	if (!Spawned)
	{
		return;
	}

	Spawned->OnEnemyDied.AddDynamic(this, &AEMBERVEILEnemySpawner::OnSpawnedEnemyDied);
	ActiveEnemyCount++;
	TotalSpawnedCount++;
}

void AEMBERVEILEnemySpawner::OnSpawnedEnemyDied(AActor* DeadEnemy)
{
	if (AEMBERVEILEnemyCharacter* E = Cast<AEMBERVEILEnemyCharacter>(DeadEnemy))
	{
		E->OnEnemyDied.RemoveDynamic(this, &AEMBERVEILEnemySpawner::OnSpawnedEnemyDied);
	}

	ActiveEnemyCount = FMath::Max(0, ActiveEnemyCount - 1);

	if (!HasAuthority())
	{
		return;
	}

	TryScheduleRespawn();
}

void AEMBERVEILEnemySpawner::TryScheduleRespawn()
{
	if (!bRespawnInfinitely || !EnemyClass)
	{
		return;
	}

	if (TotalSpawnLimit >= 0 && TotalSpawnedCount >= TotalSpawnLimit)
	{
		return;
	}

	if (ActiveEnemyCount >= MaxEnemiesAlive)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&AEMBERVEILEnemySpawner::SpawnEnemy,
		RespawnDelay,
		false
	);
}
