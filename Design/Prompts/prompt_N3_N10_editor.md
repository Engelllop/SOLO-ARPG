# PROMPTS DE EDITOR — N3 a N10

## Archivos Individuales

Cada piso tiene su propio prompt detallado en:

| Piso | Tema | Archivo |
|------|------|---------|
| **N3** Aelthar | Tráfico infantil | `prompt_N3_editor.md` |
| **N4** Bosque Petrificado | Canibalismo forzado | `prompt_N4_editor.md` |
| **N5** Thornwall | Esclavitud sexual | `prompt_N5_editor.md` |
| **N6** Cámaras de Jade | Explotación infantil industrial | `prompt_N6_editor.md` |
| **N7** Sahal | Genocidio / Desmembramiento | `prompt_N7_editor.md` |
| **N8** Jardines Flotantes | Eugenesia | `prompt_N8_editor.md` |
| **N9** Argentia | Tortura política | `prompt_N9_editor.md` |
| **N10** Centinela | El mal como sistema | `prompt_N10_editor.md` |

## Archivos de Código C++ (ya compilados)

Contenido en carpeta: `Source/SOLOCore/Public/Character/N3/` a `N10/`.
Y `Source/SOLOCore/Public/Character/N3/NPCs/`, `World/N3/`, etc.

Todos los 35 archivos compilan sin errores en UE 5.7.

## Orden de Compilación Recomendado

1. N3 (Aelthar) — introducción de mecánicas de espejo + Garrik NPC
2. N4 (Bosque) — sistema de hambre, puzzle de corazón
3. N5 (Thornwall) — combate directo con Carnicero
4. N6 (Jade) — sistemas PTSD, ChildSingers/ChildMiners
5. N7 (Sahal) — boss más complejo con cutscenes guionizadas
6. N8 (Jardines) — diálogos de elección con Criada Bronce
7. N9 (Argentia) — decisiones que afectan Ending B
8. N10 (Final) — secuencia final sin combate

## Requisitos Técnicos

- Engine: UE 5.7
- Proyecto: SOLO.uproject
- Source: `Source/SOLOCore/`
- AnimBlueprint existente para ThirdPersonCharacter debe tener attach de sistema de hambre y PTSD
- Cada nivel debe configurarse con PostProcessVolume, LightmassImportanceVolume

## Notas de Compilación

Si al agregar BlueprintImplementableEvents en Blueprints existen errores:
- Asegurar que están declarados en protected: (no public:)
- .generated.h debe ser el último include
- Si un BlueprintNativeEvent no se implementa, el motor no crashea — usa la versión _Implementation por defecto
