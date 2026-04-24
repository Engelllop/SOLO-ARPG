#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "EMBERVEILShopListingRowButton.generated.h"

class UEMBERVEILShopWidget;

/**
 * Botón de una fila del listado de tienda: enlaza OnClicked dinámico con índice de listing.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILShopListingRowButton : public UButton
{
	GENERATED_BODY()

public:
	void InitializeRow(UEMBERVEILShopWidget* InShop, int32 InListingIndex);

private:
	UPROPERTY()
	TObjectPtr<UEMBERVEILShopWidget> OwnerShop;

	int32 ListingIndex = INDEX_NONE;

	UFUNCTION()
	void HandleClicked();
};
