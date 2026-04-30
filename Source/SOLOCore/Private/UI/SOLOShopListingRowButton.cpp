#include "UI/SOLOShopListingRowButton.h"
#include "UI/SOLOShopWidget.h"

void USOLOShopListingRowButton::InitializeRow(USOLOShopWidget* InShop, int32 InListingIndex)
{
	OwnerShop = InShop;
	ListingIndex = InListingIndex;

	OnClicked.RemoveAll(this);
	OnClicked.AddDynamic(this, &USOLOShopListingRowButton::HandleClicked);
}

void USOLOShopListingRowButton::HandleClicked()
{
	if (OwnerShop)
	{
		OwnerShop->OnListingEntryClicked(ListingIndex);
	}
}
