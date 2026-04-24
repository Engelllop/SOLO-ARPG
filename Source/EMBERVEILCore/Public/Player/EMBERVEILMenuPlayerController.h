#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EMBERVEILMenuPlayerController.generated.h"

/**
 * PlayerController para mapas de menú: cursor, foco UI, sin binds de combate.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
