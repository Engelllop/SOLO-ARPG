#include "GameFramework/SOLOMainMenuGameMode.h"
#include "Player/SOLOMenuPlayerController.h"
#include "UI/Widgets/WG_MainMenu.h"
#include "GameFramework/SpectatorPawn.h"
#include "Blueprint/UserWidget.h"

ASOLOMainMenuGameMode::ASOLOMainMenuGameMode()
{
	PlayerControllerClass = ASOLOMenuPlayerController::StaticClass();
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	HUDClass = nullptr;
	MenuWidgetClass = UWG_MainMenu::StaticClass();
}

void ASOLOMainMenuGameMode::StartPlay()
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
