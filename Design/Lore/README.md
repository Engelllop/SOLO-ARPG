# Solo — Lore Bible

Documento vivo. Estos archivos son la **biblia narrativa** del juego: la fuente única de verdad sobre cosmología, antagonista, mundo y reglas internas. Toda quest, diálogo de NPC, descripción de ítem y cinemática debe respetar lo escrito aquí.

> Para detalles mecánicos del juego, ver [`Design/GDD/GAME_DESIGN_DOCUMENT.md`](../GDD/GAME_DESIGN_DOCUMENT.md).
> Para fases de producción, ver [`Docs/ROADMAP_FASES.md`](../../Docs/ROADMAP_FASES.md).

---

## Premisa de una línea

> **El mundo es un borrador. La Torre es el procesador. El Arquitecto compila.**

---

## Tono

- **Misterio metafísico, no fantasía épica al uso.** El jugador empieza creyendo que está en un mundo medieval mágico común y descubre, capa a capa, que está dentro de un proceso.
- **Lenguaje evocativo, no técnico.** Los conceptos de "compilación", "papelera" y "borrador" **nunca** se mencionan literalmente in-universe. Se hablan en metáforas religiosas, místicas o folclóricas. Los sabios hablan de *El Tejido*, *El Patrón*, *Las Esquirlas*, *La Última Iteración*. Los campesinos hablan de *un dios que reescribe el mundo cada mil años*.
- **El metalenguaje informático está reservado para el Acto V** (pisos 76–100), cuando el velo se rompe y el jugador empieza a oír a El Arquitecto hablar en términos extraños — *"build"*, *"merge"*, *"resolución"*, *"diff"*. Antes de eso, ni una palabra.
- **Esperanza amarga.** El mundo está roto y todos lo saben en algún nivel; pero la gente sigue amando, comerciando, peleando. La torre no es un castigo: es una oportunidad.

---

## Estructura de archivos

| Archivo | Contenido |
|---|---|
| [`00_la_compilacion.md`](00_la_compilacion.md) | Cosmología base. Qué es el mundo, qué es la Torre, qué es la Compilación. |
| [`01_el_arquitecto.md`](01_el_arquitecto.md) | El antagonista cósmico. Quién es, por qué reescribe, qué quiere. |
| [`02_el_borrador.md`](02_el_borrador.md) | El mundo como borrador. Razas como ramas. Tierra Corrupta = bug. |
| [`03_la_torre.md`](03_la_torre.md) | Naturaleza de la Torre. 100 pisos, 4 Actos. RPG = pisos N0-N10. |
| [`04_isla_de_la_nada.md`](04_isla_de_la_nada.md) | La papelera del universo. Qué hay allí. Quién la habita. |
| [`05_folklore_primer_ataque.md`](05_folklore_primer_ataque.md) | El sistema *Primer Ataque* y su forma in-world (tablones, estatuas, refranes). |
| [`06_el_protagonista.md`](06_el_protagonista.md) | Identidad canon del Persistente. Lo que sí y lo que no es. |
| [`07_companeros.md`](07_companeros.md) | Caín, Mira, Vassen, Sera, Garrik. NPCs recurrentes (Hadda, Cael, Custodio, Veteranos). |
| [`08_acto_i_arco_inicial.md`](08_acto_i_arco_inicial.md) | Outline del Acto I (pre-Torre) en 5 capítulos, beat por beat. |
| [`99_glosario.md`](99_glosario.md) | Términos in-universe. **Léxico oficial.** Lo que sí se dice y lo que jamás. |
| [`bosses/`](bosses/) | Lore individual de los 10 jefes de piso MMO. **Completados.** (P10-P100) |
| [`bosses_rpg/`](bosses_rpg/) | Lore de los 10 jefes del RPG base. **Completados.** (N0-N10) |
| [`classes/`](classes/) | Las 12 clases jugables con roles, mecánicas, subclases. |

---

## Reglas de coherencia (no romper sin discusión)

1. **Ningún personaje del Acto I–IV sabe que el mundo es una compilación.** Lo intuyen como herejía, mito o demencia.
2. **El Arquitecto no es maligno por elección.** Es metódico. Esa es su tragedia.
3. **La muerte recolecta datos.** Las almas que mueren en el Borrador retornan al Tejido y enriquecen la próxima iteración. Por eso la resurrección es trivial in-game pero los sabios la temen.
4. **Cada raza es una hipótesis.** Ninguna es "la correcta". El Arquitecto está probando cuál sobrevive.
5. **La Torre nunca cambia, el mundo sí.** Cada iteración del Borrador es distinta; la Torre es la única constante.
6. **El protagonista es una anomalía persistente.** Algo en él/ella se mantiene entre iteraciones, y eso es lo que el Arquitecto está investigando — o intentando borrar.

---

## Decisiones de diseño — RPG base (N0-N10)

> El juego base es un RPG single-player de ~20h. La Torre de 100 pisos y el contenido MMO se desarrollan después.

### Estructura del RPG
- **N0 — Emberveil:** Ciudad overworld donde inicia todo. Jefe: El Errante de Emberveil.
- **N1-N10:** Pisos de la Torre. Ciudades cada 2 pisos (N2=Velmar, N4=Aelthar, N6=Sahal, N8=Dum, N10=Nynth). Los pisos impares son biomas salvajes con campamento.
- **Portales:** Se puede bajar libremente. Para subir hay que derrotar al jefe del piso anterior otra vez (cooldown de reaparición).
- **Jefe final RPG:** El Testigo (N10). La puerta al Piso 11 se abre después = contenido post-RPG.

### Compañeros del RPG
- **Caín** (amigo de infancia) — muere en N1 (Madre del Nido), no evitable.
- **Mira** (cartógrafa) — muere en N7 (La Costurera), no evitable.
- **Vassen** (medio-orco templario) — muere en N7 (La Costurera), no evitable.
- **Sera** (eco del Tejido, alegre) — muere en N10 (El Testigo), no evitable.
- **Garrik** (niño que no crece) — sobrevive. Único que llega al final.

### Final del RPG
El protagonista derrota al Testigo y la puerta al Piso 11 se abre. No hay créditos. El juego continúa.

### Clases
12 clases jugables, independientes de la raza. Detalle completo en [`classes/`](classes/).

### Jefes nuevos (RPG)
10 jefes del N0 al N10. Lore completo en [`bosses_rpg/`](bosses_rpg/). Los 10 jefes originales (Centinela → Arquitecto) quedan intactos para el contenido post-RPG.