# Roadmap por fases (FASE 0 / 1 / 2)

Documento de **prioridad de producto** para Solo: quÃ© **no puede faltar** para una primera salida jugable y coherente, quÃ© **debe entrar** pero puede ir en iteraciones cortas, y quÃ© **se aplaza** sin bloquear el camino actual (**un jugador primero**; multijugador â€œde verdadâ€ mÃ¡s adelante).

**CÃ³mo usarlo:** al planear un hito, etiquetad tareas con `FASE0` / `FASE1` / `FASE2`. Si el alcance cambia, **movÃ© Ã­tems entre fases** explÃ­citamente (y una lÃ­nea en el Changelog si afecta al equipo).

**RelaciÃ³n con diseÃ±o tÃ©cnico:** detalle de implementaciÃ³n por sistema â†’ `Docs/Systems/`; visiÃ³n de juego â†’ `Design/GDD/GAME_DESIGN_DOCUMENT.md`.

---

## Criterio rÃ¡pido

| Fase | Pregunta guÃ­a |
|------|----------------|
| **FASE 0** | Â¿Sin esto el juego **no se puede considerar â€œSoloâ€** o **no se puede publicar** como experiencia de un jugador coherente? |
| **FASE 1** | Â¿Mejora **clara** retenciÃ³n, claridad o calidad percibida, pero el juego **podrÃ­a** salir sin ello en un early access muy honesto? |
| **FASE 2** | Â¿Es **post-endgame**, **multijugador a escala**, **live ops pesado** o **contenido que asume** que el nÃºcleo ya estÃ¡ maduro? |

---

## FASE 0 â€” Tiene que ir sÃ­ o sÃ­ (un jugador)

Lo que el **primer producto jugable comercial** (o demo larga muy seria) suele exigir. Sin esto, o el juego **no se entiende**, o **se rompe el bucle**, o **no hay confianza** en guardar progreso.

**Bucle y fantasÃ­a**

- Combate y movimiento **jugables de punta a punta** (input, feedback mÃ­nimo de impacto, derrota/victoria, no softlocks constantes).
- ProgresiÃ³n bÃ¡sica **comprensible** (nivel o equivalente, stats, equipamiento que se nota).
- **Contenido mÃ­nimo** que respalde la promesa del GDD (no hace falta el mundo entero, sÃ­ un **camino crÃ­tico** claro: exploraciÃ³n â†’ combate â†’ recompensa â†’ siguiente paso).

**Persistencia y confianza**

- **Guardado y carga** fiables del personaje y del estado del mundo que importe para la v1.
- **Estabilidad:** cierres inesperados por debajo de un umbral razonable en el camino crÃ­tico.

**Sistemas que sostienen el bucle (segÃºn vuestro GDD)**

- Inventario / loot / equipamiento en la medida en que el diseÃ±o **los declare obligatorios** para el loop.
- Quests o objetivos **suficientes** para guiar al jugador si el juego no es sandbox puro.
- Crafting **solo** si la economÃ­a de materiales es parte del loop de la v1 (si ya estÃ¡ en FASE 0 por diseÃ±o, aquÃ­ queda consolidado y balanceado con el resto).

**Torre (ejemplo de lista interna)**

- Si la torre es **pilar del pitch**, FASE 0 incluye **una implementaciÃ³n jugable** de pisos (aunque sean pocos al inicio), con reglas claras de entrada/salida y recompensa â€” no hace falta tener los 100 pisos terminados; sÃ­ el **sistema** y **contenido mÃ­nimo** que demuestre el juego.

**EconomÃ­a y gremio en v1 solo jugador**

- **EconomÃ­a FASE 0:** intercambio con NPCs, precios, sinks y recompensas que **no rompan** el progreso (aunque sea simple).
- **Gremio FASE 0:** solo si en la v1 de **un jugador** el gremio es promesa central; si no, puede bajar a FASE 1 (buffos solos, lore) o FASE 2 (social fuerte). Ajustad segÃºn el GDD.

**Entrega**

- **Build** que se pueda empaquetar o distribuir para prueba (aunque sea solo Windows al principio).
- **CrÃ©ditos / licencias** mÃ­nimos si usÃ¡is assets de terceros.

---

## FASE 1 â€” Tiene que ir, pero no es â€œindispensableâ€ para el primer corte

PequeÃ±as y medianas mejoras que **suben mucho la barra** sin ser las que definen si â€œexisteâ€ el juego. Ideal para **parches** pre-lanzamiento o justo despuÃ©s del primer early access.

**Jugador**

- **FTUE / tutorial** pulido (primeros 15â€“30 min sin fricciÃ³n innecesaria).
- **Accesibilidad bÃ¡sica** (tamaÃ±os de texto, remapeo razonable, opciones de dificultad si aplica).
- **Calidad de vida:** inventario mÃ¡s rÃ¡pido, comparaciÃ³n de Ã­tems, filtros, mapa, marcas de misiÃ³n, etc.

**Contenido y variedad**

- MÃ¡s biomas, enemigos, skills o variaciones de encuentros **sobre** el mÃ­nimo de FASE 0.
- **Audio** mÃ¡s completo (capas de combate, mÃ¡s variedad de SFX, mÃºsica por contexto).

**ProducciÃ³n**

- **Herramientas internas** (debug, telemetrÃ­a ligera, cheats de QA).
- **OptimizaciÃ³n** puntual (FPS estable en hardware objetivo en zonas clave).
- **DocumentaciÃ³n de sistemas** en `Docs/Systems/` para lo que ya tocÃ¡is a menudo.

**EconomÃ­a / gremio â€œsolo SPâ€**

- Profundidad extra (contratos, reputaciÃ³n, mÃ¡s NPCs) **sin** asumir multijugador todavÃ­a.

---

## FASE 2 â€” MÃ¡s adelante (postâ€“100+, multijugador serio, live ops grande)

Cosas **vÃ¡lidas** pero que **no deberÃ­an bloquear** el camino mientras el **un jugador** no estÃ© sÃ³lido. AquÃ­ encaja lo que dijiste: **post nivel 100** falta mucho; **multijugador** cuando el single player estÃ© bastante avanzado.

**ProgresiÃ³n tardÃ­a**

- Meta de **post-juego** (transcendencia, T1â€“T5, etc. del GDD), rebalances masivos de endgame, sistemas que asumen **muchas horas** de cuenta.

**Multijugador y online â€œde verdadâ€**

- **Sesiones** estables, predicciÃ³n o lockstep segÃºn diseÃ±o, servidor de autoridad o dedicados.
- **EOS / subsistemas** al nivel de cuenta: amigos, presencia, cloud save remoto si aplica.
- **Mercado entre jugadores** con anti-abuso, logs, rate limits, moderaciÃ³n mÃ­nima.
- **Gremio multiplayer:** baÃºl compartido, permisos, roles, instancias de gremio, etc.

**OperaciÃ³n y alcance**

- **LocalizaciÃ³n completa** a varios idiomas con revisiÃ³n profesional.
- **Seasons**, rankings globales, eventos temporales a escala.
- **CI/CD** agresivo, entornos de staging, feature flags para producciÃ³n.

**Contenido masivo**

- **PCG a gran escala**, world partition lleno, cantidades de contenido que solo tienen sentido con base tÃ©cnica y de datos madura.

---

## Notas para no mezclar fases

1. **Escribid en el GDD** quÃ© pertenece a â€œv1 un jugadorâ€ vs â€œexpansiÃ³n / fase 2â€; este archivo es el **espejo de prioridad**, no sustituye al GDD.
2. Lo que implementÃ©is **pensando en multijugador** en FASE 0â€“1 puede ser solo **acuerdos de diseÃ±o** (IDs de jugador, no hardcodear â€œsolo localâ€, separar autoridad simulada) **sin** construir aÃºn sesiones EOS.
3. Revisad esta lista **cada hito mayor**; lo que era FASE 1 a veces sube a FASE 0 cuando la fecha de salida se acerca.

---

## Referencia rÃ¡pida a vuestra lista interna (ejemplo)

| Ãtem tÃ­pico | Nota de fase (orientativa; ajustad al GDD) |
|-------------|---------------------------------------------|
| Tower Floor Manager | Suele ser **FASE 0** si la torre es el corazÃ³n del pitch; el â€œ100 pisosâ€ completo puede ser **FASE 1** en contenido mientras el **sistema** sea FASE 0. |
| Crafting | Si ya estÃ¡ listo, pasad a **balance + integraciÃ³n econÃ³mica** como FASE 0â€“1 segÃºn dependa el loop. |
| Economy / Marketplace | **EconomÃ­a SP** â†’ FASE 0â€“1; **mercado entre jugadores** â†’ **FASE 2**. |
| Guild System | **FantasÃ­a / buffs solo SP** â†’ FASE 1; **gremio online y baÃºl compartido** â†’ **FASE 2**. |

---

*Ãšltima actualizaciÃ³n del documento: abril 2026.*