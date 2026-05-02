#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SOLOEndingChoiceWidget.generated.h"

UENUM(BlueprintType)
enum class EEndingChoice : uint8
{
    Destroy UMETA(DisplayName = "Destroy the Tower"),
    Control UMETA(DisplayName = "Take Control"),
    Exit    UMETA(DisplayName = "Exit (Secret)")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingChosen, EEndingChoice, Choice);

UCLASS()
class SOLOCORE_API USOLOEndingChoiceWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Ending")
    FOnEndingChosen OnEndingChosen;

    UFUNCTION(BlueprintCallable, Category = "Ending")
    void Show(bool bCanExit);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ending")
    void OnChoiceHighlighted(EEndingChoice Choice);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ending")
    void OnChoiceMade(EEndingChoice Choice);

protected:
    UFUNCTION()
    void OnDestroyClicked();

    UFUNCTION()
    void OnControlClicked();

    UFUNCTION()
    void OnExitClicked();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> DestroyButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ControlButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DestroyDescription;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ControlDescription;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ExitDescription;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundBlur;
};
