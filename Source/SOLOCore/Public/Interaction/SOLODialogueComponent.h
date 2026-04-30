#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/SOLODialogueDataAsset.h"
#include "SOLODialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStartedSignature, USOLODialogueDataAsset*, DialogueAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChangedSignature, const FSOLODialogueNode&, NewNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEndedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEventTriggeredSignature, FName, EventName);

/**
 * Componente responsable de manejar el estado de una conversaciÃ³n activa.
 * Idealmente se le agrega al PlayerController o al PlayerCharacter.
 */
UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class SOLOCORE_API USOLODialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USOLODialogueComponent();

	/** Inicia una conversaciÃ³n usando un DataAsset de diÃ¡logo. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(USOLODialogueDataAsset* NewDialogueAsset);

	/** Selecciona una respuesta del nodo actual usando su Ã­ndice (0 a N). */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectResponse(int32 ResponseIndex);

	/** Avanza al siguiente nodo si no hay opciones de respuesta, o termina el diÃ¡logo. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ContinueDialogue();

	/** Fuerza el cierre de la conversaciÃ³n actual. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	/** Indica si hay una conversaciÃ³n en progreso. */
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
	TObjectPtr<USOLODialogueDataAsset> ActiveDialogue;

	FName CurrentNodeID;

	void GoToNode(FName NodeID);
};
