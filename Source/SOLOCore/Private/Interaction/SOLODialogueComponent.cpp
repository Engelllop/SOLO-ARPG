#include "Interaction/SOLODialogueComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"

USOLODialogueComponent::USOLODialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActiveDialogue = nullptr;
}

void USOLODialogueComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USOLODialogueComponent::StartDialogue(USOLODialogueDataAsset* NewDialogueAsset)
{
	if (!NewDialogueAsset) return;

	ActiveDialogue = NewDialogueAsset;
	OnDialogueStarted.Broadcast(ActiveDialogue);

	GoToNode(ActiveDialogue->StartingNodeID);
}

void USOLODialogueComponent::GoToNode(FName NodeID)
{
	if (!ActiveDialogue) return;

	if (NodeID == NAME_None)
	{
		EndDialogue();
		return;
	}

	const FSOLODialogueNode* Node = ActiveDialogue->GetNode(NodeID);
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

void USOLODialogueComponent::SelectResponse(int32 ResponseIndex)
{
	if (!ActiveDialogue) return;

	const FSOLODialogueNode* Node = ActiveDialogue->GetNode(CurrentNodeID);
	if (!Node || !Node->Responses.IsValidIndex(ResponseIndex)) return;

	const FSOLODialogueResponse& Response = Node->Responses[ResponseIndex];

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

void USOLODialogueComponent::ContinueDialogue()
{
	if (!ActiveDialogue) return;

	const FSOLODialogueNode* Node = ActiveDialogue->GetNode(CurrentNodeID);
	if (!Node) return;

	// Si no hay respuestas, asumimos que "Continue" lleva al final (NAME_None)
	// PodrÃ­amos expandir para tener un DefaultNextNodeID
	if (Node->Responses.Num() == 0)
	{
		EndDialogue();
	}
	else
	{
		// Si hay respuestas, "Continue" no deberÃ­a hacer nada hasta que elija una
	}
}

void USOLODialogueComponent::EndDialogue()
{
	if (ActiveDialogue)
	{
		ActiveDialogue = nullptr;
		CurrentNodeID = NAME_None;
		OnDialogueEnded.Broadcast();
	}
}
