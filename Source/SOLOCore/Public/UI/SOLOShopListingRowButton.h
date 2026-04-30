#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SOLOShopListingRowButton.generated.h"

class USOLOShopWidget;

/**
 * BotÃ³n de una fila del listado de tienda: enlaza OnClicked dinÃ¡mico con Ã­ndice de listing.
 */
UCLASS()
class SOLOCORE_API USOLOShopListingRowButton : public UButton
{
	GENERATED_BODY()

public:
	void InitializeRow(USOLOShopWidget* InShop, int32 InListingIndex);

private:
	UPROPERTY()
	TObjectPtr<USOLOShopWidget> OwnerShop;

	int32 ListingIndex = INDEX_NONE;

	UFUNCTION()
	void HandleClicked();
};
