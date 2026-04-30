# Crea DataAssets USOLOItemBase bajo Content/Data/Items/...
#
# UnrealEditor-Cmd.exe "E:\SOLO\SOLO.uproject" -nullrhi -unattended -ExecutePythonScript="E:\SOLO\Tools\setup_item_data_assets.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def txt(s: str) -> unreal.Text:
    return unreal.Text(s)


def create_item_da(package_path: str, asset_name: str, **props) -> unreal.Object:
    full = f"{package_path}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        da = unreal.EditorAssetLibrary.load_asset(full)
    else:
        cls = unreal.load_class(None, "/Script/SOLOCore.SOLOItemBase")
        if not cls:
            raise RuntimeError("LoadClass SOLOItemBase failed")
        at = unreal.AssetToolsHelpers.get_asset_tools()
        da = at.create_asset(asset_name, package_path, cls, None)
        if not da:
            raise RuntimeError(f"create_asset failed: {full}")

    for k, v in props.items():
        da.set_editor_property(k, v)

    unreal.EditorAssetLibrary.save_loaded_asset(da)
    return da


def main() -> None:
    unreal.log("setup_item_data_assets: start")

    it = unreal.SOLOItemType
    r = unreal.SOLOItemRarity
    slot = unreal.SOLOEquipmentSlot

    base = "/Game/Data/Items"
    for sub in ("Weapons", "Armor", "Consumables", "Materials"):
        ensure_dir(f"{base}/{sub}")

    # Weapons
    create_item_da(
        f"{base}/Weapons",
        "DA_IronSword",
        item_id=unreal.Name("IRON_SWORD"),
        item_name=txt("Iron Sword"),
        item_description=txt("A sturdy iron blade."),
        item_type=it.WEAPON,
        rarity=r.COMMON,
        max_stack_size=1,
        bIsEquipable=True,
        equipment_slot=slot.MAIN_HAND,
        base_value=50,
    )
    create_item_da(
        f"{base}/Weapons",
        "DA_SteelSword",
        item_id=unreal.Name("STEEL_SWORD"),
        item_name=txt("Steel Sword"),
        item_description=txt("Forged from refined steel."),
        item_type=it.WEAPON,
        rarity=r.UNCOMMON,
        max_stack_size=1,
        bIsEquipable=True,
        equipment_slot=slot.MAIN_HAND,
        base_value=150,
    )
    create_item_da(
        f"{base}/Weapons",
        "DA_FireDagger",
        item_id=unreal.Name("FIRE_DAGGER"),
        item_name=txt("Fire Dagger"),
        item_description=txt("A dagger imbued with flame."),
        item_type=it.WEAPON,
        rarity=r.RARE,
        max_stack_size=1,
        bIsEquipable=True,
        equipment_slot=slot.MAIN_HAND,
        base_value=400,
    )

    # Armor
    create_item_da(
        f"{base}/Armor",
        "DA_LeatherChest",
        item_id=unreal.Name("LEATHER_CHEST"),
        item_name=txt("Leather Chest"),
        item_description=txt("Light leather torso protection."),
        item_type=it.ARMOR,
        rarity=r.COMMON,
        max_stack_size=1,
        bIsEquipable=True,
        equipment_slot=slot.CHEST,
        base_value=40,
    )
    create_item_da(
        f"{base}/Armor",
        "DA_IronHelm",
        item_id=unreal.Name("IRON_HELM"),
        item_name=txt("Iron Helm"),
        item_description=txt("Basic head protection."),
        item_type=it.ARMOR,
        rarity=r.COMMON,
        max_stack_size=1,
        bIsEquipable=True,
        equipment_slot=slot.HEAD,
        base_value=35,
    )

    # Consumables
    create_item_da(
        f"{base}/Consumables",
        "DA_HealthPotion",
        item_id=unreal.Name("HEALTH_POTION"),
        item_name=txt("Health Potion"),
        item_description=txt("Restores health when used."),
        item_type=it.CONSUMABLE,
        rarity=r.COMMON,
        max_stack_size=20,
        bIsEquipable=False,
        equipment_slot=slot.NONE,
        base_value=25,
    )
    create_item_da(
        f"{base}/Consumables",
        "DA_ManaPotion",
        item_id=unreal.Name("MANA_POTION"),
        item_name=txt("Mana Potion"),
        item_description=txt("Restores mana when used."),
        item_type=it.CONSUMABLE,
        rarity=r.COMMON,
        max_stack_size=20,
        bIsEquipable=False,
        equipment_slot=slot.NONE,
        base_value=25,
    )

    # Materials
    create_item_da(
        f"{base}/Materials",
        "DA_IronOre",
        item_id=unreal.Name("IRON_ORE"),
        item_name=txt("Iron Ore"),
        item_description=txt("Raw iron for smithing."),
        item_type=it.MATERIAL,
        rarity=r.COMMON,
        max_stack_size=99,
        bIsEquipable=False,
        equipment_slot=slot.NONE,
        base_value=5,
    )
    create_item_da(
        f"{base}/Materials",
        "DA_Coal",
        item_id=unreal.Name("COAL"),
        item_name=txt("Coal"),
        item_description=txt("Fuel for the forge."),
        item_type=it.MATERIAL,
        rarity=r.COMMON,
        max_stack_size=99,
        bIsEquipable=False,
        equipment_slot=slot.NONE,
        base_value=3,
    )

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("setup_item_data_assets: done")


main()
