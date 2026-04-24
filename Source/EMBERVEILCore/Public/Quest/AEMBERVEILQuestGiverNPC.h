#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/IEMBERVEILInteractable.h"
#include "Quest/EMBERVEILQuestTypes.h"
#include "AEMBERVEILQuestGiverNPC.generated.h"

class USphereComponent;
class UWidgetComponent;
class UTexture2D;
class UUserWidget;
class UEMBERVEILQuestDataAsset;
class AEMBERVEILPlayerCharacter;
class UEMBERVEILQuestComponent;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/** Disparado cuando un jugador entra en el radio de interacción del NPC */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredInteractionRange,
    AEMBERVEILPlayerCharacter*, Player);

/** Disparado cuando un jugador sale del radio de interacción del NPC */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeftInteractionRange,
    AEMBERVEILPlayerCharacter*, Player);

// ─────────────────────────────────────────────────────────────────────────────

/**
 * Actor NPC que puede dar quests al jugador.
 *
 * Coloca instancias de esta clase (o Blueprints derivados) en el nivel.
 * Asigna las quests disponibles desde AvailableQuests en el editor.
 *
 * El widget QuestIndicatorWidget encima del NPC muestra:
 * - "!" si tiene una quest disponible no aceptada
 * - "?" si el jugador tiene una quest ReadyToComplete para este NPC
 * - Nada si no hay quests pendientes
 *
 * Para añadir mesh, animaciones y diálogos, crea un Blueprint hijo.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API AEMBERVEILQuestGiverNPC : public AActor, public IEMBERVEILInteractable
{
	GENERATED_BODY()

public:
	AEMBERVEILQuestGiverNPC();

	virtual void BeginPlay() override;

	// ─── IEMBERVEILInteractable ───────────────────────────────────────────────
	virtual void Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const override;

    // ─── DELEGATES ────────────────────────────────────────────────────────────

    /** Notificación cuando un jugador entra en rango de interacción */
    UPROPERTY(BlueprintAssignable, Category = "QuestGiver|Interaction")
    FOnPlayerEnteredInteractionRange OnPlayerEnteredInteractionRange;

    /** Notificación cuando un jugador sale del rango de interacción */
    UPROPERTY(BlueprintAssignable, Category = "QuestGiver|Interaction")
    FOnPlayerLeftInteractionRange OnPlayerLeftInteractionRange;

    // ─── IDENTIDAD ────────────────────────────────────────────────────────────

    /** Identificador único de este NPC en el juego. Usado por el QuestComponent para GiverNPCID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    FName NPCID;

    /** Nombre mostrado en la UI de diálogo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    FText NPCName;

    /** Portrait del NPC para la UI de diálogo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    TSoftObjectPtr<UTexture2D> NPCPortrait;

    // ─── QUESTS ───────────────────────────────────────────────────────────────

    /**
     * Lista de quests que este NPC puede ofrecer.
     * Se ofrecen en orden: la primera disponible que el jugador no haya completado
     * (o que sea repetible) será la que se ofrezca.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Quests")
    TArray<TSoftObjectPtr<UEMBERVEILQuestDataAsset>> AvailableQuests;

    // ─── DIÁLOGO ──────────────────────────────────────────────────────────────

    /**
     * ID de la tabla de diálogo asociada a este NPC.
     * Usado por el futuro sistema de diálogo para cargar las líneas correctas.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Dialogue")
    FName QuestGiverDialogue;

    // ─── INTERACCIÓN ──────────────────────────────────────────────────────────
    // La interfaz IEMBERVEILInteractable expone Interact como BlueprintNativeEvent;
    // no declarar aquí otro `Interact` con el mismo nombre (UHT lo rechaza en UE 5.7).

    /**
     * Evalúa el estado del NPC desde la perspectiva del QuestComponent del jugador.
     *
     * @param PlayerQuestComp  QuestComponent del jugador a evaluar
     * @return                 Estado del NPC (sin quests, tiene disponible, tiene a completar, etc.)
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "QuestGiver|Interaction")
    EQuestGiverState GetQuestGiverState(UEMBERVEILQuestComponent* PlayerQuestComp) const;

    /**
     * Devuelve el primer DataAsset de quest disponible que el jugador puede aceptar.
     * Carga los assets de forma síncrona — llamar solo cuando sea necesario.
     *
     * @param PlayerQuestComp  QuestComponent del jugador
     * @return                 Puntero al DataAsset, o nullptr si no hay quests disponibles
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|Interaction")
    UEMBERVEILQuestDataAsset* GetFirstAvailableQuest(UEMBERVEILQuestComponent* PlayerQuestComp) const;

    /**
     * Actualiza la visibilidad y el estado del widget indicador sobre el NPC.
     * Llamado automáticamente cuando un jugador entra/sale del radio.
     *
     * @param PlayerQuestComp  QuestComponent del jugador cercano (puede ser nullptr)
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|UI")
    void RefreshQuestIndicator(UEMBERVEILQuestComponent* PlayerQuestComp);

    /**
     * Lógica de interacción (quests disponibles / completar / broadcast).
     * Llamado desde Interact_Implementation; nombre distinto de Interact para no colisionar con la interfaz.
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|Interaction")
    void HandlePlayerInteract(AEMBERVEILPlayerCharacter* Player);

protected:
    // ─── COMPONENTES ──────────────────────────────────────────────────────────

    /** Esfera de detección de proximidad para mostrar el indicador de quest */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestGiver|Components")
    TObjectPtr<USphereComponent> InteractionCollision;

    /**
     * Widget flotante encima del NPC que muestra !, ? o nada.
     * Asigna la clase del widget en el Blueprint hijo.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestGiver|Components")
    TObjectPtr<UWidgetComponent> QuestIndicatorWidget;

    // ─── OVERLAP CALLBACKS ────────────────────────────────────────────────────

    UFUNCTION()
    void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

private:
    /**
     * Obtiene el QuestComponent del jugador dado, buscando en su PlayerState.
     * Devuelve nullptr si el PlayerState o el componente no existen.
     */
    UEMBERVEILQuestComponent* GetPlayerQuestComponent(AEMBERVEILPlayerCharacter* Player) const;
};
