#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/IEMBERVEILInteractable.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "AEMBERVEILSavePoint.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class AEMBERVEILPlayerCharacter;

/**
 * Punto de guardado en el mundo. Implementa IEMBERVEILInteractable.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API AEMBERVEILSavePoint : public AActor, public IEMBERVEILInteractable
{
	GENERATED_BODY()

public:
	AEMBERVEILSavePoint();

	virtual void BeginPlay() override;

	// ─── IEMBERVEILInteractable ───────────────────────────────────────────────
	virtual void Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const override;

	// ─── Componentes ──────────────────────────────────────────────────────────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<USphereComponent> InteractionCollision;

	/** VFX opcional mientras el punto está activo (carga perezosa desde soft ptr). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SavePoint|Components")
	TObjectPtr<UNiagaraComponent> IdleVFX;

	// ─── Configuración ────────────────────────────────────────────────────────

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint|VFX")
	TSoftObjectPtr<UNiagaraSystem> IdleVFXSoft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint|Audio")
	TObjectPtr<USoundBase> InteractSound;

	/** Si true, CanInteract devuelve false tras la primera interacción con éxito. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SavePoint")
	bool bSingleUse = false;

	UPROPERTY(BlueprintReadOnly, Category = "SavePoint")
	bool bHasBeenUsed = false;

protected:
	void PerformSaveInteraction(AEMBERVEILPlayerCharacter* Interactor);

	UFUNCTION(Server, Reliable)
	void ServerPerformSave(AEMBERVEILPlayerCharacter* Interactor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySaveEffects();
};
