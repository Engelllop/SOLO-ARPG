#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/ISOLOInteractable.h"
#include "Quest/SOLOQuestTypes.h"
#include "ASOLOQuestGiverNPC.generated.h"

class USphereComponent;
class UWidgetComponent;
class UTexture2D;
class UUserWidget;
class USOLOQuestDataAsset;
class ASOLOPlayerCharacter;
class USOLOQuestComponent;

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Disparado cuando un jugador entra en el radio de interacciÃ³n del NPC */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredInteractionRange,
    ASOLOPlayerCharacter*, Player);

/** Disparado cuando un jugador sale del radio de interacciÃ³n del NPC */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeftInteractionRange,
    ASOLOPlayerCharacter*, Player);

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

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
 * Para aÃ±adir mesh, animaciones y diÃ¡logos, crea un Blueprint hijo.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLOQuestGiverNPC : public AActor, public ISOLOInteractable
{
	GENERATED_BODY()

public:
	ASOLOQuestGiverNPC();

	virtual void BeginPlay() override;

	// â”€â”€â”€ ISOLOInteractable â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	virtual void Interact_Implementation(ASOLOPlayerCharacter* Interactor) override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual bool CanInteract_Implementation(ASOLOPlayerCharacter* Interactor) const override;

    // â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** NotificaciÃ³n cuando un jugador entra en rango de interacciÃ³n */
    UPROPERTY(BlueprintAssignable, Category = "QuestGiver|Interaction")
    FOnPlayerEnteredInteractionRange OnPlayerEnteredInteractionRange;

    /** NotificaciÃ³n cuando un jugador sale del rango de interacciÃ³n */
    UPROPERTY(BlueprintAssignable, Category = "QuestGiver|Interaction")
    FOnPlayerLeftInteractionRange OnPlayerLeftInteractionRange;

    // â”€â”€â”€ IDENTIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Identificador Ãºnico de este NPC en el juego. Usado por el QuestComponent para GiverNPCID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    FName NPCID;

    /** Nombre mostrado en la UI de diÃ¡logo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    FText NPCName;

    /** Portrait del NPC para la UI de diÃ¡logo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Identity")
    TSoftObjectPtr<UTexture2D> NPCPortrait;

    // â”€â”€â”€ QUESTS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Lista de quests que este NPC puede ofrecer.
     * Se ofrecen en orden: la primera disponible que el jugador no haya completado
     * (o que sea repetible) serÃ¡ la que se ofrezca.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Quests")
    TArray<TSoftObjectPtr<USOLOQuestDataAsset>> AvailableQuests;

    // â”€â”€â”€ DIÃLOGO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * ID de la tabla de diÃ¡logo asociada a este NPC.
     * Usado por el futuro sistema de diÃ¡logo para cargar las lÃ­neas correctas.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver|Dialogue")
    FName QuestGiverDialogue;

    // â”€â”€â”€ INTERACCIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    // La interfaz ISOLOInteractable expone Interact como BlueprintNativeEvent;
    // no declarar aquÃ­ otro `Interact` con el mismo nombre (UHT lo rechaza en UE 5.7).

    /**
     * EvalÃºa el estado del NPC desde la perspectiva del QuestComponent del jugador.
     *
     * @param PlayerQuestComp  QuestComponent del jugador a evaluar
     * @return                 Estado del NPC (sin quests, tiene disponible, tiene a completar, etc.)
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "QuestGiver|Interaction")
    EQuestGiverState GetQuestGiverState(USOLOQuestComponent* PlayerQuestComp) const;

    /**
     * Devuelve el primer DataAsset de quest disponible que el jugador puede aceptar.
     * Carga los assets de forma sÃ­ncrona â€” llamar solo cuando sea necesario.
     *
     * @param PlayerQuestComp  QuestComponent del jugador
     * @return                 Puntero al DataAsset, o nullptr si no hay quests disponibles
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|Interaction")
    USOLOQuestDataAsset* GetFirstAvailableQuest(USOLOQuestComponent* PlayerQuestComp) const;

    /**
     * Actualiza la visibilidad y el estado del widget indicador sobre el NPC.
     * Llamado automÃ¡ticamente cuando un jugador entra/sale del radio.
     *
     * @param PlayerQuestComp  QuestComponent del jugador cercano (puede ser nullptr)
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|UI")
    void RefreshQuestIndicator(USOLOQuestComponent* PlayerQuestComp);

    /**
     * LÃ³gica de interacciÃ³n (quests disponibles / completar / broadcast).
     * Llamado desde Interact_Implementation; nombre distinto de Interact para no colisionar con la interfaz.
     */
    UFUNCTION(BlueprintCallable, Category = "QuestGiver|Interaction")
    void HandlePlayerInteract(ASOLOPlayerCharacter* Player);

protected:
    // â”€â”€â”€ COMPONENTES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Esfera de detecciÃ³n de proximidad para mostrar el indicador de quest */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestGiver|Components")
    TObjectPtr<USphereComponent> InteractionCollision;

    /**
     * Widget flotante encima del NPC que muestra !, ? o nada.
     * Asigna la clase del widget en el Blueprint hijo.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuestGiver|Components")
    TObjectPtr<UWidgetComponent> QuestIndicatorWidget;

    // â”€â”€â”€ OVERLAP CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

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
    USOLOQuestComponent* GetPlayerQuestComponent(ASOLOPlayerCharacter* Player) const;
};
