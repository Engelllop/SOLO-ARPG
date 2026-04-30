#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Quest/SOLOQuestTypes.h"
#include "SOLOQuestDataAsset.generated.h"

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
class SOLOCORE_API USOLOQuestDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    USOLOQuestDataAsset();

    // â”€â”€â”€ IDENTIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Identificador Ãºnico de la quest â€” debe ser Ãºnico en todo el proyecto */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Identity")
    FName QuestID;

    /** Nombre localizable de la quest mostrado en la UI */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    FText QuestName;

    /** DescripciÃ³n completa de la quest (contexto narrativo) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity",
        meta = (MultiLine = true))
    FText QuestDescription;

    /** Texto corto mostrado en el tracker de quests activas */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    FText QuestSummary;

    /** Icono de la quest para la UI */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest|Identity")
    TSoftObjectPtr<UTexture2D> QuestIcon;

    // â”€â”€â”€ OBJETIVOS Y RECOMPENSA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

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

    // â”€â”€â”€ REQUISITOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Nivel mÃ­nimo del jugador para poder aceptar esta quest */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements",
        meta = (ClampMin = "1"))
    int32 RequiredLevel = 1;

    /**
     * IDs de quests que deben estar completadas antes de poder aceptar esta.
     * Si el array estÃ¡ vacÃ­o, la quest no tiene prerequisitos.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Requirements")
    TArray<FName> PrerequisiteQuestIDs;

    // â”€â”€â”€ CONFIGURACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Si true, esta quest forma parte de la historia principal */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config")
    bool bIsMainQuest = false;

    /**
     * Si true, el jugador puede volver a aceptar la quest despuÃ©s de completarla.
     * Ãštil para quests de grinding (matar X enemigos por dÃ­a, etc.)
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config")
    bool bIsRepeatable = false;

    /**
     * Tiempo lÃ­mite en segundos para completar la quest desde que se acepta.
     * 0.0 significa sin lÃ­mite de tiempo.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest|Config",
        meta = (ClampMin = "0.0", UIMin = "0.0"))
    float TimeLimit = 0.f;

    // â”€â”€â”€ PRIMARYDATAASSET â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Registra este asset con tipo "Quest" para el AssetManager */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
