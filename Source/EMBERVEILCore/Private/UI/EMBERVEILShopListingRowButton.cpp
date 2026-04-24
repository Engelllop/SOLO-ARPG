#include "UI/EMBERVEILShopListingRowButton.h"
#include "UI/EMBERVEILShopWidget.h"

void UEMBERVEILShopListingRowButton::InitializeRow(UEMBERVEILShopWidget* InShop, int32 InListingIndex)
{
	OwnerShop = InShop;
	ListingIndex = InListingIndex;

	OnClicked.RemoveAll(this);
	OnClicked.AddDynamic(this, &UEMBERVEILShopListingRowButton::HandleClicked);
}

void UEMBERVEILShopListingRowButton::HandleClicked()
{
	if (OwnerShop)
	{
		OwnerShop->OnListingEntryClicked(ListingIndex);
	}
}
