# N9 — Argentia: La Plataforma de los Mártires
## Tema: Tortura Política y Ejecuciones

### Contexto
Argentia no es una ciudad. Es una **plataforma de ejecuciones**. Durante la Tercera Edad élfica, aquí se ejecutaba a los disidentes políticos. Los que se oponían al régimen de los elfos puros eran traídos a Argentia, torturados durante días frente al público, y luego ejecutados.

La plataforma de plata está manchada de sangre. La sangre de siglos ha creado capas, como un pastel. Los edificios no son viviendas — son **salas de interrogación**. Los "ancianos" que quedan no son sobrevivientes. Son los **verdugos que se quedaron**.

### La Verdad
Los "ancianos amables" que ofrecen reparar tu arma y darte refugio son los mismos que torturaron a miles. Oswin no es herrero — era el **verdugo jefe**. Forjaba los instrumentos de tortura. Elia no es vidente — era la **interrogadora principal**. Perdió la vista de tanto mirar la sangre.

Ellos no se arrepienten. Están **orgullosos**.

### Señales para el jugador

1. **Oswin** te recibe con una sonrisa. Su forja tiene **instrumentos de tortura** colgados (SM_Torture_Tools ×12 — tenazas, sierras, agujas, garfios). Si preguntas: "Ah, son herramientas. Para trabajar el metal."
2. **Elia** te dice: "Veo más allá de lo que mis ojos pueden ver." Lo que "ve" son los fantasmas de los que ejecutó. Están detrás de ti mientras habla. No te lo dice.
3. **Las "campanas" que escuchas** no son campanas. Son **cadenas colgando** (SM_Chain_Executioner, 4m de largo, gancho en el extremo). El viento las mueve.
4. **Calles con nombres:** "Calle de los Gritos", "Calle de los Colgados", "Plaza de la Confesión Final".
5. **Las "fuentes secas"** tienen un agujero en el centro. El agua que corría era sangre. El agujero drenaba los cuerpos.

### Mecánica Especial — "La Confesión"
- Argentia tiene **salas de interrogación** (SM_Room_Interrogation ×4)
- Cada sala tiene una silla (SM_Chair_Torture, 1m, correas para brazos y piernas)
- Al entrar en una sala: **el jugador es sentado en la silla y no puede moverse por 10s**
- Durante esos 10s: escenas aleatorias de tortura (flashbacks — no del jugador, de víctimas anteriores)
  - Una mujer pide agua. Le cortan la lengua.
  - Un niño llora. Le queman los pies.
  - Un anciano confiesa crímenes que no cometió. Lo ahorcan de todas formas.
- **Sin UI.** Solo la escena. El jugador no puede saltarla.
- Después de 10s: el jugador se libera. El juego no comenta lo que pasó.

### Zonas del Nivel

1. **La Plaza de la Confesión** (centro) — Plataforma de 40m diámetro. En el centro: un poste (SM_Pole_Execution, 6m alto, cadenas colgando). Alrededor: gradas de piedra (SM_Stand_Stone, capacidad para 500 espectadores).
   - **Los fantasmas están aquí.** No son visibles al principio. Pero si usas Eco del Tejido (habilidad de Sera), ves a 500 fantasmas sentados, mirando al poste.

2. **La Forja de Oswin** — No tiene yunque. Tiene **mesa de tortura** (SM_Table_Torture, 3m×2m, surcos para sangre, ganchos en los bordes).
   - **Oswin te ofrece "mejorar tu arma":**
     - "Tráeme los materiales correctos. Te forjaré algo especial."
     - Los "materiales" son: hueso de víctima (N4), lágrima de niño esclavo (N6), sangre de ejecutado (N7).
     - Si le das todo: forja la **Espada del Verdugo** (55 daño, +25% daño a humanos)
     - **Si le preguntas por los materiales:** "Son solo nombres, viajero. No pienses demasiado."

3. **El Templo de la Última Luz** — No es un templo. Es la **sala de selección**. Aquí los elfos decidían quién vivía y quién moría.
   - **Elia** está sentada en el altar.
   - **Diálogo (si la confrontas):**
     - Protagonista: "Eras la interrogadora, ¿verdad?"
     - Elia: (sonríe) "Yo los hacía hablar. No era mi culpa si no sobrevivían."
     - Protagonista: "Torturaste a miles."
     - Elia: "Torturé a 12,847. Llevaba cuenta. Y sabes qué, viajero? La mayoría confesaba. No porque fuera cierto. Porque querían morir. Eso es poder."
   - **Opción A:** Matar a Elia → +50 XP, pero todos los verdugos de la ciudad se vuelven hostiles
   - **Opción B:** Dejarla vivir → te da el **Fragmento del Arquitecto** (item clave para N10)
     - "Toma. Es el único recuerdo de la Tercera Edad que vale la pena guardar. El Arquitecto nos supervisaba. Cada ejecución. Cada grito. Lo veía todo. Y no hizo nada."

4. **El Ojo del Centinela** (escondido, detrás de la forja) — Una trampilla en el suelo. Escalera hacia abajo. Al fondo: una celda (SM_Cell_Solitary, 2m×2m×3m).
   - En la celda: un esqueleto sentado (SM_Skeleton_Tortured). Un ojo de jade (SM_Eye_Jade) en su mano.
   - **Al tocar el ojo:** visión del Centinela (N10, 15s de cinemática de combate)
   - **Diálogo automático:** "El Centinela no es una máquina. Es un torturador. Como nosotros."

5. **Las Celdas de los Condenados** — 8 celdas (SM_Cell_Torture, 2m×3m cada una). En algunas: esqueletos. En otras: cuerpos frescos. **Hay un sobreviviente.**
   - BP_Prisoner_Survivor (NPC hombre, sin piernas, arrastrándose)
   - **Diálogo:**
     - "Llevo aquí... no sé cuánto. Me cortaron las piernas hace... ¿días? ¿años?"
     - "Los verdugos... Oswin... Elia... no los mates. Déjalos vivir. Que sufran como nosotros."
   - **Opción:** Matarlo (misericordia) o dejarlo (vive en agonía). El juego no te dice cuál es la buena.

### El Verdugo de Plata — Reescrito

No es un guardián. Es **el mejor ejecutor de Argentia**. Un elfo que torturó por siglos, su alma se fusionó con su armadura. Ahora es un autómata de plata que ejecuta a cualquiera que entre a Argentia — porque "todos son culpables de algo".

**Fase 1 — El Interrogador**
- BP_VerdugoPlata (ASOLOEnemyCharacter):
  - Mesh: SM_Knight_SilverTorture (2m, armadura de plata oxidada, máscara sin rostro, capa de cuero manchada)
  - HP: 300
  - **Ataques:**
    - **Golpe de Confesión:** Golpe con el mango de la espada, 15 daño + inmoviliza 1s. "Confiesa."
    - **Corte de Lengua:** Barrido bajo, 20 daño + silencio 3s (no puedes usar habilidades)
    - **Rueda de Tortura:** Invoca una rueda (SM_Wheel_Torture, 3m diámetro, gira, si toca al jugador: 10 daño/s durante 3s)
  - **Diálogo (mecánico, con tono de diversión):**
    - "Todos confiesan al final."
    - "¿Dónde duele más? ¿Aquí? ¿O aquí?"
    - "Tus gritos son música."
  - **Mecánica "El Patrón":** El Verdugo sigue patrones de ataque como un ritual. Exactamente igual cada vez.
    - Secuencia: Golpe de Confesión → (espera 2s) → Corte de Lengua → (espera 3s) → Rueda → (espera 4s) → repite
    - **Ventana de ataque:** Después de la Rueda (4s)
    - Si atacas fuera de la ventana: el Verdugo te agarra de la garganta. "Desobediente." 25 daño. Te tira al suelo.

**Fase 2 — El Algoritmo de Tortura**
- Trigger: 150 HP perdidos
- El Verdugo se arrodilla. Su armadura se abre. Emerge un núcleo de plata con forma de **cerebro** (SM_Brain_Silver, 1m diámetro, late, con venas plateadas)
- **Nuevos ataques:**
  - **Recuerdo de Dolor:** AOE 5m de ondas de plata. 20 daño. Cada onda reproduce un grito diferente (12 gritos diferentes).
  - **Aguja Cerebral:** Proyectil que perfora, 25 daño + confusión 3s
- **Mecánica "El Ojo":** Con el Ojo del Centinela (item de N9), ves un punto débil: una cicatriz en el cerebro.
  - Golpear la cicatriz: 30 daño
  - Sin el Ojo: el cerebro se mueve erráticamente, difícil de golpear
- **Objetivo:** 10 hits al cerebro (o 7 a la cicatriz)
- **Diálogo final (al morir):**
  - "Nunca... nunca pregunté... si querían confesar..."
  - "Solo... me gustaba el sonido..."

**Fase 3 — El Espíritu Roto**
- Trigger: cerebro destruido
- El cuerpo de plata cae. Emerge un espíritu (SM_Spirit_Silver_Tortured): la forma original del elfo antes de ser verdugo. Un hombre joven, con uniforme de soldado, sin armadura.
- **Diálogo (cambia de tono — ahora es humano, roto):**
  - "Tenía 19 años cuando empecé. Me dijeron que era por la seguridad de la ciudad."
  - "La primera vez... vomité."
  - "La centésima... ya no sentía nada."
  - "La milésima... empecé a disfrutarlo."
  - "No me perdones. No lo merezco. Solo... recuérdame como fui antes."
- **No hay combate.** El espíritu se desvanece solo después del diálogo.

### Post-Combate
1. La armadura del Verdugo cae vacía.
2. Oswin y Elia aparecen en la plaza. Están de rodillas. Han estado viendo todo.
3. **Oswin:** "Mátanos. Por favor. Es lo único que nos queda."
4. **Opción A:** Matarlos → se desvanecen en luz. +50 XP.
5. **Opción B:** Dejarlos vivir → se quedan en la plaza, solos, en silencio.
   - Más tarde, cuando subes la escalera a N10, si miras atrás: Oswin se ha ahorcado en el poste de ejecución. Elia está sentada junto a su cuerpo.
6. **Transición:** Escalera de plata → N10
7. Recompensa: 400 XP, 200 oro, **Fragmento del Arquitecto** (si no lo tenías), **Llave de Argentia**

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Fragmento del Arquitecto | +30% daño a jefes. Esencial para N10. |
| Ojo del Centinela | Revela punto débil en N10 |
| Espada del Verdugo (Oswin) | 55 daño, +25% daño a humanos |
| Instrumento de Tortura | Arma cuerpo a cuerpo, 20 daño, 10% de sangrar |
| Diario de un Verdugo | Lore completo de Argentia y la Tercera Edad |
