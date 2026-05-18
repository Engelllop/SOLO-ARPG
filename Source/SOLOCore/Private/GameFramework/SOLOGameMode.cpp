#include "GameFramework/SOLOGameMode.h"
#include "UI/HUD/SOLOHUDBase.h"
#include "Character/SOLOPlayerCharacter.h"
#include "Player/SOLOPlayerState.h"
#include "Player/SOLOPlayerController.h"
#include "Profile/SOLOSaveManager.h"
#include "UI/SOLONotificationSystem.h"
#include "Profile/SOLOLocalProfileSave.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Text.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ASOLOGameMode::ASOLOGameMode()
{
	// C++ por defecto. Si existe BP en Content/SOLO/, lo usa.
	DefaultPawnClass = ASOLOPlayerCharacter::StaticClass();
	{
		static ConstructorHelpers::FClassFinder<APawn> BpPawnFinder(TEXT("/Game/SOLO/Characters/Player/BP_SOLOPlayerCharacter"));
		if (BpPawnFinder.Succeeded())
		{
			DefaultPawnClass = BpPawnFinder.Class;
		}
	}

	PlayerStateClass = ASOLOPlayerState::StaticClass();

	PlayerControllerClass = ASOLOPlayerController::StaticClass();
	{
		static ConstructorHelpers::FClassFinder<APlayerController> BpPlayerControllerFinder(
			TEXT("/Game/SOLO/Blueprints/BP_SOLOPlayerController"));
		if (BpPlayerControllerFinder.Succeeded())
		{
			PlayerControllerClass = BpPlayerControllerFinder.Class;
		}
	}

	HUDClass = ASOLOHUDBase::StaticClass();
	{
		static ConstructorHelpers::FClassFinder<AHUD> BpHUDFinder(
			TEXT("/Game/SOLO/UI/BP_SOLOHUD"));
		if (BpHUDFinder.Succeeded())
		{
			HUDClass = BpHUDFinder.Class;
		}
	}
}

void ASOLOGameMode::PlayerDied(APlayerController* PlayerController)
{
	if (!PlayerController) return;

	if (RespawnDelay <= 0.f)
	{
		DelayedRespawn(PlayerController);
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &ASOLOGameMode::DelayedRespawn, PlayerController),
			RespawnDelay,
			false
		);
	}
}

void ASOLOGameMode::DelayedRespawn(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	AActor* StartSpot = FindPlayerStart(PlayerController);
	RestartPlayerAtPlayerStart(PlayerController, StartSpot);

	if (!bRespawnAtLastSave)
	{
		return;
	}

	UGameInstance* GI = PlayerController->GetGameInstance();
	if (!GI)
	{
		return;
	}

	USOLOSaveManager* SaveMgr = GI->GetSubsystem<USOLOSaveManager>();
	if (!SaveMgr)
	{
		return;
	}

	static const FString RespawnSlot(TEXT("Slot1"));
	if (!SaveMgr->DoesSaveExist(RespawnSlot))
	{
		return;
	}

	USOLOLocalProfileSave* SaveData = Cast<USOLOLocalProfileSave>(
		UGameplayStatics::LoadGameFromSlot(RespawnSlot, USOLOSaveManager::DefaultUserIndex));
	if (!SaveData)
	{
		return;
	}

	if (APawn* Pawn = PlayerController->GetPawn())
	{
		Pawn->SetActorLocationAndRotation(
			SaveData->LastSaveLocation,
			SaveData->LastSaveRotation,
			false,
			nullptr,
			ETeleportType::ResetPhysics);
		PlayerController->SetControlRotation(SaveData->LastSaveRotation);
	}
}

void ASOLOGameMode::OnPlayerActivatedSavePoint(ASOLOPlayerCharacter* Player)
{
	if (!Player || !HasAuthority())
	{
		return;
	}

	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return;
	}

	USOLOSaveManager* SaveManager = GI->GetSubsystem<USOLOSaveManager>();
	if (SaveManager)
	{
		SaveManager->SavePlayerGame(Player, TEXT("Slot1"));

		if (APlayerController* PC = Player->GetController<APlayerController>())
		{
			FSOLONotification N;
			N.Message = NSLOCTEXT("SOLOGameMode", "SaveConfirm", "Partida guardada");
			N.Type = ENotificationType::Success;
			N.Duration = 2.5f;
			USOLONotificationSystem::NotifyOwningClient(PC, N);
		}
	}
}
