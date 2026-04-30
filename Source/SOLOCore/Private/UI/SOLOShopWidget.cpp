#include "UI/SOLOShopWidget.h"
#include "UI/SOLOShopListingRowButton.h"
#include "Economy/ASOLOShopNPC.h"
#include "Economy/SOLOMarketplaceComponent.h"
#include "Inventory/SOLOItemBase.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "Player/SOLOPlayerState.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/PlayerController.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "SOLOShopWidget"

void USOLOShopWidget::NativeConstruct()
{
    Super::NativeConstruct();
    BuildDefaultLayoutIfNeeded();
    BindInventoryCurrency();
    EnsureShopDelegates();
    RefreshShopUI();
}

void USOLOShopWidget::NativeDestruct()
{
    UnbindMarketplaceDelegates();
    UnbindInventoryCurrency();
    Super::NativeDestruct();
}

void USOLOShopWidget::RefreshShopUI()
{
    EnsureShopDelegates();
    RefreshCurrencyText();
    RefreshListingsPanel();
    RefreshDetailPanel();
    UpdateBuySellEnabled();
}

void USOLOShopWidget::BuildDefaultLayoutIfNeeded()
{
    if (bBuiltDefaultLayout)
    {
        return;
    }

    UWidgetTree* const Tree = WidgetTree.Get();
    check(Tree);

    RootVertical = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ShopRootVertical"));
    Tree->RootWidget = RootVertical;

    // --- Fila superior: monedas a la derecha ---
    UHorizontalBox* TopRow = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ShopTopRow"));
    USpacer* Spacer = Tree->ConstructWidget<USpacer>(USpacer::StaticClass(), TEXT("ShopTopSpacer"));
    UHorizontalBoxSlot* SpacerSlot = TopRow->AddChildToHorizontalBox(Spacer);
    SpacerSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

    Text_Currency = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopTextCurrency"));
    Text_Currency->SetJustification(ETextJustify::Right);
    UHorizontalBoxSlot* CoinSlot = TopRow->AddChildToHorizontalBox(Text_Currency);
    CoinSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
    CoinSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);

    RootVertical->AddChildToVerticalBox(TopRow);

    // --- Lista + detalle ---
    RowListingsAndDetail = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ShopSplitRow"));

    ScrollBox_Listings = Tree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("ShopScrollListings"));
    UHorizontalBoxSlot* ListSlot = RowListingsAndDetail->AddChildToHorizontalBox(ScrollBox_Listings);
    ListSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

    DetailColumn = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("ShopDetailColumn"));
    Image_ItemIcon = Tree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("ShopItemIcon"));
    USizeBox* IconBox = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("ShopItemIconBox"));
    IconBox->SetWidthOverride(96.f);
    IconBox->SetHeightOverride(96.f);
    IconBox->AddChild(Image_ItemIcon);
    DetailColumn->AddChildToVerticalBox(IconBox);

    Text_ItemName = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopItemName"));
    Text_ItemName->SetAutoWrapText(true);
    DetailColumn->AddChildToVerticalBox(Text_ItemName);

    Text_ItemDescription = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopItemDesc"));
    Text_ItemDescription->SetAutoWrapText(true);
    Text_ItemDescription->SetColorAndOpacity(FSlateColor(FLinearColor(0.75f, 0.75f, 0.75f)));
    DetailColumn->AddChildToVerticalBox(Text_ItemDescription);

    Text_ItemPrice = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopItemPrice"));
    DetailColumn->AddChildToVerticalBox(Text_ItemPrice);

    UHorizontalBoxSlot* DetailSlot = RowListingsAndDetail->AddChildToHorizontalBox(DetailColumn);
    DetailSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

    UVerticalBoxSlot* SplitSlot = RootVertical->AddChildToVerticalBox(RowListingsAndDetail);
    SplitSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

    // --- Botones ---
    UHorizontalBox* ActionRow = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ShopActionRow"));
    Button_Buy = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ShopBtnBuy"));
    {
        UTextBlock* Lbl = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopLblBuy"));
        Lbl->SetText(LOCTEXT("Buy", "Comprar"));
        Button_Buy->AddChild(Lbl);
    }
    Button_Buy->OnClicked.AddDynamic(this, &USOLOShopWidget::OnBuyClicked);
    ActionRow->AddChildToHorizontalBox(Button_Buy);

    Button_Sell = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ShopBtnSell"));
    {
        UTextBlock* Lbl = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopLblSell"));
        Lbl->SetText(LOCTEXT("Sell", "Vender"));
        Button_Sell->AddChild(Lbl);
    }
    Button_Sell->OnClicked.AddDynamic(this, &USOLOShopWidget::OnSellClicked);
    ActionRow->AddChildToHorizontalBox(Button_Sell);

    Button_Close = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ShopBtnClose"));
    {
        UTextBlock* Lbl = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ShopLblClose"));
        Lbl->SetText(LOCTEXT("Close", "Cerrar"));
        Button_Close->AddChild(Lbl);
    }
    Button_Close->OnClicked.AddDynamic(this, &USOLOShopWidget::OnCloseClicked);
    ActionRow->AddChildToHorizontalBox(Button_Close);

    RootVertical->AddChildToVerticalBox(ActionRow);

    bBuiltDefaultLayout = true;
}

void USOLOShopWidget::EnsureShopDelegates()
{
    USOLOMarketplaceComponent* const Current
        = (ShopNPC && ShopNPC->MarketplaceComponent) ? ShopNPC->MarketplaceComponent : nullptr;
    if (BoundMarketplace.Get() == Current)
    {
        return;
    }

    UnbindMarketplaceDelegates();
    BindMarketplaceDelegates();
}

void USOLOShopWidget::BindMarketplaceDelegates()
{
    if (!ShopNPC || !ShopNPC->MarketplaceComponent)
    {
        BoundMarketplace.Reset();
        return;
    }

    USOLOMarketplaceComponent* MP = ShopNPC->MarketplaceComponent;
    BoundMarketplace = MP;
    MP->OnPurchaseCompleted.AddDynamic(this, &USOLOShopWidget::HandlePurchaseCompleted);
    MP->OnSaleCompleted.AddDynamic(this, &USOLOShopWidget::HandleSaleCompleted);
    MP->OnStockChanged.AddDynamic(this, &USOLOShopWidget::HandleStockChanged);
}

void USOLOShopWidget::UnbindMarketplaceDelegates()
{
    if (USOLOMarketplaceComponent* MP = BoundMarketplace.Get())
    {
        MP->OnPurchaseCompleted.RemoveDynamic(this, &USOLOShopWidget::HandlePurchaseCompleted);
        MP->OnSaleCompleted.RemoveDynamic(this, &USOLOShopWidget::HandleSaleCompleted);
        MP->OnStockChanged.RemoveDynamic(this, &USOLOShopWidget::HandleStockChanged);
    }
    BoundMarketplace.Reset();
}

void USOLOShopWidget::BindInventoryCurrency()
{
    UnbindInventoryCurrency();

    APlayerController* PC = GetOwningPlayer();
    if (!PC)
    {
        return;
    }

    ASOLOPlayerState* PS = PC->GetPlayerState<ASOLOPlayerState>();
    if (!PS)
    {
        return;
    }

    USOLOInventoryComponent* Inv = PS->FindComponentByClass<USOLOInventoryComponent>();
    if (!Inv)
    {
        return;
    }

    BoundInventory = Inv;
    Inv->OnCurrencyChanged.AddDynamic(this, &USOLOShopWidget::HandleCurrencyChanged);
}

void USOLOShopWidget::UnbindInventoryCurrency()
{
    if (USOLOInventoryComponent* Inv = BoundInventory.Get())
    {
        Inv->OnCurrencyChanged.RemoveDynamic(this, &USOLOShopWidget::HandleCurrencyChanged);
    }
    BoundInventory.Reset();
}

void USOLOShopWidget::HandleCurrencyChanged(int32 NewAmount)
{
    RefreshCurrencyText();
}

void USOLOShopWidget::HandlePurchaseCompleted(const FTransactionRecord& Record)
{
    RefreshShopUI();
}

void USOLOShopWidget::HandleSaleCompleted(const FTransactionRecord& Record)
{
    RefreshShopUI();
}

void USOLOShopWidget::HandleStockChanged(int32 ListingIndex, int32 NewStock)
{
    RefreshShopUI();
}

void USOLOShopWidget::OnBuyClicked()
{
    if (!ShopNPC || !ShopNPC->MarketplaceComponent)
    {
        return;
    }

    APlayerController* PC = GetOwningPlayer();
    if (!PC || SelectedOriginalListingIndex == INDEX_NONE)
    {
        return;
    }

    ShopNPC->MarketplaceComponent->BuyItem(PC, SelectedOriginalListingIndex, 1);
    RefreshShopUI();
}

void USOLOShopWidget::OnSellClicked()
{
    if (!ShopNPC || !ShopNPC->MarketplaceComponent)
    {
        return;
    }

    APlayerController* PC = GetOwningPlayer();
    if (!PC || SelectedOriginalListingIndex == INDEX_NONE)
    {
        return;
    }

    const TArray<FShopListing>& Listings = ShopNPC->MarketplaceComponent->Listings;
    if (!Listings.IsValidIndex(SelectedOriginalListingIndex))
    {
        return;
    }

    USOLOItemBase* ItemAsset = Listings[SelectedOriginalListingIndex].Item.LoadSynchronous();
    if (!ItemAsset)
    {
        return;
    }

    ShopNPC->MarketplaceComponent->SellItem(PC, ItemAsset->ItemID, 1);
    RefreshShopUI();
}

void USOLOShopWidget::OnCloseClicked()
{
    RemoveFromParent();
}

void USOLOShopWidget::OnListingEntryClicked(int32 OriginalListingIndex)
{
    SelectedOriginalListingIndex = OriginalListingIndex;
    RefreshDetailPanel();
    UpdateBuySellEnabled();
}

void USOLOShopWidget::RefreshListingsPanel()
{
    if (!ScrollBox_Listings)
    {
        return;
    }

    ScrollBox_Listings->ClearChildren();

    if (!ShopNPC || !ShopNPC->MarketplaceComponent)
    {
        return;
    }

    UWidgetTree* const Tree = WidgetTree.Get();
    check(Tree);

    USOLOMarketplaceComponent* MP = ShopNPC->MarketplaceComponent;
    APlayerController* PC = GetOwningPlayer();

    int32 PlayerLevelReal = 1;
    if (PC)
    {
        if (ASOLOPlayerState* PS = PC->GetPlayerState<ASOLOPlayerState>())
        {
            PlayerLevelReal = PS->GetPlayerLevel();
        }
    }

    for (int32 i = 0; i < MP->Listings.Num(); ++i)
    {
        const FShopListing& L = MP->Listings[i];
        if (PlayerLevelReal < L.RequiredLevel)
        {
            continue;
        }
        if (L.CurrentStock == 0)
        {
            continue;
        }

        const FName RowName(*FString::Printf(TEXT("ListingRow_%d"), i));
        const FName TxtName(*FString::Printf(TEXT("ListingTxt_%d"), i));
        USOLOShopListingRowButton* RowBtn =
            Tree->ConstructWidget<USOLOShopListingRowButton>(USOLOShopListingRowButton::StaticClass(), RowName);
        UTextBlock* RowTxt = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TxtName);

        USOLOItemBase* ItemAsset = L.Item.LoadSynchronous();
        const int32 BuyPrice = MP->GetBuyPrice(i);
        const FText Line = ItemAsset
            ? FText::Format(LOCTEXT("ListingFmt", "{0} â€” {1} monedas"), ItemAsset->ItemName,
                  FText::AsNumber(BuyPrice))
            : FText::Format(LOCTEXT("ListingUnknownFmt", "(Sin item) â€” {0} monedas"), FText::AsNumber(BuyPrice));

        RowTxt->SetText(Line);
        RowTxt->SetAutoWrapText(true);
        RowBtn->AddChild(RowTxt);

        RowBtn->InitializeRow(this, i);

        ScrollBox_Listings->AddChild(RowBtn);
    }

    if (SelectedOriginalListingIndex != INDEX_NONE)
    {
        const TArray<FShopListing>& Listings = MP->Listings;
        if (!Listings.IsValidIndex(SelectedOriginalListingIndex)
            || PlayerLevelReal < Listings[SelectedOriginalListingIndex].RequiredLevel
            || Listings[SelectedOriginalListingIndex].CurrentStock == 0)
        {
            SelectedOriginalListingIndex = INDEX_NONE;
        }
    }
}

void USOLOShopWidget::RefreshDetailPanel()
{
    if (!Text_ItemName || !Text_ItemDescription || !Text_ItemPrice || !Image_ItemIcon)
    {
        return;
    }

    if (!ShopNPC || !ShopNPC->MarketplaceComponent || SelectedOriginalListingIndex == INDEX_NONE)
    {
        Text_ItemName->SetText(LOCTEXT("PickItem", "Selecciona un artÃ­culo"));
        Text_ItemDescription->SetText(FText::GetEmpty());
        Text_ItemPrice->SetText(FText::GetEmpty());
        Image_ItemIcon->SetBrush(FSlateBrush());
        Image_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    USOLOMarketplaceComponent* MP = ShopNPC->MarketplaceComponent;
    if (!MP->Listings.IsValidIndex(SelectedOriginalListingIndex))
    {
        return;
    }

    const FShopListing& L = MP->Listings[SelectedOriginalListingIndex];
    USOLOItemBase* ItemAsset = L.Item.LoadSynchronous();
    if (!ItemAsset)
    {
        Text_ItemName->SetText(LOCTEXT("BadItem", "Item no disponible"));
        Text_ItemDescription->SetText(FText::GetEmpty());
        Text_ItemPrice->SetText(FText::GetEmpty());
        Image_ItemIcon->SetBrush(FSlateBrush());
        Image_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
    Image_ItemIcon->SetBrushFromSoftTexture(ItemAsset->Icon);

    Text_ItemName->SetText(ItemAsset->ItemName);
    Text_ItemDescription->SetText(ItemAsset->ItemDescription);

    const int32 Buy = MP->GetBuyPrice(SelectedOriginalListingIndex);
    const int32 Sell = MP->GetSellPrice(ItemAsset->ItemID);
    Text_ItemPrice->SetText(FText::Format(
        LOCTEXT("PriceFmt", "Comprar: {0}  |  Vender: {1}"), FText::AsNumber(Buy), FText::AsNumber(Sell)));
}

void USOLOShopWidget::RefreshCurrencyText()
{
    if (!Text_Currency)
    {
        return;
    }

    int32 Coins = 0;
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (ASOLOPlayerState* PS = PC->GetPlayerState<ASOLOPlayerState>())
        {
            if (USOLOInventoryComponent* Inv = PS->FindComponentByClass<USOLOInventoryComponent>())
            {
                Coins = Inv->Currency;
            }
        }
    }

    Text_Currency->SetText(FText::Format(LOCTEXT("CoinsFmt", "Monedas: {0}"), FText::AsNumber(Coins)));
}

void USOLOShopWidget::UpdateBuySellEnabled()
{
    const bool bHasShop = ShopNPC && ShopNPC->MarketplaceComponent;
    const bool bHasSelection
        = bHasShop && SelectedOriginalListingIndex != INDEX_NONE
        && ShopNPC->MarketplaceComponent->Listings.IsValidIndex(SelectedOriginalListingIndex);

    if (Button_Buy)
    {
        Button_Buy->SetIsEnabled(bHasSelection);
    }
    if (Button_Sell)
    {
        Button_Sell->SetIsEnabled(bHasSelection);
    }
}

#undef LOCTEXT_NAMESPACE
