#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILQuestTypes.generated.h"

class UEMBERVEILItemBase;
class UEMBERVEILQuestDataAsset;

// ─── ENUMS ────────────────────────────────────────────────────────────────────

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
    NotStarted       UMETA(DisplayName = "Not Started"),
    InProgress       UMETA(DisplayName = "In Progress"),
    ReadyToComplete  UMETA(DisplayName = "Ready To Complete"),   // todos los objetivos cumplidos, esperando entrega
    Completed        UMETA(DisplayName = "Completed"),
    Failed           UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
    KillEnemies   UMETA(DisplayName = "Kill Enemies"),      // matar N enemigos (por tag o EEnemyType)
    CollectItems  UMETA(DisplayName = "Collect Items"),     // recoger N items (por ItemID)
    ReachLocation UMETA(DisplayName = "Reach Location"),    // llegar a una ubicación (trigger volume)
    TalkToNPC     UMETA(DisplayName = "Talk To NPC"),       // hablar con NPC específico (por FName NPCID)
    UseAbility    UMETA(DisplayName = "Use Ability"),       // usar una ability N veces (por GameplayTag)
    EscortNPC     UMETA(DisplayName = "Escort NPC"),        // acompañar NPC al destino
    SurviveTime   UMETA(DisplayName = "Survive Time")       // sobrevivir X segundos
};

UENUM(BlueprintType)
enum class EQuestGiverState : uint8
{
    NoQuests           UMETA(DisplayName = "No Quests"),
    HasQuestAvailable  UMETA(DisplayName = "Has Quest Available"),
    HasQuestToComplete UMETA(DisplayName = "Has Quest To Complete"),
    AllQuestsDone      UMETA(DisplayName = "All Quests Done")
};

// ─── STRUCTS ──────────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FQuestObjective
{
    GENERATED_BODY()

    /** Identificador único del objetivo dentro de la quest */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
    FName ObjectiveID;

    /** Texto descriptivo mostrado en el tracker de quests */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
    FText Description;

    /** Tipo de objetivo que determina cómo se actualiza el progreso */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective")
    EQuestObjectiveType ObjectiveType = EQuestObjectiveType::KillEnemies;

    /**
     * Tag del objetivo. Usos:
     * - KillEnemies: tag del enemigo a matar (p.ej. "Enemy.Type.Goblin")
     * - UseAbility:  tag de la ability a usar (p.ej. "Ability.Fireball")
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective",
        meta = (EditCondition = "ObjectiveType == EQuestObjectiveType::KillEnemies || ObjectiveType == EQuestObjectiveType::UseAbility"))
    FGameplayTag TargetTag;

    /** Para CollectItems: ID del item a recolectar */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective",
        meta = (EditCondition = "ObjectiveType == EQuestObjectiveType::CollectItems"))
    FName TargetItemID;

    /**
     * Para TalkToNPC / EscortNPC: ID del NPC destino.
     * Para ReachLocation: ID del volumen de trigger destino.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective",
        meta = (EditCondition = "ObjectiveType == EQuestObjectiveType::TalkToNPC || ObjectiveType == EQuestObjectiveType::EscortNPC || ObjectiveType == EQuestObjectiveType::ReachLocation"))
    FName TargetNPCID;

    /** Cantidad requerida para completar el objetivo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective", meta = (ClampMin = "1"))
    int32 RequiredCount = 1;

    /** Progreso actual (actualizado en runtime por QuestComponent) */
    UPROPERTY(BlueprintReadOnly, Category = "Objective")
    int32 CurrentCount = 0;

    /** Indica si este objetivo está marcado como completado */
    UPROPERTY(BlueprintReadOnly, Category = "Objective")
    bool bIsCompleted = false;

    /** Devuelve true si el CurrentCount alcanzó o superó RequiredCount */
    bool IsComplete() const { return CurrentCount >= RequiredCount; }

    /** Devuelve el progreso entre 0.0 y 1.0 */
    float GetProgress() const
    {
        return RequiredCount > 0 ? FMath::Clamp(static_cast<float>(CurrentCount) / static_cast<float>(RequiredCount), 0.f, 1.f) : 0.f;
    }
};

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FQuestReward
{
    GENERATED_BODY()

    /** Puntos de experiencia otorgados al completar la quest */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward", meta = (ClampMin = "0"))
    int32 ExperiencePoints = 0;

    /** Moneda otorgada al completar la quest */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward", meta = (ClampMin = "0"))
    int32 Currency = 0;

    /** Items otorgados como recompensa (referencias suaves para no cargar en memoria siempre) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    TArray<TSoftObjectPtr<UEMBERVEILItemBase>> RewardItems;

    /**
     * Cantidades correspondientes a cada RewardItem.
     * Debe tener la misma longitud que RewardItems.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    TArray<int32> RewardItemQuantities;
};

/** Estado runtime completo de una quest activa en el QuestComponent */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FQuestState
{
    GENERATED_BODY()

    /** Referencia al DataAsset que define esta quest */
    UPROPERTY(BlueprintReadOnly, Category = "QuestState")
    TSoftObjectPtr<UEMBERVEILQuestDataAsset> QuestData;

    /** Estado actual de la quest */
    UPROPERTY(BlueprintReadOnly, Category = "QuestState")
    EQuestStatus Status = EQuestStatus::NotStarted;

    /** Copia mutable de los objetivos con el progreso actual */
    UPROPERTY(BlueprintReadOnly, Category = "QuestState")
    TArray<FQuestObjective> Objectives;

    /** ID del NPC que dio la quest (para saber a quién volver a entregar) */
    UPROPERTY(BlueprintReadOnly, Category = "QuestState")
    FName GiverNPCID;

    /**
     * Devuelve true si todos los objetivos están completos y el status es InProgress.
     * Una quest ReadyToComplete aún no ha sido entregada al NPC.
     */
    bool IsReadyToComplete() const;
};
