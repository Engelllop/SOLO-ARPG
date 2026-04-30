#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SOLOSkillData.h"
#include "SOLOSkillTreeWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOSkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void PopulateTree(FName ClassID);
	UFUNCTION(BlueprintCallable) void OnSkillNodeClicked(FName SkillID);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UCanvasPanel* SkillCanvas;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class USOLOSkillNodeWidget> NodeWidgetClass;

	UPROPERTY(BlueprintReadOnly) FName CurrentClassID;
	UPROPERTY(BlueprintReadOnly) TArray<FSOLOSkillData> CurrentSkills;

private:
	void DrawConnections();
	TMap<FName, USOLOSkillNodeWidget*> NodeMap;
};
