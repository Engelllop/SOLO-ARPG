#include "Player/EMBERVEILMenuPlayerController.h"

void AEMBERVEILMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}
