#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOMenuWidget.generated.h"

UENUM(BlueprintType)
enum class ESOLOMenuTab : uint8 { Inventory, Equipment, Skills, Map, QuestJournal, Options };

UCLASS()
class SOLOCORE_API USOLOMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void OpenTab(ESOLOMenuTab Tab);
	UFUNCTION(BlueprintCallable) void CloseMenu();

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeKeyDown(const FGeometry& Geometry, const FKeyEvent& KeyEvent) override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UWidgetSwitcher* TabSwitcher;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOInventoryGridWidget* InventoryGrid;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOEquipmentScreenWidget* EquipmentScreen;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOSkillTreeWidget* SkillTree;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOMapWidget* MapPanel;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class USOLOQuestJournalWidget* QuestJournal;

	UPROPERTY(BlueprintReadOnly) ESOLOMenuTab ActiveTab = ESOLOMenuTab::Inventory;
};
