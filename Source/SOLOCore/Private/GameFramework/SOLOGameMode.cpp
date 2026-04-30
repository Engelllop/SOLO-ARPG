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
	// C++ por defecto (hay Blueprint aÃºn). Si existe BP_PlayerCharacter, lo usa.
	DefaultPawnClass = ASOLOPlayerCharacter::StaticClass();
	{
		static ConstructorHelpers::FClassFinder<APawn> BpPawnFinder(TEXT("/Game/Blueprints/Characters/BP_PlayerCharacter"));
		if (BpPawnFinder.Succeeded())
		{
			DefaultPawnClass = BpPawnFinder.Class;
		}
		else
		{
			static ConstructorHelpers::FClassFinder<APawn> LegacyPawnFinder(TEXT("/Game/Characters/Player/BP_PlayerCharacter"));
			if (LegacyPawnFinder.Succeeded())
			{
				DefaultPawnClass = LegacyPawnFinder.Class;
			}
		}
	}

	PlayerStateClass = ASOLOPlayerState::StaticClass();

	PlayerControllerClass = ASOLOPlayerController::StaticClass();
	{
		static ConstructorHelpers::FClassFinder<APlayerController> BpPlayerControllerFinder(
			TEXT("/Game/Blueprints/Player/BP_PlayerController"));
		if (BpPlayerControllerFinder.Succeeded())
		{
			PlayerControllerClass = BpPlayerControllerFinder.Class;
		}
		else
		{
			static ConstructorHelpers::FClassFinder<APlayerController> LegacyPcFinder(
				TEXT("/Game/Player/BP_SOLOPlayerController"));
			if (LegacyPcFinder.Succeeded())
			{
				PlayerControllerClass = LegacyPcFinder.Class;
			}
		}
	}

	HUDClass = ASOLOHUDBase::StaticClass();
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
