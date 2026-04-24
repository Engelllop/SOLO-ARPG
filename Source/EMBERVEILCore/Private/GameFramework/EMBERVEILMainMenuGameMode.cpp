#include "GameFramework/EMBERVEILMainMenuGameMode.h"
#include "Player/EMBERVEILMenuPlayerController.h"
#include "UI/Widgets/WG_MainMenu.h"
#include "GameFramework/SpectatorPawn.h"
#include "Blueprint/UserWidget.h"

AEMBERVEILMainMenuGameMode::AEMBERVEILMainMenuGameMode()
{
	PlayerControllerClass = AEMBERVEILMenuPlayerController::StaticClass();
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	HUDClass = nullptr;
	MenuWidgetClass = UWG_MainMenu::StaticClass();
}

void AEMBERVEILMainMenuGameMode::StartPlay()
{
	Super::StartPlay();

	if (!MenuWidgetClass)
	{
		return;
	}

	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (!PC)
	{
		return;
	}

	if (UUserWidget* Menu = CreateWidget<UUserWidget>(PC, MenuWidgetClass))
	{
		Menu->AddToViewport(100);
	}
}
