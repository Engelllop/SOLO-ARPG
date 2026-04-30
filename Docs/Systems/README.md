# DocumentaciÃ³n por sistema

Â¿Primera vez en el repo? Antes, **[OrientaciÃ³n al proyecto (ONBOARDING.md)](../ONBOARDING.md)**.

AquÃ­ van las guÃ­as tÃ©cnicas **por sistema** de SOLO: quÃ© hace el sistema, dÃ³nde vive en el repo, dependencias (plugins, otros sistemas) y cÃ³mo extenderlo sin romper convenciones.

## CÃ³mo aÃ±adir una guÃ­a nueva

1. Crear un archivo en esta carpeta con un nombre claro, por ejemplo `inventario.md`, `gas-combate.md`.
2. Incluir siempre estas secciones (pueden ser breves):
   - **PropÃ³sito** â€” quÃ© problema resuelve para el jugador o para el equipo.
   - **UbicaciÃ³n en cÃ³digo** â€” rutas bajo `Source/SOLOCore/` (u otros mÃ³dulos) y prefijos de clase relevantes.
   - **Datos y config** â€” `Config/`, Data Assets, JSON, Gameplay Tags, etc.
   - **Dependencias** â€” otros sistemas, plugins UE, red.
   - **Extensiones** â€” cÃ³mo aÃ±adir un caso nuevo (checklist).
3. AÃ±adir una fila en la tabla de abajo con enlace relativo al archivo.

## Ãndice

| Sistema | Documento | Notas |
|---------|------------|--------|
| MenÃº y perfil local | [menu-y-perfil-local.md](menu-y-perfil-local.md) | Arranque, nombre visible, SaveGame local. |
| Torre | [torre.md](torre.md) | Piso actual, delegados, enlace a `TowerFloorReached`. |

## Convenciones del repo

- **C++ de gameplay:** mÃ³dulo `SOLOCore` (ver README raÃ­z del proyecto).
- **Widgets UMG en C++:** prefijo `WG_` en nombres de clase/archivo cuando aplique.
- **DiseÃ±o de producto:** `Design/GDD/GAME_DESIGN_DOCUMENT.md`.
- **Datos en JSON:** `Config/Data/` (enemigos, Ã­tems, skills; quests reservado).

Si una guÃ­a contradice el GDD, **alinead primero el diseÃ±o** y luego actualizad cÃ³digo o doc.
