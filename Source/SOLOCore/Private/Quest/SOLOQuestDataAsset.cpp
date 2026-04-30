#include "Quest/SOLOQuestDataAsset.h"

USOLOQuestDataAsset::USOLOQuestDataAsset()
{
    // Los defaults estÃ¡n definidos en el header mediante los inicializadores de UPROPERTY
}

FPrimaryAssetId USOLOQuestDataAsset::GetPrimaryAssetId() const
{
    // Registra este asset con tipo "Quest" para que el AssetManager pueda encontrarlo.
    // El nombre del asset es el QuestID si estÃ¡ configurado, o el nombre del objeto.
    const FName AssetName = QuestID.IsNone() ? GetFName() : QuestID;
    return FPrimaryAssetId(FPrimaryAssetType("Quest"), AssetName);
}
