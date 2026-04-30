#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOMapWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void UpdatePlayerPosition(FVector2D NormalizedPos);
	UFUNCTION(BlueprintCallable) void AddMapMarker(FName MarkerID, FVector2D NormalizedPos, FLinearColor Color);
	UFUNCTION(BlueprintCallable) void RemoveMapMarker(FName MarkerID);
	UFUNCTION(BlueprintCallable) void SetFloor(int32 FloorIndex);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UImage* MapImage;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UCanvasPanel* MarkerCanvas;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UImage* PlayerIcon;

	UPROPERTY(BlueprintReadOnly) int32 CurrentFloor = 0;
};
