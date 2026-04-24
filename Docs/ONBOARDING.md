# Orientación al proyecto (no perderse)

Guía corta para **nuevas personas** o para volver al repo después de tiempo. El detalle de carpetas está en el [README](../README.md) raíz; aquí va el **mapa mental** y decisiones que suelen confundir.

---

## Primeros 15 minutos

1. Leer el [README](../README.md) (inicio rápido + módulos + plugins) y, si solo buscás docs, el índice [Docs/README.md](README.md).
2. Hojear el [GDD](../Design/GDD/GAME_DESIGN_DOCUMENT.md) para contexto de producto.
3. Si vas a **documentar un sistema**, leer [Systems/README.md](Systems/README.md) y añadir tu guía al índice.
4. Si vas a **anotar un cambio de build**, usar [Changelog/CHANGELOG.md](Changelog/CHANGELOG.md).
5. Si vas a **planear alcance** (qué va antes del lanzamiento vs después), leer [ROADMAP_FASES.md](ROADMAP_FASES.md).

---

## “Necesito…” → “Voy a…”

| Necesito… | Abro primero… |
|-----------|----------------|
| Entender qué es el juego y hasta dónde llega el diseño | `Design/GDD/GAME_DESIGN_DOCUMENT.md` |
| Lore, referencias narrativas | `Design/Lore/` (y el GDD) |
| Referencias de arte o audio de diseño | `Design/Art/`, `Design/Audio/` |
| Balance o tablas en JSON | `Config/Balance/`, `Config/Data/` |
| Gameplay Tags | `Config/DefaultGameplayTags.ini` |
| **C++ de gameplay** (GAS, inventario, UI, IA, etc.) | `Source/EMBERVEILCore/` → `Public/` / `Private/` |
| Definición del módulo UE (dependencias de build) | `Source/EMBERVEILCore/EMBERVEILCore.Build.cs` y `Source/EMBERVEIL/EMBERVEIL.Build.cs` |
| Motor, módulos y plugins del proyecto | `EMBERVEIL.uproject` |
| Arte “plano” o fuera del Content Browser (sprites, modelos fuente, etc.) | `Assets/` |
| Índice de guías técnicas por sistema | `Docs/Systems/README.md` |
| Menú / perfil local antes del gameplay | `Docs/Systems/menu-y-perfil-local.md` |
| Estado global de la torre (pisos) | `Docs/Systems/torre.md` |
| Historial de cambios del producto | `Docs/Changelog/CHANGELOG.md` |
| Prioridad FASE 0 / 1 / 2 (obligatorio vs después) | `Docs/ROADMAP_FASES.md` |
| Siglas y términos (GAS, EOS, …) | `Docs/GLOSSARIO.md` |
| Avisos legales / terceros (Epic, EOS) | `Docs/LEGAL_NOTICES.md` |
| Índice maestro de documentación | `Docs/README.md` |

---

## Trampas habituales

- **`Content/` existe pero puede estar vacío.** En Unreal, el contenido del editor vive en `Content/` (assets `.uasset`). Este repo también usa **`Assets/`** para material fuente o referencias fuera del Content Browser. Si el equipo decide versionar mucho contenido pesado, considerad Git LFS y documentadlo.
- **`Source/` tiene carpetas “por dominio”** (Characters, Systems, World…) que sirven de **organización y futuro BP**; el **C++ que ya compila para gameplay** está sobre todo en **`EMBERVEILCore`**, no disperso en esas carpetas.
- **`Config/Data/Quests/`** existe como lugar reservado; los JSON de quests pueden añadirse cuando haya formato acordado (y enlazarse desde el GDD y desde aquí).
- **`Docs/API/`** es para referencia generada o índices; las explicaciones narrativas van mejor en **`Docs/Systems/`**.

---

## Siglas útiles (UE / este repo)

- **GAS** — Gameplay Ability System (`GameplayAbilities` plugin).
- **GA / GE** — Gameplay Ability / Gameplay Effect (términos UE).
- **EOS** — Epic Online Services (plugin `OnlineSubsystemEOS` en el `.uproject`).
- **`WG_`** — prefijo habitual de widgets UMG en C++ en este proyecto (ver README raíz).

---

## Flujo mínimo de documentación

1. Cambio **de diseño** → reflejarlo en el GDD o en `Design/` si es lore/arte.
2. Cambio **de sistema técnico** → guía en `Docs/Systems/` + fila en su README + línea en el Changelog si es visible para el equipo o QA.
3. Cambio **solo de datos** → JSON o balance en `Config/` + Changelog si afecta gameplay notable.

Si algo de esta guía deja de ser verdad (por ejemplo, aparece `Content/`), **actualizad este archivo** en el mismo PR o entrega.
