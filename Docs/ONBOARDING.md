# OrientaciÃ³n al proyecto (no perderse)

GuÃ­a corta para **nuevas personas** o para volver al repo despuÃ©s de tiempo. El detalle de carpetas estÃ¡ en el [README](../README.md) raÃ­z; aquÃ­ va el **mapa mental** y decisiones que suelen confundir.

---

## Primeros 15 minutos

1. Leer el [README](../README.md) (inicio rÃ¡pido + mÃ³dulos + plugins) y, si solo buscÃ¡s docs, el Ã­ndice [Docs/README.md](README.md).
2. Hojear el [GDD](../Design/GDD/GAME_DESIGN_DOCUMENT.md) para contexto de producto.
3. Si vas a **documentar un sistema**, leer [Systems/README.md](Systems/README.md) y aÃ±adir tu guÃ­a al Ã­ndice.
4. Si vas a **anotar un cambio de build**, usar [Changelog/CHANGELOG.md](Changelog/CHANGELOG.md).
5. Si vas a **planear alcance** (quÃ© va antes del lanzamiento vs despuÃ©s), leer [ROADMAP_FASES.md](ROADMAP_FASES.md).

---

## â€œNecesitoâ€¦â€ â†’ â€œVoy aâ€¦â€

| Necesitoâ€¦ | Abro primeroâ€¦ |
|-----------|----------------|
| Entender quÃ© es el juego y hasta dÃ³nde llega el diseÃ±o | `Design/GDD/GAME_DESIGN_DOCUMENT.md` |
| Lore, referencias narrativas | `Design/Lore/` (y el GDD) |
| Referencias de arte o audio de diseÃ±o | `Design/Art/`, `Design/Audio/` |
| Balance o tablas en JSON | `Config/Balance/`, `Config/Data/` |
| Gameplay Tags | `Config/DefaultGameplayTags.ini` |
| **C++ de gameplay** (GAS, inventario, UI, IA, etc.) | `Source/SOLOCore/` â†’ `Public/` / `Private/` |
| DefiniciÃ³n del mÃ³dulo UE (dependencias de build) | `Source/SOLOCore/SOLOCore.Build.cs` y `Source/Solo/Solo.Build.cs` |
| Motor, mÃ³dulos y plugins del proyecto | `Solo.uproject` |
| Arte â€œplanoâ€ o fuera del Content Browser (sprites, modelos fuente, etc.) | `Assets/` |
| Ãndice de guÃ­as tÃ©cnicas por sistema | `Docs/Systems/README.md` |
| MenÃº / perfil local antes del gameplay | `Docs/Systems/menu-y-perfil-local.md` |
| Estado global de la torre (pisos) | `Docs/Systems/torre.md` |
| Historial de cambios del producto | `Docs/Changelog/CHANGELOG.md` |
| Prioridad FASE 0 / 1 / 2 (obligatorio vs despuÃ©s) | `Docs/ROADMAP_FASES.md` |
| Siglas y tÃ©rminos (GAS, EOS, â€¦) | `Docs/GLOSSARIO.md` |
| Avisos legales / terceros (Epic, EOS) | `Docs/LEGAL_NOTICES.md` |
| Ãndice maestro de documentaciÃ³n | `Docs/README.md` |

---

## Trampas habituales

- **`Content/` existe pero puede estar vacÃ­o.** En Unreal, el contenido del editor vive en `Content/` (assets `.uasset`). Este repo tambiÃ©n usa **`Assets/`** para material fuente o referencias fuera del Content Browser. Si el equipo decide versionar mucho contenido pesado, considerad Git LFS y documentadlo.
- **`Source/` tiene carpetas â€œpor dominioâ€** (Characters, Systems, Worldâ€¦) que sirven de **organizaciÃ³n y futuro BP**; el **C++ que ya compila para gameplay** estÃ¡ sobre todo en **`SOLOCore`**, no disperso en esas carpetas.
- **`Config/Data/Quests/`** existe como lugar reservado; los JSON de quests pueden aÃ±adirse cuando haya formato acordado (y enlazarse desde el GDD y desde aquÃ­).
- **`Docs/API/`** es para referencia generada o Ã­ndices; las explicaciones narrativas van mejor en **`Docs/Systems/`**.

---

## Siglas Ãºtiles (UE / este repo)

- **GAS** â€” Gameplay Ability System (`GameplayAbilities` plugin).
- **GA / GE** â€” Gameplay Ability / Gameplay Effect (tÃ©rminos UE).
- **EOS** â€” Epic Online Services (plugin `OnlineSubsystemEOS` en el `.uproject`).
- **`WG_`** â€” prefijo habitual de widgets UMG en C++ en este proyecto (ver README raÃ­z).

---

## Flujo mÃ­nimo de documentaciÃ³n

1. Cambio **de diseÃ±o** â†’ reflejarlo en el GDD o en `Design/` si es lore/arte.
2. Cambio **de sistema tÃ©cnico** â†’ guÃ­a en `Docs/Systems/` + fila en su README + lÃ­nea en el Changelog si es visible para el equipo o QA.
3. Cambio **solo de datos** â†’ JSON o balance en `Config/` + Changelog si afecta gameplay notable.

Si algo de esta guÃ­a deja de ser verdad (por ejemplo, aparece `Content/`), **actualizad este archivo** en el mismo PR o entrega.