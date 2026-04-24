# Roadmap por fases (FASE 0 / 1 / 2)

Documento de **prioridad de producto** para EMBERVEIL: qué **no puede faltar** para una primera salida jugable y coherente, qué **debe entrar** pero puede ir en iteraciones cortas, y qué **se aplaza** sin bloquear el camino actual (**un jugador primero**; multijugador “de verdad” más adelante).

**Cómo usarlo:** al planear un hito, etiquetad tareas con `FASE0` / `FASE1` / `FASE2`. Si el alcance cambia, **mové ítems entre fases** explícitamente (y una línea en el Changelog si afecta al equipo).

**Relación con diseño técnico:** detalle de implementación por sistema → `Docs/Systems/`; visión de juego → `Design/GDD/GAME_DESIGN_DOCUMENT.md`.

---

## Criterio rápido

| Fase | Pregunta guía |
|------|----------------|
| **FASE 0** | ¿Sin esto el juego **no se puede considerar “EMBERVEIL”** o **no se puede publicar** como experiencia de un jugador coherente? |
| **FASE 1** | ¿Mejora **clara** retención, claridad o calidad percibida, pero el juego **podría** salir sin ello en un early access muy honesto? |
| **FASE 2** | ¿Es **post-endgame**, **multijugador a escala**, **live ops pesado** o **contenido que asume** que el núcleo ya está maduro? |

---

## FASE 0 — Tiene que ir sí o sí (un jugador)

Lo que el **primer producto jugable comercial** (o demo larga muy seria) suele exigir. Sin esto, o el juego **no se entiende**, o **se rompe el bucle**, o **no hay confianza** en guardar progreso.

**Bucle y fantasía**

- Combate y movimiento **jugables de punta a punta** (input, feedback mínimo de impacto, derrota/victoria, no softlocks constantes).
- Progresión básica **comprensible** (nivel o equivalente, stats, equipamiento que se nota).
- **Contenido mínimo** que respalde la promesa del GDD (no hace falta el mundo entero, sí un **camino crítico** claro: exploración → combate → recompensa → siguiente paso).

**Persistencia y confianza**

- **Guardado y carga** fiables del personaje y del estado del mundo que importe para la v1.
- **Estabilidad:** cierres inesperados por debajo de un umbral razonable en el camino crítico.

**Sistemas que sostienen el bucle (según vuestro GDD)**

- Inventario / loot / equipamiento en la medida en que el diseño **los declare obligatorios** para el loop.
- Quests o objetivos **suficientes** para guiar al jugador si el juego no es sandbox puro.
- Crafting **solo** si la economía de materiales es parte del loop de la v1 (si ya está en FASE 0 por diseño, aquí queda consolidado y balanceado con el resto).

**Torre (ejemplo de lista interna)**

- Si la torre es **pilar del pitch**, FASE 0 incluye **una implementación jugable** de pisos (aunque sean pocos al inicio), con reglas claras de entrada/salida y recompensa — no hace falta tener los 100 pisos terminados; sí el **sistema** y **contenido mínimo** que demuestre el juego.

**Economía y gremio en v1 solo jugador**

- **Economía FASE 0:** intercambio con NPCs, precios, sinks y recompensas que **no rompan** el progreso (aunque sea simple).
- **Gremio FASE 0:** solo si en la v1 de **un jugador** el gremio es promesa central; si no, puede bajar a FASE 1 (buffos solos, lore) o FASE 2 (social fuerte). Ajustad según el GDD.

**Entrega**

- **Build** que se pueda empaquetar o distribuir para prueba (aunque sea solo Windows al principio).
- **Créditos / licencias** mínimos si usáis assets de terceros.

---

## FASE 1 — Tiene que ir, pero no es “indispensable” para el primer corte

Pequeñas y medianas mejoras que **suben mucho la barra** sin ser las que definen si “existe” el juego. Ideal para **parches** pre-lanzamiento o justo después del primer early access.

**Jugador**

- **FTUE / tutorial** pulido (primeros 15–30 min sin fricción innecesaria).
- **Accesibilidad básica** (tamaños de texto, remapeo razonable, opciones de dificultad si aplica).
- **Calidad de vida:** inventario más rápido, comparación de ítems, filtros, mapa, marcas de misión, etc.

**Contenido y variedad**

- Más biomas, enemigos, skills o variaciones de encuentros **sobre** el mínimo de FASE 0.
- **Audio** más completo (capas de combate, más variedad de SFX, música por contexto).

**Producción**

- **Herramientas internas** (debug, telemetría ligera, cheats de QA).
- **Optimización** puntual (FPS estable en hardware objetivo en zonas clave).
- **Documentación de sistemas** en `Docs/Systems/` para lo que ya tocáis a menudo.

**Economía / gremio “solo SP”**

- Profundidad extra (contratos, reputación, más NPCs) **sin** asumir multijugador todavía.

---

## FASE 2 — Más adelante (post–100+, multijugador serio, live ops grande)

Cosas **válidas** pero que **no deberían bloquear** el camino mientras el **un jugador** no esté sólido. Aquí encaja lo que dijiste: **post nivel 100** falta mucho; **multijugador** cuando el single player esté bastante avanzado.

**Progresión tardía**

- Meta de **post-juego** (transcendencia, T1–T5, etc. del GDD), rebalances masivos de endgame, sistemas que asumen **muchas horas** de cuenta.

**Multijugador y online “de verdad”**

- **Sesiones** estables, predicción o lockstep según diseño, servidor de autoridad o dedicados.
- **EOS / subsistemas** al nivel de cuenta: amigos, presencia, cloud save remoto si aplica.
- **Mercado entre jugadores** con anti-abuso, logs, rate limits, moderación mínima.
- **Gremio multiplayer:** baúl compartido, permisos, roles, instancias de gremio, etc.

**Operación y alcance**

- **Localización completa** a varios idiomas con revisión profesional.
- **Seasons**, rankings globales, eventos temporales a escala.
- **CI/CD** agresivo, entornos de staging, feature flags para producción.

**Contenido masivo**

- **PCG a gran escala**, world partition lleno, cantidades de contenido que solo tienen sentido con base técnica y de datos madura.

---

## Notas para no mezclar fases

1. **Escribid en el GDD** qué pertenece a “v1 un jugador” vs “expansión / fase 2”; este archivo es el **espejo de prioridad**, no sustituye al GDD.
2. Lo que implementéis **pensando en multijugador** en FASE 0–1 puede ser solo **acuerdos de diseño** (IDs de jugador, no hardcodear “solo local”, separar autoridad simulada) **sin** construir aún sesiones EOS.
3. Revisad esta lista **cada hito mayor**; lo que era FASE 1 a veces sube a FASE 0 cuando la fecha de salida se acerca.

---

## Referencia rápida a vuestra lista interna (ejemplo)

| Ítem típico | Nota de fase (orientativa; ajustad al GDD) |
|-------------|---------------------------------------------|
| Tower Floor Manager | Suele ser **FASE 0** si la torre es el corazón del pitch; el “100 pisos” completo puede ser **FASE 1** en contenido mientras el **sistema** sea FASE 0. |
| Crafting | Si ya está listo, pasad a **balance + integración económica** como FASE 0–1 según dependa el loop. |
| Economy / Marketplace | **Economía SP** → FASE 0–1; **mercado entre jugadores** → **FASE 2**. |
| Guild System | **Fantasía / buffs solo SP** → FASE 1; **gremio online y baúl compartido** → **FASE 2**. |

---

*Última actualización del documento: abril 2026.*
