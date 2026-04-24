#include "Interaction/EMBERVEILDialogueDataAsset.h"

const FEMBERVEILDialogueNode* UEMBERVEILDialogueDataAsset::GetNode(FName NodeID) const
{
	return Nodes.Find(NodeID);
}
