#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_QuestLogWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UWidgetAnimation;

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FQuestLogEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	FText QuestName;

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	bool bIsMainQuest = false;

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	TArray<FText> Objectives;

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	TArray<bool> bObjectivesCompleted;

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	TArray<int32> ObjectiveCurrent;

	UPROPERTY(BlueprintReadWrite, Category = "QuestLog")
	TArray<int32> ObjectiveRequired;
};

/**
 * Quest log widget con hover expandible.
 * Muestra la misión principal colapsada por defecto, expande al hover.
 * Estilo sketchy — bordes tenues, colores cálidos.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UWG_QuestLogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/** Reemplaza todas las entradas del quest log */
	UFUNCTION(BlueprintCallable, Category = "QuestLog")
	void SetQuestEntries(const TArray<FQuestLogEntry>& Entries);

	/** Añade o actualiza una entrada individual */
	UFUNCTION(BlueprintCallable, Category = "QuestLog")
	void UpdateQuestEntry(const FQuestLogEntry& Entry);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> QuestContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HintText; // "[M] ver todas las misiones"

	UPROPERTY(EditAnywhere, Category = "QuestLog|Style")
	FLinearColor MainQuestColor = FLinearColor(0.96f, 0.85f, 0.48f, 0.8f); // #f5d87a

	UPROPERTY(EditAnywhere, Category = "QuestLog|Style")
	FLinearColor SideQuestColor = FLinearColor(0.8f, 0.8f, 0.8f, 0.6f);

	UPROPERTY(EditAnywhere, Category = "QuestLog|Style")
	FLinearColor ObjectiveColor = FLinearColor(0.91f, 0.49f, 0.42f, 0.8f); // #e87c6a

	UPROPERTY(EditAnywhere, Category = "QuestLog|Style")
	FLinearColor CompleteColor = FLinearColor(0.42f, 0.66f, 0.49f, 0.8f); // #6aa87c

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> ExpandAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> CollapseAnimation;

private:
	bool bExpanded = false;

	void RebuildList();
	UUserWidget* CreateQuestRow(const FQuestLogEntry& Entry, int32 Index);
	UUserWidget* CreateObjectiveRow(const FText& Desc, bool bComplete, int32 Current, int32 Required);
	void ShowHint();
};
