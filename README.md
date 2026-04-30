# SOLO — Sunlight of the Last Online

**RPG medieval oscuro.** Unreal Engine 5.7. Un aventurero, una Torre, dos finales. Sin elegido, sin salvador, sin plot armor.

---

## ⚡ En Una Línea

Entrás a una Torre que reemplazó al mundo. Subís. Perdés gente. Llegás a la cima. Elegís: salvar el mundo exterior o quedarte a reconstruir la Torre.

---

## 📊 Stats del Proyecto

| Métrica | Valor |
|---------|-------|
| Archivos de diseño | **73** |
| Tamaño total diseño | **~1.5 MB** |
| Pisos (N0-N10) | **11** |
| Prompts para Claude Code | **39** |
| Jefes de piso | **11** (más jefes legendarios) |
| Clases jugables | **12** |
| Hechizos | **28** (4 escuelas) |
| Misiones principales | **12** |
| Misiones secundarias | **24** |
| NPCs con nombre | **35+** |
| Enemigos | **38** (con entradas de lore) |
| Fogatas | **26** |
| Eventos de mundo | **23** |

---

## 🗺️ Estructura de la Torre

```
N0  — Emberveil (Overworld — 2km²)
       ↓ Guardián de la Primera Garganta — CAÍN MUERE
N1  — Velmar (Ciudad Colgante) — La Voz
       ↓
N2  — Pastor de Cenizas (Llanuras Quemadas)
       ↓
N3  — Aelthar (Ciudad Elfa del Lago) — REENCUENTRO GARRIK / HADDA
       ↓
N4  — Corredor de los Nombres (Cañón de Piedra)
       ↓
N5  — Thornwall (Ciudad Fortaleza)
       ↓
N6  — Cámaras de Jade (Cavernas de Cristal)
       ↓
N7  — Sahal (Ciudad Desierto) — MIRA Y VASSEN MUEREN / LA COSTURERA
       ↓
N8  — Jardines Flotantes (Plataformas Verticales) — APARECE SERA
       ↓
N9  — Argentia (Ciudad de Plata)
       ↓
N10 — El Centinela de Granito (Arena Final)
       ├── Final A: El Escape (sol real, esperanza)
       └── Final B: El Nuevo Arquitecto (quedarse, reconstruir)
```

---

## 🧍 Compañeros

| Personaje | Arma | Aparece | Muere | Nota |
|-----------|------|---------|-------|------|
| **Caín** | Espada ancha | Inicio | N0 (Guardián Garganta) | Amigo de infancia. Su muerte desbloquea Cortes Simultáneos |
| **Mira** | Arco/flechas | Inicio | N7 (Costurera) | Despierta Flecha del Tejido en N0 |
| **Vassen** | Escudo/lanza | Inicio | N7 (Costurera) | La más fuerte del grupo. Callada. |
| **Garrik** | Hojas gemelas | N3 (Aelthar) | — | Sobrevive. Pícaro, amigo de la infancia |
| **Sera** | Katana (velocidad) | N8 (Jardines) | — | Sobrevive. Eco del Tejido. Posible romance. |

---

## ⚔️ Sistema de Combate

- **Atributos:** HP, MP, SP (Stamina), Fuerza, Destreza, Inteligencia, Constitución
- **Daño:** 10 tipos (Corte, Perforación, Contundente, Fuego, Hielo, Eléctrico, Oscuridad, Luz, Veneno, Verdad)
- **Postura:** Barra de postura enemiga. Romperla = +50% daño. Parry = aturde 2s.
- **Esquiva:** I-frames 0.2s, costo 15 SP, cooldown 0.5s
- **Combos:** Data-driven. 3-5 golpes ligeros, 2-3 pesados. Finale 2× daño.
- **AI Compañeros:** 4 arquetipos, rueda de comandos (Atacar/Defender/Disparar/Retirada)
- **Jefes:** 2-3 fases, memoria de combate, zonas de peligro, cadenas

---

## 🧙‍♂️ Sistema de Magia

- **Maná:** regeneración base lenta (1 MP/3s). Overload si gastas todo.
- **Escuelas:** 4 — Mago (elemental), Nigromante (sangre/huesos), Druida (naturaleza), Bardo (música/Tejido)
- **Runas:** 7 runas de arma, 5 encantamientos de armadura
- **Tejido:** Habilidades únicas que se desbloquean narrativamente:
  - Cortes Simultáneos (protagonista — N0, muerte de Caín)
  - Flecha del Tejido (Mira — N0, muerte de Caín)
  - Eco del Tejido (Sera — nativa)

---

## 📦 Progresión de Equipo

13 sets completos (N0-N10), 4 rarezas (Común → Legendario), mejora de ítems, forja, 6 items únicos.

---

## 🎮 Clases (12)

Centinela, Guerrero, Pícaro, Arquero, Mago, Nigromante, Druida, Bardo, Paladín, Monje, Cazador, Mercenario.

Cada una con 9 habilidades en árbol. Sin reespecificación fuera del Gremio.

---

## 🗺️ Diseño de Niveles

- N0: Overworld 2km² abierto
- N1-N9: Ciudades (300-500m) y Biomas (500m-1km)
- N10: Arena 50m diámetro
- 26 fogatas, ~37 cofres importantes
- Eventos de mundo: tormentas, migraciones, alucinaciones, podas

---

## 🎵 Sonido (Filosofía)

Minimalista. Silencio es instrumento. Música solo en momentos clave. Cada jefe tiene tema único. Leitmotivs por personaje (8 personajes con instrumento propio). 7 momentos de silencio absoluto (muerte de Caín, entierro, muerte de Mira/Vassen, N4, N10, ambos finales).

---

## 🏛️ Sistema de Reputación

| Facción | Base | Hostil | Máximo | Recompensa tope |
|---------|------|--------|--------|-----------------|
| Gremio (N0) | Neutro | -50 | 300 | Aldric da item único para N10 |
| Elfos (N3) | Neutro | -40 | 100+ | Hadda da Fragmento sin condición |
| Thornwall (N5) | Neutro | -60 | 150+ | Escolta militar a N7 |
| Argentia (N9) | Neutro | N/A | 100+ | Poción revive en N10 |
| Mercaderes | — | — | 30% desc | Hakeem |

---

## 🎬 Finales

### Final A — El Escape
Condición: Cualquiera. Derrotar al Arquitecto → elegir "Me voy".
Resultado: Amanecer en el desierto. Esperanza.

### Final B — El Nuevo Arquitecto
Condiciones: Fragmento del Arquitecto (Hadda N3 o Argentia N9) + elegir "Me quedo".
Resultado: Reconstruir la Torre. Garrik se va a contar la historia. Sera se queda.

---

## 📂 Archivos de Diseño

### Lore (16 archivos)

| Archivo | KB | Descripción |
|---------|:--:|-------------|
| `Lore/README.md` | 6 | Índice del lore |
| `Lore/00_la_compilacion.md` | 6 | La Compilación del mundo |
| `Lore/01_el_arquitecto.md` | 8 | El Arquitecto — entidad |
| `Lore/02_el_borrador.md` | 10 | El Borrador — mecanismo |
| `Lore/03_la_torre.md` | 11 | Historia de la Torre |
| `Lore/04_isla_de_la_nada.md` | 7 | Isla de la Nada (MMO future) |
| `Lore/05_folklore_primer_ataque.md` | 9 | Folklore del primer ataque |
| `Lore/06_el_protagonista.md` | 8 | El protagonista — Persistente |
| `Lore/07_companeros.md` | 15 | Todos los compañeros |
| `Lore/08_acto_i_arco_inicial.md` | 14 | Arco inicial del Acto I |
| `Lore/99_glosario.md` | 9 | Glosario de términos |
| `Lore/bestiario_diario.md` | 40 | Entradas de lore de enemigos |
| `Lore/hadda_el_bardo_ciego.md` | 57 | Hadda — bardo, quest, Final B |
| `Lore/mundo_exterior.md` | 42 | El mundo afuera, El Borrador |
| `Lore/npcs_secundarios.md` | 52 | 35+ NPCs con historia |
| `Lore/companero_garrik.md` | 2 | Garrik |
| `Lore/companero_sera.md` | 5 | Sera |

### Bestiario (1 + 10 bosses legendarios)

| Archivo | KB |
|---------|:--:|
| `Bestiario/bestiario_completo.md` | 68 |
| `Lore/bosses/README.md` | 6 |
| `Lore/bosses_rpg/` (10 jefes RPG) | ~55 total |

### Clases

| Archivo | KB |
|---------|:--:|
| `Lore/classes/README.md` | 1 |
| `Lore/classes/detalle.md` | 6 |

### Quests (20 archivos)

| Archivo | KB | Descripción |
|---------|:--:|-------------|
| `Quests/inicio_rpg.md` | 14 | Estructura N0-N10 con 8 escenas |
| `Quests/overworld_emberveil.md` | 24 | Misiones E, rango D, transición |
| `Quests/mision_jabali.md` | 5 | Misión tutorial |
| `Quests/misiones_E.md` | 5 | 7 misiones rango E |
| `Quests/ataque_al_gremio.md` | 5 | Evento del Errante |
| `Quests/rango_D.md` | 11 | 4 misiones bronce + evento |
| `Quests/la_ciudad_se_cierra.md` | 8 | Transición a Torre |
| `Quests/el_camino_a_la_torre.md` | 16 | 12 escenas al camino |
| `Quests/la_primera_garganta.md` | 31 | Guardián, muerte Caín |
| `Quests/guardián_primera_garganta.md` | 23 | Entrada completa |
| `Quests/N1_velmar.md` | 26 | Velmar completo |
| `Quests/N2_el_pastor_de_cenizas.md` | 24 | N2 |
| `Quests/N3_aelthar.md` | 19 | N3 |
| `Quests/N4_*` (2 archivos) | 33 | N4 |
| `Quests/N5_thornwall.md` | 18 | N5 |
| `Quests/N6_camaras_de_jade.md` | 19 | N6 |
| `Quests/N7_sahal.md` | 21 | N7 |
| `Quests/N8_jardines_flotantes.md` | 18 | N8 |
| `Quests/N9_argentia.md` | 17 | N9 |
| `Quests/N10_el_centinela_de_granito.md` | 19 | N10 + ambos finales |
| `Quests/misiones_secundarias.md` | 87 | 24 misiones descripción |
| `Quests/misiones_secundarias_dialogos.md` | 57 | 24 misiones con diálogos |
| `Quests/HISTORIA_COMPLETA_N0_N10.md` | 11 | Historia resumida |
| `Quests/ActI/` (5 caps) | 53 | Acto I original (antes de reestructura) |

### Sistemas (10 archivos)

| Archivo | KB | Descripción |
|---------|:--:|-------------|
| `Sistemas/sistema_combate.md` | 72 | Combate completo |
| `Sistemas/sistema_magia.md` | 47 | 4 escuelas, 28 hechizos |
| `Sistemas/crafting_completo.md` | 40 | 36 recetas, maestrías |
| `Sistemas/progresion_equipo.md` | 42 | 13 sets, mejoras |
| `Sistemas/clima_dia_noche.md` | 26 | 5 climas, efectos |
| `Sistemas/monturas.md` | 26 | 4 monturas, establos |
| `Sistemas/eventos_mundo.md` | 54 | 23 eventos de mundo |
| `Sistemas/reputacion_facciones.md` | 38 | 5 facciones |
| `Sistemas/sistema_sonido.md` | 48 | Música + leitmotivs |
| `Sistemas/niveles_y_rangos.md` | 5 | Rangos E → C |
| `Sistemas/tiendas_emberveil.md` | 6 | Tiendas N0 |

### Skills / Niveles

| Archivo | KB |
|---------|:--:|
| `Skills/arbol_habilidades.md` | 161 |
| `Levels/mapa_niveles.md` | 49 |

### Prompts (4 archivos, 39 prompts total)

| Archivo | KB | Prompts |
|---------|:--:|:-------:|
| `Prompts/prompt_combate.md` | 28 | 9 |
| `Prompts/prompt_magia_clases.md` | 33 | 9 |
| `Prompts/prompt_equipo_ui.md` | 26 | 12 |
| `Prompts/prompt_quests.md` | 50 | 9 |

---

## 🛠️ Orden de Implementación Recomendado

Meter los prompts en Claude Code en este orden:

### Fase Base (día 1)
1. `prompt_combate.md` → **Prompt 1** (AttributeSet) — base de todo
2. `prompt_combate.md` → **Prompt 2** (Damage Calculation) — sin esto no hay combate
3. `prompt_combate.md` → **Prompt 3** (Posture) — mecánica central
4. `prompt_magia_clases.md` → **Prompt 1** (Maná/Fatiga) — recurso secundario
5. `prompt_equipo_ui.md` → **Prompt 1** (Inventario) — necesario desde el inicio
6. `prompt_equipo_ui.md` → **Prompt 7** (HUD) — vital para ver stats
7. `prompt_quests.md` → **Prompt 1** (QuestManager) — narrativa

### Fase Movimiento (día 1-2)
8. `prompt_combate.md` → **Prompt 4** (Esquiva/Movimiento)
9. `prompt_combate.md` → **Prompt 5** (Combos)
10. `prompt_combate.md` → **Prompt 9** (Muerte/Respawn) + sistema guardado

### Fase Clases (día 2)
11. `prompt_magia_clases.md` → **Prompt 7** (Sistema de Clases)
12. `prompt_magia_clases.md` → **Prompt 8** (Árbol de Habilidades UI)
13. Clase por clase: Centinela, Guerrero, Pícaro, Arquero (prioridad física)

### Fase Hechizos (día 2-3)
14-20. Escuelas de magia + runas

### Fase Compañeros (día 3)
21-28. Compañeros AI, Garrik, Sera, eventos de muerte scripteados

### Fase Quests (día 3-4)
29-39. Diálogos, misiones, jefes, finales, UI

### Fase Niveles (día 4+)
- Diseñar cada piso en UE5 según `mapa_niveles.md`
- Poblar con enemigos, cofres, fogatas

---

## 🚫 Descartenado del RPG (MMO futuro)

- Pisos P11-P100 (contenido MMO)
- Isla de la Nada (MMO)
- Jefes legendarios (P20-P90) — entran como invocables opcionales en overworld
- Mazmorras opcionales existentes — SÍ entran al RPG

---

*Documentación generada el 2026-04-29 — ~1.5 MB de diseño, 73 archivos, 39 prompts para Claude Code.*
