#include "Tower/SOLOTowerSubsystem.h"
#include "GameFramework/SOLOGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSOLOTower, Log, All);

void USOLOTowerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentFloorIndex = 0;
	UE_LOG(LogSOLOTower, Log, TEXT("TowerSubsystem inicializado (piso=%d)."), CurrentFloorIndex);
}

void USOLOTowerSubsystem::SetCurrentFloorIndex(int32 NewFloorIndex)
{
	NewFloorIndex = FMath::Max(0, NewFloorIndex);
	if (NewFloorIndex == CurrentFloorIndex)
	{
		return;
	}

	CurrentFloorIndex = NewFloorIndex;

	if (USOLOGameInstance* GI = Cast<USOLOGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.TowerFloorReached = FMath::Max(GI->ActivePlayerData.TowerFloorReached, CurrentFloorIndex);
	}

	OnFloorChanged.Broadcast(CurrentFloorIndex);
	UE_LOG(LogSOLOTower, Log, TEXT("Torre: piso actual -> %d"), CurrentFloorIndex);
}
