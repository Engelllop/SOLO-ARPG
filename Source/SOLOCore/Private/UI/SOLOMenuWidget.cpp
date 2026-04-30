#include "UI/SOLOMenuWidget.h"
#include "Components/WidgetSwitcher.h"

void USOLOMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OpenTab(ESOLOMenuTab::Inventory);
}

void USOLOMenuWidget::OpenTab(ESOLOMenuTab Tab)
{
	ActiveTab = Tab;
	if (TabSwitcher) TabSwitcher->SetActiveWidgetIndex(static_cast<int32>(Tab));
}

void USOLOMenuWidget::CloseMenu()
{
	SetVisibility(ESlateVisibility::Collapsed);
	if (APlayerController* PC = GetOwningPlayer())
		PC->SetInputMode(FInputModeGameOnly());
}

FReply USOLOMenuWidget::NativeKeyDown(const FGeometry& Geometry, const FKeyEvent& KeyEvent)
{
	if (KeyEvent.GetKey() == EKeys::Escape)
	{
		CloseMenu();
		return FReply::Handled();
	}
	return Super::NativeKeyDown(Geometry, KeyEvent);
}
