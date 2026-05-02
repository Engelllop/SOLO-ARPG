#include "Save/SOLOSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Inventory/SOLOItemInstance.h"

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

	FString SlotName = SlotIndex < 0 ? AutoSaveSlotName : FString::Printf(TEXT("Save_%d"), SlotIndex);
	bool bOK = UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
	OnSaveCompleted.Broadcast(bOK);
}

void USOLOSaveSubsystem::AutoSave()
{
	SaveGame(-1);
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
		CurrentSave->InventorySlots = Inv->GetAllItems();
		CurrentSave->Gold = Inv->Currency;
	}
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
		{
			if (Slot.ItemInstance && Slot.ItemInstance->ItemData)
			{
				Inv->AddItem(Slot.ItemInstance->ItemData, Slot.Quantity);
			}
		}
		Inv->AddCurrency(CurrentSave->Gold);
	}
}

// --- Floor transition ---

void USOLOSaveSubsystem::SaveOnFloorTransition(int32 NextFloorID, const FVector& NewPosition)
{
	if (!CurrentSave)
	{
		CurrentSave = Cast<USOLOSaveGame>(UGameplayStatics::CreateSaveGameObject(USOLOSaveGame::StaticClass()));
	}
	if (!CurrentSave) return;

	CurrentSave->FloorID = NextFloorID;
	CurrentSave->PlayerPosition = NewPosition;
	CurrentSave->SaveTimestamp = FDateTime::Now();

	GatherSaveData();

	FString SlotName = FString::Printf(TEXT("AutoSave_Floor%d"), NextFloorID);
	UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
}

// ========== v3 — Ending Requirement Checks ==========

bool USOLOSaveSubsystem::HasAllEndingBFragments() const
{
	if (!CurrentSave) return false;
	return CheckEndingBRequirements();
}

bool USOLOSaveSubsystem::CheckEndingBRequirements() const
{
	if (!CurrentSave) return false;

	// v3 requirements: Fragmento del Arquitecto + Escuchar al Verdugo
	if (!CurrentSave->CollectedFragments.Contains(TEXT("Fragment_Arquitecto")))
		return false;
	if (!CurrentSave->bListenedVerdugoN9)
		return false;

	// Additional: need the actual fragment item
	if (!CurrentSave->StoryFlags.FindRef(TEXT("N9.FragmentArquitecto")))
		return false;

	return true;
}

bool USOLOSaveSubsystem::CheckEndingCRequirements() const
{
	if (!CurrentSave) return false;

	// v3: comprension tematica — opciones mas oscuras
	if (!CurrentSave->bAteHumanFleshN4) return false;
	if (CurrentSave->bGaveCriadaName) return false; // must NOT name her
	if (!CurrentSave->bListenedVerdugoN9) return false;
	if (!CurrentSave->StoryFlags.FindRef(TEXT("N9.VerdugoListened"))) return false;

	return true;
}

// --- NG+ ---

void USOLOSaveSubsystem::StartNewGamePlus()
{
	if (!CurrentSave)
	{
		CurrentSave = Cast<USOLOSaveGame>(UGameplayStatics::CreateSaveGameObject(USOLOSaveGame::StaticClass()));
	}
	if (!CurrentSave) return;

	CurrentSave->bIsNewGamePlus = true;
	CurrentSave->NGPlusCount++;
	CurrentSave->bIsNewGamePlus = true;

	// Keep unlocked skills but reset position
	CurrentSave->PlayerPosition = FVector::ZeroVector;
	CurrentSave->PlayerRotation = FRotator::ZeroRotator;
	CurrentSave->FloorID = 0;

	CurrentSave->SaveTimestamp = FDateTime::Now();

	FString SlotName = FString::Printf(TEXT("NGPlus_%d"), CurrentSave->NGPlusCount);
	UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
}

bool USOLOSaveSubsystem::IsNewGamePlus() const
{
	return CurrentSave && CurrentSave->bIsNewGamePlus;
}

int32 USOLOSaveSubsystem::GetNGPlusCount() const
{
	return CurrentSave ? CurrentSave->NGPlusCount : 0;
}

// --- Choice tracking (v3) ---

void USOLOSaveSubsystem::RecordCollectedFragment(FName FragmentID)
{
	if (!CurrentSave) return;
	if (!CurrentSave->CollectedFragments.Contains(FragmentID))
	{
		CurrentSave->CollectedFragments.Add(FragmentID);
	}
}

void USOLOSaveSubsystem::RecordAbyssLooked()
{
	if (CurrentSave) CurrentSave->bLookedIntoAbyss = true;
}

void USOLOSaveSubsystem::RecordSparedNPC(const FString& NPCName, bool bSpared)
{
	if (!CurrentSave) return;
	if (NPCName == TEXT("Elia")) CurrentSave->bSparedElia = bSpared;
	if (NPCName == TEXT("Oswin")) CurrentSave->bSparedOswin = bSpared;
}

void USOLOSaveSubsystem::RecordCriadaName(const FString& Name)
{
	if (!CurrentSave) return;
	CurrentSave->bGaveCriadaName = true;
	CurrentSave->GivenNameCriada = Name;
}

void USOLOSaveSubsystem::RecordCloneDefeat(bool bPeaceful)
{
	if (CurrentSave) CurrentSave->bClonedDefeatedPeacefully = bPeaceful;
}

void USOLOSaveSubsystem::RecordEnding(FName EndingID)
{
	if (CurrentSave && !CurrentSave->PreviousEndings.Contains(EndingID))
	{
		CurrentSave->PreviousEndings.Add(EndingID);
	}
}

// v3 — New choice tracking
void USOLOSaveSubsystem::RecordAteHumanFlesh()
{
	if (CurrentSave) CurrentSave->bAteHumanFleshN4 = true;
}

void USOLOSaveSubsystem::RecordListenedVerdugo()
{
	if (CurrentSave)
	{
		CurrentSave->bListenedVerdugoN9 = true;
		CurrentSave->StoryFlags.Add(TEXT("N9.VerdugoListened"), true);
	}
}

void USOLOSaveSubsystem::RecordSawN85Subfloor()
{
	if (CurrentSave) CurrentSave->bSawN85Subfloor = true;
}

void USOLOSaveSubsystem::RecordCodexEntry(FName EntryID)
{
	if (CurrentSave && !CurrentSave->UnlockedCodexEntries.Contains(EntryID))
	{
		CurrentSave->UnlockedCodexEntries.Add(EntryID);
	}
}

bool USOLOSaveSubsystem::HasCodexEntry(FName EntryID) const
{
	return CurrentSave && CurrentSave->UnlockedCodexEntries.Contains(EntryID);
}
