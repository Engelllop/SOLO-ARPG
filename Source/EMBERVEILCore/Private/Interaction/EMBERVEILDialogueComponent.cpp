#include "Interaction/EMBERVEILDialogueComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"

UEMBERVEILDialogueComponent::UEMBERVEILDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActiveDialogue = nullptr;
}

void UEMBERVEILDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEMBERVEILDialogueComponent::StartDialogue(UEMBERVEILDialogueDataAsset* NewDialogueAsset)
{
	if (!NewDialogueAsset) return;

	ActiveDialogue = NewDialogueAsset;
	OnDialogueStarted.Broadcast(ActiveDialogue);

	GoToNode(ActiveDialogue->StartingNodeID);
}

void UEMBERVEILDialogueComponent::GoToNode(FName NodeID)
{
	if (!ActiveDialogue) return;

	if (NodeID == NAME_None)
	{
		EndDialogue();
		return;
	}

	const FEMBERVEILDialogueNode* Node = ActiveDialogue->GetNode(NodeID);
	if (Node)
	{
		CurrentNodeID = NodeID;

		if (Node->EventToTrigger != NAME_None)
		{
			OnDialogueEventTriggered.Broadcast(Node->EventToTrigger);
		}

		OnDialogueNodeChanged.Broadcast(*Node);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue Node %s not found in asset %s"), *NodeID.ToString(), *ActiveDialogue->GetName());
		EndDialogue();
	}
}

void UEMBERVEILDialogueComponent::SelectResponse(int32 ResponseIndex)
{
	if (!ActiveDialogue) return;

	const FEMBERVEILDialogueNode* Node = ActiveDialogue->GetNode(CurrentNodeID);
	if (!Node || !Node->Responses.IsValidIndex(ResponseIndex)) return;

	const FEMBERVEILDialogueResponse& Response = Node->Responses[ResponseIndex];

	// Validar RequiredTags contra el ASC del owner (jugador)
	if (!Response.RequiredTags.IsEmpty())
	{
		UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		if (!ASC || !ASC->HasAllMatchingGameplayTags(Response.RequiredTags))
		{
			return;
		}
	}

	GoToNode(Response.NextNodeID);
}

void UEMBERVEILDialogueComponent::ContinueDialogue()
{
	if (!ActiveDialogue) return;

	const FEMBERVEILDialogueNode* Node = ActiveDialogue->GetNode(CurrentNodeID);
	if (!Node) return;

	// Si no hay respuestas, asumimos que "Continue" lleva al final (NAME_None)
	// Podríamos expandir para tener un DefaultNextNodeID
	if (Node->Responses.Num() == 0)
	{
		EndDialogue();
	}
	else
	{
		// Si hay respuestas, "Continue" no debería hacer nada hasta que elija una
	}
}

void UEMBERVEILDialogueComponent::EndDialogue()
{
	if (ActiveDialogue)
	{
		ActiveDialogue = nullptr;
		CurrentNodeID = NAME_None;
		OnDialogueEnded.Broadcast();
	}
}
