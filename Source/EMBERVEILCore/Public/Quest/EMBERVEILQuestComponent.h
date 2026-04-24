#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Quest/EMBERVEILQuestTypes.h"
#include "EMBERVEILQuestComponent.generated.h"

class UEMBERVEILQuestDataAsset;
class AEMBERVEILPlayerCharacter;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/** Disparado cuando el jugador acepta una nueva quest */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted,
    const FQuestState&, NewQuestState);

/** Disparado cuando el progreso de un objetivo cambia */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnQuestObjectiveUpdated,
    FName, QuestID,
    FName, ObjectiveID,
    int32, NewCount);

/** Disparado cuando todos los objetivos de una quest están completos y puede entregarse */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestReadyToComplete,
    FName, QuestID);

/** Disparado cuando la quest es entregada y completada con éxito */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestCompleted,
    FName, QuestID,
    const FQuestReward&, Reward);

/** Disparado cuando una quest timed out o el jugador fracasa en los requisitos */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed,
    FName, QuestID);

// ─────────────────────────────────────────────────────────────────────────────

/**
 * Componente que gestiona todas las quests del jugador.
 *
 * Debe colocarse en AEMBERVEILPlayerState para que persista entre respawns
 * y sea accesible tanto desde el Character como desde los NPCs.
 *
 * Toda modificación de estado debe ejecutarse en el servidor.
 * Los delegates se usan en el cliente local para actualizar la UI.
 */
UCLASS(ClassGroup = (EMBERVEIL), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILQuestComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEMBERVEILQuestComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ─── DELEGATES ────────────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
    FOnQuestAccepted OnQuestAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
    FOnQuestObjectiveUpdated OnQuestObjectiveUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
    FOnQuestReadyToComplete OnQuestReadyToComplete;

    UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Quest|Delegates")
    FOnQuestFailed OnQuestFailed;

    // ─── CONFIGURACIÓN ────────────────────────────────────────────────────────

    /** Número máximo de quests activas simultáneas */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Config",
        meta = (ClampMin = "1", ClampMax = "100"))
    int32 MaxActiveQuests = 20;

    // ─── DATOS REPLICADOS ─────────────────────────────────────────────────────

    /** Estado runtime de todas las quests activas (máximo MaxActiveQuests) */
    UPROPERTY(BlueprintReadOnly, Category = "Quest|Data",
        ReplicatedUsing = OnRep_ActiveQuests)
    TArray<FQuestState> ActiveQuests;

    /** IDs de todas las quests completadas (TArray; TSet replicado no soportado). */
    UPROPERTY(BlueprintReadOnly, Category = "Quest|Data",
        ReplicatedUsing = OnRep_CompletedQuestIDs)
    TArray<FName> CompletedQuestIDs;

    /** IDs de todas las quests fallidas */
    UPROPERTY(BlueprintReadOnly, Category = "Quest|Data",
        ReplicatedUsing = OnRep_FailedQuestIDs)
    TArray<FName> FailedQuestIDs;

    // ─── GESTIÓN DE QUESTS ────────────────────────────────────────────────────

    /**
     * Intenta aceptar una quest del NPC dado.
     *
     * Devuelve false si:
     * - La quest ya está activa
     * - La quest ya fue completada (y no es repetible)
     * - El nivel del jugador es insuficiente
     * - No se cumplieron los prerequisitos
     * - Se alcanzó el límite de quests activas
     *
     * @param QuestData   DataAsset que define la quest a aceptar
     * @param GiverNPCID  ID del NPC que ofrece la quest
     * @return            true si la quest fue aceptada correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Management",
        meta = (GameplayTagFilter = ""))
    bool AcceptQuest(UEMBERVEILQuestDataAsset* QuestData, FName GiverNPCID);

    /**
     * Completa la quest, otorgando todas las recompensas al jugador.
     *
     * Debe llamarse desde el servidor. Verifica que todos los objetivos
     * estén completos antes de otorgar recompensas.
     *
     * @param QuestID  ID de la quest a completar
     * @return         true si la quest fue completada correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Management")
    bool CompleteQuest(FName QuestID);

    /**
     * Abandona una quest activa sin marcarla como fallida.
     * No devuelve los items usados ni el progreso obtenido.
     *
     * @param QuestID  ID de la quest a abandonar
     * @return         true si la quest fue abandonada
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Management")
    bool AbandonQuest(FName QuestID);

    /**
     * Marca una quest como fallida (por timeout u otras condiciones de fallo).
     * Llama a este método desde el timer de TimeLimit.
     *
     * @param QuestID  ID de la quest que ha fallado
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Management")
    void FailQuest(FName QuestID);

    // ─── NOTIFICACIONES DE PROGRESO ───────────────────────────────────────────

    /**
     * Notifica que el jugador mató a un actor con los tags dados.
     * Actualiza objetivos KillEnemies cuyo TargetTag esté en ActorTags.
     *
     * @param KilledActor  Actor que fue eliminado
     * @param ActorTags    Tags de gameplay del actor eliminado
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Progress")
    void NotifyKill(AActor* KilledActor, FGameplayTagContainer ActorTags);

    /**
     * Notifica que el jugador recolectó un item.
     * Actualiza objetivos CollectItems cuyo TargetItemID coincide.
     *
     * @param ItemID    ID del item recolectado
     * @param Quantity  Cantidad recolectada
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Progress")
    void NotifyItemCollected(FName ItemID, int32 Quantity);

    /**
     * Notifica que el jugador llegó a una ubicación (trigger volume).
     * Actualiza objetivos ReachLocation cuyo TargetNPCID coincide con LocationID.
     *
     * @param LocationID  ID del trigger volume alcanzado
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Progress")
    void NotifyLocationReached(FName LocationID);

    /**
     * Notifica que el jugador habló con un NPC.
     * Actualiza objetivos TalkToNPC y EscortNPC cuyo TargetNPCID coincide.
     *
     * @param NPCID  ID del NPC con el que se habló
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Progress")
    void NotifyNPCTalked(FName NPCID);

    /**
     * Notifica que el jugador usó una ability.
     * Actualiza objetivos UseAbility cuyo TargetTag coincide con AbilityTag.
     *
     * @param AbilityTag  Tag de la ability usada
     */
    UFUNCTION(BlueprintCallable, Category = "Quest|Progress")
    void NotifyAbilityUsed(FGameplayTag AbilityTag);

    // ─── CONSULTAS ────────────────────────────────────────────────────────────

    /**
     * Devuelve el estado actual de una quest.
     * Si la quest no existe en ninguna lista, devuelve NotStarted.
     *
     * @param QuestID  ID de la quest a consultar
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest|Query")
    EQuestStatus GetQuestStatus(FName QuestID) const;

    /**
     * Devuelve un puntero mutable al FQuestState de una quest activa.
     * Devuelve nullptr si la quest no está activa.
     *
     * @param QuestID  ID de la quest a buscar
     */
    FQuestState* GetActiveQuestState(FName QuestID);

    /** Versión const de GetActiveQuestState */
    const FQuestState* GetActiveQuestStateConst(FName QuestID) const;

    /**
     * Devuelve true si la quest con el ID dado fue completada alguna vez.
     *
     * @param QuestID  ID de la quest a verificar
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest|Query")
    bool HasCompletedQuest(FName QuestID) const;

    /**
     * Devuelve la lista de quests principales activas (bIsMainQuest == true).
     * Usado por el HUD para mostrar el tracker principal.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest|Query")
    TArray<FQuestState> GetActiveMainQuests() const;

    /**
     * Devuelve la lista de quests secundarias activas (bIsMainQuest == false).
     * Usado por el journal de quests.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest|Query")
    TArray<FQuestState> GetActiveSideQuests() const;

protected:
    // ─── OnRep CALLBACKS ──────────────────────────────────────────────────────

    UFUNCTION()
    void OnRep_ActiveQuests();

    UFUNCTION()
    void OnRep_CompletedQuestIDs();

    UFUNCTION()
    void OnRep_FailedQuestIDs();

private:
    // ─── HELPERS INTERNOS ─────────────────────────────────────────────────────

    /**
     * Verifica si todos los prerequisitos de una quest están satisfechos.
     * Comprueba que cada PrerequisiteQuestID esté en CompletedQuestIDs.
     */
    bool ArePrerequisitesMet(const UEMBERVEILQuestDataAsset* QuestData) const;

    /**
     * Incrementa el CurrentCount de un objetivo y lo marca como completado
     * si alcanza RequiredCount. Luego verifica si la quest entera está lista.
     *
     * @param QuestState     Estado de la quest a modificar
     * @param ObjectiveIndex Índice del objetivo dentro de QuestState.Objectives
     * @param Amount         Cantidad a incrementar (default 1)
     */
    void IncrementObjective(FQuestState& QuestState, int32 ObjectiveIndex, int32 Amount = 1);

    /**
     * Verifica si todos los objetivos de un FQuestState están completos.
     * Si lo están, transiciona el Status a ReadyToComplete y dispara el delegate.
     */
    void CheckQuestCompletion(FQuestState& QuestState);

    /**
     * Otorga XP al jugador via modificación directa del AttributeSet.
     * Se ejecuta solo en servidor.
     */
    void GrantExperienceReward(int32 ExperiencePoints);

    /**
     * Otorga items de recompensa al inventario del jugador.
     * Carga los SoftObjectPtr de forma síncrona (solo en servidor).
     */
    void GrantItemRewards(const FQuestReward& Reward);

    /**
     * Devuelve el nivel actual del jugador consultando el PlayerState y su AttributeSet.
     * Devuelve 1 si no puede obtener el nivel.
     */
    int32 GetOwnerLevel() const;

    /**
     * Mapa de timers de tiempo límite. Clave = QuestID, Valor = handle del timer.
     * Permite cancelar el timer si el jugador completa o abandona la quest antes.
     */
    TMap<FName, FTimerHandle> QuestTimerHandles;
};
