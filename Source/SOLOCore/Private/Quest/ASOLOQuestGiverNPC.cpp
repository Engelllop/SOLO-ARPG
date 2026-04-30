#include "Quest/ASOLOQuestGiverNPC.h"

#include "Internationalization/Text.h"
#include "Quest/SOLOQuestDataAsset.h"
#include "Quest/SOLOQuestComponent.h"
#include "Character/SOLOPlayerCharacter.h"
#include "Player/SOLOPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ASOLOQuestGiverNPC::ASOLOQuestGiverNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    // â”€â”€â”€ Interaction Collision â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
    InteractionCollision->SetSphereRadius(200.f);
    InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    InteractionCollision->SetGenerateOverlapEvents(true);
    SetRootComponent(InteractionCollision);

    // â”€â”€â”€ Quest Indicator Widget â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    QuestIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIndicatorWidget"));
    QuestIndicatorWidget->SetupAttachment(RootComponent);
    QuestIndicatorWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    QuestIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
    QuestIndicatorWidget->SetDrawSize(FVector2D(64.f, 64.f));
    QuestIndicatorWidget->SetVisibility(false);   // oculto por defecto hasta que el jugador se acerque
}

void ASOLOQuestGiverNPC::BeginPlay()
{
    Super::BeginPlay();

    // Enlazar callbacks de overlap
    InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ASOLOQuestGiverNPC::OnInteractionBeginOverlap);
    InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ASOLOQuestGiverNPC::OnInteractionEndOverlap);
}

// â”€â”€â”€ INTERACCIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOQuestGiverNPC::HandlePlayerInteract(ASOLOPlayerCharacter* Player)
{
    if (!Player)
    {
        return;
    }

    USOLOQuestComponent* QuestComp = GetPlayerQuestComponent(Player);
    if (!QuestComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("ASOLOQuestGiverNPC::HandlePlayerInteract â€” El jugador no tiene QuestComponent"));
        return;
    }

    // â”€â”€ Prioridad 1: completar quest pendiente â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    // Buscar si el jugador tiene una quest ReadyToComplete que fue dada por este NPC
    for (FQuestState& QuestState : QuestComp->ActiveQuests)
    {
        if (QuestState.Status == EQuestStatus::ReadyToComplete && QuestState.GiverNPCID == NPCID)
        {
            USOLOQuestDataAsset* QuestData = QuestState.QuestData.Get();
            if (QuestData)
            {
                QuestComp->CompleteQuest(QuestData->QuestID);
                RefreshQuestIndicator(QuestComp);
                return;
            }
        }
    }

    // â”€â”€ Prioridad 2: ofrecer quest disponible â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    USOLOQuestDataAsset* AvailableQuest = GetFirstAvailableQuest(QuestComp);
    if (AvailableQuest)
    {
        QuestComp->AcceptQuest(AvailableQuest, NPCID);
        RefreshQuestIndicator(QuestComp);
        return;
    }

    // â”€â”€ Prioridad 3: sin quests pendientes â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    UE_LOG(LogTemp, Log, TEXT("ASOLOQuestGiverNPC::HandlePlayerInteract â€” NPC '%s' no tiene quests disponibles para el jugador"), *NPCID.ToString());
    // AquÃ­ se puede lanzar el sistema de diÃ¡logo con QuestGiverDialogue para lÃ­neas genÃ©ricas
}

EQuestGiverState ASOLOQuestGiverNPC::GetQuestGiverState(USOLOQuestComponent* PlayerQuestComp) const
{
    if (!PlayerQuestComp)
    {
        return EQuestGiverState::NoQuests;
    }

    if (AvailableQuests.Num() == 0)
    {
        return EQuestGiverState::NoQuests;
    }

    // Verificar si hay una quest ReadyToComplete para este NPC
    for (const FQuestState& QuestState : PlayerQuestComp->ActiveQuests)
    {
        if (QuestState.Status == EQuestStatus::ReadyToComplete && QuestState.GiverNPCID == NPCID)
        {
            return EQuestGiverState::HasQuestToComplete;
        }
    }

    // Verificar si hay alguna quest disponible para aceptar
    for (const TSoftObjectPtr<USOLOQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        const USOLOQuestDataAsset* QuestData = QuestSoftRef.Get();
        if (!QuestData)
        {
            // Si no estÃ¡ cargado en memoria, lo consideramos potencialmente disponible
            // No forzamos carga sÃ­ncrona aquÃ­ para evitar hitches en el tick/UI
            continue;
        }

        const EQuestStatus Status = PlayerQuestComp->GetQuestStatus(QuestData->QuestID);

        // Disponible si no estÃ¡ activa, no fue completada (o es repetible), y el prereq aplica
        if (Status == EQuestStatus::NotStarted || (Status == EQuestStatus::Completed && QuestData->bIsRepeatable))
        {
            return EQuestGiverState::HasQuestAvailable;
        }
    }

    // Verificar si todas las quests estÃ¡n completadas
    bool bAllDone = true;
    for (const TSoftObjectPtr<USOLOQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        const USOLOQuestDataAsset* QuestData = QuestSoftRef.Get();
        if (!QuestData)
        {
            bAllDone = false;
            break;
        }

        const EQuestStatus Status = PlayerQuestComp->GetQuestStatus(QuestData->QuestID);
        if (Status != EQuestStatus::Completed)
        {
            bAllDone = false;
            break;
        }
    }

    return bAllDone ? EQuestGiverState::AllQuestsDone : EQuestGiverState::NoQuests;
}

USOLOQuestDataAsset* ASOLOQuestGiverNPC::GetFirstAvailableQuest(USOLOQuestComponent* PlayerQuestComp) const
{
    if (!PlayerQuestComp)
    {
        return nullptr;
    }

    for (const TSoftObjectPtr<USOLOQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        // Carga sÃ­ncrona al momento de ofrecer la quest â€” se asume que el asset
        // es pequeÃ±o y que esta operaciÃ³n ocurre en respuesta a la interacciÃ³n del usuario
        USOLOQuestDataAsset* QuestData = QuestSoftRef.LoadSynchronous();
        if (!QuestData)
        {
            continue;
        }

        const EQuestStatus Status = PlayerQuestComp->GetQuestStatus(QuestData->QuestID);

        // Puede ofrecerse si: no estÃ¡ activa, no completada (o es repetible) y prereqs OK
        const bool bNotActive    = Status == EQuestStatus::NotStarted;
        const bool bRepeatableOK = Status == EQuestStatus::Completed && QuestData->bIsRepeatable;

        if (bNotActive || bRepeatableOK)
        {
            return QuestData;
        }
    }

    return nullptr;
}

void ASOLOQuestGiverNPC::RefreshQuestIndicator(USOLOQuestComponent* PlayerQuestComp)
{
    if (!QuestIndicatorWidget)
    {
        return;
    }

    const EQuestGiverState State = GetQuestGiverState(PlayerQuestComp);

    switch (State)
    {
        case EQuestGiverState::HasQuestAvailable:
        case EQuestGiverState::HasQuestToComplete:
            QuestIndicatorWidget->SetVisibility(true);
            break;

        case EQuestGiverState::NoQuests:
        case EQuestGiverState::AllQuestsDone:
        default:
            QuestIndicatorWidget->SetVisibility(false);
            break;
    }

    // Notificar al widget Blueprint del estado actual para que muestre ! o ? correctamente.
    // El widget puede bindear a este evento via la clase del widget asignada en el Blueprint hijo.
    UUserWidget* Widget = QuestIndicatorWidget->GetUserWidgetObject();
    if (Widget)
    {
        // El widget implementa una funciÃ³n "SetQuestGiverState" en Blueprint para cambiar el icono
        // Se puede llamar vÃ­a BPImplementableEvent o vÃ­a InterfaceCall si se define la interfaz
        // Por ahora se deja el hook comentado para implementaciÃ³n en BP hijo:
        // Widget->SetQuestGiverState(State);
    }
}

// â”€â”€â”€ OVERLAP CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOQuestGiverNPC::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASOLOPlayerCharacter* Player = Cast<ASOLOPlayerCharacter>(OtherActor);
    if (!Player)
    {
        return;
    }

    USOLOQuestComponent* QuestComp = GetPlayerQuestComponent(Player);

    RefreshQuestIndicator(QuestComp);
    OnPlayerEnteredInteractionRange.Broadcast(Player);
}

void ASOLOQuestGiverNPC::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    ASOLOPlayerCharacter* Player = Cast<ASOLOPlayerCharacter>(OtherActor);
    if (!Player)
    {
        return;
    }

    // Ocultar indicador al salir del rango
    if (QuestIndicatorWidget)
    {
        QuestIndicatorWidget->SetVisibility(false);
    }

    OnPlayerLeftInteractionRange.Broadcast(Player);
}

// â”€â”€â”€ HELPERS INTERNOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

USOLOQuestComponent* ASOLOQuestGiverNPC::GetPlayerQuestComponent(ASOLOPlayerCharacter* Player) const
{
    if (!Player)
    {
        return nullptr;
    }

    APlayerState* PS = Player->GetPlayerState<APlayerState>();
    if (!PS)
    {
        return nullptr;
    }

    return PS->FindComponentByClass<USOLOQuestComponent>();
}

// â”€â”€â”€ ISOLOInteractable â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void ASOLOQuestGiverNPC::Interact_Implementation(ASOLOPlayerCharacter* Interactor)
{
    HandlePlayerInteract(Interactor);
}

FText ASOLOQuestGiverNPC::GetInteractPrompt_Implementation() const
{
    if (const UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController<APlayerController>())
        {
            if (ASOLOPlayerCharacter* PlayerChar = Cast<ASOLOPlayerCharacter>(PC->GetPawn()))
            {
                if (USOLOQuestComponent* QuestComp = GetPlayerQuestComponent(PlayerChar))
                {
                    if (GetQuestGiverState(QuestComp) == EQuestGiverState::HasQuestToComplete)
                    {
                        return NSLOCTEXT("QuestGiver", "DeliverQuest", "Entregar misiÃ³n");
                    }
                }
            }
        }
    }

    return NSLOCTEXT("QuestGiver", "Talk", "Hablar");
}

bool ASOLOQuestGiverNPC::CanInteract_Implementation(ASOLOPlayerCharacter* Interactor) const
{
    return true;
}
