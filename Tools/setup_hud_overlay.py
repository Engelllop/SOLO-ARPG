# Crea WBP_HUD_Overlay (padre UEMBERVEILHUDOverlayWidget) en Content/UI/HUD/
#
# UnrealEditor-Cmd.exe "E:\EMBERVEIL\EMBERVEIL.uproject" -nullrhi -unattended -ExecutePythonScript="E:\EMBERVEIL\Tools\setup_hud_overlay.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_widget_blueprint(asset_name: str, package_path: str) -> unreal.WidgetBlueprint:
    full = f"{package_path}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        return unreal.EditorAssetLibrary.load_asset(full)

    parent = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILHUDOverlayWidget")
    if not parent:
        raise RuntimeError("LoadClass EMBERVEILHUDOverlayWidget failed — compile EMBERVEILCore first")

    factory = unreal.WidgetBlueprintFactory()
    factory.set_editor_property("parent_class", parent)

    at = unreal.AssetToolsHelpers.get_asset_tools()
    wb = at.create_asset(asset_name, package_path, unreal.WidgetBlueprint, factory)
    if not wb:
        raise RuntimeError(f"Failed to create {full}")

    unreal.BlueprintEditorLibrary.compile_blueprint(wb)
    unreal.EditorAssetLibrary.save_loaded_asset(wb)
    return wb


def main() -> None:
    unreal.log("EMBERVEIL setup_hud_overlay: start")
    hud_pkg = "/Game/UI/HUD"
    ensure_dir(hud_pkg)
    create_widget_blueprint("WBP_HUD_Overlay", hud_pkg)
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("EMBERVEIL setup_hud_overlay: done")


main()
