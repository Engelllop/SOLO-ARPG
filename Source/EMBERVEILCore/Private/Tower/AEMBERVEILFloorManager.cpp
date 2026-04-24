#include "Tower/AEMBERVEILFloorManager.h"

#include "Tower/EMBERVEILFloorDataAsset.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogEMBERVEILFloorManager, Log, All);

AEMBERVEILFloorManager::AEMBERVEILFloorManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
}

void AEMBERVEILFloorManager::BeginPlay()
{
	Super::BeginPlay();
}

UEMBERVEILFloorDataAsset* AEMBERVEILFloorManager::FindFloorData(int32 FloorIndex)
{
	for (TSoftObjectPtr<UEMBERVEILFloorDataAsset>& Soft : Floors)
	{
		UEMBERVEILFloorDataAsset* Asset = Soft.IsValid() ? Soft.Get() : Soft.LoadSynchronous();
		if (Asset && Asset->FloorIndex == FloorIndex)
		{
			return Asset;
		}
	}
	return nullptr;
}

void AEMBERVEILFloorManager::LoadFloor(int32 FloorIndex)
{
	UEMBERVEILFloorDataAsset* FloorData = FindFloorData(FloorIndex);
	if (!FloorData)
	{
		UE_LOG(LogEMBERVEILFloorManager, Warning, TEXT("LoadFloor: no hay DataAsset con FloorIndex=%d"), FloorIndex);
		return;
	}

	if (FloorData->FloorLevel.IsNull())
	{
		UE_LOG(LogEMBERVEILFloorManager, Warning, TEXT("LoadFloor: FloorLevel vacío para piso %d"), FloorIndex);
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

void AEMBERVEILFloorManager::SpawnEnemiesForFloor(UEMBERVEILFloorDataAsset* FloorData)
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

	auto TrySpawnAt = [&](TSubclassOf<AEMBERVEILEnemyCharacter> Class) -> AEMBERVEILEnemyCharacter*
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
		return World->SpawnActor<AEMBERVEILEnemyCharacter>(Class, SpawnLoc, SpawnRot, Params);
	};

	for (int32 i = 0; i < NormalCount; ++i)
	{
		if (FloorData->EnemyTypes.Num() == 0)
		{
			break;
		}
		const int32 TypeIdx = FMath::RandRange(0, FloorData->EnemyTypes.Num() - 1);
		TSubclassOf<AEMBERVEILEnemyCharacter> Chosen = FloorData->EnemyTypes[TypeIdx];
		if (AEMBERVEILEnemyCharacter* Spawned = TrySpawnAt(Chosen))
		{
			RegisterSpawnedEnemy(Spawned);
		}
	}

	if (FloorData->BossClass)
	{
		if (AEMBERVEILEnemyCharacter* Boss = TrySpawnAt(FloorData->BossClass))
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

void AEMBERVEILFloorManager::RegisterSpawnedEnemy(AEMBERVEILEnemyCharacter* Enemy)
{
	if (!Enemy)
	{
		return;
	}

	TrackedEnemies.Add(Enemy);
	Enemy->OnDestroyed.AddDynamic(this, &AEMBERVEILFloorManager::OnTrackedEnemyDestroyed);
}

void AEMBERVEILFloorManager::OnTrackedEnemyDestroyed(AActor* DestroyedActor)
{
	TrackedEnemies.RemoveAllSwap(
		[DestroyedActor](const TWeakObjectPtr<AEMBERVEILEnemyCharacter>& Ptr)
		{
			return !Ptr.IsValid() || Ptr.Get() == DestroyedActor;
		});

	if (bTrackingFloorEnemies && TrackedEnemies.Num() == 0)
	{
		bTrackingFloorEnemies = false;
		OnFloorCleared.Broadcast();
	}
}
