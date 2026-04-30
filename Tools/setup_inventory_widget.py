# Crea WBP_Inventory (UserWidget vacÃ­o) y lo asigna a BP_SOLOPlayerController.InventoryWidgetClass.
#
# UnrealEditor-Cmd.exe "E:\SOLO\SOLO.uproject" -nullrhi -unattended -ExecutePythonScript="E:\SOLO\Tools\setup_inventory_widget.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_widget_blueprint(asset_name: str, package_path: str) -> unreal.WidgetBlueprint:
    full = f"{package_path}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        return unreal.EditorAssetLibrary.load_asset(full)

    parent = unreal.load_class(None, "/Script/UMG.UserWidget")
    if not parent:
        raise RuntimeError("Could not load UUserWidget class")

    factory = unreal.WidgetBlueprintFactory()
    factory.set_editor_property("parent_class", parent)

    at = unreal.AssetToolsHelpers.get_asset_tools()
    wb = at.create_asset(asset_name, package_path, unreal.WidgetBlueprint, factory)
    if not wb:
        raise RuntimeError(f"Failed to create {full}")

    unreal.BlueprintEditorLibrary.compile_blueprint(wb)
    unreal.EditorAssetLibrary.save_loaded_asset(wb)
    return wb


def assign_inventory_to_pc() -> None:
    pc_path = "/Game/Player/BP_SOLOPlayerController"
    if not unreal.EditorAssetLibrary.does_asset_exist(pc_path):
        raise RuntimeError(f"Missing {pc_path} â€” run setup_default_input.py first")

    widget_class = unreal.load_class(None, "/Game/UI/HUD/WBP_Inventory.WBP_Inventory_C")
    if not widget_class:
        raise RuntimeError("Could not load WBP_Inventory_C")

    bp = unreal.EditorAssetLibrary.load_asset(pc_path)
    gen = bp.generated_class()
    cdo = unreal.get_default_object(gen)
    cdo.set_editor_property("inventory_widget_class", widget_class)

    unreal.BlueprintEditorLibrary.compile_blueprint(bp)
    unreal.EditorAssetLibrary.save_loaded_asset(bp)


def main() -> None:
    unreal.log("SOLO setup_inventory_widget: start")
    hud_pkg = "/Game/UI/HUD"
    ensure_dir(hud_pkg)
    create_widget_blueprint("WBP_Inventory", hud_pkg)
    assign_inventory_to_pc()
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("SOLO setup_inventory_widget: done")


main()
