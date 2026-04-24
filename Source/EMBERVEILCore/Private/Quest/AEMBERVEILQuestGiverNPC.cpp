#include "Quest/AEMBERVEILQuestGiverNPC.h"

#include "Internationalization/Text.h"
#include "Quest/EMBERVEILQuestDataAsset.h"
#include "Quest/EMBERVEILQuestComponent.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "Player/EMBERVEILPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AEMBERVEILQuestGiverNPC::AEMBERVEILQuestGiverNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    // ─── Interaction Collision ─────────────────────────────────────────────────
    InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
    InteractionCollision->SetSphereRadius(200.f);
    InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    InteractionCollision->SetGenerateOverlapEvents(true);
    SetRootComponent(InteractionCollision);

    // ─── Quest Indicator Widget ────────────────────────────────────────────────
    QuestIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestIndicatorWidget"));
    QuestIndicatorWidget->SetupAttachment(RootComponent);
    QuestIndicatorWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
    QuestIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
    QuestIndicatorWidget->SetDrawSize(FVector2D(64.f, 64.f));
    QuestIndicatorWidget->SetVisibility(false);   // oculto por defecto hasta que el jugador se acerque
}

void AEMBERVEILQuestGiverNPC::BeginPlay()
{
    Super::BeginPlay();

    // Enlazar callbacks de overlap
    InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AEMBERVEILQuestGiverNPC::OnInteractionBeginOverlap);
    InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &AEMBERVEILQuestGiverNPC::OnInteractionEndOverlap);
}

// ─── INTERACCIÓN ──────────────────────────────────────────────────────────────

void AEMBERVEILQuestGiverNPC::HandlePlayerInteract(AEMBERVEILPlayerCharacter* Player)
{
    if (!Player)
    {
        return;
    }

    UEMBERVEILQuestComponent* QuestComp = GetPlayerQuestComponent(Player);
    if (!QuestComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("AEMBERVEILQuestGiverNPC::HandlePlayerInteract — El jugador no tiene QuestComponent"));
        return;
    }

    // ── Prioridad 1: completar quest pendiente ─────────────────────────────────
    // Buscar si el jugador tiene una quest ReadyToComplete que fue dada por este NPC
    for (FQuestState& QuestState : QuestComp->ActiveQuests)
    {
        if (QuestState.Status == EQuestStatus::ReadyToComplete && QuestState.GiverNPCID == NPCID)
        {
            UEMBERVEILQuestDataAsset* QuestData = QuestState.QuestData.Get();
            if (QuestData)
            {
                QuestComp->CompleteQuest(QuestData->QuestID);
                RefreshQuestIndicator(QuestComp);
                return;
            }
        }
    }

    // ── Prioridad 2: ofrecer quest disponible ──────────────────────────────────
    UEMBERVEILQuestDataAsset* AvailableQuest = GetFirstAvailableQuest(QuestComp);
    if (AvailableQuest)
    {
        QuestComp->AcceptQuest(AvailableQuest, NPCID);
        RefreshQuestIndicator(QuestComp);
        return;
    }

    // ── Prioridad 3: sin quests pendientes ─────────────────────────────────────
    UE_LOG(LogTemp, Log, TEXT("AEMBERVEILQuestGiverNPC::HandlePlayerInteract — NPC '%s' no tiene quests disponibles para el jugador"), *NPCID.ToString());
    // Aquí se puede lanzar el sistema de diálogo con QuestGiverDialogue para líneas genéricas
}

EQuestGiverState AEMBERVEILQuestGiverNPC::GetQuestGiverState(UEMBERVEILQuestComponent* PlayerQuestComp) const
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
    for (const TSoftObjectPtr<UEMBERVEILQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        const UEMBERVEILQuestDataAsset* QuestData = QuestSoftRef.Get();
        if (!QuestData)
        {
            // Si no está cargado en memoria, lo consideramos potencialmente disponible
            // No forzamos carga síncrona aquí para evitar hitches en el tick/UI
            continue;
        }

        const EQuestStatus Status = PlayerQuestComp->GetQuestStatus(QuestData->QuestID);

        // Disponible si no está activa, no fue completada (o es repetible), y el prereq aplica
        if (Status == EQuestStatus::NotStarted || (Status == EQuestStatus::Completed && QuestData->bIsRepeatable))
        {
            return EQuestGiverState::HasQuestAvailable;
        }
    }

    // Verificar si todas las quests están completadas
    bool bAllDone = true;
    for (const TSoftObjectPtr<UEMBERVEILQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        const UEMBERVEILQuestDataAsset* QuestData = QuestSoftRef.Get();
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

UEMBERVEILQuestDataAsset* AEMBERVEILQuestGiverNPC::GetFirstAvailableQuest(UEMBERVEILQuestComponent* PlayerQuestComp) const
{
    if (!PlayerQuestComp)
    {
        return nullptr;
    }

    for (const TSoftObjectPtr<UEMBERVEILQuestDataAsset>& QuestSoftRef : AvailableQuests)
    {
        // Carga síncrona al momento de ofrecer la quest — se asume que el asset
        // es pequeño y que esta operación ocurre en respuesta a la interacción del usuario
        UEMBERVEILQuestDataAsset* QuestData = QuestSoftRef.LoadSynchronous();
        if (!QuestData)
        {
            continue;
        }

        const EQuestStatus Status = PlayerQuestComp->GetQuestStatus(QuestData->QuestID);

        // Puede ofrecerse si: no está activa, no completada (o es repetible) y prereqs OK
        const bool bNotActive    = Status == EQuestStatus::NotStarted;
        const bool bRepeatableOK = Status == EQuestStatus::Completed && QuestData->bIsRepeatable;

        if (bNotActive || bRepeatableOK)
        {
            return QuestData;
        }
    }

    return nullptr;
}

void AEMBERVEILQuestGiverNPC::RefreshQuestIndicator(UEMBERVEILQuestComponent* PlayerQuestComp)
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
        // El widget implementa una función "SetQuestGiverState" en Blueprint para cambiar el icono
        // Se puede llamar vía BPImplementableEvent o vía InterfaceCall si se define la interfaz
        // Por ahora se deja el hook comentado para implementación en BP hijo:
        // Widget->SetQuestGiverState(State);
    }
}

// ─── OVERLAP CALLBACKS ────────────────────────────────────────────────────────

void AEMBERVEILQuestGiverNPC::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AEMBERVEILPlayerCharacter* Player = Cast<AEMBERVEILPlayerCharacter>(OtherActor);
    if (!Player)
    {
        return;
    }

    UEMBERVEILQuestComponent* QuestComp = GetPlayerQuestComponent(Player);

    RefreshQuestIndicator(QuestComp);
    OnPlayerEnteredInteractionRange.Broadcast(Player);
}

void AEMBERVEILQuestGiverNPC::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    AEMBERVEILPlayerCharacter* Player = Cast<AEMBERVEILPlayerCharacter>(OtherActor);
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

// ─── HELPERS INTERNOS ─────────────────────────────────────────────────────────

UEMBERVEILQuestComponent* AEMBERVEILQuestGiverNPC::GetPlayerQuestComponent(AEMBERVEILPlayerCharacter* Player) const
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

    return PS->FindComponentByClass<UEMBERVEILQuestComponent>();
}

// ─── IEMBERVEILInteractable ───────────────────────────────────────────────────

void AEMBERVEILQuestGiverNPC::Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor)
{
    HandlePlayerInteract(Interactor);
}

FText AEMBERVEILQuestGiverNPC::GetInteractPrompt_Implementation() const
{
    if (const UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController<APlayerController>())
        {
            if (AEMBERVEILPlayerCharacter* PlayerChar = Cast<AEMBERVEILPlayerCharacter>(PC->GetPawn()))
            {
                if (UEMBERVEILQuestComponent* QuestComp = GetPlayerQuestComponent(PlayerChar))
                {
                    if (GetQuestGiverState(QuestComp) == EQuestGiverState::HasQuestToComplete)
                    {
                        return NSLOCTEXT("QuestGiver", "DeliverQuest", "Entregar misión");
                    }
                }
            }
        }
    }

    return NSLOCTEXT("QuestGiver", "Talk", "Hablar");
}

bool AEMBERVEILQuestGiverNPC::CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const
{
    return true;
}
