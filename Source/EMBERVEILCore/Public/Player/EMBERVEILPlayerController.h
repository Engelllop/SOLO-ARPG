#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AEMBERVEILPlayerState;
class UUserWidget;

/**
 * PlayerController de EMBERVEIL.
 * Gestiona Enhanced Input, comunicación con el HUD y lógica de cámara.
 * No contiene lógica de juego — delega todo al Character y al ASC.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEMBERVEILPlayerController();

	/** Servidor → cliente: muestra notificación en el subsistema local. */
	UFUNCTION(Client, Reliable)
	void ClientReceiveNotification(const FText& Message, uint8 NotificationType, float Duration);

	/** Servidor → cliente poseedor: número de daño flotante (ExecCalc en servidor). */
	UFUNCTION(Client, Reliable)
	void Client_ShowDamageNumber(float DamageAmount, FVector WorldLocation, uint8 DamageType, bool bIsCritical);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	// ─── Enhanced Input ───────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BasicAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_ToggleInventory;

	// ─── Inventory widget ─────────────────────────────────────────────────────

	/** Clase del widget de inventario. Asignar en el Blueprint derivado. */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	/** Instancia del widget de inventario (creado en demanda, persistente). */
	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryWidget;

	// ─── Input callbacks ──────────────────────────────────────────────────────
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJump();
	void HandleDodge();
	void HandleBasicAttack();
	void HandleInteract();
	void HandleToggleInventory();

	/** Si no hay IMC/acciones en el CDO, usa ejes legacy (WASD, ratón, espacio). */
	void BindLegacyMovementAxes();

	static void EnsureDefaultLegacyMappings();

	void Legacy_MoveForward(float Value);
	void Legacy_MoveRight(float Value);
	void Legacy_Turn(float Value);
	void Legacy_LookUp(float Value);
	void Legacy_JumpPressed();
	void Legacy_JumpReleased();

	// Referencia al PlayerState cacheada para evitar casteos repetidos
	UPROPERTY()
	TObjectPtr<AEMBERVEILPlayerState> EMBERVEILPlayerState;

	void CachePlayerState();

	/** Distancia máxima del line trace de interacción (en unidades). */
	static constexpr float InteractTraceDistance = 300.f;
};
