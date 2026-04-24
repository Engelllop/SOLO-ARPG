#pragma once

#include "CoreMinimal.h"
#include "Character/EMBERVEILCharacterBase.h"
#include "EMBERVEILPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 * Personaje controlado por el jugador.
 *
 * ASC y AttributeSet viven en AEMBERVEILPlayerState — se obtienen en PossessedBy /
 * OnRep_PlayerState para soportar tanto servidor como clientes en multiplayer.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILPlayerCharacter : public AEMBERVEILCharacterBase
{
	GENERATED_BODY()

public:
	AEMBERVEILPlayerCharacter();

	// ─── ICombatInterface ─────────────────────────────────────────────────────
	virtual int32 GetCharacterLevel_Implementation() const override;

	// ─── Lock-on ──────────────────────────────────────────────────────────────

	/** Alterna el lock-on al enemigo más cercano dentro de LockOnRadius. */
	UFUNCTION(BlueprintCallable, Category = "Combat|LockOn")
	void ToggleLockOn();

	UFUNCTION(BlueprintPure, Category = "Combat|LockOn")
	bool IsLockedOn() const { return LockOnTarget != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Combat|LockOn")
	AActor* GetLockOnTarget() const { return LockOnTarget; }

	// ─── Combo ────────────────────────────────────────────────────────────────

	/**
	 * Llamado por la GameplayAbility de ataque básico cuando el golpe conecta.
	 * Abre la ventana de combo y programa el reset automático.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void OnBasicAttackLanded();

	/** Resetea el índice de combo. Llamado por timer o al cancelar el combo. */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void ResetCombo();

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	int32 GetComboIndex() const { return ComboIndex; }

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	bool CanCombo() const { return bCanCombo; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// ─── GAS init ─────────────────────────────────────────────────────────────

	/** Servidor: el Pawn es poseído → obtiene ASC del PlayerState e init GAS. */
	virtual void PossessedBy(AController* NewController) override;

	/** Cliente: replicación del PlayerState → mismo init que PossessedBy. */
	virtual void OnRep_PlayerState() override;

	// ─── Cámara ───────────────────────────────────────────────────────────────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Longitud del brazo de cámara en reposo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float CameraArmLength = 400.f;

	/** Velocidad de interpolación del lag de posición de cámara. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float CameraLagSpeed = 10.f;

	// ─── Lock-on config ───────────────────────────────────────────────────────

	/** Radio de búsqueda inicial de targets para el lock-on. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|LockOn")
	float LockOnRadius = 1500.f;

	// ─── Combo config ─────────────────────────────────────────────────────────

	/** Tiempo máximo entre ataques para mantener el combo activo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Combo")
	float ComboResetTime = 1.2f;

private:
	/**
	 * Centraliza el handshake ASC con InitAbilityActorInfo(PlayerState, this).
	 * Llamado tanto en PossessedBy (servidor) como en OnRep_PlayerState (cliente).
	 */
	void InitAbilityActorInfo();

	/**
	 * Actualiza la rotación del SpringArm durante el lock-on para que la cámara
	 * orbite al rededor del target, interpolando suavemente con DeltaSeconds.
	 */
	void UpdateLockOnCamera(float DeltaSeconds);

	/** Cancela el lock-on activo y restaura el comportamiento de cámara libre. */
	void ClearLockOn();

	// ─── Lock-on state ────────────────────────────────────────────────────────

	/** Actor al que estamos lockeados. nullptr = sin lock. */
	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;

	// ─── Combo state ──────────────────────────────────────────────────────────

	int32 ComboIndex    = 0;
	bool  bCanCombo     = false;

	FTimerHandle ComboResetTimer;
};
