#include "World/ASOLOEnemySpawner.h"

#include "Character/SOLOEnemyCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASOLOEnemySpawner::ASOLOEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASOLOEnemySpawner::BeginPlay()
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

void ASOLOEnemySpawner::SpawnEnemy()
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

	ASOLOEnemyCharacter* Spawned = GetWorld()->SpawnActor<ASOLOEnemyCharacter>(
		EnemyClass,
		SpawnLocation,
		GetActorRotation(),
		Params
	);

	if (!Spawned)
	{
		return;
	}

	Spawned->OnEnemyDied.AddDynamic(this, &ASOLOEnemySpawner::OnSpawnedEnemyDied);
	ActiveEnemyCount++;
	TotalSpawnedCount++;
}

void ASOLOEnemySpawner::OnSpawnedEnemyDied(AActor* DeadEnemy)
{
	if (ASOLOEnemyCharacter* E = Cast<ASOLOEnemyCharacter>(DeadEnemy))
	{
		E->OnEnemyDied.RemoveDynamic(this, &ASOLOEnemySpawner::OnSpawnedEnemyDied);
	}

	ActiveEnemyCount = FMath::Max(0, ActiveEnemyCount - 1);

	if (!HasAuthority())
	{
		return;
	}

	TryScheduleRespawn();
}

void ASOLOEnemySpawner::TryScheduleRespawn()
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
		&ASOLOEnemySpawner::SpawnEnemy,
		RespawnDelay,
		false
	);
}
