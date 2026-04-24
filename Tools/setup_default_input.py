# Proyecto EMBERVEIL — setup de Enhanced Input + BP del PlayerController.
#
# 1) Copia desde la plantilla UE 5.7 High (ajusta la ruta del motor si hace falta):
#    New-Item -ItemType Directory -Force "Content/Input/Actions"
#    Copy-Item "<UE>/Templates/TemplateResources/High/Input/Content/IMC_Default.uasset" "Content/Input/"
#    Copy-Item "<UE>/Templates/TemplateResources/High/Input/Content/Actions/IA_Move.uasset" "Content/Input/Actions/"
#    Copy-Item "<UE>/Templates/TemplateResources/High/Input/Content/Actions/IA_Look.uasset" "Content/Input/Actions/"
#    Copy-Item "<UE>/Templates/TemplateResources/High/Input/Content/Actions/IA_Jump.uasset" "Content/Input/Actions/"
#
# 2) Ejecuta (crea IA_Dodge, IA_BasicAttack, IA_Interact, IA_ToggleInventory; mapea teclas;
#    IA_Look = Mouse2D + Gamepad_Right2D; genera /Game/Player/BP_EMBERVEILPlayerController con defaults):
#
#    UnrealEditor-Cmd.exe "E:\EMBERVEIL\EMBERVEIL.uproject" -stdout -FullStdOutLogOutput -UTF8Output -nullrhi -unattended -ExecutePythonScript="E:\EMBERVEIL\Tools\setup_default_input.py"

import unreal


def make_key(key_name: str) -> unreal.Key:
    k = unreal.Key()
    k.import_text(key_name)
    return k


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def load_or_fail(path: str):
    if not unreal.EditorAssetLibrary.does_asset_exist(path):
        raise RuntimeError(f"Missing asset (copy from UE template first): {path}")
    return unreal.EditorAssetLibrary.load_asset(path)


def create_input_action(asset_name: str, package_path: str, value_type) -> unreal.InputAction:
    full_path = f"{package_path}/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full_path):
        ia = unreal.EditorAssetLibrary.load_asset(full_path)
        ia.set_editor_property("value_type", value_type)
        unreal.EditorAssetLibrary.save_loaded_asset(ia)
        return ia
    at = unreal.AssetToolsHelpers.get_asset_tools()
    ia = at.create_asset(asset_name, package_path, unreal.InputAction, None)
    if not ia:
        raise RuntimeError(f"Failed to create {full_path}")
    ia.set_editor_property("value_type", value_type)
    unreal.EditorAssetLibrary.save_loaded_asset(ia)
    return ia


def unmap_action_keys(imc: unreal.InputMappingContext, action: unreal.InputAction) -> None:
    if imc and action:
        imc.unmap_all_keys_from_action(action)


def bind_key(imc: unreal.InputMappingContext, action: unreal.InputAction, key_name: str) -> None:
    imc.map_key(action, make_key(key_name))


def ensure_look_mouse_and_gamepad(imc: unreal.InputMappingContext, ia_look: unreal.InputAction) -> None:
    bind_key(imc, ia_look, "Mouse2D")
    bind_key(imc, ia_look, "Gamepad_Right2D")


def create_or_update_pc_blueprint(
    package_path: str,
    imc: unreal.InputMappingContext,
    actions: dict,
) -> unreal.Blueprint:
    asset_name = "BP_EMBERVEILPlayerController"
    full_path = f"{package_path}/{asset_name}"
    # UObject path uses UHT name without leading AActor prefix
    parent = unreal.load_class(None, "/Script/EMBERVEILCore.EMBERVEILPlayerController")
    if not parent:
        raise RuntimeError("LoadClass failed for EMBERVEILPlayerController")

    at = unreal.AssetToolsHelpers.get_asset_tools()
    if unreal.EditorAssetLibrary.does_asset_exist(full_path):
        bp = unreal.EditorAssetLibrary.load_asset(full_path)
    else:
        factory = unreal.BlueprintFactory()
        factory.set_editor_property("parent_class", parent)
        bp = at.create_asset(asset_name, package_path, unreal.Blueprint, factory)
        if not bp:
            raise RuntimeError("BlueprintFactory failed")

    gen = bp.generated_class()
    cdo = unreal.get_default_object(gen)
    cdo.set_editor_property("default_mapping_context", imc)
    cdo.set_editor_property("move_action", actions["move"])
    cdo.set_editor_property("look_action", actions["look"])
    cdo.set_editor_property("jump_action", actions["jump"])
    cdo.set_editor_property("dodge_action", actions["dodge"])
    cdo.set_editor_property("basic_attack_action", actions["basic_attack"])
    cdo.set_editor_property("interact_action", actions["interact"])
    cdo.set_editor_property("toggle_inventory_action", actions["toggle_inventory"])

    unreal.BlueprintEditorLibrary.compile_blueprint(bp)
    unreal.EditorAssetLibrary.save_loaded_asset(bp)
    return bp


def main() -> None:
    unreal.log("EMBERVEIL setup_default_input: start")

    actions_pkg = "/Game/Input/Actions"
    input_pkg = "/Game/Input"
    player_pkg = "/Game/Player"

    ensure_dir(actions_pkg)
    ensure_dir(input_pkg)
    ensure_dir(player_pkg)

    ia_move = load_or_fail(f"{actions_pkg}/IA_Move")
    ia_look = load_or_fail(f"{actions_pkg}/IA_Look")
    ia_jump = load_or_fail(f"{actions_pkg}/IA_Jump")
    imc = load_or_fail(f"{input_pkg}/IMC_Default")

    vt_bool = unreal.InputActionValueType.BOOLEAN
    ia_dodge = create_input_action("IA_Dodge", actions_pkg, vt_bool)
    ia_basic = create_input_action("IA_BasicAttack", actions_pkg, vt_bool)
    ia_interact = create_input_action("IA_Interact", actions_pkg, vt_bool)
    ia_inv = create_input_action("IA_ToggleInventory", actions_pkg, vt_bool)

    # IA_Look: mouse delta + right stick (Axis2D)
    unmap_action_keys(imc, ia_look)
    ensure_look_mouse_and_gamepad(imc, ia_look)

    for ia in (ia_dodge, ia_basic, ia_interact, ia_inv):
        unmap_action_keys(imc, ia)

    bind_key(imc, ia_dodge, "LeftShift")
    bind_key(imc, ia_dodge, "Gamepad_RightShoulder")

    bind_key(imc, ia_basic, "LeftMouseButton")
    bind_key(imc, ia_basic, "Gamepad_RightTrigger")

    bind_key(imc, ia_interact, "E")
    bind_key(imc, ia_interact, "Gamepad_FaceButton_Right")

    bind_key(imc, ia_inv, "I")
    bind_key(imc, ia_inv, "Gamepad_Special_Left")

    unreal.EditorAssetLibrary.save_loaded_asset(imc)

    actions = {
        "move": ia_move,
        "look": ia_look,
        "jump": ia_jump,
        "dodge": ia_dodge,
        "basic_attack": ia_basic,
        "interact": ia_interact,
        "toggle_inventory": ia_inv,
    }
    create_or_update_pc_blueprint(player_pkg, imc, actions)

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("EMBERVEIL setup_default_input: done")


main()
