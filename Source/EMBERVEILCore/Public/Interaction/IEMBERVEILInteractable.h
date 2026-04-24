#pragma once

#include "CoreMinimal.h"
#include "Internationalization/Text.h"
#include "UObject/Interface.h"
#include "IEMBERVEILInteractable.generated.h"

class AEMBERVEILPlayerCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class UEMBERVEILInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interfaz genérica para actores con los que el jugador puede interactuar (E / Interact).
 */
class EMBERVEILCORE_API IEMBERVEILInteractable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(AEMBERVEILPlayerCharacter* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractPrompt() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(AEMBERVEILPlayerCharacter* Interactor) const;
};
