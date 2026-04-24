#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/EMBERVEILDialogueDataAsset.h"
#include "EMBERVEILDialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStartedSignature, UEMBERVEILDialogueDataAsset*, DialogueAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChangedSignature, const FEMBERVEILDialogueNode&, NewNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEventTriggeredSignature, FName, EventName);

/**
 * Componente responsable de manejar el estado de una conversación activa.
 * Idealmente se le agrega al PlayerController o al PlayerCharacter.
 */
UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class EMBERVEILCORE_API UEMBERVEILDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEMBERVEILDialogueComponent();

	/** Inicia una conversación usando un DataAsset de diálogo. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UEMBERVEILDialogueDataAsset* NewDialogueAsset);

	/** Selecciona una respuesta del nodo actual usando su índice (0 a N). */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectResponse(int32 ResponseIndex);

	/** Avanza al siguiente nodo si no hay opciones de respuesta, o termina el diálogo. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ContinueDialogue();

	/** Fuerza el cierre de la conversación actual. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	/** Indica si hay una conversación en progreso. */
	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsInDialogue() const { return ActiveDialogue != nullptr; }

	// Delegados para notificar a la UI (Blueprints)
	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueStartedSignature OnDialogueStarted;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueNodeChangedSignature OnDialogueNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueEndedSignature OnDialogueEnded;

	/** Broadcast cuando un nodo tiene un EventToTrigger configurado (quests, cutscenes, etc.). */
	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueEventTriggeredSignature OnDialogueEventTriggered;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UEMBERVEILDialogueDataAsset> ActiveDialogue;

	FName CurrentNodeID;

	void GoToNode(FName NodeID);
};
