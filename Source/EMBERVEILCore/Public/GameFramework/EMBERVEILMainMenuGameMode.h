#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EMBERVEILMainMenuGameMode.generated.h"

class UWG_MainMenu;

/**
 * GameMode del menú principal: muestra el widget de perfil/nombre y arranque.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEMBERVEILMainMenuGameMode();

protected:
	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWG_MainMenu> MenuWidgetClass;
};
