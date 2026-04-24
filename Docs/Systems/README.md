# Documentación por sistema

¿Primera vez en el repo? Antes, **[Orientación al proyecto (ONBOARDING.md)](../ONBOARDING.md)**.

Aquí van las guías técnicas **por sistema** de EMBERVEIL: qué hace el sistema, dónde vive en el repo, dependencias (plugins, otros sistemas) y cómo extenderlo sin romper convenciones.

## Cómo añadir una guía nueva

1. Crear un archivo en esta carpeta con un nombre claro, por ejemplo `inventario.md`, `gas-combate.md`.
2. Incluir siempre estas secciones (pueden ser breves):
   - **Propósito** — qué problema resuelve para el jugador o para el equipo.
   - **Ubicación en código** — rutas bajo `Source/EMBERVEILCore/` (u otros módulos) y prefijos de clase relevantes.
   - **Datos y config** — `Config/`, Data Assets, JSON, Gameplay Tags, etc.
   - **Dependencias** — otros sistemas, plugins UE, red.
   - **Extensiones** — cómo añadir un caso nuevo (checklist).
3. Añadir una fila en la tabla de abajo con enlace relativo al archivo.

## Índice

| Sistema | Documento | Notas |
|---------|------------|--------|
| Menú y perfil local | [menu-y-perfil-local.md](menu-y-perfil-local.md) | Arranque, nombre visible, SaveGame local. |
| Torre | [torre.md](torre.md) | Piso actual, delegados, enlace a `TowerFloorReached`. |

## Convenciones del repo

- **C++ de gameplay:** módulo `EMBERVEILCore` (ver README raíz del proyecto).
- **Widgets UMG en C++:** prefijo `WG_` en nombres de clase/archivo cuando aplique.
- **Diseño de producto:** `Design/GDD/GAME_DESIGN_DOCUMENT.md`.
- **Datos en JSON:** `Config/Data/` (enemigos, ítems, skills; quests reservado).

Si una guía contradice el GDD, **alinead primero el diseño** y luego actualizad código o doc.
