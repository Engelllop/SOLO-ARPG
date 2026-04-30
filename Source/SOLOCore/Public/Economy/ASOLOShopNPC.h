#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Economy/SOLOMarketplaceComponent.h"
#include "Interaction/ISOLOInteractable.h"
#include "ASOLOShopNPC.generated.h"

class USphereComponent;
class UWidgetComponent;
class USOLOMarketplaceComponent;
class ASOLOPlayerCharacter;

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * Disparado cuando un jugador abre la tienda.
 * La UI (UMG) debe suscribirse a este delegate para mostrar el panel de comercio.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShopOpened,
    ASOLOShopNPC*, ShopNPC,
    ASOLOPlayerCharacter*, Player);

/**
 * Actor NPC vendedor de SOLO.
 *
 * Combina un MarketplaceComponent (lÃ³gica de tienda) con colisiÃ³n de interacciÃ³n
 * y un widget indicador flotante. Al interactuar, dispara el delegate FOnShopOpened
 * para que la UI lo escuche y muestre el panel de comercio.
 *
 * Configurar ShopID, ShopName y los Listings del MarketplaceComponent en el editor.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLOShopNPC : public AActor, public ISOLOInteractable
{
	GENERATED_BODY()

public:
	ASOLOShopNPC();

	virtual void BeginPlay() override;

	// â”€â”€â”€ ISOLOInteractable â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	virtual void Interact_Implementation(ASOLOPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(ASOLOPlayerCharacter* Interactor) const override;

    // â”€â”€â”€ DELEGATE â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Notifica a la UI cuando el jugador abre esta tienda */
    UPROPERTY(BlueprintAssignable, Category = "Shop|Delegates")
    FOnShopOpened OnShopOpened;

    // â”€â”€â”€ IDENTIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Identificador Ãºnico de esta tienda (usado para guardar estado, analytics, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    FName ShopID;

    /** Nombre localizable de la tienda mostrado en la UI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    FText ShopName;

    /** Retrato del tendero mostrado en el panel de comercio */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Identity")
    TSoftObjectPtr<UTexture2D> ShopkeeperPortrait;

    // â”€â”€â”€ COMPONENTES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Componente de lÃ³gica de marketplace (listings, precios, compra/venta) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop|Components")
    TObjectPtr<USOLOMarketplaceComponent> MarketplaceComponent;

    /**
     * Esfera de colisiÃ³n para detectar al jugador en el rango de interacciÃ³n.
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

    // â”€â”€â”€ INTERACCIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    // ISOLOInteractable define Interact como BlueprintNativeEvent; no duplicar el nombre aquÃ­.

    /** Abre la tienda (delegate). Llamado desde Interact_Implementation. */
    UFUNCTION(BlueprintCallable, Category = "Shop|Interaction")
    void HandlePlayerInteract(ASOLOPlayerCharacter* Player);

protected:
    // â”€â”€â”€ COLISIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Callback del overlap de la esfera de interacciÃ³n.
     * Muestra/oculta el ShopIndicatorWidget segÃºn si el jugador entra o sale del radio.
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
