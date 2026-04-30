#include "UI/SOLOMapWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void USOLOMapWidget::UpdatePlayerPosition(FVector2D NormalizedPos)
{
	if (PlayerIcon)
	{
		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(PlayerIcon->Slot))
		{
			FVector2D MapSize = Slot->GetSize();
			Slot->SetPosition(NormalizedPos * MapSize);
		}
	}
}

void USOLOMapWidget::AddMapMarker(FName MarkerID, FVector2D NormalizedPos, FLinearColor Color)
{
	// Marker widgets created in Blueprint; C++ stores position data
}

void USOLOMapWidget::RemoveMapMarker(FName MarkerID)
{
	// Removal handled in Blueprint
}

void USOLOMapWidget::SetFloor(int32 FloorIndex)
{
	CurrentFloor = FloorIndex;
	// Map texture swap handled in Blueprint
}
