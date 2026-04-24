#include "Quest/EMBERVEILQuestDataAsset.h"

UEMBERVEILQuestDataAsset::UEMBERVEILQuestDataAsset()
{
    // Los defaults están definidos en el header mediante los inicializadores de UPROPERTY
}

FPrimaryAssetId UEMBERVEILQuestDataAsset::GetPrimaryAssetId() const
{
    // Registra este asset con tipo "Quest" para que el AssetManager pueda encontrarlo.
    // El nombre del asset es el QuestID si está configurado, o el nombre del objeto.
    const FName AssetName = QuestID.IsNone() ? GetFName() : QuestID;
    return FPrimaryAssetId(FPrimaryAssetType("Quest"), AssetName);
}
