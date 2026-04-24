#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Quest/EMBERVEILQuestTypes.h"
#include "EMBERVEILQuestDataAsset.generated.h"

class UTexture2D;

/**
 * DataAsset que define completamente una quest del juego.
 *
 * Crear una instancia en el Content Browser para cada quest.
 * El AssetManager puede cargarlas por tipo "Quest" usando GetPrimaryAssetId().
 * Las referencias a items de recompensa son suaves (TSoftObjectPtr) para
 * no forzar la carga de todos los assets al inicio del juego.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UEMBERVEILQuestDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UEMBERVEILQuestDataAsset();

    // ─── IDENTIDAD ────────────────────────────────────────────────────────────

    /** Identificador único de la quest — debe ser único en todo el proyecto */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Identity")
    FName QuestID;

    /** Nombre localizable de la quest mostrado en la UI */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    FText QuestName;

    /** Descripción completa de la quest (contexto narrativo) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity",
        meta = (MultiLine = true))
    FText QuestDescription;

    /** Texto corto mostrado en el tracker de quests activas */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    FText QuestSummary;

    /** Icono de la quest para la UI */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    TSoftObjectPtr<UTexture2D> QuestIcon;

    // ─── OBJETIVOS Y RECOMPENSA ───────────────────────────────────────────────

    /** Lista de objetivos que el jugador debe completar */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Objectives")
    TArray<FQuestObjective> Objectives;

    /** Items otorgados como recompensa */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Reward")
    TArray<FPrimaryAssetId> ItemRewards;

    /** XP otorgada como recompensa */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Reward")
    int32 XPReward = 0;

    /** Oro otorgado como recompensa */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Reward")
    int32 GoldReward = 0;

    /** Recompensas otorgadas al completar todos los objetivos y entregar la quest (Obsoleto: usar campos directos) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Reward")
    FQuestReward Reward;

    // ─── REQUISITOS ───────────────────────────────────────────────────────────

    /** Nivel mínimo del jugador para poder aceptar esta quest */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements",
        meta = (ClampMin = "1"))
    int32 RequiredLevel = 1;

    /**
     * IDs de quests que deben estar completadas antes de poder aceptar esta.
     * Si el array está vacío, la quest no tiene prerequisitos.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements")
    TArray<FName> PrerequisiteQuestIDs;

    // ─── CONFIGURACIÓN ────────────────────────────────────────────────────────

    /** Si true, esta quest forma parte de la historia principal */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config")
    bool bIsMainQuest = false;

    /**
     * Si true, el jugador puede volver a aceptar la quest después de completarla.
     * Útil para quests de grinding (matar X enemigos por día, etc.)
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config")
    bool bIsRepeatable = false;

    /**
     * Tiempo límite en segundos para completar la quest desde que se acepta.
     * 0.0 significa sin límite de tiempo.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config",
        meta = (ClampMin = "0.0", UIMin = "0.0"))
    float TimeLimit = 0.f;

    // ─── PRIMARYDATAASSET ─────────────────────────────────────────────────────

    /** Registra este asset con tipo "Quest" para el AssetManager */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
