# Crea Content/UI/HUD/WBP_HUD_Overlay (Widget Blueprint) con layout Canvas descrito en la tarea.
#
# UnrealEditor-Cmd.exe "E:\SOLO\SOLO.uproject" -nullrhi -unattended -ExecutePythonScript="E:\SOLO\Tools\setup_wbp_hud_overlay.py"
#
# Requiere Python del editor. Si falla alguna llamada, monta el mismo Ã¡rbol a mano en el Designer.

import unreal


def ensure_dir(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def anchors_tl() -> unreal.Anchors:
    return unreal.Anchors(
        minimum=unreal.Vector2D(0.0, 0.0),
        maximum=unreal.Vector2D(0.0, 0.0),
    )


def anchors_bl() -> unreal.Anchors:
    return unreal.Anchors(
        minimum=unreal.Vector2D(0.0, 1.0),
        maximum=unreal.Vector2D(0.0, 1.0),
    )


def anchors_br() -> unreal.Anchors:
    return unreal.Anchors(
        minimum=unreal.Vector2D(1.0, 1.0),
        maximum=unreal.Vector2D(1.0, 1.0),
    )


def new_widget(cls, outer, name: str) -> unreal.Object:
    return unreal.new_object(cls, outer, unreal.Name(name))


def canvas_add(canvas: unreal.CanvasPanel, child: unreal.Widget, a: unreal.Anchors, margin: unreal.Margin, align: unreal.Vector2D):
    slot = canvas.add_child_to_canvas(child)
    slot.set_anchors(a)
    slot.set_offsets(margin)
    slot.set_alignment(align)
    return slot


def attach_single_child(host: unreal.Widget, child: unreal.Widget) -> None:
    """USizeBox / UContentWidget: asigna el Ãºnico hijo (API varÃ­a ligeramente entre versiones)."""
    for fn in ("set_content",):
        if hasattr(host, fn):
            getattr(host, fn)(child)
            return
    for prop in ("content", "child"):
        try:
            host.set_editor_property(prop, child)
            return
        except Exception:
            continue
    host.add_child(child)


def sizeboxed_bar(outer, name_sz: str, name_pb: str, w: float, h: float, color: unreal.LinearColor) -> unreal.SizeBox:
    """ProgressBar con tamaÃ±o fijo dentro de un SizeBox."""
    sz = new_widget(unreal.SizeBox, outer, name_sz)
    sz.set_editor_property("width_override", w)
    sz.set_editor_property("height_override", h)
    pb = new_widget(unreal.ProgressBar, sz, name_pb)
    pb.set_editor_property("fill_color_and_opacity", color)
    attach_single_child(sz, pb)
    return sz


def build_hud_overlay(wb: unreal.WidgetBlueprint) -> None:
    wt = wb.widget_tree
    if not wt:
        raise RuntimeError("WidgetBlueprint has no widget_tree")

    root = new_widget(unreal.CanvasPanel, wt, "RootCanvas")
    wt.root_widget = root

    # --- Arriba izquierda (20, 20) ---
    vb_top = new_widget(unreal.VerticalBox, wt, "VB_TopLeft")

    txt_health_val = new_widget(unreal.TextBlock, wt, "TXT_HealthValue")
    txt_health_val.set_editor_property("text", unreal.Text("HP: 100/100"))

    s_txt_h = vb_top.add_child_to_vertical_box(txt_health_val)
    s_txt_h.set_padding(unreal.Margin(0.0, 0.0, 0.0, 4.0))

    s_pb_h = vb_top.add_child_to_vertical_box(
        sizeboxed_bar(
            wt,
            "SZ_PB_Health",
            "PB_Health",
            250.0,
            20.0,
            unreal.LinearColor(0.82, 0.12, 0.1, 1.0),
        )
    )
    s_pb_h.set_padding(unreal.Margin(0.0, 0.0, 0.0, 4.0))

    s_pb_m = vb_top.add_child_to_vertical_box(
        sizeboxed_bar(
            wt,
            "SZ_PB_Mana",
            "PB_Mana",
            250.0,
            16.0,
            unreal.LinearColor(0.18, 0.42, 0.95, 1.0),
        )
    )
    s_pb_m.set_padding(unreal.Margin(0.0, 0.0, 0.0, 4.0))

    s_pb_s = vb_top.add_child_to_vertical_box(
        sizeboxed_bar(
            wt,
            "SZ_PB_Stamina",
            "PB_Stamina",
            250.0,
            12.0,
            unreal.LinearColor(0.95, 0.82, 0.12, 1.0),
        )
    )
    s_pb_s.set_padding(unreal.Margin(0.0, 0.0, 0.0, 0.0))

    canvas_add(
        root,
        vb_top,
        anchors_tl(),
        unreal.Margin(20.0, 20.0, 0.0, 0.0),
        unreal.Vector2D(0.0, 0.0),
    )

    # --- Abajo izquierda ---
    vb_bot_left = new_widget(unreal.VerticalBox, wt, "VB_BottomLeft")

    txt_level = new_widget(unreal.TextBlock, wt, "TXT_Level")
    txt_level.set_editor_property("text", unreal.Text("Nivel 1"))

    sz_xp = new_widget(unreal.SizeBox, wt, "SZ_PB_Experience")
    sz_xp.set_editor_property("width_override", 200.0)
    sz_xp.set_editor_property("height_override", 8.0)
    pb_xp = new_widget(unreal.ProgressBar, sz_xp, "PB_Experience")
    pb_xp.set_editor_property("fill_color_and_opacity", unreal.LinearColor(0.48, 0.22, 0.78, 1.0))
    attach_single_child(sz_xp, pb_xp)

    vb_bot_left.add_child_to_vertical_box(txt_level)
    vb_bot_left.add_child_to_vertical_box(sz_xp)

    canvas_add(
        root,
        vb_bot_left,
        anchors_bl(),
        unreal.Margin(20.0, 0.0, 0.0, 20.0),
        unreal.Vector2D(0.0, 1.0),
    )

    # --- Abajo derecha ---
    hb_cur = new_widget(unreal.HorizontalBox, wt, "HB_Currency")

    sz_img = new_widget(unreal.SizeBox, wt, "SZ_IMG_Currency")
    sz_img.set_editor_property("width_override", 28.0)
    sz_img.set_editor_property("height_override", 28.0)
    img_coin = new_widget(unreal.Image, sz_img, "IMG_Currency")
    coin_tex = unreal.EditorAssetLibrary.load_asset("/Engine/EditorResources/SequenceRecorder/RecordingIndicator")
    if coin_tex:
        brush = unreal.SlateBrush()
        brush.set_editor_property("resource_object", coin_tex)
        img_coin.set_editor_property("brush", brush)
    attach_single_child(sz_img, img_coin)

    txt_currency = new_widget(unreal.TextBlock, wt, "TXT_Currency")
    txt_currency.set_editor_property("text", unreal.Text("0 G"))

    s_img = hb_cur.add_child_to_horizontal_box(sz_img)
    s_img.set_vertical_alignment(unreal.VerticalAlignment.V_ALIGN_CENTER)
    s_img.set_padding(unreal.Margin(0.0, 0.0, 8.0, 0.0))

    s_txt = hb_cur.add_child_to_horizontal_box(txt_currency)
    s_txt.set_vertical_alignment(unreal.VerticalAlignment.V_ALIGN_CENTER)

    canvas_add(
        root,
        hb_cur,
        anchors_br(),
        unreal.Margin(0.0, 0.0, 20.0, 20.0),
        unreal.Vector2D(1.0, 1.0),
    )


def main() -> None:
    unreal.log("setup_wbp_hud_overlay: start")
    pkg = "/Game/UI/HUD"
    asset_name = "WBP_HUD_Overlay"
    full = f"{pkg}/{asset_name}"

    ensure_dir(pkg)

    if unreal.EditorAssetLibrary.does_asset_exist(full):
        unreal.log_warning(f"setup_wbp_hud_overlay: ya existe {full} â€” se reconstruye el Ã¡rbol.")
        wb = unreal.EditorAssetLibrary.load_asset(full)
    else:
        factory = unreal.WidgetBlueprintFactory()
        parent = unreal.load_class(None, "/Script/UMG.UserWidget")
        if parent:
            factory.set_editor_property("parent_class", parent)
        at = unreal.AssetToolsHelpers.get_asset_tools()
        wb = at.create_asset(asset_name, pkg, unreal.WidgetBlueprint, factory)
        if not wb:
            raise RuntimeError(f"No se pudo crear {full}")

    build_hud_overlay(wb)
    unreal.BlueprintEditorLibrary.compile_blueprint(wb)
    unreal.EditorAssetLibrary.save_loaded_asset(wb)
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("setup_wbp_hud_overlay: done")


main()
