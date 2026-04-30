#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLODialogueWidget.generated.h"

USTRUCT(BlueprintType)
struct FSOLODialogueLine
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) FText SpeakerName;
	UPROPERTY(BlueprintReadWrite) FText Line;
	UPROPERTY(BlueprintReadWrite) UTexture2D* Portrait = nullptr;
};

USTRUCT(BlueprintType)
struct FSOLODialogueChoice
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) FText ChoiceText;
	UPROPERTY(BlueprintReadWrite) FName NextNodeID;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueChoiceSelected, FName, NextNodeID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueFinished);

UCLASS()
class SOLOCORE_API USOLODialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void ShowLine(const FSOLODialogueLine& Line);
	UFUNCTION(BlueprintCallable) void ShowChoices(const TArray<FSOLODialogueChoice>& Choices);
	UFUNCTION(BlueprintCallable) void ClearChoices();
	UFUNCTION(BlueprintCallable) void Advance();

	UPROPERTY(BlueprintAssignable) FOnDialogueChoiceSelected OnChoiceSelected;
	UPROPERTY(BlueprintAssignable) FOnDialogueFinished OnDialogueFinished;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* SpeakerText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* DialogueText;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UImage* PortraitImage;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UVerticalBox* ChoiceBox;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> ChoiceButtonClass;

private:
	bool bIsTyping = false;
	FTimerHandle TypewriterTimer;
	FString FullLine;
	int32 TypeIndex = 0;
};
