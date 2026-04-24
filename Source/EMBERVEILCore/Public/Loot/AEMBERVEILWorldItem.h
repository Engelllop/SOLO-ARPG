#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AEMBERVEILWorldItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UEMBERVEILItemBase;

/**
 * Actor que representa un item dropeado en el mundo.
 * Spawneado por UEMBERVEILLootComponent cuando un enemigo muere.
 *
 * Características:
 *  - Carga el mesh 3D del item de forma asíncrona al inicializarse.
 *  - Muestra el nombre del item en un widget flotante (world-space).
 *  - Flota suavemente usando una función seno en Tick.
 *  - Desaparece automáticamente tras ItemLifetime segundos.
 *  - Al overlappear con un PlayerCharacter, añade el item a su inventario y se destruye.
 *  - Replicado: el servidor controla el pickup y destruye el actor en todos los clientes.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILWorldItem : public AActor
{
    GENERATED_BODY()

public:
    AEMBERVEILWorldItem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ─── COMPONENTES ─────────────────────────────────────────────────────────

    /** Mesh 3D del item visible en el mundo. El mesh se asigna de forma asíncrona desde ItemData. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<UStaticMeshComponent> MeshComponent;

    /** Esfera de colisión que activa el pickup al overlappear con el jugador. Radio = 80u. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<USphereComponent> PickupCollision;

    /** Widget que muestra el nombre del item sobre él en espacio de mundo. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item|Components")
    TObjectPtr<UWidgetComponent> ItemNameWidget;

    // ─── DATOS DEL ITEM ───────────────────────────────────────────────────────

    /** DataAsset del item que representa este actor. Asignado via InitializeItem(). */
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ItemData, Category = "World Item|Data")
    TObjectPtr<UEMBERVEILItemBase> ItemData;

    /** Cantidad de unidades de este item. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Data")
    int32 Quantity = 1;

    // ─── MONEDA ───────────────────────────────────────────────────────────────

    /** Si true, este WorldItem representa moneda en lugar de un item de inventario. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Currency")
    bool bIsCurrency = false;

    /** Cantidad de moneda si bIsCurrency == true. */
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "World Item|Currency")
    int32 CurrencyAmount = 0;

    // ─── VIDA DEL ACTOR ───────────────────────────────────────────────────────

    /** Segundos antes de que el actor se destruya si no es recogido. Asignado al inicializar. */
    UPROPERTY(BlueprintReadOnly, Category = "World Item|Lifetime")
    float Lifetime = 60.f;

    // ─── INICIALIZACIÓN ───────────────────────────────────────────────────────

    /**
     * Inicializa el WorldItem con un item de inventario.
     * Asigna ItemData y Quantity, inicia la carga asíncrona del mesh y el timer de lifetime.
     * Llamar inmediatamente después de SpawnActor.
     *
     * @param InItemData   DataAsset del item. No debe ser nulo.
     * @param InQuantity   Cantidad de unidades.
     * @param InLifetime   Segundos antes de destruirse automáticamente.
     */
    UFUNCTION(BlueprintCallable, Category = "World Item")
    void InitializeItem(UEMBERVEILItemBase* InItemData, int32 InQuantity, float InLifetime);

    /**
     * Inicializa el WorldItem como un drop de moneda.
     * Activa bIsCurrency, asigna CurrencyAmount e inicia el timer de lifetime.
     * Llamar inmediatamente después de SpawnActor.
     *
     * @param Amount      Cantidad de moneda que representa.
     * @param InLifetime  Segundos antes de destruirse automáticamente.
     */
    UFUNCTION(BlueprintCallable, Category = "World Item")
    void InitializeCurrency(int32 Amount, float InLifetime);

    // ─── PICKUP ───────────────────────────────────────────────────────────────

    /**
     * Callback de overlap con la PickupCollision.
     * Si el overlapping actor es un PlayerCharacter con InventoryComponent, añade el item
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
    // ─── ESTADO REPLICADO ─────────────────────────────────────────────────────

    /**
     * True una vez que el item fue recogido por un jugador.
     * Replicado para que el cliente oculte inmediatamente el actor antes de que
     * el servidor lo destruya.
     */
    UPROPERTY(ReplicatedUsing = OnRep_IsPickedUp)
    bool bIsPickedUp = false;

    // ─── ANIMACIÓN DE FLOTACIÓN ───────────────────────────────────────────────

    /** Posición Z original en el momento de spawn, usada como base para la animación seno. */
    float BaseZ = 0.f;

    // ─── ONREP CALLBACKS ─────────────────────────────────────────────────────

    UFUNCTION()
    void OnRep_IsPickedUp();

    UFUNCTION()
    void OnRep_ItemData();

    // ─── HELPERS INTERNOS ─────────────────────────────────────────────────────

    /**
     * Inicia la carga asíncrona del TSoftObjectPtr<UStaticMesh> del ItemData
     * y asigna el mesh al MeshComponent cuando termina.
     */
    void AsyncLoadItemMesh();

    /**
     * Actualiza el texto del ItemNameWidget con el nombre del ItemData.
     * Solo tiene efecto si el widget y el ItemData son válidos.
     */
    void UpdateNameWidget();

    /** Handle del timer de lifetime para poder cancelarlo si se recoge antes. */
    FTimerHandle LifetimeTimerHandle;

    /** Handle de la carga asíncrona del mesh, para cancelarla si el actor se destruye antes. */
    TSharedPtr<struct FStreamableHandle> MeshLoadHandle;
};
