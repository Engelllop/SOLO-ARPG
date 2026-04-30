#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/ISOLOInteractable.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "ASOLOSavePoint.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class ASOLOPlayerCharacter;

/**
 * Punto de guardado en el mundo. Implementa ISOLOInteractable.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLOSavePoint : public AActor, public ISOLOInteractable
{
	GENERATED_BODY()

public:
	ASOLOSavePoint();

	virtual void BeginPlay() override;

	// â”€â”€â”€ ISOLOInteractable â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	virtual void Interact_Implementation(ASOLOPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(ASOLOPlayerCharacter* Interactor) const override;

	// â”€â”€â”€ Componentes â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<USphereComponent> InteractionCollision;

	/** VFX opcional mientras el punto estÃ¡ activo (carga perezosa desde soft ptr). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<UNiagaraComponent> IdleVFX;

	// â”€â”€â”€ ConfiguraciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint|VFX")
	TSoftObjectPtr<UNiagaraSystem> IdleVFXSoft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint|Audio")
	TObjectPtr<USoundBase> InteractSound;

	/** Si true, CanInteract devuelve false tras la primera interacciÃ³n con Ã©xito. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint")
	bool bSingleUse = false;

	UPROPERTY(BlueprintReadOnly, Category = "SavePoint")
	bool bHasBeenUsed = false;

protected:
	void PerformSaveInteraction(ASOLOPlayerCharacter* Interactor);

	UFUNCTION(Server, Reliable)
	void ServerPerformSave(ASOLOPlayerCharacter* Interactor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySaveEffects();
};
