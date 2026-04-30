#include "Inventory/SOLOItemBase.h"

USOLOItemBase::USOLOItemBase()
{
    // MaxStackSize por defecto = 1 (equipables).
    // Las subclases de consumibles/materiales lo deben sobreescribir a 99 en sus defaults.
    MaxStackSize = 1;
    bIsEquipable = false;
    EquipmentSlot = ESOLOEquipmentSlot::None;
    RequiredLevel = 1;
    BaseValue = 0;
    ItemType = ESOLOItemType::Material;
    Rarity = ESOLOItemRarity::Common;
}

bool USOLOItemBase::CanBeUsedBy_Implementation(ESOLOClass CharClass,
    ESOLORace Race) const
{
    // ImplementaciÃ³n base: cualquier clase y raza puede usar este item.
    // Sobreescribir en Blueprint para aÃ±adir restricciones especÃ­ficas.
    return true;
}

FPrimaryAssetId USOLOItemBase::GetPrimaryAssetId() const
{
    // El AssetManager identifica este DataAsset por su tipo y su nombre de asset.
    // El tipo "SOLOItem" debe registrarse en ProjectSettings > AssetManager.
    return FPrimaryAssetId(TEXT("SOLOItem"), GetFName());
}
