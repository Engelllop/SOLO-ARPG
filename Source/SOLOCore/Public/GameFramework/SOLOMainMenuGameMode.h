#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SOLOMainMenuGameMode.generated.h"

class UWG_MainMenu;

/**
 * GameMode del menÃº principal: muestra el widget de perfil/nombre y arranque.
 */
UCLASS()
class SOLOCORE_API ASOLOMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASOLOMainMenuGameMode();

protected:
	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWG_MainMenu> MenuWidgetClass;
};
