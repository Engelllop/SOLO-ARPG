# Source â€” cÃ³digo y organizaciÃ³n

## MÃ³dulos de compilaciÃ³n

| Carpeta | Rol |
|---------|-----|
| **`SOLO/`** | MÃ³dulo de juego: depende de `SOLOCore`, `EnhancedInput`, `UMG`. |
| **`SOLOCore/`** | NÃºcleo de gameplay en C++ (`Public/` / `Private/`). |

Detalle de dependencias: `SOLO.Build.cs` y `SOLOCore/SOLOCore.Build.cs`.

## Targets (build)

Los targets viven en `Source/`:

- `SOLO.Target.cs` (Game)
- `SOLOEditor.Target.cs` (Editor)

## Carpetas por dominio

`Audio/`, `Characters/`, `Core/`, `Items/`, `Systems/`, `UI/`, `World/` bajo `Source/`
sirven para **organizaciÃ³n** y para **Blueprints o futuros mÃ³dulos**. El C++ activo del
gameplay documentado en el README raÃ­z estÃ¡ **concentrado en `SOLOCore/`**.

## Lecturas relacionadas

- [`../README.md`](../README.md) â€” visiÃ³n global.
- [`../Docs/ONBOARDING.md`](../Docs/ONBOARDING.md) â€” mapa de rutas.
- [`../CONTRIBUTING.md`](../CONTRIBUTING.md) â€” cÃ³mo contribuir.
- [`../AGENTS.md`](../AGENTS.md) â€” instrucciones para asistentes de IA.
