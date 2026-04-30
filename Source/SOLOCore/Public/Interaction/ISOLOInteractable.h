#pragma once

#include "CoreMinimal.h"
#include "Internationalization/Text.h"
#include "UObject/Interface.h"
#include "ISOLOInteractable.generated.h"

class ASOLOPlayerCharacter;

UINTERFACE(MinimalAPI, Blueprintable)
class USOLOInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interfaz genÃ©rica para actores con los que el jugador puede interactuar (E / Interact).
 */
class SOLOCORE_API ISOLOInteractable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(ASOLOPlayerCharacter* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractPrompt() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool CanInteract(ASOLOPlayerCharacter* Interactor) const;
};
