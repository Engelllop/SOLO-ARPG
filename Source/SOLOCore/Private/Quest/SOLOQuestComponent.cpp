#include "Quest/SOLOQuestComponent.h"

#include "Quest/SOLOQuestDataAsset.h"
#include "Player/SOLOPlayerState.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Inventory/SOLOItemBase.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "UI/SOLONotificationSystem.h"
#include "Internationalization/Text.h"
#include "TimerManager.h"

// â”€â”€â”€ FQuestState â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool FQuestState::IsReadyToComplete() const
{
    if (Status != EQuestStatus::InProgress)
    {
        return false;
    }

    for (const FQuestObjective& Objective : Objectives)
    {
        if (!Objective.IsComplete())
        {
            return false;
        }
    }

    return true;
}

// â”€â”€â”€ USOLOQuestComponent â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

USOLOQuestComponent::USOLOQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
}

void USOLOQuestComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USOLOQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USOLOQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USOLOQuestComponent, ActiveQuests);
    DOREPLIFETIME(USOLOQuestComponent, CompletedQuestIDs);
    DOREPLIFETIME(USOLOQuestComponent, FailedQuestIDs);
}

// â”€â”€â”€ GESTIÃ“N DE QUESTS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool USOLOQuestComponent::AcceptQuest(USOLOQuestDataAsset* QuestData, FName GiverNPCID)
{
    if (!QuestData)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” QuestData es nullptr"));
        return false;
    }

    const FName QuestID = QuestData->QuestID;

    // Verificar lÃ­mite de quests activas
    if (ActiveQuests.Num() >= MaxActiveQuests)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” LÃ­mite de quests activas alcanzado (%d)"), MaxActiveQuests);
        return false;
    }

    // Verificar si ya estÃ¡ activa
    if (GetActiveQuestState(QuestID) != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” Quest '%s' ya estÃ¡ activa"), *QuestID.ToString());
        return false;
    }

    // Verificar si ya fue completada (y no es repetible)
    if (CompletedQuestIDs.Contains(QuestID) && !QuestData->bIsRepeatable)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” Quest '%s' ya completada y no es repetible"), *QuestID.ToString());
        return false;
    }

    // Verificar nivel requerido
    const int32 PlayerLevel = GetOwnerLevel();
    if (PlayerLevel < QuestData->RequiredLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” Nivel insuficiente. Requerido: %d, Actual: %d"),
            QuestData->RequiredLevel, PlayerLevel);
        return false;
    }

    // Verificar prerequisitos
    if (!ArePrerequisitesMet(QuestData))
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AcceptQuest â€” Prerequisitos no cumplidos para quest '%s'"), *QuestID.ToString());
        return false;
    }

    // Construir el FQuestState inicial
    FQuestState NewQuestState;
    NewQuestState.QuestData  = QuestData;
    NewQuestState.Status     = EQuestStatus::InProgress;
    NewQuestState.Objectives = QuestData->Objectives;   // copia mutable
    NewQuestState.GiverNPCID = GiverNPCID;

    // Inicializar contadores a 0
    for (FQuestObjective& Objective : NewQuestState.Objectives)
    {
        Objective.CurrentCount = 0;
        Objective.bIsCompleted = false;
    }

    ActiveQuests.Add(NewQuestState);

    // Iniciar timer de tiempo lÃ­mite si aplica
    if (QuestData->TimeLimit > 0.f)
    {
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("FailQuest"), QuestID);

        UWorld* World = GetWorld();
        if (World)
        {
            World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, QuestData->TimeLimit, false);
            QuestTimerHandles.Add(QuestID, TimerHandle);
        }
    }

    OnQuestAccepted.Broadcast(NewQuestState);

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::AcceptQuest â€” Quest '%s' aceptada correctamente"), *QuestID.ToString());
    return true;
}

bool USOLOQuestComponent::CompleteQuest(FName QuestID)
{
    FQuestState* QuestState = GetActiveQuestState(QuestID);
    if (!QuestState)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::CompleteQuest â€” Quest '%s' no estÃ¡ activa"), *QuestID.ToString());
        return false;
    }

    // Verificar que todos los objetivos estÃ©n completos
    for (const FQuestObjective& Objective : QuestState->Objectives)
    {
        if (!Objective.IsComplete())
        {
            UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::CompleteQuest â€” Objetivo '%s' no completado en quest '%s'"),
                *Objective.ObjectiveID.ToString(), *QuestID.ToString());
            return false;
        }
    }

    // Guardar la recompensa antes de modificar el estado
    USOLOQuestDataAsset* QuestData = QuestState->QuestData.Get();
    FQuestReward Reward;
    if (QuestData)
    {
        Reward = QuestData->Reward;
    }

    // Cancelar timer de tiempo lÃ­mite si existe
    FTimerHandle* TimerHandle = QuestTimerHandles.Find(QuestID);
    UWorld* World = GetWorld();
    if (TimerHandle && World)
    {
        World->GetTimerManager().ClearTimer(*TimerHandle);
        QuestTimerHandles.Remove(QuestID);
    }

    // Otorgar recompensas (solo en servidor)
    if (GetOwnerRole() == ROLE_Authority)
    {
        GrantExperienceReward(Reward.ExperiencePoints);

        if (Reward.Currency > 0)
        {
            ASOLOPlayerState* OwnerPlayerState = Cast<ASOLOPlayerState>(GetOwner());
            if (OwnerPlayerState)
            {
                USOLOInventoryComponent* InventoryComp = OwnerPlayerState->FindComponentByClass<USOLOInventoryComponent>();
                if (InventoryComp)
                {
                    InventoryComp->AddCurrency(Reward.Currency);
                }
            }
        }

        GrantItemRewards(Reward);
    }

    // Mover a completadas y remover de activas
    QuestState->Status = EQuestStatus::Completed;
    CompletedQuestIDs.AddUnique(QuestID);

    ActiveQuests.RemoveAll([&QuestID](const FQuestState& State)
    {
        USOLOQuestDataAsset* Data = State.QuestData.Get();
        return Data && Data->QuestID == QuestID;
    });

    OnQuestCompleted.Broadcast(QuestID, Reward);

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::CompleteQuest â€” Quest '%s' completada"), *QuestID.ToString());
    return true;
}

bool USOLOQuestComponent::AbandonQuest(FName QuestID)
{
    FQuestState* QuestState = GetActiveQuestState(QuestID);
    if (!QuestState)
    {
        UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::AbandonQuest â€” Quest '%s' no estÃ¡ activa"), *QuestID.ToString());
        return false;
    }

    // Cancelar timer de tiempo lÃ­mite si existe
    FTimerHandle* TimerHandle = QuestTimerHandles.Find(QuestID);
    UWorld* World = GetWorld();
    if (TimerHandle && World)
    {
        World->GetTimerManager().ClearTimer(*TimerHandle);
        QuestTimerHandles.Remove(QuestID);
    }

    // Remover de activas (no va a failed, simplemente se abandona)
    ActiveQuests.RemoveAll([&QuestID](const FQuestState& State)
    {
        USOLOQuestDataAsset* Data = State.QuestData.Get();
        return Data && Data->QuestID == QuestID;
    });

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::AbandonQuest â€” Quest '%s' abandonada"), *QuestID.ToString());
    return true;
}

void USOLOQuestComponent::FailQuest(FName QuestID)
{
    FQuestState* QuestState = GetActiveQuestState(QuestID);
    if (!QuestState)
    {
        return;
    }

    QuestState->Status = EQuestStatus::Failed;
    FailedQuestIDs.AddUnique(QuestID);

    // Remover timer si aÃºn estÃ¡ activo
    FTimerHandle* TimerHandle = QuestTimerHandles.Find(QuestID);
    UWorld* World = GetWorld();
    if (TimerHandle && World)
    {
        World->GetTimerManager().ClearTimer(*TimerHandle);
        QuestTimerHandles.Remove(QuestID);
    }

    ActiveQuests.RemoveAll([&QuestID](const FQuestState& State)
    {
        USOLOQuestDataAsset* Data = State.QuestData.Get();
        return Data && Data->QuestID == QuestID;
    });

    OnQuestFailed.Broadcast(QuestID);

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::FailQuest â€” Quest '%s' fallida"), *QuestID.ToString());
}

// â”€â”€â”€ NOTIFICACIONES DE PROGRESO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOQuestComponent::NotifyKill(AActor* KilledActor, FGameplayTagContainer ActorTags)
{
    if (!KilledActor)
    {
        return;
    }

    for (FQuestState& QuestState : ActiveQuests)
    {
        if (QuestState.Status != EQuestStatus::InProgress)
        {
            continue;
        }

        for (int32 i = 0; i < QuestState.Objectives.Num(); ++i)
        {
            FQuestObjective& Objective = QuestState.Objectives[i];
            if (Objective.bIsCompleted)
            {
                continue;
            }

            if (Objective.ObjectiveType != EQuestObjectiveType::KillEnemies)
            {
                continue;
            }

            // Si el objetivo tiene un tag, verificar que el actor lo tenga
            // Si el tag estÃ¡ vacÃ­o, cualquier kill cuenta
            if (!Objective.TargetTag.IsValid() || ActorTags.HasTag(Objective.TargetTag))
            {
                IncrementObjective(QuestState, i, 1);
            }
        }
    }
}

void USOLOQuestComponent::NotifyItemCollected(FName ItemID, int32 Quantity)
{
    if (ItemID.IsNone() || Quantity <= 0)
    {
        return;
    }

    for (FQuestState& QuestState : ActiveQuests)
    {
        if (QuestState.Status != EQuestStatus::InProgress)
        {
            continue;
        }

        for (int32 i = 0; i < QuestState.Objectives.Num(); ++i)
        {
            FQuestObjective& Objective = QuestState.Objectives[i];
            if (Objective.bIsCompleted)
            {
                continue;
            }

            if (Objective.ObjectiveType == EQuestObjectiveType::CollectItems
                && Objective.TargetItemID == ItemID)
            {
                IncrementObjective(QuestState, i, Quantity);
            }
        }
    }
}

void USOLOQuestComponent::NotifyLocationReached(FName LocationID)
{
    if (LocationID.IsNone())
    {
        return;
    }

    for (FQuestState& QuestState : ActiveQuests)
    {
        if (QuestState.Status != EQuestStatus::InProgress)
        {
            continue;
        }

        for (int32 i = 0; i < QuestState.Objectives.Num(); ++i)
        {
            FQuestObjective& Objective = QuestState.Objectives[i];
            if (Objective.bIsCompleted)
            {
                continue;
            }

            // TargetNPCID se reutiliza como LocationID para ReachLocation
            if (Objective.ObjectiveType == EQuestObjectiveType::ReachLocation
                && Objective.TargetNPCID == LocationID)
            {
                IncrementObjective(QuestState, i, 1);
            }
        }
    }
}

void USOLOQuestComponent::NotifyNPCTalked(FName NPCID)
{
    if (NPCID.IsNone())
    {
        return;
    }

    for (FQuestState& QuestState : ActiveQuests)
    {
        if (QuestState.Status != EQuestStatus::InProgress)
        {
            continue;
        }

        for (int32 i = 0; i < QuestState.Objectives.Num(); ++i)
        {
            FQuestObjective& Objective = QuestState.Objectives[i];
            if (Objective.bIsCompleted)
            {
                continue;
            }

            const bool bIsTalkObjective   = Objective.ObjectiveType == EQuestObjectiveType::TalkToNPC;
            const bool bIsEscortObjective = Objective.ObjectiveType == EQuestObjectiveType::EscortNPC;

            if ((bIsTalkObjective || bIsEscortObjective) && Objective.TargetNPCID == NPCID)
            {
                IncrementObjective(QuestState, i, 1);
            }
        }
    }
}

void USOLOQuestComponent::NotifyAbilityUsed(FGameplayTag AbilityTag)
{
    if (!AbilityTag.IsValid())
    {
        return;
    }

    for (FQuestState& QuestState : ActiveQuests)
    {
        if (QuestState.Status != EQuestStatus::InProgress)
        {
            continue;
        }

        for (int32 i = 0; i < QuestState.Objectives.Num(); ++i)
        {
            FQuestObjective& Objective = QuestState.Objectives[i];
            if (Objective.bIsCompleted)
            {
                continue;
            }

            if (Objective.ObjectiveType == EQuestObjectiveType::UseAbility
                && Objective.TargetTag == AbilityTag)
            {
                IncrementObjective(QuestState, i, 1);
            }
        }
    }
}

// â”€â”€â”€ CONSULTAS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

EQuestStatus USOLOQuestComponent::GetQuestStatus(FName QuestID) const
{
    const FQuestState* QuestState = GetActiveQuestStateConst(QuestID);
    if (QuestState)
    {
        return QuestState->Status;
    }

    if (CompletedQuestIDs.Contains(QuestID))
    {
        return EQuestStatus::Completed;
    }

    if (FailedQuestIDs.Contains(QuestID))
    {
        return EQuestStatus::Failed;
    }

    return EQuestStatus::NotStarted;
}

FQuestState* USOLOQuestComponent::GetActiveQuestState(FName QuestID)
{
    for (FQuestState& State : ActiveQuests)
    {
        USOLOQuestDataAsset* Data = State.QuestData.Get();
        if (Data && Data->QuestID == QuestID)
        {
            return &State;
        }
    }
    return nullptr;
}

const FQuestState* USOLOQuestComponent::GetActiveQuestStateConst(FName QuestID) const
{
    for (const FQuestState& State : ActiveQuests)
    {
        const USOLOQuestDataAsset* Data = State.QuestData.Get();
        if (Data && Data->QuestID == QuestID)
        {
            return &State;
        }
    }
    return nullptr;
}

bool USOLOQuestComponent::HasCompletedQuest(FName QuestID) const
{
    return CompletedQuestIDs.Contains(QuestID);
}

TArray<FQuestState> USOLOQuestComponent::GetActiveMainQuests() const
{
    TArray<FQuestState> MainQuests;
    for (const FQuestState& State : ActiveQuests)
    {
        const USOLOQuestDataAsset* Data = State.QuestData.Get();
        if (Data && Data->bIsMainQuest)
        {
            MainQuests.Add(State);
        }
    }
    return MainQuests;
}

TArray<FQuestState> USOLOQuestComponent::GetActiveSideQuests() const
{
    TArray<FQuestState> SideQuests;
    for (const FQuestState& State : ActiveQuests)
    {
        const USOLOQuestDataAsset* Data = State.QuestData.Get();
        if (Data && !Data->bIsMainQuest)
        {
            SideQuests.Add(State);
        }
    }
    return SideQuests;
}

// â”€â”€â”€ OnRep CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOQuestComponent::OnRep_ActiveQuests()
{
    // La UI se actualiza vÃ­a delegates. AquÃ­ podemos aÃ±adir lÃ³gica de cliente si es necesario.
}

void USOLOQuestComponent::OnRep_CompletedQuestIDs()
{
    // NotificaciÃ³n al cliente de que el set de completadas cambiÃ³.
}

void USOLOQuestComponent::OnRep_FailedQuestIDs()
{
    // NotificaciÃ³n al cliente de que el set de fallidas cambiÃ³.
}

// â”€â”€â”€ HELPERS INTERNOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool USOLOQuestComponent::ArePrerequisitesMet(const USOLOQuestDataAsset* QuestData) const
{
    if (!QuestData)
    {
        return false;
    }

    for (const FName& PrereqID : QuestData->PrerequisiteQuestIDs)
    {
        if (!CompletedQuestIDs.Contains(PrereqID))
        {
            return false;
        }
    }

    return true;
}

void USOLOQuestComponent::IncrementObjective(FQuestState& QuestState, int32 ObjectiveIndex, int32 Amount)
{
    if (!QuestState.Objectives.IsValidIndex(ObjectiveIndex))
    {
        return;
    }

    FQuestObjective& Objective = QuestState.Objectives[ObjectiveIndex];
    if (Objective.bIsCompleted)
    {
        return;
    }

    Objective.CurrentCount = FMath::Min(Objective.CurrentCount + Amount, Objective.RequiredCount);

    // Obtener el QuestID para los delegates
    USOLOQuestDataAsset* QuestData = QuestState.QuestData.Get();
    const FName QuestID = QuestData ? QuestData->QuestID : NAME_None;

    OnQuestObjectiveUpdated.Broadcast(QuestID, Objective.ObjectiveID, Objective.CurrentCount);

    if (APlayerState* OwnerPS = Cast<APlayerState>(GetOwner()))
    {
        if (APlayerController* PC = OwnerPS->GetPlayerController())
        {
            const FText QuestTitle = QuestData ? QuestData->QuestName : FText::GetEmpty();
            const FText ObjLine = FText::Format(
                NSLOCTEXT("SOLOQuest", "ObjProgressShort", "{0} ({1}/{2})"),
                Objective.Description,
                FText::AsNumber(Objective.CurrentCount),
                FText::AsNumber(Objective.RequiredCount));

            FSOLONotification N;
            N.Type = ENotificationType::QuestUpdate;
            N.Duration = 4.f;
            N.Message = FText::Format(
                NSLOCTEXT("SOLOQuest", "QuestNotifFmt", "MisiÃ³n: {0} â€” {1}"),
                QuestTitle,
                ObjLine);
            USOLONotificationSystem::NotifyOwningClient(PC, N);
        }
    }

    if (Objective.IsComplete())
    {
        Objective.bIsCompleted = true;
        CheckQuestCompletion(QuestState);
    }
}

void USOLOQuestComponent::CheckQuestCompletion(FQuestState& QuestState)
{
    for (const FQuestObjective& Objective : QuestState.Objectives)
    {
        if (!Objective.IsComplete())
        {
            return;
        }
    }

    // Todos los objetivos completos
    QuestState.Status = EQuestStatus::ReadyToComplete;

    USOLOQuestDataAsset* QuestData = QuestState.QuestData.Get();
    const FName QuestID = QuestData ? QuestData->QuestID : NAME_None;

    OnQuestReadyToComplete.Broadcast(QuestID);

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::CheckQuestCompletion â€” Quest '%s' lista para entregar"), *QuestID.ToString());
}

void USOLOQuestComponent::GrantExperienceReward(int32 ExperiencePoints)
{
    if (ExperiencePoints <= 0)
    {
        return;
    }

    ASOLOPlayerState* OwnerPlayerState = Cast<ASOLOPlayerState>(GetOwner());
    if (!OwnerPlayerState)
    {
        return;
    }

    USOLOAttributeSet* AttributeSet = OwnerPlayerState->GetAttributeSet();
    if (!AttributeSet)
    {
        return;
    }

    // Incrementar la experiencia directamente en el AttributeSet.
    // El AttributeSet::PostGameplayEffectExecute maneja el level-up al detectar
    // que Experience supera ExperienceToNextLevel.
    const float CurrentXP  = AttributeSet->GetExperience();
    const float NewXP      = CurrentXP + static_cast<float>(ExperiencePoints);
    AttributeSet->SetExperience(NewXP);
    AttributeSet->FlushExperienceLevelUps();

    UE_LOG(LogTemp, Log, TEXT("USOLOQuestComponent::GrantExperienceReward â€” Otorgados %d XP (Total: %.0f)"),
        ExperiencePoints, NewXP);
}

void USOLOQuestComponent::GrantItemRewards(const FQuestReward& Reward)
{
    if (Reward.RewardItems.Num() == 0)
    {
        return;
    }

    ASOLOPlayerState* OwnerPlayerState = Cast<ASOLOPlayerState>(GetOwner());
    if (!OwnerPlayerState)
    {
        return;
    }

    USOLOInventoryComponent* InventoryComp = OwnerPlayerState->FindComponentByClass<USOLOInventoryComponent>();
    if (!InventoryComp)
    {
        return;
    }

    for (int32 i = 0; i < Reward.RewardItems.Num(); ++i)
    {
        // Carga sÃ­ncrona â€” solo se ejecuta en servidor durante la entrega de quest
        USOLOItemBase* ItemBase = Reward.RewardItems[i].LoadSynchronous();
        if (!ItemBase)
        {
            UE_LOG(LogTemp, Warning, TEXT("USOLOQuestComponent::GrantItemRewards â€” No se pudo cargar item de recompensa en Ã­ndice %d"), i);
            continue;
        }

        const int32 Quantity = Reward.RewardItemQuantities.IsValidIndex(i) ? Reward.RewardItemQuantities[i] : 1;
        InventoryComp->AddItem(ItemBase, FMath::Max(1, Quantity));
    }
}

int32 USOLOQuestComponent::GetOwnerLevel() const
{
    const ASOLOPlayerState* OwnerPlayerState = Cast<ASOLOPlayerState>(GetOwner());
    if (!OwnerPlayerState)
    {
        return 1;
    }

    const USOLOAttributeSet* AttributeSet = OwnerPlayerState->GetAttributeSet();
    if (!AttributeSet)
    {
        return 1;
    }

    return FMath::Max(1, static_cast<int32>(AttributeSet->GetLevel()));
}
