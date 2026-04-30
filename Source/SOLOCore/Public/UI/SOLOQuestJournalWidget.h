#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOLOQuestJournalWidget.generated.h"

USTRUCT(BlueprintType)
struct FSOLOQuestEntry
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) FName QuestID;
	UPROPERTY(BlueprintReadWrite) FText QuestTitle;
	UPROPERTY(BlueprintReadWrite) FText Description;
	UPROPERTY(BlueprintReadWrite) bool bCompleted = false;
	UPROPERTY(BlueprintReadWrite) TArray<FText> Objectives;
};

UCLASS()
class SOLOCORE_API USOLOQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void RefreshJournal(const TArray<FSOLOQuestEntry>& Quests);
	UFUNCTION(BlueprintCallable) void SelectQuest(FName QuestID);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UScrollBox* QuestList;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* SelectedQuestTitle;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* SelectedQuestDescription;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> QuestEntryWidgetClass;
};
