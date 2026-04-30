#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLOWorldItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class USOLOItemBase;

/**
 * Actor que representa un item dropeado en el mundo.
 * Spawneado por USOLOLootComponent cuando un enemigo muere.
 *
 * CaracterÃ­sticas:
 *  - Carga el mesh 3D del item de forma asÃ­ncrona al inicializarse.
 *  - Muestra el nombre del item en un widget flotante (world-space).
 *  - Flota suavemente usando una funciÃ³n seno en Tick.
 *  - Desaparece automÃ¡ticamente tras ItemLifetime segundos.
 *  - Al overlappear con un PlayerCharacter, aÃ±ade el item a su inventario y se destruye.
 *  - Replicado: el servidor controla el pickup y destruye el actor en todos los clientes.
 */
UCLASS()
class SOLOCORE_API ASOLOWorldItem : public AActor
{
    GENERATED_BODY()

public:
    ASOLOWorldItem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // â”€â”€â”€ COMPONENTES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Mesh 3D del item visible en el mundo. El mesh se asigna de forma asÃ­ncrona desde ItemData. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    /** Esfera de colisiÃ³n que activa el pickup al overlappear con el jugador. Radio = 80u. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<USphereComponent> PickupCollision;

    /** Widget que muestra el nombre del item sobre Ã©l en espacio de mundo. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<UWidgetComponent> ItemNameWidget;

    // â”€â”€â”€ DATOS DEL ITEM â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** DataAsset del item que representa este actor. Asignado via InitializeItem(). */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemData, Category = "World Item|Data")
    TObjectPtr<USOLOItemBase> ItemData;

    /** Cantidad de unidades de este item. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Data")
    int32 Quantity = 1;

    // â”€â”€â”€ MONEDA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Si true, este WorldItem representa moneda en lugar de un item de inventario. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Currency")
    bool bIsCurrency = false;

    /** Cantidad de moneda si bIsCurrency == true. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Currency")
    int32 CurrencyAmount = 0;

    // â”€â”€â”€ VIDA DEL ACTOR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Segundos antes de que el actor se destruya si no es recogido. Asignado al inicializar. */
    UPROPERTY(BlueprintReadOnly, Category = "World Item|Lifetime")
    float Lifetime = 60.f;

    // â”€â”€â”€ INICIALIZACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Inicializa el WorldItem con un item de inventario.
     * Asigna ItemData y Quantity, inicia la carga asÃ­ncrona del mesh y el timer de lifetime.
     * Llamar inmediatamente despuÃ©s de SpawnActor.
     *
     * @param InItemData   DataAsset del item. No debe ser nulo.
     * @param InQuantity   Cantidad de unidades.
     * @param InLifetime   Segundos antes de destruirse automÃ¡ticamente.
     */
    UFUNCTION(BlueprintCallable, Category = "World Item")
    void InitializeItem(USOLOItemBase* InItemData, int32 InQuantity, float InLifetime);

    /**
     * Inicializa el WorldItem como un drop de moneda.
     * Activa bIsCurrency, asigna CurrencyAmount e inicia el timer de lifetime.
     * Llamar inmediatamente despuÃ©s de SpawnActor.
     *
     * @param Amount      Cantidad de moneda que representa.
     * @param InLifetime  Segundos antes de destruirse automÃ¡ticamente.
     */
    UFUNCTION(BlueprintCallable, Category = "World Item")
    void InitializeCurrency(int32 Amount, float InLifetime);

    // â”€â”€â”€ PICKUP â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Callback de overlap con la PickupCollision.
     * Si el overlapping actor es un PlayerCharacter con InventoryComponent, aÃ±ade el item
     * (o moneda) y destruye este actor en el servidor.
     */
    UFUNCTION()
    void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);

private:
    // â”€â”€â”€ ESTADO REPLICADO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * True una vez que el item fue recogido por un jugador.
     * Replicado para que el cliente oculte inmediatamente el actor antes de que
     * el servidor lo destruya.
     */
    UPROPERTY(ReplicatedUsing = OnRep_IsPickedUp)
    bool bIsPickedUp = false;

    // â”€â”€â”€ ANIMACIÃ“N DE FLOTACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** PosiciÃ³n Z original en el momento de spawn, usada como base para la animaciÃ³n seno. */
    float BaseZ = 0.f;

    // â”€â”€â”€ ONREP CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    UFUNCTION()
    void OnRep_IsPickedUp();

    UFUNCTION()
    void OnRep_ItemData();

    // â”€â”€â”€ HELPERS INTERNOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Inicia la carga asÃ­ncrona del TSoftObjectPtr<UStaticMesh> del ItemData
     * y asigna el mesh al MeshComponent cuando termina.
     */
    void AsyncLoadItemMesh();

    /**
     * Actualiza el texto del ItemNameWidget con el nombre del ItemData.
     * Solo tiene efecto si el widget y el ItemData son vÃ¡lidos.
     */
    void UpdateNameWidget();

    /** Handle del timer de lifetime para poder cancelarlo si se recoge antes. */
    FTimerHandle LifetimeTimerHandle;

    /** Handle de la carga asÃ­ncrona del mesh, para cancelarla si el actor se destruye antes. */
    TSharedPtr<struct FStreamableHandle> MeshLoadHandle;
};
