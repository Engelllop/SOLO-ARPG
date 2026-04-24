#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Economy/EMBERVEILEconomyTypes.h"
#include "EMBERVEILShopWidget.generated.h"

class UButton;
class UHorizontalBox;
class UImage;
class UScrollBox;
class UTextBlock;
class UVerticalBox;
class AEMBERVEILShopNPC;
class UEMBERVEILInventoryComponent;
class UEMBERVEILMarketplaceComponent;
class UEMBERVEILShopListingRowButton;

/**
 * Panel de tienda: lista de FShopListing, detalle, compra/venta y monedas del jugador.
 * Asignar ShopNPC al abrir (p. ej. desde FOnShopOpened) o vía CreateWidget con ExposeOnSpawn.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILShopWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    /** Tienda activa; puede pasarse en CreateWidget (ExposeOnSpawn) o asignarse en Blueprint. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (ExposeOnSpawn = "true"))
    TObjectPtr<AEMBERVEILShopNPC> ShopNPC;

    /** Vuelve a leer listings, monedas y el panel de detalle. */
    UFUNCTION(BlueprintCallable, Category = "Shop")
    void RefreshShopUI();

private:
    void BuildDefaultLayoutIfNeeded();
    void EnsureShopDelegates();
    void BindMarketplaceDelegates();
    void UnbindMarketplaceDelegates();
    void BindInventoryCurrency();
    void UnbindInventoryCurrency();

    UFUNCTION()
    void HandleCurrencyChanged(int32 NewAmount);

    UFUNCTION()
    void HandlePurchaseCompleted(const FTransactionRecord& Record);

    UFUNCTION()
    void HandleSaleCompleted(const FTransactionRecord& Record);

    UFUNCTION()
    void HandleStockChanged(int32 ListingIndex, int32 NewStock);

    UFUNCTION()
    void OnBuyClicked();

    UFUNCTION()
    void OnSellClicked();

    UFUNCTION()
    void OnCloseClicked();

    friend class UEMBERVEILShopListingRowButton;
    void OnListingEntryClicked(int32 OriginalListingIndex);
    void RefreshListingsPanel();
    void RefreshDetailPanel();
    void RefreshCurrencyText();
    void UpdateBuySellEnabled();

    UPROPERTY(Transient)
    TObjectPtr<UScrollBox> ScrollBox_Listings;

    UPROPERTY(Transient)
    TObjectPtr<UImage> Image_ItemIcon;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> Text_ItemName;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> Text_ItemDescription;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> Text_ItemPrice;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> Text_Currency;

    UPROPERTY(Transient)
    TObjectPtr<UButton> Button_Buy;

    UPROPERTY(Transient)
    TObjectPtr<UButton> Button_Sell;

    UPROPERTY(Transient)
    TObjectPtr<UButton> Button_Close;

    UPROPERTY(Transient)
    TObjectPtr<UVerticalBox> RootVertical;

    UPROPERTY(Transient)
    TObjectPtr<UHorizontalBox> RowListingsAndDetail;

    UPROPERTY(Transient)
    TObjectPtr<UVerticalBox> DetailColumn;

    bool bBuiltDefaultLayout = false;
    int32 SelectedOriginalListingIndex = INDEX_NONE;

    TWeakObjectPtr<UEMBERVEILInventoryComponent> BoundInventory;
    TWeakObjectPtr<UEMBERVEILMarketplaceComponent> BoundMarketplace;
};
