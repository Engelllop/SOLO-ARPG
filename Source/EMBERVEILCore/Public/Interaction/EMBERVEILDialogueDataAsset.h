#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interaction/EMBERVEILDialogueTypes.h"
#include "EMBERVEILDialogueDataAsset.generated.h"

/**
 * Contenedor de un árbol de diálogos.
 * Los diseñadores pueden crear instancias de esta clase en el Editor
 * para definir conversaciones ramificadas completas.
 */
UCLASS(BlueprintType)
class EMBERVEILCORE_API UEMBERVEILDialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Nodo por el que comienza siempre este diálogo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName StartingNodeID = FName("Start");

	/** Mapa de todos los nodos de este árbol de diálogo, accesibles por su ID único. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TMap<FName, FEMBERVEILDialogueNode> Nodes;

	/**
	 * Busca y devuelve un nodo de diálogo por su ID.
	 * Devuelve un puntero constante o nullptr si no existe.
	 */
	const FEMBERVEILDialogueNode* GetNode(FName NodeID) const;
};
