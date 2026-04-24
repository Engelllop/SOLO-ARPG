#include "Inventory/EMBERVEILItemBase.h"

UEMBERVEILItemBase::UEMBERVEILItemBase()
{
    // MaxStackSize por defecto = 1 (equipables).
    // Las subclases de consumibles/materiales lo deben sobreescribir a 99 en sus defaults.
    MaxStackSize = 1;
    bIsEquipable = false;
    EquipmentSlot = EEMBERVEILEquipmentSlot::None;
    RequiredLevel = 1;
    BaseValue = 0;
    ItemType = EEMBERVEILItemType::Material;
    Rarity = EEMBERVEILItemRarity::Common;
}

bool UEMBERVEILItemBase::CanBeUsedBy_Implementation(EEMBERVEILClass CharClass,
    EEMBERVEILRace Race) const
{
    // Implementación base: cualquier clase y raza puede usar este item.
    // Sobreescribir en Blueprint para añadir restricciones específicas.
    return true;
}

FPrimaryAssetId UEMBERVEILItemBase::GetPrimaryAssetId() const
{
    // El AssetManager identifica este DataAsset por su tipo y su nombre de asset.
    // El tipo "EMBERVEILItem" debe registrarse en ProjectSettings > AssetManager.
    return FPrimaryAssetId(TEXT("EMBERVEILItem"), GetFName());
}
