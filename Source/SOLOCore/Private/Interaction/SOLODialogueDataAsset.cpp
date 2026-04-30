#include "Interaction/SOLODialogueDataAsset.h"

const FSOLODialogueNode* USOLODialogueDataAsset::GetNode(FName NodeID) const
{
	return Nodes.Find(NodeID);
}
