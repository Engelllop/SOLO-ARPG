#include "Economy/AEMBERVEILShopNPC.h"
#include "Economy/EMBERVEILMarketplaceComponent.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "Internationalization/Text.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/Texture2D.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────

AEMBERVEILShopNPC::AEMBERVEILShopNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    // ── Esfera de colisión de interacción (radio 200u)
    InteractionCollision = CreateDefaultSubobject<USphereComponent>(
        TEXT("InteractionCollision"));
    InteractionCollision->SetSphereRadius(200.f);
    InteractionCollision->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = InteractionCollision;

    // ── Widget indicador flotante
    ShopIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(
        TEXT("ShopIndicatorWidget"));
    ShopIndicatorWidget->SetupAttachment(RootComponent);
    ShopIndicatorWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    ShopIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
    ShopIndicatorWidget->SetVisibility(false);  // Oculto hasta que el jugador se acerque

    // ── Componente de marketplace
    MarketplaceComponent = CreateDefaultSubobject<UEMBERVEILMarketplaceComponent>(
        TEXT("MarketplaceComponent"));
}

// ─── BEGINPLAY ────────────────────────────────────────────────────────────────

void AEMBERVEILShopNPC::BeginPlay()
{
    Super::BeginPlay();

    // Vincular callbacks de colisión para mostrar/ocultar el indicador
    if (InteractionCollision)
    {
        InteractionCollision->OnComponentBeginOverlap.AddDynamic(
            this, &AEMBERVEILShopNPC::OnInteractionBeginOverlap);

        InteractionCollision->OnComponentEndOverlap.AddDynamic(
            this, &AEMBERVEILShopNPC::OnInteractionEndOverlap);
    }
}

// ─── INTERACT ─────────────────────────────────────────────────────────────────

void AEMBERVEILShopNPC::HandlePlayerInteract(AEMBERVEILPlayerCharacter* Player)
{
    if (!Player)
    {
        return;
    }

    // Disparar el delegate: la UI (UMG/CommonUI) lo escucha y abre el panel de tienda
    OnShopOpened.Broadcast(this, Player);
}

// ─── CALLBACKS DE COLISIÓN ────────────────────────────────────────────────────

void AEMBERVEILShopNPC::OnInteractionBeginOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Solo mostrar el indicador cuando el jugador (AEMBERVEILPlayerCharacter) entra al radio
    AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(OtherActor);
    if (PlayerChar && ShopIndicatorWidget)
    {
        ShopIndicatorWidget->SetVisibility(true);
    }
}

void AEMBERVEILShopNPC::OnInteractionEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    // Ocultar el indicador cuando el jugador sale del radio
    AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(OtherActor);
    if (PlayerChar && ShopIndicatorWidget)
    {
        ShopIndicatorWidget->SetVisibility(false);
    }
}

// ─── IEMBERVEILInteractable ───────────────────────────────────────────────────

void AEMBERVEILShopNPC::Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor)
{
    HandlePlayerInteract(Interactor);
}

FText AEMBERVEILShopNPC::GetInteractPrompt_Implementation() const
{
    return NSLOCTEXT("ShopNPC", "OpenShop", "Abrir tienda");
}

bool AEMBERVEILShopNPC::CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const
{
    return true;
}
