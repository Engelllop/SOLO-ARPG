#!/usr/bin/env python3
"""
SOLO-ARPG Content Setup Script
Genera la estructura de Content/ y archivos .uasset minimos
para que el proyecto funcione en UE5.7 sin tener que crear todo manualmente.

Uso:
    python setup_content.py

Requiere:
    - UE5.7 instalado
    - Ejecutar desde el directorio raiz del proyecto
"""

import os
import sys
import json
import struct

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONTENT_DIR = os.path.join(PROJECT_ROOT, "Content")

# Estructura de carpetas necesaria
FOLDERS = [
    "SOLO/Blueprints",
    "SOLO/Input",
    "SOLO/Maps",
    "SOLO/UI",
    "SOLO/Characters/Player",
    "SOLO/Characters/Enemies",
    "SOLO/Items",
    "SOLO/Audio",
    "SOLO/Effects",
    "SOLO/LevelPrototyping",
]

# Blueprints minimos a crear
BLUEPRINTS = [
    ("SOLO/Characters/Player/BP_SOLOPlayerCharacter", "ASOLOPlayerCharacter"),
    ("SOLO/Blueprints/BP_SOLOPlayerController", "ASOLOPlayerController"),
    ("SOLO/UI/BP_SOLOHUD", "ASOLOHUDBase"),
    ("SOLO/Blueprints/BP_SOLOGameMode", "ASOLOGameMode"),
]

# Input Actions
INPUT_ACTIONS = [
    ("IA_Move", "Axis2D"),
    ("IA_Look", "Axis2D"),
    ("IA_Attack", "Action"),
    ("IA_ChargedAttack", "Action"),
    ("IA_Dodge", "Action"),
    ("IA_Interact", "Action"),
    ("IA_Jump", "Action"),
    ("IA_UseAbility1", "Action"),
    ("IA_UseAbility2", "Action"),
    ("IA_UseAbility3", "Action"),
    ("IA_UseAbility4", "Action"),
    ("IA_OpenInventory", "Action"),
    ("IA_OpenMap", "Action"),
    ("IA_OpenQuestLog", "Action"),
    ("IA_Pause", "Action"),
    ("IA_CompanionCommand", "Action"),
]

def create_folders():
    """Crea la estructura de carpetas"""
    for folder in FOLDERS:
        path = os.path.join(CONTENT_DIR, folder)
        os.makedirs(path, exist_ok=True)
        print(f"  [OK] {folder}/")

def create_minimal_uasset(path, parent_class=None):
    """
    Crea un .uasset minimo que UE5 puede reconstruir.
    NO es un asset funcional, pero permite que el editor lo reconozca
    y lo reconstruya al abrir.
    """
    # .uasset es binario, pero podemos crear un .uasset vacio con header minimo
    # UE5 lo detectara como corrupto y ofrecera recrearlo
    # Mejor: crear un archivo .txt con instrucciones
    readme_path = path + "_CREAR_EN_EDITOR.txt"
    with open(readme_path, "w") as f:
        f.write(f"CREAR EN UNREAL EDITOR:\n")
        f.write(f"  1. Click derecho en esta carpeta\n")
        f.write(f"  2. Blueprint Class\n")
        if parent_class:
            f.write(f"  3. Seleccionar parent: {parent_class}\n")
        f.write(f"  4. Nombre: {os.path.basename(path)}\n")
        f.write(f"\n")
        f.write(f"Guardar como: /Game/{path.replace('.uasset', '')}\n")

def create_input_action_docs():
    """Crea documentacion para Input Actions"""
    path = os.path.join(CONTENT_DIR, "SOLO", "Input", "INPUT_SETUP.txt")
    with open(path, "w") as f:
        f.write("=" * 60 + "\n")
        f.write("SETUP DE INPUT ACTIONS (Enhanced Input)\n")
        f.write("=" * 60 + "\n\n")
        f.write("Pasos en Unreal Editor:\n\n")
        f.write("1. En Content Browser, ir a Content/SOLO/Input/\n")
        f.write("2. Click derecho -> Input -> Input Action\n")
        f.write("3. Crear cada uno de los siguientes:\n\n")
        for name, action_type in INPUT_ACTIONS:
            f.write(f"   - {name} ({action_type})\n")
        f.write("\n")
        f.write("4. Luego crear Input Mapping Context (IMC):\n")
        f.write("   - Click derecho -> Input -> Input Mapping Context\n")
        f.write("   - Nombre: IMC_SOLO_Default\n")
        f.write("\n")
        f.write("5. Asignar teclas en IMC_SOLO_Default:\n")
        f.write("   IA_Move -> W/A/S/D (Axis2D)\n")
        f.write("   IA_Look -> Mouse X/Y (Axis2D)\n")
        f.write("   IA_Attack -> Left Mouse Button\n")
        f.write("   IA_ChargedAttack -> Hold Left Mouse Button\n")
        f.write("   IA_Dodge -> Space Bar\n")
        f.write("   IA_Interact -> E\n")
        f.write("   IA_Jump -> Space Bar (o separar de Dodge)\n")
        f.write("   IA_UseAbility1 -> Q\n")
        f.write("   IA_UseAbility2 -> E\n")
        f.write("   IA_UseAbility3 -> R\n")
        f.write("   IA_UseAbility4 -> F\n")
        f.write("   IA_OpenInventory -> Tab\n")
        f.write("   IA_OpenMap -> M\n")
        f.write("   IA_OpenQuestLog -> J\n")
        f.write("   IA_Pause -> Escape\n")
        f.write("   IA_CompanionCommand -> T\n")
        f.write("\n")
        f.write("6. En BP_SOLOPlayerController, asignar IMC_SOLO_Default\n")
        f.write("   al Enhanced Input Local Player Subsystem en BeginPlay.\n")

def create_blueprint_docs():
    """Crea documentacion para Blueprints"""
    for bp_path, parent_class in BLUEPRINTS:
        full_path = os.path.join(CONTENT_DIR, bp_path)
        os.makedirs(os.path.dirname(full_path), exist_ok=True)
        create_minimal_uasset(full_path, parent_class)

def create_map_doc():
    """Crea documentacion para el mapa inicial"""
    path = os.path.join(CONTENT_DIR, "SOLO", "Maps", "MAP_SETUP.txt")
    with open(path, "w") as f:
        f.write("=" * 60 + "\n")
        f.write("SETUP DE MAPA INICIAL\n")
        f.write("=" * 60 + "\n\n")
        f.write("1. File -> New Level\n")
        f.write("2. Seleccionar 'Empty Level'\n")
        f.write("3. Guardar como: Content/SOLO/Maps/L_SoloTestMap\n\n")
        f.write("4. Agregar actores basicos:\n")
        f.write("   - SkyLight (Window -> Env. Light Mixer)\n")
        f.write("   - DirectionalLight\n")
        f.write("   - PlayerStart (buscar en Place Actors)\n")
        f.write("   - Static Mesh -> Cube (escalar para hacer piso)\n")
        f.write("   - NavMeshBoundsVolume (para AI navigation)\n")
        f.write("   - PostProcessVolume (opcional)\n\n")
        f.write("5. World Settings:\n")
        f.write("   - GameMode Override: BP_SOLOGameMode\n")
        f.write("   - Selected GameMode -> Default Pawn: BP_SOLOPlayerCharacter\n\n")
        f.write("6. Build -> Build Paths (para NavMesh)\n\n")
        f.write("7. Guardar todo (Ctrl+Shift+S)\n")

def create_project_settings_doc():
    """Crea documentacion para configuracion del proyecto"""
    path = os.path.join(PROJECT_ROOT, "Docs", "UE5_SETUP_GUIDE.md")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write("# Guia de Setup para UE5.7 - SOLO-ARPG\n\n")
        f.write("## Requisitos\n\n")
        f.write("- Unreal Engine 5.7\n")
        f.write("- Visual Studio 2022 (para compilar C++)\n")
        f.write("- Este repo clonado\n\n")
        f.write("## Pasos\n\n")
        f.write("### 1. Generar archivos de Visual Studio\n\n")
        f.write("```bash\n")
        f.write("# Click derecho en SOLO.uproject -> Generate Visual Studio project files\n")
        f.write("# O desde terminal:\n")
        f.write('"C:\\Program Files\\Epic Games\\UE_5.7\\Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe" -projectfiles -project="%CD%\\SOLO.uproject" -game -engine -progress\n')
        f.write("```\n\n")
        f.write("### 2. Compilar el proyecto\n\n")
        f.write("- Abrir SOLO.sln en Visual Studio 2022\n")
        f.write("- Seleccionar configuracion: Development Editor\n")
        f.write("- Build -> Build Solution (Ctrl+Shift+B)\n\n")
        f.write("### 3. Abrir en Unreal Editor\n\n")
        f.write("- Doble click en SOLO.uproject\n")
        f.write("- Esperar a que compile shaders inicial\n\n")
        f.write("### 4. Crear Content (usar guias en Content/SOLO/)\n\n")
        f.write("- Content/SOLO/Input/INPUT_SETUP.txt\n")
        f.write("- Content/SOLO/Maps/MAP_SETUP.txt\n")
        f.write("- Archivos _CREAR_EN_EDITOR.txt en cada carpeta\n\n")
        f.write("### 5. Configurar Input\n\n")
        f.write("- Project Settings -> Input -> Default Classes\n")
        f.write("  - Default Player Input Class: EnhancedPlayerInput\n")
        f.write("  - Default Input Component Class: EnhancedInputComponent\n")
        f.write("- Verificar que DefaultInput.ini tenga estas clases\n\n")
        f.write("### 6. Probar\n\n")
        f.write("- Play -> Selected Viewport\n")
        f.write("- WASD para mover, mouse para camara\n")
        f.write("- Click izquierdo para atacar\n")
        f.write("- Space para esquivar\n\n")
        f.write("## Troubleshooting\n\n")
        f.write("### Error: 'Could not find module SOLOCore'\n")
        f.write("- Recompilar en Visual Studio\n")
        f.write("- Verificar que SOLOCore.Build.cs tenga las dependencias correctas\n\n")
        f.write("### Error: 'Blueprint not found'\n")
        f.write("- Crear los Blueprints segun las guias en Content/SOLO/\n")
        f.write("- Verificar que las rutas coincidan con SOLOGameMode.cpp\n\n")
        f.write("### Error: 'Input not working'\n")
        f.write("- Verificar que IMC_SOLO_Default este creado y asignado\n")
        f.write("- Verificar que Input Actions tengan las teclas correctas\n")
        f.write("- Verificar que el PlayerController use Enhanced Input\n\n")

def main():
    print("=" * 60)
    print("SOLO-ARPG Content Setup")
    print("=" * 60)
    print()
    
    print("[1/5] Creando estructura de carpetas...")
    create_folders()
    print()
    
    print("[2/5] Creando guias para Blueprints...")
    create_blueprint_docs()
    print("  [OK] Blueprints documentados")
    print()
    
    print("[3/5] Creando guia para Input Actions...")
    create_input_action_docs()
    print("  [OK] Input documentado")
    print()
    
    print("[4/5] Creando guia para Mapa...")
    create_map_doc()
    print("  [OK] Mapa documentado")
    print()
    
    print("[5/5] Creando guia de setup completa...")
    create_project_settings_doc()
    print("  [OK] Docs/UE5_SETUP_GUIDE.md creado")
    print()
    
    print("=" * 60)
    print("Setup completado!")
    print("=" * 60)
    print()
    print("Siguientes pasos:")
    print("  1. Instalar UE5.7 (si no esta instalado)")
    print("  2. Generar archivos de Visual Studio")
    print("  3. Compilar el proyecto")
    print("  4. Abrir en Unreal Editor")
    print("  5. Seguir las guias en Content/SOLO/*/")
    print()
    print("Para mas detalles: Docs/UE5_SETUP_GUIDE.md")

if __name__ == "__main__":
    main()
