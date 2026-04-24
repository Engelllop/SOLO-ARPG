#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "EMBERVEILGameMode.generated.h"

/**
 * GameMode base de EMBERVEIL.
 * Cada mapa (overworld, mazmorra, torre) puede tener su propio GameMode derivado.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEMBERVEILGameMode();

	// Llamado cuando un personaje jugador muere
	virtual void PlayerDied(APlayerController* PlayerController);

	/**
	 * Llamar cuando el jugador activa un punto de guardado (interacción, volumen, etc.).
	 * Delega en UEMBERVEILSaveManager::SaveGame (solo servidor).
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void OnPlayerActivatedSavePoint(AEMBERVEILPlayerCharacter* Player);

protected:
	// Tiempo en segundos antes del respawn (0 = instantáneo)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	float RespawnDelay = 5.f;

	// Si true, el jugador respawnea en el último punto de guardado
	// Si false, respawnea en el PlayerStart más cercano al punto de muerte
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameMode")
	bool bRespawnAtLastSave = true;

private:
	void DelayedRespawn(APlayerController* PlayerController);
};
