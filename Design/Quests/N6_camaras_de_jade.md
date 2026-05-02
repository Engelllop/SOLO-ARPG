# N6 — Las Cámaras de Jade
## Tema: Explotación Infantil Industrial

### Contexto
Una red de cuevas donde funcionan **fábricas de jade**. Los niños de los pisos superiores (Thornwall, Aelthar) son traídos aquí para **trabajar en las minas**. El jade no es una roca — es la **cristalización del sufrimiento**. Literalmente. Cuando un niño llora lo suficiente, sus lágrimas se cristalizan en jade. La fábrica "cultiva" jade obligando a los niños a llorar.

### La Verdad
El zumbido de las Cámaras no es magia. Es el **llanto acumulado de miles de niños** que resuena en las paredes. Los que trabajan aquí pierden la cordura en días — no por magia, sino por el trauma. Los "constructos de jade" que protegen la mina son **niños petrificados** que se volvieron locos y se fusionaron con el mineral.

### Mecánica Especial — Estrés Postraumático (EPT)
- **UI:** Barra de 5 segmentos. No sube por magia — sube por **lo que ves**:
  - +1 por ver a un niño trabajando (BP_ChildMiner, SM_Child_BentOver)
  - +2 por ver a un niño muerto (BP_ChildDead)
  - +3 por ver a un niño siendo castigado (BP_Guard_BeatingChild)
- **Niveles del EPT:**
  - 2: Flashbacks. La pantalla se distorsiona (2s). Ves a Caín diciendo "¿Por qué no me salvaste?"
  - 4: **Parálisis por trauma.** No puedes moverte 3s, mientras un niño fantasma te mira.
  - 5: **Ataque de pánico.** La pantalla se vuelve blanca 5s. Apareces en otro lugar del mapa. Pierdes 10 HP.
- **Recuperación:** Alejarte de las zonas de trabajo. Descansar en zonas sin niños.

### Zonas del Nivel

1. **La Entrada de los Niños** — Vías de tren miniatura (SM_Rail_Toy). Los niños llegan aquí en jaulas.
   - Cartel: "BIENVENIDO — USTED AHORA VALE 50 DE ORO" (en élfico y humano)

2. **El Dormitorio** — Cama de piedra para 100 niños. Literas de 3 niveles (SM_Bed_Stone ×100). Sin mantas. Sin almohadas.
   - NPC: BP_ChildSleeper (niño durmiendo, no despierta, respira)
   - Si lo despiertas: "¿Ya es hora de trabajar?" (mira al techo, vuelve a dormir)

3. **La Cámara de Lágrimas** — Sala de 15m×15m. En el centro: una niña (BP_ChildCrying) sentada sobre un pedestal de jade. Está llorando. Sus lágrimas caen al pedestal y se cristalizan.
   - Alrededor: 5 hombres adultos riéndose (BP_Guard_Lauging)
   - **Opciones:**
     - Matar a los guardias → la nija deja de llorar, te mira, sonríe → se petrifica instantáneamente
     - No hacer nada → sigue llorando. El jade crece. El zumbido aumenta.
   - **No hay opción buena.**

4. **El Taller de Jade** — Niños de 6-12 años tallando jade (BP_ChildCarver ×6). Herramientas en manos pequeñas. Sangre en las manos.
   - Si te acercas, no dejan de trabajar. Ni siquiera te miran.
   - **Diálogo (un niño):** "Si dejo de trabajar, me pegan. Si me pegan, lloro. Si lloro, hago más jade."
   - **Diálogo (Garrik):** "No puedo... no puedo mirar."

5. **La Cámara del Supervisor** — Oficina del capataz (BP_Overseer_Sahal). Hombre humano, gordo, fumando.
   - Escritorio: SM_Desk_Overseer (papeles, taza de té, látigo SM_Whip)
   - **Papeles en el escritorio:** "Producción de jade, Semana 47: 47 niños activos, 12 muertos, 35 kg de jade producido."
   - **Diálogo si lo confrontas:**
     - "Son huérfanos. les damos techo y comida. Es un negocio."
     - "¿Crees que eres mejor? Tu ropa está hecha en fábricas. Tu comida la cultivan esclavos. No eres diferente."
     - **El jugador no tiene respuesta.** El juego no se la da.

### El Coro de Jade — Reescrito

**Escenario:** Cámara Central, donde niños son forzados a cantar mientras trabajan. El "zumbido" que escuchabas es su canto.

**Fase 1 — Los Solistas**
- 6 BP_ChildSinger (niños de 6-10 años, ropas rotas, ojos vacíos, HP 30 cada uno)
- **Mecánica "El Canto":** Mientras cantan, el jugador pierde 1 HP/s. Mientras más cantan, más fuerte el daño.
- **No se puede atacar a los niños.** El juego no permite dañarlos (no-target).
- **Objetivo:** Romper los cristales de jade que los rodean (SM_Crystal_Jade_Singer ×6, 10 hits cada uno). Cada cristal roto → un niño deja de cantar → cae de rodillas, agotado.
- **Diálogo (niño al ser liberado):** "Gracias... puedo descansar ahora..." (se acuesta, se queda dormido. O muerto. No se sabe.)

**Fase 2 — El Capataz de Jade**
- Los cristales rotos, el capataz emerge del suelo:
  - BP_CapatazJade (ASOLOEnemyCharacter): Humanoide de 3m, armadura de jade, látigo de cristal
  - HP: 100
  - **Ataques:** Látigo (AOE 5m, 20 daño + sangra), Pisotón, Grito de Mando (invoca 2 niños zombie que atacan lentamente)
  - **Objetivo:** 20 hits. Cada hit rompe la armadura de jade.
- Al morir: "Los niños... van a estar libres... gracias..."

**Post-Combate:**
- El capataz muere. Los niños sobrevivientes (6-8) se sientan en el suelo. No hablan.
- **CG:** El grupo se sienta con ellos. Silencio de 5s. Luego, uno de los niños toma la mano del protagonista.
- **Diálogo:** "Nunca había visto el sol."
- **Transición:** Los niños señalan una escalera de jade → N7

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Lágrima de Jade | Consumible. Restaura 50 HP. Descripción: "Todavía tibia." |
| Látigo del Capataz | Arma. 18 daño, sangra. Descripción: "Todavía huele a sangre de niño." |
| Carta de un Niño (no enviada) | "Mami, ven a buscarme. No quiero estar más aquí." |
| Registro de Producción | Lore. 47 niños activos. 12 muertos. 35 kg jade. |

### El Eco de Caín (EPT 5)
Si llegas a EPT 5, Caín aparece. Pero no habla. Solo se sienta junto a un niño muerto. Lo abraza. Después de 10s, desaparecen.

**Diálogo (protagonista, interno):** "Caín... ¿tú también pasaste por aquí?" (no hay respuesta)
