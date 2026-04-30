#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "SOLOPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASOLOPlayerState;
class UUserWidget;

/**
 * PlayerController de SOLO.
 * Gestiona Enhanced Input, comunicaciÃ³n con el HUD y lÃ³gica de cÃ¡mara.
 * No contiene lÃ³gica de juego â€” delega todo al Character y al ASC.
 */
UCLASS()
class SOLOCORE_API ASOLOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASOLOPlayerController();

	/** Servidor â†’ cliente: muestra notificaciÃ³n en el subsistema local. */
	UFUNCTION(Client, Reliable)
	void ClientReceiveNotification(const FText& Message, uint8 NotificationType, float Duration);

	/** Servidor â†’ cliente poseedor: nÃºmero de daÃ±o flotante (ExecCalc en servidor). */
	UFUNCTION(Client, Reliable)
	void Client_ShowDamageNumber(float DamageAmount, FVector WorldLocation, uint8 DamageType, bool bIsCritical);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	// â”€â”€â”€ Enhanced Input â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
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

	// â”€â”€â”€ Inventory widget â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Clase del widget de inventario. Asignar en el Blueprint derivado. */
	UPROPERTY(EditDefaultsOnly, Category = "UI|Inventory")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	/** Instancia del widget de inventario (creado en demanda, persistente). */
	UPROPERTY()
	TObjectPtr<UUserWidget> InventoryWidget;

	// â”€â”€â”€ Input callbacks â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);
	void HandleJump();
	void HandleStopJump();
	void HandleDodge();
	void HandleBasicAttack();
	void HandleInteract();
	void HandleToggleInventory();

	/** Si no hay IMC/acciones en el CDO, usa ejes legacy (WASD, ratÃ³n, espacio). */
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
	TObjectPtr<ASOLOPlayerState> SOLOPlayerState;

	void CachePlayerState();

	/** Distancia mÃ¡xima del line trace de interacciÃ³n (en unidades). */
	static constexpr float InteractTraceDistance = 300.f;
};
