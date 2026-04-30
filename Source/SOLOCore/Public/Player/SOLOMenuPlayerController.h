#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SOLOMenuPlayerController.generated.h"

/**
 * PlayerController para mapas de menÃº: cursor, foco UI, sin binds de combate.
 */
UCLASS()
class SOLOCORE_API ASOLOMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
