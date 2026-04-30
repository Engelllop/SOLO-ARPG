#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/SOLOSkillData.h"
#include "SOLOSkillNodeWidget.generated.h"

UCLASS()
class SOLOCORE_API USOLOSkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitNode(const FSOLOSkillData& InSkillData);
	void SetUnlocked(bool bUnlocked);

	const FSOLOSkillData& GetSkillData() const { return SkillData; }

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UButton* NodeButton;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UImage* NodeIcon;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget)) class UTextBlock* SkillNameText;

	UFUNCTION() void OnNodeClicked();

	UPROPERTY(BlueprintReadOnly) FSOLOSkillData SkillData;
	UPROPERTY(BlueprintReadOnly) bool bIsUnlocked = false;
};
