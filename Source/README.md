# Source — código y organización

## Módulos de compilación

| Carpeta | Rol |
|---------|-----|
| **`EMBERVEIL/`** | Módulo de juego: depende de `EMBERVEILCore`, `EnhancedInput`, `UMG`. |
| **`EMBERVEILCore/`** | Núcleo de gameplay en C++ (`Public/` / `Private/`). |

Detalle de dependencias: `EMBERVEIL.Build.cs` y `EMBERVEILCore/EMBERVEILCore.Build.cs`.

## Targets (build)

Los targets viven en `Source/`:

- `EMBERVEIL.Target.cs` (Game)
- `EMBERVEILEditor.Target.cs` (Editor)

## Carpetas por dominio

`Audio/`, `Characters/`, `Core/`, `Items/`, `Systems/`, `UI/`, `World/` bajo `Source/`
sirven para **organización** y para **Blueprints o futuros módulos**. El C++ activo del
gameplay documentado en el README raíz está **concentrado en `EMBERVEILCore/`**.

## Lecturas relacionadas

- [`../README.md`](../README.md) — visión global.
- [`../Docs/ONBOARDING.md`](../Docs/ONBOARDING.md) — mapa de rutas.
- [`../CONTRIBUTING.md`](../CONTRIBUTING.md) — cómo contribuir.
- [`../AGENTS.md`](../AGENTS.md) — instrucciones para asistentes de IA.
