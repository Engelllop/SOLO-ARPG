#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILDialogueTypes.generated.h"

class USoundBase;

/** Opciones de respuesta que puede dar el jugador en un diálogo. */
USTRUCT(BlueprintType)
struct FEMBERVEILDialogueResponse
{
	GENERATED_BODY()

	/** Texto de la respuesta. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText ResponseText;

	/** ID del siguiente nodo al que lleva esta respuesta. Si es 'None', termina la conversación. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName NextNodeID = NAME_None;

	/** Tags requeridos para que esta respuesta sea visible (ej. Quest.Completada, Faction.Friendly). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FGameplayTagContainer RequiredTags;
};

/** Nodo que representa una frase dicha por el NPC. */
USTRUCT(BlueprintType)
struct FEMBERVEILDialogueNode
{
	GENERATED_BODY()

	/** Nombre de quien habla (Opcional, si está vacío se usa el del NPC). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerName;

	/** Texto o subtítulo que dice el NPC. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (MultiLine = "true"))
	FText DialogueText;

	/** Audio opcional para reproducir cuando se muestra este nodo. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<USoundBase> VoiceAudio = nullptr;

	/** Opciones de respuesta para el jugador. Si está vacío, el diálogo se considera terminado tras un clic de continuar. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FEMBERVEILDialogueResponse> Responses;

	/** Evento opcional para gatillar quests o scripts al llegar a este nodo. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName EventToTrigger = NAME_None;
};
