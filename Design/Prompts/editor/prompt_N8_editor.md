# N8 — Jardines Flotantes (Eugenesia y Experimentación Humana) — Editor Setup
> Basado en: Design/Quests/N8_jardines_flotantes.md
> Código C++: Source/SOLOCore/Public/Character/N8/

---

## Resumen Temático
**TEMA: Eugenesia y experimentación humana.** Laboratorio élfico donde cultivan cuerpos humanos como plantas. Wombs abiertas, fetos en tanques. La Criada Bronce (jefe) fue el "jardinero" que podaba a los niños no perfectos.

---

## C++ Classes

### ASOLON8_CriadaBronce (Jefe)
- `Public/Character/N8/ASOLON8_CriadaBronce.h`
- `Private/Character/N8/ASOLON8_CriadaBronce.cpp`
- Jardinera/jefe. Cultiva humanos. Pelea con herramientas de jardinería (tijeras gigantes, hoz).
- 2 fases: Combate normal → Fase 2 cuando le quedan ≤30% HP (se vuelve frenética).
- `OnDefeated()` → Pide un nombre antes de morir. No tiene nombre propio. Toda su vida fue "Criada Bronce".
- Si el jugador le da un nombre: sonríe, dice gracias, muere en paz.
- Si no: muere sin sonrisa. "Nunca tuve nombre."

### BP_Sera (NPC, superviviente)
- `Public/Character/N8/BP_Sera.h`
- `Private/Character/N8/BP_Sera.cpp`
- Una de las "cosechas" que escapó del proceso de modificación.
- Mitad elfa, mitad planta. Tiene ramas creciendo de su espalda.
- Guía al jugador por el laboratorio.
- No pelea. Si el jugador la ataca (no debería), huye.

### SOLON8_Constants.h

---

## Blueprint Tasks

### 1. Nivel: L_N8_JardinesFlotantes
- **Tamaño:** ~250x250 unidades (varias islas flotantes)
- **Atmósfera:** Hermoso exterior → horrible interior. Por fuera: jardines colgantes, flores, mariposas. Por dentro: laboratorio de carne, vidrio y metal.
- **Layout (exterior):**
  - **Isla de Entrada:** Puente de piedra con enredaderas. Sera espera aquí.
  - **Jardines de Arriba:** Campos de flores. Abajo se ven raíces que... tienen forma de dedos humanos.
  - **Invernadero Principal:** Edificio de vidrio. Dentro: hileras de tanques.
- **Layout (interior):**
  - **Sala de Crecimiento:** 30+ tanques de vidrio. Fetos en diferentes etapas de desarrollo. Algunos completos. Sus ojos se mueven.
  - **Sala de Poda:** Mesa de operaciones. Herramientas colgadas. Manchas de sangre en el piso. "Los imperfectos se reciclan."
  - **Cámaras de Modificación:** Cuerpos abiertos, conectados a tubos. Algunos están despiertos pero no se mueven.
  - **Santuario de la Cosecha:** Donde la Criada Bronce hace su trabajo final. Aquí está ella.

### 2. BP_CriadaBronce (Jefe)
- Mesh: Mujer elfa (2.0x). Brazos largos y delgados. Tijeras de podar gigantes como manos. Mandil de cuero manchado. Gafas de aumento.
- **Implementar:**
  - `AttackPrune` → Tijeretazo que corta horizontalmente.
  - `AttackGraft` → Clava una rama en el jugador (bleed DoT).
  - `AttackHarvest` → Barrido con hoz.
  - `OnPhase2Dialog` → "Solo hago mi trabajo. Ellos me dijeron qué podar."
  - `OnDeathDialog` → "¿Cómo me llamo? Criada Bronce... es lo único que tengo. ¿Tú... tienes un nombre para mí?"
  - Si el jugador escribe un nombre (vía input) → sonríe. Si no → "No importa. No iba a recordarlo."

### 3. BP_Sera
- Mesh: Mujer joven (1.6x). Brazos tienen cortezas. De su espalda crecen ramas con hojas. Ojos de diferente color (uno humano, uno de planta).
- Postura encorvada. Voz baja. "Gracias por venir. No sabía que existía un... afuera."
- Diálogo: Guía al jugador, señala las salas, tiembla en la Sala de Poda.
- **No se une al combate.** Se queda en la entrada: "No puedo entrar ahí. Ella me hizo."

### 4. Ambientación
- **Tanques de feto:** Modelos de bebés/niños en líquido verde. Algunos tienen múltiples ojos, extremidades extras o faltantes.
- **Madres sustitutas:** Cuerpos de mujeres (muertas/vivas?) conectadas a máquinas. Wombs abiertas quirúrgicamente con tanques conectados.
- **Registros de laboratorio:** "Experimento 847: Modificación de huesos. Resultado: fallo. Sujeto desechado." "Experimento 848: Crecimiento acelerado. 3 años en 3 meses. Resultado: éxito parcial. Sujeto donado a N5."
- **"La Cosecha Perfecta":** Un niño/niña humanoide perfecto flotando en un tanque central. Sin imperfecciones. Ojos abiertos. No parpadea.

### 5. Secuencia
1. **Llegada:** El jugador ve los jardines. Todo parece pacífico. Sera sale de entre las flores.
2. **Revelación:** Sera lleva al jugador al invernadero. Los tanques. "Esto no es un jardín. Es una granja."
3. **Exploración:** Salas. Sera da contexto. El jugador ve los registros.
4. **Sala de Poda:** Sera se queda afuera. "Allí dentro... ella me arregló."
5. **Criada Bronce:** Combate. Ella no entiende por qué está mal lo que hace.
6. **Post-muerte:** El jugador puede liberar los tanques (abrir válvulas → líquido drena, fetos mueren) o dejarlos.
7. **Sera:** Se para frente al tanque de la "Cosecha Perfecta". "Podría haber sido yo." Se sienta. No sigue al jugador. Se queda en el laboratorio vacío.
8. **Puerta a N9** en el santuario.

---

## Notas de Diseño
- La Criada Bronce no es mala. Es una jardinera que recibió un trabajo horrible y lo hizo bien.
- Pedir un nombre antes de morir es el momento más humano del juego. El jugador decide si dárselo.
- Sera es la única "buena" que queda viva. Su destino es quedarse en el lugar que la creó. No sabe hacer otra cosa.
- La escena de los tanques debe ser clínica, no sensacionalista. Como un documental de Nature pero con cuerpos humanos.
- **Post-N8:** Fragmento de Jardín (item clave Ending B).
