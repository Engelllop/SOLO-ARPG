# Crea WBP_Shop (Widget Blueprint) con padre USOLOShopWidget en Content/UI/HUD/
#
# UnrealEditor-Cmd.exe "E:\SOLO\SOLO.uproject" -nullrhi -unattended -ExecutePythonScript="E:\SOLO\Tools\setup_shop_widget.py"

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_shop_widget_blueprint(asset_name: str, package_path: str) -> unreal.WidgetBlueprint:
    full = f"{package_path}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full):
        return unreal.EditorAssetLibrary.load_asset(full)

    parent = unreal.load_class(None, "/Script/SOLOCore.SOLOShopWidget")
    if not parent:
        raise RuntimeError("LoadClass SOLOShopWidget failed â€” compile SOLOCore first")

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
    unreal.log("SOLO setup_shop_widget: start")
    hud_pkg = "/Game/UI/HUD"
    ensure_dir(hud_pkg)
    create_shop_widget_blueprint("WBP_Shop", hud_pkg)
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("SOLO setup_shop_widget: done")


main()
