#include "GameFramework/EMBERVEILGameMode.h"
#include "UI/HUD/EMBERVEILHUDBase.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Player/EMBERVEILPlayerController.h"
#include "Profile/EMBERVEILSaveManager.h"
#include "UI/EMBERVEILNotificationSystem.h"
#include "Profile/EMBERVEILLocalProfileSave.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Text.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AEMBERVEILGameMode::AEMBERVEILGameMode()
{
	// C++ por defecto (hay Blueprint aún). Si existe BP_PlayerCharacter, lo usa.
	DefaultPawnClass = AEMBERVEILPlayerCharacter::StaticClass();
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

	PlayerStateClass = AEMBERVEILPlayerState::StaticClass();

	PlayerControllerClass = AEMBERVEILPlayerController::StaticClass();
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
				TEXT("/Game/Player/BP_EMBERVEILPlayerController"));
			if (LegacyPcFinder.Succeeded())
			{
				PlayerControllerClass = LegacyPcFinder.Class;
			}
		}
	}

	HUDClass = AEMBERVEILHUDBase::StaticClass();
}

void AEMBERVEILGameMode::PlayerDied(APlayerController* PlayerController)
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
			FTimerDelegate::CreateUObject(this, &AEMBERVEILGameMode::DelayedRespawn, PlayerController),
			RespawnDelay,
			false
		);
	}
}

void AEMBERVEILGameMode::DelayedRespawn(APlayerController* PlayerController)
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

	UEMBERVEILSaveManager* SaveMgr = GI->GetSubsystem<UEMBERVEILSaveManager>();
	if (!SaveMgr)
	{
		return;
	}

	static const FString RespawnSlot(TEXT("Slot1"));
	if (!SaveMgr->DoesSaveExist(RespawnSlot))
	{
		return;
	}

	UEMBERVEILLocalProfileSave* SaveData = Cast<UEMBERVEILLocalProfileSave>(
		UGameplayStatics::LoadGameFromSlot(RespawnSlot, UEMBERVEILSaveManager::DefaultUserIndex));
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

void AEMBERVEILGameMode::OnPlayerActivatedSavePoint(AEMBERVEILPlayerCharacter* Player)
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

	UEMBERVEILSaveManager* SaveManager = GI->GetSubsystem<UEMBERVEILSaveManager>();
	if (SaveManager)
	{
		SaveManager->SavePlayerGame(Player, TEXT("Slot1"));

		if (APlayerController* PC = Player->GetController<APlayerController>())
		{
			FEMBERVEILNotification N;
			N.Message = NSLOCTEXT("EMBERVEILGameMode", "SaveConfirm", "Partida guardada");
			N.Type = ENotificationType::Success;
			N.Duration = 2.5f;
			UEMBERVEILNotificationSystem::NotifyOwningClient(PC, N);
		}
	}
}
