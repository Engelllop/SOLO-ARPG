# Crea UEMBERVEILRecipeDataAsset en Content/Data/Recipes/ (prefijo DA_Recipe_*)
#
# UnrealEditor-Cmd.exe "E:\EMBERVEIL\EMBERVEIL.uproject" -nullrhi -unattended -ExecutePythonScript="E:\EMBERVEIL\Tools\setup_recipes.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def txt(s: str) -> unreal.Text:
    return unreal.Text(s)


def crafting_ingredient(item_path: str, quantity: int):
    item = unreal.EditorAssetLibrary.load_asset(item_path)
    if not item:
        raise RuntimeError(f"Missing item: {item_path}")
    ing = unreal.CraftingIngredient()
    ing.set_editor_property("item_id", item.get_editor_property("item_id"))
    ing.set_editor_property("item", item)
    ing.set_editor_property("quantity", quantity)
    return ing


def create_recipe(asset_name: str, **props) -> unreal.Object:
    pkg = "/Game/Data/Recipes"
    full = f"{pkg}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        rec = unreal.EditorAssetLibrary.load_asset(full)
    else:
        cls = unreal.load_class(None, "/Script/EMBERVEILCore.UEMBERVEILRecipeDataAsset")
        if not cls:
            cls = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILRecipeDataAsset")
        if not cls:
            raise RuntimeError("LoadClass UEMBERVEILRecipeDataAsset failed")
        at = unreal.AssetToolsHelpers.get_asset_tools()
        rec = at.create_asset(asset_name, pkg, cls, None)
        if not rec:
            raise RuntimeError(f"create_asset failed: {full}")
    for k, v in props.items():
        rec.set_editor_property(k, v)
    unreal.EditorAssetLibrary.save_loaded_asset(rec)
    return rec


def main() -> None:
    unreal.log("setup_recipes: start")
    ensure_dir("/Game/Data/Recipes")

    cat = unreal.ECraftingCategory
    items = "/Game/Data/Items"
    iron_ore = f"{items}/Materials/DA_IronOre"
    coal = f"{items}/Materials/DA_Coal"
    iron_sword = f"{items}/Weapons/DA_IronSword"
    iron_helm = f"{items}/Armor/DA_IronHelm"
    health_potion = f"{items}/Consumables/DA_HealthPotion"
    mana_potion = f"{items}/Consumables/DA_ManaPotion"
    leather_chest = f"{items}/Armor/DA_LeatherChest"

    create_recipe(
        "DA_Recipe_HealthPotion",
        recipe_id=unreal.Name("Recipe_HealthPotion"),
        recipe_name=txt("Health Potion"),
        recipe_description=txt("Brew health potions."),
        category=cat.ALCHEMY,
        ingredients=[crafting_ingredient(coal, 1)],
        result_item=unreal.EditorAssetLibrary.load_asset(health_potion),
        result_quantity=2,
        required_player_level=1,
        bIsDiscoveredByDefault=True,
        crafting_time=2.0,
    )

    create_recipe(
        "DA_Recipe_ManaPotion",
        recipe_id=unreal.Name("Recipe_ManaPotion"),
        recipe_name=txt("Mana Potion"),
        recipe_description=txt("Brew mana potions."),
        category=cat.ALCHEMY,
        ingredients=[crafting_ingredient(coal, 1)],
        result_item=unreal.EditorAssetLibrary.load_asset(mana_potion),
        result_quantity=2,
        required_player_level=1,
        bIsDiscoveredByDefault=True,
        crafting_time=2.0,
    )

    create_recipe(
        "DA_Recipe_IronSword",
        recipe_id=unreal.Name("Recipe_IronSword"),
        recipe_name=txt("Iron Sword"),
        recipe_description=txt("Forge an iron sword from ore and coal."),
        category=cat.WEAPONSMITHING,
        ingredients=[
            crafting_ingredient(iron_ore, 5),
            crafting_ingredient(coal, 2),
        ],
        result_item=unreal.EditorAssetLibrary.load_asset(iron_sword),
        result_quantity=1,
        required_player_level=1,
        bIsDiscoveredByDefault=False,
        crafting_time=0.0,
    )

    create_recipe(
        "DA_Recipe_LeatherChest",
        recipe_id=unreal.Name("Recipe_LeatherChest"),
        recipe_name=txt("Leather Chest"),
        recipe_description=txt("Craft a leather chest piece."),
        category=cat.ARMORSMITHING,
        ingredients=[
            crafting_ingredient(iron_ore, 3),
            crafting_ingredient(coal, 1),
        ],
        result_item=unreal.EditorAssetLibrary.load_asset(leather_chest),
        result_quantity=1,
        required_player_level=1,
        bIsDiscoveredByDefault=False,
        crafting_time=0.0,
    )

    create_recipe(
        "DA_Recipe_IronHelm",
        recipe_id=unreal.Name("Recipe_IronHelm"),
        recipe_name=txt("Iron Helm"),
        recipe_description=txt("Forge an iron helm."),
        category=cat.ARMORSMITHING,
        ingredients=[
            crafting_ingredient(iron_ore, 2),
            crafting_ingredient(coal, 1),
        ],
        result_item=unreal.EditorAssetLibrary.load_asset(iron_helm),
        result_quantity=1,
        required_player_level=1,
        bIsDiscoveredByDefault=False,
        crafting_time=0.0,
    )

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("setup_recipes: done")


main()
