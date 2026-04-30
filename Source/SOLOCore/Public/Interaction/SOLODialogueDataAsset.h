#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interaction/SOLODialogueTypes.h"
#include "SOLODialogueDataAsset.generated.h"

/**
 * Contenedor de un Ã¡rbol de diÃ¡logos.
 * Los diseÃ±adores pueden crear instancias de esta clase en el Editor
 * para definir conversaciones ramificadas completas.
 */
UCLASS(BlueprintType)
class SOLOCORE_API USOLODialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Nodo por el que comienza siempre este diÃ¡logo. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName StartingNodeID = FName("Start");

	/** Mapa de todos los nodos de este Ã¡rbol de diÃ¡logo, accesibles por su ID Ãºnico. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TMap<FName, FSOLODialogueNode> Nodes;

	/**
	 * Busca y devuelve un nodo de diÃ¡logo por su ID.
	 * Devuelve un puntero constante o nullptr si no existe.
	 */
	const FSOLODialogueNode* GetNode(FName NodeID) const;
};
