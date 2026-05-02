# N9 — Argentia (Tortura Política y Ejecuciones) — Editor Setup
> Basado en: Design/Quests/N9_argentia.md
> Código C++: Source/SOLOCore/Public/Character/N9/

---

## Resumen Temático
**TEMA: Tortura política y ejecuciones sistematizadas.** Ciudad-prisión donde "los viejos" (Oswin y Elia) fabricaban instrumentos de tortura y el Verdugo Plata los usaba. Argentia es la burocracia del dolor.

---

## C++ Classes

### ASOLON9_VerdugoPlata (Jefe)
- `Public/Character/N9/ASOLON9_VerdugoPlata.h`
- `Private/Character/N9/ASOLON9_VerdugoPlata.cpp`
- 2 fases:
  - **Fase 1:** Combate normal. Máscara de plata puesta. Ataca con garrote, rueda de tortura.
  - **Fase 2:** Máscara rota (≤50% HP). Cerebro expuesto (bHasBrainExposed). Más rápido, más errático.
- `OnDefeated()` → muerte lenta. Habla con el jugador después de morir (OnSpiritDialog): "Tenía 19 cuando empecé. Me dijeron que los prisioneros eran enemigos. Para mí eran personas. Pero aprendí a no escucharlos."
- `OnDeathDialog` → "Los viejos... ellos me enseñaron. Oswin hacía las pinzas. Elia los látigos. Yo solo... ejecutaba."

### BP_Elia (NPC Opcional)
- `Public/NPCs/N9/BP_Elia.h`
- `Private/NPCs/N9/BP_Elia.cpp`
- Anciana. Fabricante de látigos y flagelos.
- 12,847 instrumentos fabricados en su vida. Lo recuerda con orgullo.
- "Este látigo en particular... lo usaron 300 veces antes de romperse. Buena calidad."
- NO pelea. **Si el jugador la mata:** pierde acceso a Ending B.
- Si habla con ella: cuenta su historia con normalidad. No ve maldad en lo que hizo. "Era mi oficio."

### BP_Oswin (NPC Opcional)
- `Public/NPCs/N9/BP_Oswin.h`
- `Private/NPCs/N9/BP_Oswin.cpp`
- Herrero de instrumentos de tortura. Pinzas, torniquetes, "sierras de confesión".
- Muestra su taller con orgullo. "Esta sierra la hice yo. 47 confesiones sacó."
- **Si el jugador lo mata:** pierde acceso a Ending B.
- Si habla: "La gente dice que era brutal. Yo digo que era eficiente. El dolor saca la verdad más rápido que las palabras."

### SOLON9_Constants.h

---

## Blueprint Tasks

### 1. Nivel: L_N9_Argentia
- **Tamaño:** ~200x200 unidades
- **Atmósfera:** Prisión de piedra gris. Corredores largos con celdas. Olores a humedad, metal y sangre vieja. Luz tenue de antorchas. Suena goteo constante.
- **Layout:**
  - **Puerta de Entrada:** Pesada puerta de acero con remaches. Grabado: "Argentia — Justicia Eterna." Debajo, en letra pequeña: "Población: 0."
  - **Patio de Ejecuciones:** Plataforma de madera con sogas. Tablón manchado. Alrededor: gradas para espectadores.
  - **Taller de Oswin:** Herrero. Instrumentos colgados en las paredes. Yunque. Fuego.
  - **Taller de Elia:** Telar. Pero en vez de telas: látigos de diferentes grosores. Cadenas. Pinzas. Una silla de tortura en la esquina.
  - **Cámaras de Tortura:** 5 salas con diferentes herramientas. Manchas en las paredes y pisos.
  - **Sala del Verdugo:** Celda grande. El Verdugo espera sentado, con la máscara en la mano. Se la pone cuando el jugador entra.
  - **Azoteas de la Memoria:** Terraza con vista a la ciudad vacía. Aquí se pueden leer archivos de los "juicios".

### 2. BP_VerdugoPlata (Jefe)
- Mesh: Hombre alto (2.0x). Túnica negra. Máscara de plata sin rasgos.
- En Fase 2: máscara rota, cara visible. Joven. Muy joven. No más de 25 años.
- **Implementar:**
  - `AttackConfessionBlow` → Golpe con garrote que aturde.
  - `AttackTongueCut` → No corta lengua realmente (sería instakill). Es un corte rápido horizontal.
  - `AttackTortureWheel` → Rueda de tortura que rueda por el piso.
  - Fase 2:
    - `AttackPainMemory` → El Verdugo "recuerda" un grito — el jugador escucha el grito original de esa ejecución. Daño psíquico.
    - `AttackBrainNeedle` → Estocada rápida con aguja.
  - `OnAttackLine` → Durante Fase 1: solo números. "Confesión 7,304." "Ejecución 2,111."
  - `OnPhase2Dialog` → "No recuerdo sus caras. Solo los números. Los números no duelen."
  - `OnDeathDialog` → "¿Sabes lo peor? No era malo. Solo obedecía."

### 3. BP_Elia (NPC)
- Mesh: Anciana. Bata de taller. Gafas. Canas recogidas en un moño.
- Ubicación: Taller de látigos.
- Habla con amabilidad. "¿Viniste a ver mi trabajo? Siéntate, te muestro mis favoritos."
- Si el jugador pregunta si siente remordimiento: "Remordimiento? Hacía látigos. No azotaba a nadie. Allá ellos con su conciencia."

### 4. BP_Oswin (NPC)
- Mesh: Anciano. Brazos musculosos (herrero). Mandil de cuero. Calvo. Ciego de un ojo (accidente de trabajo — "me saltó una esquirla de la sierra de confesión").
- Ubicación: Herrero.
- "¿Ves esta sierra? La hice para el cuello. 47 confesiones. Récord en Argentia."
- Si el jugador lo mata: el martillo cae. Suena hueco. No hay pelea.

### 5. Ambientación
- **Archivos:** Pilas de documentos. "Caso 12,847 — Acusado de herejía. Confesó tras 6 horas de torsión. Ejecutado al amanecer."
- **Gradas:** Asientos numerados. Programa de ejecuciones. "Hoy: 3 herejes, 2 traidores. Matiné a las 10 AM."
- **Campana:** En el patio. "La campana de la justicia." Al tocarla: suena hueca.
- **Fotos en el taller de Elia:** Las herramientas favoritas de cada verdugo. "Martillo de Ezequiel — 503 usos."
- **El Verdugo Anterior:** Esqueleto en una celda. Murió de vejez. Su registro: 20,000+ ejecuciones.

---

## Secuencia
1. **Entrada:** Jugador entra. Ciudad vacía. Ecos. La campana suena sola ocasionalmente.
2. **Exploración:** Talleres. Cámaras. Archivos. Oswin y Elia son encontrables.
3. **Encuentro con Elia/Oswin:** Opcional. Decisiones aquí afectan Ending B.
4. **Sala del Verdugo:** Combate. Fase 1 → Fase 2.
5. **Post-muerte:** El jugador puede dejar a Oswin y Elia vivos (Ending B path) o matarlos.
6. **Puerta a N10** en la plataforma de ejecuciones. Estaba allí todo el tiempo. "La puerta del cielo."

---

## Notas de Diseño
- Argentia es el piso más "tranquilo" en apariencia. No hay monstruos deambulando. Solo dos ancianos en sus talleres.
- La violencia aquí es pasada. Archivos. herramientas. Recuerdos. Eso la hace más perturbadora.
- El Verdugo tiene 25 años. Empezó a los 19. Fue entrenado por el anterior Verdugo (el esqueleto).
- **NO matar Oswin/Elia** es parte del requisito para Ending B. El jugador debe decidir si la justicia es suya para tomarla.
- **Post-N9 (si miró al abismo y tiene Fragmentos N3+N5+N6+N7+N8):** "Has mirado al abismo. Ha sido mutuo."
- **Post-N9 (normal):** Puerta a N10.
