#include "UI/SOLOSkillTreeWidget.h"
#include "UI/SOLOSkillNodeWidget.h"
#include "Systems/SOLOClassSubsystem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void USOLOSkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USOLOSkillTreeWidget::PopulateTree(FName ClassID)
{
	if (!SkillCanvas || !NodeWidgetClass) return;
	CurrentClassID = ClassID;
	SkillCanvas->ClearChildren();
	NodeMap.Empty();

	USOLOClassSubsystem* ClassSub = GetGameInstance()->GetSubsystem<USOLOClassSubsystem>();
	if (!ClassSub) return;

	CurrentSkills = ClassSub->GetSkillsForClass(ClassID);

	for (const FSOLOSkillData& Skill : CurrentSkills)
	{
		USOLOSkillNodeWidget* Node = CreateWidget<USOLOSkillNodeWidget>(this, NodeWidgetClass);
		if (!Node) continue;
		Node->InitNode(Skill);

		UCanvasPanelSlot* Slot = SkillCanvas->AddChildToCanvas(Node);
		if (Slot)
		{
			Slot->SetPosition(FVector2D(Skill.TreePosition.X * 120.f, Skill.TreePosition.Y * 120.f));
			Slot->SetSize(FVector2D(80.f, 80.f));
		}
		NodeMap.Add(Skill.SkillID, Node);
	}
}

void USOLOSkillTreeWidget::OnSkillNodeClicked(FName SkillID)
{
	USOLOClassSubsystem* ClassSub = GetGameInstance()->GetSubsystem<USOLOClassSubsystem>();
	if (!ClassSub) return;

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	FString PlayerID = PC->PlayerState ? PC->PlayerState->GetPlayerName() : TEXT("Player");
	if (ClassSub->UnlockSkill(PlayerID, SkillID))
	{
		if (USOLOSkillNodeWidget** NodePtr = NodeMap.Find(SkillID))
			(*NodePtr)->SetUnlocked(true);
	}
}

void USOLOSkillTreeWidget::DrawConnections()
{
	// Connection lines drawn in Blueprint via OnPaint override using SkillData.ConnectedToIDs
}
