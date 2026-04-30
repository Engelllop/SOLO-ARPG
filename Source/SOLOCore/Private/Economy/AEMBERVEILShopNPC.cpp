#include "Economy/ASOLOShopNPC.h"
#include "Economy/SOLOMarketplaceComponent.h"
#include "Character/SOLOPlayerCharacter.h"
#include "Internationalization/Text.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/Texture2D.h"

// â”€â”€â”€ CONSTRUCTOR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

ASOLOShopNPC::ASOLOShopNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    // â”€â”€ Esfera de colisiÃ³n de interacciÃ³n (radio 200u)
    InteractionCollision = CreateDefaultSubobject<USphereComponent>(
        TEXT("InteractionCollision"));
    InteractionCollision->SetSphereRadius(200.f);
    InteractionCollision->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = InteractionCollision;

    // â”€â”€ Widget indicador flotante
    ShopIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(
        TEXT("ShopIndicatorWidget"));
    ShopIndicatorWidget->SetupAttachment(RootComponent);
    ShopIndicatorWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    ShopIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
    ShopIndicatorWidget->SetVisibility(false);  // Oculto hasta que el jugador se acerque

    // â”€â”€ Componente de marketplace
    MarketplaceComponent = CreateDefaultSubobject<USOLOMarketplaceComponent>(
        TEXT("MarketplaceComponent"));
}

// â”€â”€â”€ BEGINPLAY â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOShopNPC::BeginPlay()
{
    Super::BeginPlay();

    // Vincular callbacks de colisiÃ³n para mostrar/ocultar el indicador
    if (InteractionCollision)
    {
        InteractionCollision->OnComponentBeginOverlap.AddDynamic(
            this, &ASOLOShopNPC::OnInteractionBeginOverlap);

        InteractionCollision->OnComponentEndOverlap.AddDynamic(
            this, &ASOLOShopNPC::OnInteractionEndOverlap);
    }
}

// â”€â”€â”€ INTERACT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOShopNPC::HandlePlayerInteract(ASOLOPlayerCharacter* Player)
{
    if (!Player)
    {
        return;
    }

    // Disparar el delegate: la UI (UMG/CommonUI) lo escucha y abre el panel de tienda
    OnShopOpened.Broadcast(this, Player);
}

// â”€â”€â”€ CALLBACKS DE COLISIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOShopNPC::OnInteractionBeginOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    // Solo mostrar el indicador cuando el jugador (ASOLOPlayerCharacter) entra al radio
    ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(OtherActor);
    if (PlayerChar && ShopIndicatorWidget)
    {
        ShopIndicatorWidget->SetVisibility(true);
    }
}

void ASOLOShopNPC::OnInteractionEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    // Ocultar el indicador cuando el jugador sale del radio
    ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(OtherActor);
    if (PlayerChar && ShopIndicatorWidget)
    {
        ShopIndicatorWidget->SetVisibility(false);
    }
}

// â”€â”€â”€ ISOLOInteractable â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOShopNPC::Interact_Implementation(ASOLOPlayerCharacter* Interactor)
{
    HandlePlayerInteract(Interactor);
}

FText ASOLOShopNPC::GetInteractPrompt_Implementation() const
{
    return NSLOCTEXT("ShopNPC", "OpenShop", "Abrir tienda");
}

bool ASOLOShopNPC::CanInteract_Implementation(ASOLOPlayerCharacter* Interactor) const
{
    return true;
}
