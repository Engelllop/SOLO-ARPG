#include "UI/SOLOSkillNodeWidget.h"
#include "UI/SOLOSkillTreeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USOLOSkillNodeWidget::InitNode(const FSOLOSkillData& InSkillData)
{
	SkillData = InSkillData;
	if (SkillNameText) SkillNameText->SetText(FText::FromName(InSkillData.SkillID));
	if (NodeButton) NodeButton->OnClicked.AddDynamic(this, &USOLOSkillNodeWidget::OnNodeClicked);
}

void USOLOSkillNodeWidget::SetUnlocked(bool bUnlocked)
{
	bIsUnlocked = bUnlocked;
	// Visual state driven in BP via IsUnlocked binding
}

void USOLOSkillNodeWidget::OnNodeClicked()
{
	if (USOLOSkillTreeWidget* Tree = Cast<USOLOSkillTreeWidget>(GetParent()))
		Tree->OnSkillNodeClicked(SkillData.SkillID);
}
