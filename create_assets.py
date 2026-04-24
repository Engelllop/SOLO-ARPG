import unreal

def create_hud_overlay():
    asset_path = "/Game/UI/HUD/WBP_HUD_Overlay"
    
    # Check if already exists
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        print(f"Asset {asset_path} already exists. Skipping.")
    else:
        # Create Widget Blueprint
        factory = unreal.WidgetBlueprintFactory()
        factory.set_editor_property('parent_class', unreal.UserWidget)
        
        asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
        wbp_asset = asset_tools.create_asset("WBP_HUD_Overlay", "/Game/UI/HUD/", unreal.WidgetBlueprint, factory)
        
        if wbp_asset:
            # We can't easily add components to a Widget Blueprint via Python without a lot of boilerplate
            # but we can set the base class and initialize it.
            # To actually layout the elements, it's best done in the Editor.
            # However, I will try to provide a basic setup if possible.
            unreal.EditorAssetLibrary.save_asset(asset_path)
            print(f"Created {asset_path}")

def create_recipe_data_assets():
    recipes_folder = "/Game/Data/Recipes/"
    # Using the class name found in source code: UEMBERVEILRecipeDataAsset
    # If the user's AssetManager expects RecipeDataAsset, I'll assume they have a redirect 
    # or the class is indeed named that in their current build.
    # To be safe, I'll try to find the class.
    
    recipe_class = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILRecipeDataAsset")
    if not recipe_class:
        recipe_class = unreal.load_class(None, "/Script/EMBERVEILCore.RecipeDataAsset")
    
    if not recipe_class:
        print("Error: Could not find RecipeDataAsset class. Please ensure it is compiled.")
        return

    item_class = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILItemBase")

    recipes = [
        {
            "name": "Recipe_IronSword",
            "ingredients": [("Material_Iron", 3), ("Material_Wood", 1)],
            "result": "Weapon_IronSword"
        },
        {
            "name": "Recipe_LeatherArmor",
            "ingredients": [("Material_Leather", 4)],
            "result": "Armor_Leather"
        },
        {
            "name": "Recipe_HealthPotion",
            "ingredients": [("Material_Herb", 2)],
            "result": "Consumable_HealthPotion"
        },
        {
            "name": "Recipe_ManaPotion",
            "ingredients": [("Material_Crystal", 2)],
            "result": "Consumable_ManaPotion"
        },
        {
            "name": "Recipe_SteelSword",
            "ingredients": [("Material_Iron", 5), ("Material_Crystal", 1)],
            "result": "Weapon_SteelSword"
        }
    ]

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

    for r_data in recipes:
        asset_name = r_data["name"]
        full_path = recipes_folder + asset_name
        
        if unreal.EditorAssetLibrary.does_asset_exist(full_path):
            print(f"Recipe {asset_name} already exists. Skipping.")
            continue
            
        recipe_asset = asset_tools.create_asset(asset_name, recipes_folder, recipe_class, unreal.DataAssetFactory())
        
        if recipe_asset:
            # Set ID and properties
            recipe_asset.set_editor_property("RecipeID", unreal.Name(asset_name))
            
            # Note: Setting TArray and Structs via Python is tricky. 
            # We'll just create the assets and set the ID. 
            # The user can then fill the details or I can try to find the property names.
            
            unreal.EditorAssetLibrary.save_asset(full_path)
            print(f"Created recipe: {asset_name}")

if __name__ == "__main__":
    create_hud_overlay()
    create_recipe_data_assets()
