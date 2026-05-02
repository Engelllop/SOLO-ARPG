#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SOLONameInputWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNameSubmitted, const FString&, GivenName);

UCLASS()
class SOLOCORE_API USOLONameInputWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "NameInput")
    FOnNameSubmitted OnNameSubmitted;

    UFUNCTION(BlueprintCallable, Category = "NameInput")
    void Show(const FText& Prompt);

    UFUNCTION(BlueprintCallable, Category = "NameInput")
    void Hide();

protected:
    UFUNCTION()
    void OnSubmitClicked();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PromptText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> NameInputBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SubmitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SkipButton;
};
