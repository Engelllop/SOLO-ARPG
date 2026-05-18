# Guia de Setup para UE5.7 - SOLO-ARPG

## Requisitos

- Unreal Engine 5.7
- Visual Studio 2022 (para compilar C++)
- Este repo clonado

## Pasos

### 1. Generar archivos de Visual Studio

```bash
# Click derecho en SOLO.uproject -> Generate Visual Studio project files
# O desde terminal:
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%CD%\SOLO.uproject" -game -engine -progress
```

### 2. Compilar el proyecto

- Abrir SOLO.sln en Visual Studio 2022
- Seleccionar configuracion: Development Editor
- Build -> Build Solution (Ctrl+Shift+B)

### 3. Abrir en Unreal Editor

- Doble click en SOLO.uproject
- Esperar a que compile shaders inicial

### 4. Crear Content (usar guias en Content/SOLO/)

- Content/SOLO/Input/INPUT_SETUP.txt
- Content/SOLO/Maps/MAP_SETUP.txt
- Archivos _CREAR_EN_EDITOR.txt en cada carpeta

### 5. Configurar Input

- Project Settings -> Input -> Default Classes
  - Default Player Input Class: EnhancedPlayerInput
  - Default Input Component Class: EnhancedInputComponent
- Verificar que DefaultInput.ini tenga estas clases

### 6. Probar

- Play -> Selected Viewport
- WASD para mover, mouse para camara
- Click izquierdo para atacar
- Space para esquivar

## Troubleshooting

### Error: 'Could not find module SOLOCore'
- Recompilar en Visual Studio
- Verificar que SOLOCore.Build.cs tenga las dependencias correctas

### Error: 'Blueprint not found'
- Crear los Blueprints segun las guias en Content/SOLO/
- Verificar que las rutas coincidan con SOLOGameMode.cpp

### Error: 'Input not working'
- Verificar que IMC_SOLO_Default este creado y asignado
- Verificar que Input Actions tengan las teclas correctas
- Verificar que el PlayerController use Enhanced Input

