#include "Player/SOLOMenuPlayerController.h"

void ASOLOMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}
