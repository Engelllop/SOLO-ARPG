#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Economy/EMBERVEILMarketplaceComponent.h"
#include "Interaction/IEMBERVEILInteractable.h"
#include "AEMBERVEILShopNPC.generated.h"

class USphereComponent;
class UWidgetComponent;
class UEMBERVEILMarketplaceComponent;
class AEMBERVEILPlayerCharacter;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/**
 * Disparado cuando un jugador abre la tienda.
 * La UI (UMG) debe suscribirse a este delegate para mostrar el panel de comercio.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShopOpened,
    AEMBERVEILShopNPC*, ShopNPC,
    AEMBERVEILPlayerCharacter*, Player);

/**
 * Actor NPC vendedor de EMBERVEIL.
 *
 * Combina un MarketplaceComponent (lógica de tienda) con colisión de interacción
 * y un widget indicador flotante. Al interactuar, dispara el delegate FOnShopOpened
 * para que la UI lo escuche y muestre el panel de comercio.
 *
 * Configurar ShopID, ShopName y los Listings del MarketplaceComponent en el editor.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API AEMBERVEILShopNPC : public AActor, public IEMBERVEILInteractable
{
	GENERATED_BODY()

public:
	AEMBERVEILShopNPC();

	virtual void BeginPlay() override;

	// ─── IEMBERVEILInteractable ───────────────────────────────────────────────
	virtual void Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const override;

    // ─── DELEGATE ─────────────────────────────────────────────────────────────

    /** Notifica a la UI cuando el jugador abre esta tienda */
    UPROPERTY(BlueprintAssignable, Category = "Shop|Delegates")
    FOnShopOpened OnShopOpened;

    // ─── IDENTIDAD ────────────────────────────────────────────────────────────

    /** Identificador único de esta tienda (usado para guardar estado, analytics, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    FName ShopID;

    /** Nombre localizable de la tienda mostrado en la UI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    FText ShopName;

    /** Retrato del tendero mostrado en el panel de comercio */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    TSoftObjectPtr<UTexture2D> ShopkeeperPortrait;

    // ─── COMPONENTES ──────────────────────────────────────────────────────────

    /** Componente de lógica de marketplace (listings, precios, compra/venta) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
    TObjectPtr<UEMBERVEILMarketplaceComponent> MarketplaceComponent;

    /**
     * Esfera de colisión para detectar al jugador en el rango de interacción.
     * Radio por defecto: 200 unidades.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
    TObjectPtr<USphereComponent> InteractionCollision;

    /**
     * Widget 3D flotante que indica al jugador que puede interactuar con esta tienda.
     * Asignar la clase de widget desde el editor.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
    TObjectPtr<UWidgetComponent> ShopIndicatorWidget;

    // ─── INTERACCIÓN ──────────────────────────────────────────────────────────
    // IEMBERVEILInteractable define Interact como BlueprintNativeEvent; no duplicar el nombre aquí.

    /** Abre la tienda (delegate). Llamado desde Interact_Implementation. */
    UFUNCTION(BlueprintCallable, Category = "Shop|Interaction")
    void HandlePlayerInteract(AEMBERVEILPlayerCharacter* Player);

protected:
    // ─── COLISIÓN ─────────────────────────────────────────────────────────────

    /**
     * Callback del overlap de la esfera de interacción.
     * Muestra/oculta el ShopIndicatorWidget según si el jugador entra o sale del radio.
     */
    UFUNCTION()
    void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
