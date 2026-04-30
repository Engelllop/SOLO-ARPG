#include "Loot/ASOLOWorldItem.h"

#include "Inventory/SOLOItemBase.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "UI/SOLONotificationSystem.h"
#include "Internationalization/Text.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

// â”€â”€â”€ Constantes de animaciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

namespace WorldItemAnim
{
    /** Amplitud de la oscilaciÃ³n vertical en unidades de mundo. */
    static constexpr float FloatAmplitude = 10.f;

    /** Frecuencia de la oscilaciÃ³n en Hz (ciclos por segundo). */
    static constexpr float FloatFrequency = 0.8f;
}

// â”€â”€â”€ Constructor â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

ASOLOWorldItem::ASOLOWorldItem()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetNetUpdateFrequency(10.f);

    // RaÃ­z del actor: la esfera de pickup determina el punto de anclaje
    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->SetSphereRadius(80.f);
    PickupCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    PickupCollision->SetGenerateOverlapEvents(true);
    SetRootComponent(PickupCollision);

    // Mesh 3D del item â€” se asigna de forma asÃ­ncrona en InitializeItem
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

    // Widget flotante con el nombre del item (world-space, siempre mira a la cÃ¡mara)
    ItemNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemNameWidget"));
    ItemNameWidget->SetupAttachment(RootComponent);
    ItemNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
    ItemNameWidget->SetDrawSize(FVector2D(200.f, 40.f));
    ItemNameWidget->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
    ItemNameWidget->SetVisibility(false);   // oculto hasta que se asigne ItemData
}

// â”€â”€â”€ ReplicaciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASOLOWorldItem, ItemData);
    DOREPLIFETIME(ASOLOWorldItem, Quantity);
    DOREPLIFETIME(ASOLOWorldItem, bIsCurrency);
    DOREPLIFETIME(ASOLOWorldItem, CurrencyAmount);
    DOREPLIFETIME(ASOLOWorldItem, bIsPickedUp);
}

// â”€â”€â”€ BeginPlay â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::BeginPlay()
{
    Super::BeginPlay();

    // Guardamos la Z inicial para usarla como base de la animaciÃ³n de flotaciÃ³n.
    BaseZ = GetActorLocation().Z;

    // Registramos el overlap solo en el servidor â€” el servidor controla el pickup.
    if (HasAuthority())
    {
        PickupCollision->OnComponentBeginOverlap.AddDynamic(
            this,
            &ASOLOWorldItem::OnPickupOverlap
        );
    }
}

// â”€â”€â”€ Tick (animaciÃ³n de flotaciÃ³n) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPickedUp)
        return;

    // OscilaciÃ³n senoidal en Z: posiciÃ³n = BaseZ + Amplitud * sin(2Ï€ * Frecuencia * t)
    const float TimeSeconds = GetWorld()->GetTimeSeconds();
    const float NewZ = BaseZ + WorldItemAnim::FloatAmplitude
                       * FMath::Sin(2.f * PI * WorldItemAnim::FloatFrequency * TimeSeconds);

    FVector CurrentLocation = GetActorLocation();
    CurrentLocation.Z       = NewZ;
    SetActorLocation(CurrentLocation);
}

// â”€â”€â”€ InicializaciÃ³n de item â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::InitializeItem(USOLOItemBase* InItemData, int32 InQuantity, float InLifetime)
{
    if (!InItemData)
        return;

    ItemData  = InItemData;
    Quantity  = FMath::Max(1, InQuantity);
    Lifetime  = FMath::Max(1.f, InLifetime);

    // Cargar el mesh de forma asÃ­ncrona y actualizar el widget de nombre.
    AsyncLoadItemMesh();
    UpdateNameWidget();

    // Timer de destrucciÃ³n automÃ¡tica.
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

// â”€â”€â”€ InicializaciÃ³n de moneda â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::InitializeCurrency(int32 Amount, float InLifetime)
{
    bIsCurrency    = true;
    CurrencyAmount = FMath::Max(0, Amount);
    Lifetime       = FMath::Max(1.f, InLifetime);

    // Mostrar el widget con "Currency" como texto (el widget Blueprint puede leer bIsCurrency).
    ItemNameWidget->SetVisibility(true);

    // Timer de destrucciÃ³n automÃ¡tica.
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

// â”€â”€â”€ OnPickupOverlap â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::OnPickupOverlap(UPrimitiveComponent* /*OverlappedComponent*/,
                                           AActor* OtherActor,
                                           UPrimitiveComponent* /*OtherComp*/,
                                           int32 /*OtherBodyIndex*/,
                                           bool /*bFromSweep*/,
                                           const FHitResult& /*SweepResult*/)
{
    // GarantÃ­as: solo servidor, solo una vez.
    if (!HasAuthority() || bIsPickedUp || !OtherActor)
        return;

    // Buscamos el InventoryComponent en el actor que hizo overlap.
    USOLOInventoryComponent* InventoryComp =
        OtherActor->FindComponentByClass<USOLOInventoryComponent>();

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
                FSOLONotification PickupNote;
                PickupNote.Type = ENotificationType::ItemPickup;
                PickupNote.Duration = 3.f;
                if (bIsCurrency)
                {
                    PickupNote.Message = FText::Format(
                        NSLOCTEXT("SOLOWorldItem", "CurrencyPickup", "Obtenido: {0} monedas"),
                        FText::AsNumber(CurrencyAmount));
                }
                else if (ItemData)
                {
                    PickupNote.Message = FText::Format(
                        NSLOCTEXT("SOLOWorldItem", "ItemPickup", "Obtenido: {0} x{1}"),
                        ItemData->ItemName,
                        FText::AsNumber(Quantity));
                }
                USOLONotificationSystem::NotifyOwningClient(PC, PickupNote);
            }
        }

        // Cancelamos el timer de lifetime â€” ya no es necesario.
        if (UWorld* World = GetWorld())
            World->GetTimerManager().ClearTimer(LifetimeTimerHandle);

        // Desactivamos el pickup para que no se procese dos veces en el mismo frame.
        PickupCollision->SetGenerateOverlapEvents(false);

        // Destruimos el actor. Al estar replicado, se destruye en todos los clientes.
        Destroy();
    }
}

// â”€â”€â”€ OnRep callbacks â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::OnRep_IsPickedUp()
{
    if (bIsPickedUp)
    {
        // Ocultamos el actor en el cliente antes de que el servidor lo destruya.
        SetActorHiddenInGame(true);
        PickupCollision->SetGenerateOverlapEvents(false);
        SetActorTickEnabled(false);
    }
}

void ASOLOWorldItem::OnRep_ItemData()
{
    // En el cliente, cuando ItemData se replica, cargamos el mesh y actualizamos el widget.
    if (ItemData)
    {
        AsyncLoadItemMesh();
        UpdateNameWidget();
    }
}

// â”€â”€â”€ Helpers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOWorldItem::AsyncLoadItemMesh()
{
    if (!ItemData || ItemData->WorldMesh.IsNull())
        return;

    // Si el asset ya estÃ¡ en memoria, lo asignamos directamente.
    if (ItemData->WorldMesh.IsValid())
    {
        MeshComponent->SetStaticMesh(ItemData->WorldMesh.Get());
        return;
    }

    // Carga asÃ­ncrona â€” evita hitch en el hilo de juego.
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

void ASOLOWorldItem::UpdateNameWidget()
{
    if (!ItemNameWidget)
        return;

    if (ItemData)
    {
        // El Blueprint del widget deberÃ­a exponer una funciÃ³n SetItemName(FText).
        // AquÃ­ simplemente hacemos el widget visible; el Blueprint lo leerÃ¡
        // consultando la propiedad ItemData de este actor.
        ItemNameWidget->SetVisibility(true);
    }
}
