# Instrucciones para asistentes (IA y automatización)

Texto único de referencia para **agentes de IA**, revisores automáticos o integraciones que
deban respetar la arquitectura y la documentación de EMBERVEIL. Mantened este archivo
actualizado cuando cambien convenciones del equipo.

---

## Orden de lectura obligatorio

1. [`README.md`](README.md) — raíz del proyecto.
2. [`Docs/ONBOARDING.md`](Docs/ONBOARDING.md) — rutas y trampas habituales.
3. [`CONTRIBUTING.md`](CONTRIBUTING.md) — flujo Git, documentación y checklist.
4. [`Docs/ROADMAP_FASES.md`](Docs/ROADMAP_FASES.md) — si la tarea afecta alcance o prioridad.

No contradecir el **GDD** (`Design/GDD/GAME_DESIGN_DOCUMENT.md`) sin dejar constancia de que
es un cambio de diseño acordado.

---

## Límites de módulos (C++)

- **Gameplay y sistemas:** `Source/EMBERVEILCore/` (`Public/` / `Private/`).
- **Módulo de producto / ensamblaje:** `Source/EMBERVEIL/` — no duplicar lógica que ya
  pertenezca al Core sin motivo explícito.
- Respetad dependencias declaradas en `*.Build.cs` y plugins en `EMBERVEIL.uproject`.

---

## Reglas de proyecto (siempre)

- Leer contexto en `README.md`, `Docs/ONBOARDING.md` y `CONTRIBUTING.md` antes de cambios amplios.
- **C++ de gameplay** en `Source/EMBERVEILCore/`; módulo `EMBERVEIL` para ensamblaje sin duplicar el Core.
- **Prefijo** `EMBERVEIL` en tipos propios; widgets UMG en C++ con `WG_` donde ya se usa.
- **No** introducir secretos (tokens de servicios online, claves API, credenciales). No versionar `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`.
- Tras cambios de sistema: actualizar `Docs/Systems/` y el Changelog si afecta a otros roles; no contradecir el GDD sin dejarlo explícito.
- Plugins y módulos: respetar `EMBERVEIL.uproject` y `*.Build.cs` existentes.

---

## C++ (UE5) — bajo `Source/`

- Coincidir con el estilo del archivo tocado (tabs vs espacios según `.editorconfig` y código vecino).
- Incluir solo headers necesarios; respetar `Private/` vs `Public/` del módulo.
- **GAS:** habilidades y efectos coherentes con `GameplayAbilities` ya presentes; no acoplar lógica de negocio a widgets.
- **Red:** asumir **un jugador** como predeterminado; no romper el camino offline con dependencias duras de servicios online hasta FASE 2 (ver `Docs/ROADMAP_FASES.md`).
- Evitar refactors masivos no pedidos; cambios mínimos y revisables.

---

## Documentación tras cambios

Según [`CONTRIBUTING.md`](CONTRIBUTING.md): actualizar `Docs/Systems/`, Changelog y
`Docs/LEGAL_NOTICES.md` cuando corresponda.

---

## Conducta y seguridad

- [`CODE_OF_CONDUCT.md`](CODE_OF_CONDUCT.md)
- [`SECURITY.md`](SECURITY.md)
