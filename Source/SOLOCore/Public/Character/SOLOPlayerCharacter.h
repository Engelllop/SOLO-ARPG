#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOCharacterBase.h"
#include "SOLOPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 * Personaje controlado por el jugador.
 *
 * ASC y AttributeSet viven en ASOLOPlayerState â€” se obtienen en PossessedBy /
 * OnRep_PlayerState para soportar tanto servidor como clientes en multiplayer.
 */
UCLASS()
class SOLOCORE_API ASOLOPlayerCharacter : public ASOLOCharacterBase
{
	GENERATED_BODY()

public:
	ASOLOPlayerCharacter();

	// â”€â”€â”€ ICombatInterface â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	virtual int32 GetCharacterLevel_Implementation() const override;

	// â”€â”€â”€ Lock-on â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Alterna el lock-on al enemigo mÃ¡s cercano dentro de LockOnRadius. */
	UFUNCTION(BlueprintCallable, Category = "Combat|LockOn")
	void ToggleLockOn();

	UFUNCTION(BlueprintPure, Category = "Combat|LockOn")
	bool IsLockedOn() const { return LockOnTarget != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Combat|LockOn")
	AActor* GetLockOnTarget() const { return LockOnTarget; }

	// â”€â”€â”€ Combo â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Llamado por la GameplayAbility de ataque bÃ¡sico cuando el golpe conecta.
	 * Abre la ventana de combo y programa el reset automÃ¡tico.
	 */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void OnBasicAttackLanded();

	/** Resetea el Ã­ndice de combo. Llamado por timer o al cancelar el combo. */
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void ResetCombo();

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	int32 GetComboIndex() const { return ComboIndex; }

	UFUNCTION(BlueprintPure, Category = "Combat|Combo")
	bool CanCombo() const { return bCanCombo; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// â”€â”€â”€ GAS init â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Servidor: el Pawn es poseÃ­do â†’ obtiene ASC del PlayerState e init GAS. */
	virtual void PossessedBy(AController* NewController) override;

	/** Cliente: replicaciÃ³n del PlayerState â†’ mismo init que PossessedBy. */
	virtual void OnRep_PlayerState() override;

	// â”€â”€â”€ CÃ¡mara â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Longitud del brazo de cÃ¡mara en reposo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float CameraArmLength = 400.f;

	/** Velocidad de interpolaciÃ³n del lag de posiciÃ³n de cÃ¡mara. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float CameraLagSpeed = 10.f;

	// â”€â”€â”€ Lock-on config â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Radio de bÃºsqueda inicial de targets para el lock-on. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|LockOn")
	float LockOnRadius = 1500.f;

	// â”€â”€â”€ Combo config â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Tiempo mÃ¡ximo entre ataques para mantener el combo activo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Combo")
	float ComboResetTime = 1.2f;

private:
	/**
	 * Centraliza el handshake ASC con InitAbilityActorInfo(PlayerState, this).
	 * Llamado tanto en PossessedBy (servidor) como en OnRep_PlayerState (cliente).
	 */
	void InitAbilityActorInfo();

	/**
	 * Actualiza la rotaciÃ³n del SpringArm durante el lock-on para que la cÃ¡mara
	 * orbite al rededor del target, interpolando suavemente con DeltaSeconds.
	 */
	void UpdateLockOnCamera(float DeltaSeconds);

	/** Cancela el lock-on activo y restaura el comportamiento de cÃ¡mara libre. */
	void ClearLockOn();

	// â”€â”€â”€ Lock-on state â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Actor al que estamos lockeados. nullptr = sin lock. */
	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;

	// â”€â”€â”€ Combo state â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	int32 ComboIndex    = 0;
	bool  bCanCombo     = false;

	FTimerHandle ComboResetTimer;
};
