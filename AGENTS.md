# Instrucciones para asistentes (IA y automatizaciÃ³n)

Texto Ãºnico de referencia para **agentes de IA**, revisores automÃ¡ticos o integraciones que
deban respetar la arquitectura y la documentaciÃ³n de SOLO. Mantened este archivo
actualizado cuando cambien convenciones del equipo.

---

## Orden de lectura obligatorio

1. [`README.md`](README.md) â€” raÃ­z del proyecto.
2. [`Docs/ONBOARDING.md`](Docs/ONBOARDING.md) â€” rutas y trampas habituales.
3. [`CONTRIBUTING.md`](CONTRIBUTING.md) â€” flujo Git, documentaciÃ³n y checklist.
4. [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) â€” si la tarea afecta alcance o prioridad.

No contradecir el **GDD** (`Design/GDD/GAME_DESIGN_DOCUMENT.md`) sin dejar constancia de que
es un cambio de diseÃ±o acordado.

---

## LÃ­mites de mÃ³dulos (C++)

- **Gameplay y sistemas:** `Source/SOLOCore/` (`Public/` / `Private/`).
- **MÃ³dulo de producto / ensamblaje:** `Source/SOLO/` â€” no duplicar lÃ³gica que ya
  pertenezca al Core sin motivo explÃ­cito.
- Respetad dependencias declaradas en `*.Build.cs` y plugins en `SOLO.uproject`.

---

## Reglas de proyecto (siempre)

- Leer contexto en `README.md`, `Docs/ONBOARDING.md` y `CONTRIBUTING.md` antes de cambios amplios.
- **C++ de gameplay** en `Source/SOLOCore/`; mÃ³dulo `SOLO` para ensamblaje sin duplicar el Core.
- **Prefijo** `SOLO` en tipos propios; widgets UMG en C++ con `WG_` donde ya se usa.
- **No** introducir secretos (tokens de servicios online, claves API, credenciales). No versionar `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`.
- Tras cambios de sistema: actualizar `Docs/Systems/` y el Changelog si afecta a otros roles; no contradecir el GDD sin dejarlo explÃ­cito.
- Plugins y mÃ³dulos: respetar `SOLO.uproject` y `*.Build.cs` existentes.

---

## C++ (UE5) â€” bajo `Source/`

- Coincidir con el estilo del archivo tocado (tabs vs espacios segÃºn `.editorconfig` y cÃ³digo vecino).
- Incluir solo headers necesarios; respetar `Private/` vs `Public/` del mÃ³dulo.
- **GAS:** habilidades y efectos coherentes con `GameplayAbilities` ya presentes; no acoplar lÃ³gica de negocio a widgets.
- **Red:** asumir **un jugador** como predeterminado; no romper el camino offline con dependencias duras de servicios online hasta FASE 2 (ver `Docs/ROADMAP_FASES.md`).
- Evitar refactors masivos no pedidos; cambios mÃ­nimos y revisables.

---

## DocumentaciÃ³n tras cambios

SegÃºn [`CONTRIBUTING.md`](CONTRIBUTING.md): actualizar `Docs/Systems/`, Changelog y
`Docs/LEGAL_NOTICES.md` cuando corresponda.

---

## Conducta y seguridad

- [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md)
- [`SECURITY.md`](SECURITY.md)
