#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/SOLOPlayerCharacter.h"
#include "SOLOGameMode.generated.h"

/**
 * GameMode base de SOLO.
 * Cada mapa (overworld, mazmorra, torre) puede tener su propio GameMode derivado.
 */
UCLASS()
class SOLOCORE_API ASOLOGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASOLOGameMode();

	// Llamado cuando un personaje jugador muere
	virtual void PlayerDied(APlayerController* PlayerController);

	/**
	 * Llamar cuando el jugador activa un punto de guardado (interacciÃ³n, volumen, etc.).
	 * Delega en USOLOSaveManager::SaveGame (solo servidor).
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void OnPlayerActivatedSavePoint(ASOLOPlayerCharacter* Player);

protected:
	// Tiempo en segundos antes del respawn (0 = instantÃ¡neo)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float RespawnDelay = 5.f;

	// Si true, el jugador respawnea en el Ãºltimo punto de guardado
	// Si false, respawnea en el PlayerStart mÃ¡s cercano al punto de muerte
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	bool bRespawnAtLastSave = true;

private:
	void DelayedRespawn(APlayerController* PlayerController);
};
