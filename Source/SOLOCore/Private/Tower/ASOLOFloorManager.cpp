#include "Tower/ASOLOFloorManager.h"

#include "Tower/SOLOFloorDataAsset.h"
#include "Character/SOLOEnemyCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSOLOFloorManager, Log, All);

ASOLOFloorManager::ASOLOFloorManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
}

void ASOLOFloorManager::BeginPlay()
{
	Super::BeginPlay();
}

USOLOFloorDataAsset* ASOLOFloorManager::FindFloorData(int32 FloorIndex)
{
	for (TSoftObjectPtr<USOLOFloorDataAsset>& Soft : Floors)
	{
		USOLOFloorDataAsset* Asset = Soft.IsValid() ? Soft.Get() : Soft.LoadSynchronous();
		if (Asset && Asset->FloorIndex == FloorIndex)
		{
			return Asset;
		}
	}
	return nullptr;
}

void ASOLOFloorManager::LoadFloor(int32 FloorIndex)
{
	USOLOFloorDataAsset* FloorData = FindFloorData(FloorIndex);
	if (!FloorData)
	{
		UE_LOG(LogSOLOFloorManager, Warning, TEXT("LoadFloor: no hay DataAsset con FloorIndex=%d"), FloorIndex);
		return;
	}

	if (FloorData->FloorLevel.IsNull())
	{
		UE_LOG(LogSOLOFloorManager, Warning, TEXT("LoadFloor: FloorLevel vacÃ­o para piso %d"), FloorIndex);
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// En multijugador el viaje de mapa debe hacerse con autoridad.
	if (World->GetNetMode() == NM_Client)
	{
		return;
	}

	const FSoftObjectPath WorldPath = FloorData->FloorLevel.ToSoftObjectPath();
	if (!WorldPath.IsValid())
	{
		return;
	}

	const FString PackageName = WorldPath.GetLongPackageName();
	UGameplayStatics::OpenLevel(this, FName(*PackageName));
}

void ASOLOFloorManager::SpawnEnemiesForFloor(USOLOFloorDataAsset* FloorData)
{
	if (!FloorData)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (World->GetNetMode() != NM_Standalone && GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	TrackedEnemies.Empty();
	bTrackingFloorEnemies = false;

	int32 MinC = FMath::Min(FloorData->MinEnemies, FloorData->MaxEnemies);
	int32 MaxC = FMath::Max(FloorData->MinEnemies, FloorData->MaxEnemies);
	const int32 NormalCount = FMath::RandRange(MinC, MaxC);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto TrySpawnAt = [&](TSubclassOf<ASOLOEnemyCharacter> Class) -> ASOLOEnemyCharacter*
	{
		if (!Class)
		{
			return nullptr;
		}

		FVector SpawnLoc = GetActorLocation();
		const float Angle = FMath::FRand() * 2.f * PI;
		const float R = FMath::FRandRange(100.f, SpawnRadius);
		const FVector TraceStart = GetActorLocation() + FVector(FMath::Cos(Angle) * R, FMath::Sin(Angle) * R, SpawnTraceHeight);
		const FVector TraceEnd = TraceStart - FVector(0.f, 0.f, SpawnTraceHeight * 2.f);

		FHitResult Hit;
		if (World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
		{
			SpawnLoc = Hit.ImpactPoint + FVector(0.f, 0.f, 20.f);
		}

		const FRotator SpawnRot = FRotator::ZeroRotator;
		return World->SpawnActor<ASOLOEnemyCharacter>(Class, SpawnLoc, SpawnRot, Params);
	};

	for (int32 i = 0; i < NormalCount; ++i)
	{
		if (FloorData->EnemyTypes.Num() == 0)
		{
			break;
		}
		const int32 TypeIdx = FMath::RandRange(0, FloorData->EnemyTypes.Num() - 1);
		TSubclassOf<ASOLOEnemyCharacter> Chosen = FloorData->EnemyTypes[TypeIdx];
		if (ASOLOEnemyCharacter* Spawned = TrySpawnAt(Chosen))
		{
			RegisterSpawnedEnemy(Spawned);
		}
	}

	if (FloorData->BossClass)
	{
		if (ASOLOEnemyCharacter* Boss = TrySpawnAt(FloorData->BossClass))
		{
			RegisterSpawnedEnemy(Boss);
		}
	}

	if (TrackedEnemies.Num() == 0)
	{
		OnFloorCleared.Broadcast();
		return;
	}

	bTrackingFloorEnemies = true;
}

void ASOLOFloorManager::RegisterSpawnedEnemy(ASOLOEnemyCharacter* Enemy)
{
	if (!Enemy)
	{
		return;
	}

	TrackedEnemies.Add(Enemy);
	Enemy->OnDestroyed.AddDynamic(this, &ASOLOFloorManager::OnTrackedEnemyDestroyed);
}

void ASOLOFloorManager::OnTrackedEnemyDestroyed(AActor* DestroyedActor)
{
	TrackedEnemies.RemoveAllSwap(
		[DestroyedActor](const TWeakObjectPtr<ASOLOEnemyCharacter>& Ptr)
		{
			return !Ptr.IsValid() || Ptr.Get() == DestroyedActor;
		});

	if (bTrackingFloorEnemies && TrackedEnemies.Num() == 0)
	{
		bTrackingFloorEnemies = false;
		OnFloorCleared.Broadcast();
	}
}
