#include "Save/SOLOSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "Inventory/SOLOInventoryComponent.h"

const FString USOLOSaveSubsystem::AutoSaveSlotName = TEXT("AutoSave");

void USOLOSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USOLOSaveSubsystem::SaveGame(int32 SlotIndex)
{
	CreateBackup(SlotIndex);

	if (!CurrentSave)
		CurrentSave = Cast<USOLOSaveGame>(UGameplayStatics::CreateSaveGameObject(USOLOSaveGame::StaticClass()));

	if (!CurrentSave) { OnSaveCompleted.Broadcast(false); return; }

	CurrentSave->SaveSlot = SlotIndex;
	CurrentSave->SaveTimestamp = FDateTime::Now();
	GatherSaveData();

	FString SlotName = FString::Printf(TEXT("Save_%d"), SlotIndex);
	bool bOK = UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
	OnSaveCompleted.Broadcast(bOK);
}

void USOLOSaveSubsystem::AutoSave()
{
	SaveGame(-1); // slot -1 maps to AutoSaveSlotName in GatherSaveData
}

bool USOLOSaveSubsystem::LoadGame(int32 SlotIndex)
{
	FString SlotName = SlotIndex < 0 ? AutoSaveSlotName : FString::Printf(TEXT("Save_%d"), SlotIndex);
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) { OnLoadCompleted.Broadcast(false); return false; }

	CurrentSave = Cast<USOLOSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!CurrentSave) { OnLoadCompleted.Broadcast(false); return false; }

	ApplySaveData();
	OnLoadCompleted.Broadcast(true);
	return true;
}

void USOLOSaveSubsystem::DeleteSave(int32 SlotIndex)
{
	FString SlotName = FString::Printf(TEXT("Save_%d"), SlotIndex);
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}

bool USOLOSaveSubsystem::DoesSaveExist(int32 SlotIndex) const
{
	FString SlotName = FString::Printf(TEXT("Save_%d"), SlotIndex);
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USOLOSaveSubsystem::CreateBackup(int32 SlotIndex)
{
	if (SlotIndex < 0) return;
	FString Original = FString::Printf(TEXT("Save_%d"), SlotIndex);
	if (!UGameplayStatics::DoesSaveGameExist(Original, 0)) return;

	// Rotate backups: backup2 <- backup1 <- backup0 <- current
	for (int32 i = MaxBackups - 1; i > 0; i--)
	{
		FString From = FString::Printf(TEXT("Save_%d_Backup%d"), SlotIndex, i - 1);
		FString To   = FString::Printf(TEXT("Save_%d_Backup%d"), SlotIndex, i);
		if (UGameplayStatics::DoesSaveGameExist(From, 0))
		{
			USaveGame* BackupData = UGameplayStatics::LoadGameFromSlot(From, 0);
			if (BackupData) UGameplayStatics::SaveGameToSlot(BackupData, To, 0);
		}
	}

	USaveGame* Orig = UGameplayStatics::LoadGameFromSlot(Original, 0);
	if (Orig)
	{
		FString Backup0 = FString::Printf(TEXT("Save_%d_Backup0"), SlotIndex);
		UGameplayStatics::SaveGameToSlot(Orig, Backup0, 0);
	}
}

void USOLOSaveSubsystem::GatherSaveData()
{
	if (!CurrentSave) return;
	UWorld* World = GetGameInstance()->GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC || !PC->GetPawn()) return;

	APawn* Pawn = PC->GetPawn();
	CurrentSave->PlayerPosition = Pawn->GetActorLocation();
	CurrentSave->PlayerRotation = Pawn->GetActorRotation();

	if (USOLOInventoryComponent* Inv = Pawn->FindComponentByClass<USOLOInventoryComponent>())
	{
		CurrentSave->InventorySlots = Inv->GetAllSlots();
		CurrentSave->Gold = Inv->GetCurrency();
	}
	// Level, skills, quests, story flags gathered via Blueprint override in GameMode/PlayerState
}

void USOLOSaveSubsystem::ApplySaveData()
{
	if (!CurrentSave) return;
	UWorld* World = GetGameInstance()->GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC || !PC->GetPawn()) return;

	APawn* Pawn = PC->GetPawn();
	Pawn->SetActorLocationAndRotation(CurrentSave->PlayerPosition, CurrentSave->PlayerRotation);

	if (USOLOInventoryComponent* Inv = Pawn->FindComponentByClass<USOLOInventoryComponent>())
	{
		for (const FInventorySlot& Slot : CurrentSave->InventorySlots)
			Inv->AddItem(Slot.ItemID, Slot.Quantity);
		Inv->AddCurrency(CurrentSave->Gold);
	}
	// Remaining data (level, skills, quests) applied via Blueprint event OnSaveLoaded
}
