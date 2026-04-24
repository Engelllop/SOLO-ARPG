#include "Tower/EMBERVEILTowerSubsystem.h"
#include "GameFramework/EMBERVEILGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogEMBERVEILTower, Log, All);

void UEMBERVEILTowerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentFloorIndex = 0;
	UE_LOG(LogEMBERVEILTower, Log, TEXT("TowerSubsystem inicializado (piso=%d)."), CurrentFloorIndex);
}

void UEMBERVEILTowerSubsystem::SetCurrentFloorIndex(int32 NewFloorIndex)
{
	NewFloorIndex = FMath::Max(0, NewFloorIndex);
	if (NewFloorIndex == CurrentFloorIndex)
	{
		return;
	}

	CurrentFloorIndex = NewFloorIndex;

	if (UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance()))
	{
		GI->ActivePlayerData.TowerFloorReached = FMath::Max(GI->ActivePlayerData.TowerFloorReached, CurrentFloorIndex);
	}

	OnFloorChanged.Broadcast(CurrentFloorIndex);
	UE_LOG(LogEMBERVEILTower, Log, TEXT("Torre: piso actual -> %d"), CurrentFloorIndex);
}
