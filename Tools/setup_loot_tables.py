# Crea UEMBERVEILLootTableDataAsset en Content/Data/LootTables/
#
# UnrealEditor-Cmd.exe "E:\EMBERVEIL\EMBERVEIL.uproject" -nullrhi -unattended -ExecutePythonScript="E:\EMBERVEIL\Tools\setup_loot_tables.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def make_entry(item_pkg: str, drop_chance: float, min_q: int, max_q: int, b_guaranteed: bool = False):
    e = unreal.LootEntry()
    item_asset = unreal.EditorAssetLibrary.load_asset(item_pkg)
    if not item_asset:
        raise RuntimeError(f"Item asset not found: {item_pkg}")
    e.set_editor_property("item", item_asset)
    e.set_editor_property("drop_chance", drop_chance)
    e.set_editor_property("min_quantity", min_q)
    e.set_editor_property("max_quantity", max_q)
    e.set_editor_property("bIsGuaranteed", b_guaranteed)
    return e


def create_loot_table(asset_name: str, entries, min_currency: int, max_currency: int) -> unreal.Object:
    pkg = "/Game/Data/LootTables"
    full = f"{pkg}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        lt = unreal.EditorAssetLibrary.load_asset(full)
    else:
        cls = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILLootTableDataAsset")
        if not cls:
            raise RuntimeError("LoadClass EMBERVEILLootTableDataAsset failed")
        at = unreal.AssetToolsHelpers.get_asset_tools()
        lt = at.create_asset(asset_name, pkg, cls, None)
        if not lt:
            raise RuntimeError(f"create_asset failed: {full}")

    lt.set_editor_property("loot_entries", entries)
    lt.set_editor_property("min_currency_drop", min_currency)
    lt.set_editor_property("max_currency_drop", max_currency)
    unreal.EditorAssetLibrary.save_loaded_asset(lt)
    return lt


def main() -> None:
    unreal.log("setup_loot_tables: start")
    ensure_dir("/Game/Data/LootTables")

    base_items = "/Game/Data/Items"

    create_loot_table(
        "LT_CommonEnemy",
        [
            make_entry(f"{base_items}/Materials/DA_IronOre", 0.4, 1, 3),
            make_entry(f"{base_items}/Consumables/DA_HealthPotion", 0.2, 1, 1),
        ],
        min_currency=5,
        max_currency=20,
    )

    create_loot_table(
        "LT_EliteEnemy",
        [
            make_entry(f"{base_items}/Weapons/DA_SteelSword", 0.1, 1, 1),
            make_entry(f"{base_items}/Materials/DA_IronOre", 0.6, 2, 5),
            make_entry(f"{base_items}/Consumables/DA_HealthPotion", 0.4, 1, 2),
        ],
        min_currency=30,
        max_currency=100,
    )

    create_loot_table(
        "LT_Boss",
        [
            make_entry(f"{base_items}/Weapons/DA_FireDagger", 0.3, 1, 1, False),
            make_entry(f"{base_items}/Armor/DA_LeatherChest", 1.0, 1, 1, True),
        ],
        min_currency=150,
        max_currency=500,
    )

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("setup_loot_tables: done")


main()
