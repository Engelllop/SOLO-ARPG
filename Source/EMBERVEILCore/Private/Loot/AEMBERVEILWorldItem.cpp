#include "Loot/AEMBERVEILWorldItem.h"

#include "Inventory/EMBERVEILItemBase.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UI/EMBERVEILNotificationSystem.h"
#include "Internationalization/Text.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

// ─── Constantes de animación ──────────────────────────────────────────────────

namespace WorldItemAnim
{
    /** Amplitud de la oscilación vertical en unidades de mundo. */
    static constexpr float FloatAmplitude = 10.f;

    /** Frecuencia de la oscilación en Hz (ciclos por segundo). */
    static constexpr float FloatFrequency = 0.8f;
}

// ─── Constructor ──────────────────────────────────────────────────────────────

AEMBERVEILWorldItem::AEMBERVEILWorldItem()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetNetUpdateFrequency(10.f);

    // Raíz del actor: la esfera de pickup determina el punto de anclaje
    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->SetSphereRadius(80.f);
    PickupCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    PickupCollision->SetGenerateOverlapEvents(true);
    SetRootComponent(PickupCollision);

    // Mesh 3D del item — se asigna de forma asíncrona en InitializeItem
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

    // Widget flotante con el nombre del item (world-space, siempre mira a la cámara)
    ItemNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemNameWidget"));
    ItemNameWidget->SetupAttachment(RootComponent);
    ItemNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
    ItemNameWidget->SetDrawSize(FVector2D(200.f, 40.f));
    ItemNameWidget->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
    ItemNameWidget->SetVisibility(false);   // oculto hasta que se asigne ItemData
}

// ─── Replicación ──────────────────────────────────────────────────────────────

void AEMBERVEILWorldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEMBERVEILWorldItem, ItemData);
    DOREPLIFETIME(AEMBERVEILWorldItem, Quantity);
    DOREPLIFETIME(AEMBERVEILWorldItem, bIsCurrency);
    DOREPLIFETIME(AEMBERVEILWorldItem, CurrencyAmount);
    DOREPLIFETIME(AEMBERVEILWorldItem, bIsPickedUp);
}

// ─── BeginPlay ────────────────────────────────────────────────────────────────

void AEMBERVEILWorldItem::BeginPlay()
{
    Super::BeginPlay();

    // Guardamos la Z inicial para usarla como base de la animación de flotación.
    BaseZ = GetActorLocation().Z;

    // Registramos el overlap solo en el servidor — el servidor controla el pickup.
    if (HasAuthority())
    {
        PickupCollision->OnComponentBeginOverlap.AddDynamic(
            this,
            &AEMBERVEILWorldItem::OnPickupOverlap
        );
    }
}

// ─── Tick (animación de flotación) ────────────────────────────────────────────

void AEMBERVEILWorldItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPickedUp)
        return;

    // Oscilación senoidal en Z: posición = BaseZ + Amplitud * sin(2π * Frecuencia * t)
    const float TimeSeconds = GetWorld()->GetTimeSeconds();
    const float NewZ = BaseZ + WorldItemAnim::FloatAmplitude
                       * FMath::Sin(2.f * PI * WorldItemAnim::FloatFrequency * TimeSeconds);

    FVector CurrentLocation = GetActorLocation();
    CurrentLocation.Z       = NewZ;
    SetActorLocation(CurrentLocation);
}

// ─── Inicialización de item ───────────────────────────────────────────────────

void AEMBERVEILWorldItem::InitializeItem(UEMBERVEILItemBase* InItemData, int32 InQuantity, float InLifetime)
{
    if (!InItemData)
        return;

    ItemData  = InItemData;
    Quantity  = FMath::Max(1, InQuantity);
    Lifetime  = FMath::Max(1.f, InLifetime);

    // Cargar el mesh de forma asíncrona y actualizar el widget de nombre.
    AsyncLoadItemMesh();
    UpdateNameWidget();

    // Timer de destrucción automática.
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            LifetimeTimerHandle,
            FTimerDelegate::CreateLambda([this]()
            {
                if (IsValid(this))
                    Destroy();
            }),
            Lifetime,
            false
        );
    }
}

// ─── Inicialización de moneda ─────────────────────────────────────────────────

void AEMBERVEILWorldItem::InitializeCurrency(int32 Amount, float InLifetime)
{
    bIsCurrency    = true;
    CurrencyAmount = FMath::Max(0, Amount);
    Lifetime       = FMath::Max(1.f, InLifetime);

    // Mostrar el widget con "Currency" como texto (el widget Blueprint puede leer bIsCurrency).
    ItemNameWidget->SetVisibility(true);

    // Timer de destrucción automática.
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            LifetimeTimerHandle,
            FTimerDelegate::CreateLambda([this]()
            {
                if (IsValid(this))
                    Destroy();
            }),
            Lifetime,
            false
        );
    }
}

// ─── OnPickupOverlap ──────────────────────────────────────────────────────────

void AEMBERVEILWorldItem::OnPickupOverlap(UPrimitiveComponent* /*OverlappedComponent*/,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* /*OtherComp*/,
                                           int32 /*OtherBodyIndex*/,
                                           bool /*bFromSweep*/,
                                           const FHitResult& /*SweepResult*/)
{
    // Garantías: solo servidor, solo una vez.
    if (!HasAuthority() || bIsPickedUp || !OtherActor)
        return;

    // Buscamos el InventoryComponent en el actor que hizo overlap.
    UEMBERVEILInventoryComponent* InventoryComp =
        OtherActor->FindComponentByClass<UEMBERVEILInventoryComponent>();

    if (!InventoryComp)
        return;

    bool bPickupSucceeded = false;

    if (bIsCurrency)
    {
        InventoryComp->AddCurrency(CurrencyAmount);
        bPickupSucceeded = true;
    }
    else if (ItemData)
    {
        bPickupSucceeded = InventoryComp->AddItem(ItemData, Quantity);
    }

    if (bPickupSucceeded)
    {
        bIsPickedUp = true;

        if (APawn* Pawn = Cast<APawn>(OtherActor))
        {
            if (APlayerController* PC = Pawn->GetController<APlayerController>())
            {
                FEMBERVEILNotification PickupNote;
                PickupNote.Type = ENotificationType::ItemPickup;
                PickupNote.Duration = 3.f;
                if (bIsCurrency)
                {
                    PickupNote.Message = FText::Format(
                        NSLOCTEXT("EMBERVEILWorldItem", "CurrencyPickup", "Obtenido: {0} monedas"),
                        FText::AsNumber(CurrencyAmount));
                }
                else if (ItemData)
                {
                    PickupNote.Message = FText::Format(
                        NSLOCTEXT("EMBERVEILWorldItem", "ItemPickup", "Obtenido: {0} x{1}"),
                        ItemData->ItemName,
                        FText::AsNumber(Quantity));
                }
                UEMBERVEILNotificationSystem::NotifyOwningClient(PC, PickupNote);
            }
        }

        // Cancelamos el timer de lifetime — ya no es necesario.
        if (UWorld* World = GetWorld())
            World->GetTimerManager().ClearTimer(LifetimeTimerHandle);

        // Desactivamos el pickup para que no se procese dos veces en el mismo frame.
        PickupCollision->SetGenerateOverlapEvents(false);

        // Destruimos el actor. Al estar replicado, se destruye en todos los clientes.
        Destroy();
    }
}

// ─── OnRep callbacks ─────────────────────────────────────────────────────────

void AEMBERVEILWorldItem::OnRep_IsPickedUp()
{
    if (bIsPickedUp)
    {
        // Ocultamos el actor en el cliente antes de que el servidor lo destruya.
        SetActorHiddenInGame(true);
        PickupCollision->SetGenerateOverlapEvents(false);
        SetActorTickEnabled(false);
    }
}

void AEMBERVEILWorldItem::OnRep_ItemData()
{
    // En el cliente, cuando ItemData se replica, cargamos el mesh y actualizamos el widget.
    if (ItemData)
    {
        AsyncLoadItemMesh();
        UpdateNameWidget();
    }
}

// ─── Helpers ─────────────────────────────────────────────────────────────────

void AEMBERVEILWorldItem::AsyncLoadItemMesh()
{
    if (!ItemData || ItemData->WorldMesh.IsNull())
        return;

    // Si el asset ya está en memoria, lo asignamos directamente.
    if (ItemData->WorldMesh.IsValid())
    {
        MeshComponent->SetStaticMesh(ItemData->WorldMesh.Get());
        return;
    }

    // Carga asíncrona — evita hitch en el hilo de juego.
    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

    MeshLoadHandle = StreamableManager.RequestAsyncLoad(
        ItemData->WorldMesh.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda([this]()
        {
            if (!IsValid(this) || !ItemData)
                return;

            UStaticMesh* LoadedMesh = ItemData->WorldMesh.Get();
            if (LoadedMesh)
                MeshComponent->SetStaticMesh(LoadedMesh);
        })
    );
}

void AEMBERVEILWorldItem::UpdateNameWidget()
{
    if (!ItemNameWidget)
        return;

    if (ItemData)
    {
        // El Blueprint del widget debería exponer una función SetItemName(FText).
        // Aquí simplemente hacemos el widget visible; el Blueprint lo leerá
        // consultando la propiedad ItemData de este actor.
        ItemNameWidget->SetVisibility(true);
    }
}
