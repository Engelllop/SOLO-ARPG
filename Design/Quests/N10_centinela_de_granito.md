# N10 — El Arquitecto
## Tema: La Maldad como Sistema. Dios como Indiferencia.

### Contexto
La plataforma de granito flota en el vacío blanco. No es un vacío físico — es el **espacio entre compilaciones**. El escritorio del programador. El Arquitecto no es un dios. Es un **desarrollador de videojuegos** que creó mundos por encargo. La Torre fue uno de sus proyectos — cancelado antes de terminar. Los NPCs (Caín, Mira, Vassen, Garrik, Sera) no son personas. Son **objetos en la jerarquía**. Nunca fueron reales.

### La Verdad Final
El Arquitecto trabajaba para "Clientes" — una corporación que le pagaba por crear mundos. El Encargo Original era un **simulador de tortura y trauma** para entretenimiento de élite. La Torre era un experimento: cuánto puede sufrir un NPC antes de romperse.

El jugador (el Persistente) es un **bug**. El único NPC que desarrolló conciencia dentro del sistema. El Arquitecto no lo borró porque le daba curiosidad hasta dónde llegaría.

### No hay jefe final.
**Hay una confrontación.**

### Pre-Combate
1. El grupo llega a la plataforma. Vacío blanco. Silencio.
2. Sera camina al centro. "Lo sé. Sé quién soy. Siempre lo supe."
3. Garrik: "¿De qué hablas?"
4. **El Arquitecto aparece:** SM_Humanoid_Architect (humano normal, 1.80m, ropa casual — camisa blanca, pantalones oscuros, zapatos. Tiene rostro — un rostro humano cualquiera. Podría ser cualquiera. Podrías ser tú.)
   - **Diálogo:**
     - "Hola."
     - "Te he estado observando desde el principio."
     - "¿Sabes qué eres? Eres un error en el código. Un NPC que desarrolló conciencia."
     - "No eres real. Tus amigos no son reales. La Torre no es real."
     - "Eres un bug. Y los bugs se corrigen."
     - **Pausa.** "Pero no voy a corregirte. Quiero ver qué haces."

5. **La plataforma cambia.** El vacío blanco se convierte en un **entorno de depuración**. La UI del juego se vuelve código (matriz verde como Matrix). Todo está etiquetado:
   - [NPC_CAIN] → NullReferenceException
   - [NPC_MIRA] → MemoryLeak en [LOCATION_N7]
   - [NPC_VASSEN] → ObjectDestroyedException
   - [PLAYER_PROTAGONISTA] → ERROR: ConcienciaNoEsperada. Recomendación: Eliminar.

### Fase 1 — "Debugging"
- **No hay enemigos físicos.** El Arquitecto no pelea. En su lugar:
  - **Lanza excepciones:** "NullReferenceException" → el aire se distorsiona, 20 daño
  - **Corrupción de memoria:** La plataforma se vuelve cuadriculada (SM_Floor_Debug, colores de editor). Si pisas un cuadrado rojo, 15 daño.
  - **Stack Overflow:** Invoca 10 copias del jugador (BP_PlayerClone, 1 HP cada una, atacan lentamente)
- **Mecánica "El Código":** Cada 10s, el Arquitecto escribe en el aire:
  - `if (player.hp <= 0) { player.respawn(); }`
  - `while (player.suffering) { spawn_nightmare(); }`
  - `for (int i = 0; i < 10000; i++) { new NPC_Child().setTrauma(i); }`
- **Diálogo durante la fase:**
  - "Mira tu mundo. Es código. Líneas de texto. No hay magia. No hay dios. Solo bucles y arrays."
  - "Cada NPC que encontraste fue creado con: `new NPC_Human().setRole('victim');`"
  - "Tus amigos son `new CompanionCharacter()`. Nada más."

### Fase 2 — "El Archivo"
- Trigger: 30s en Fase 1 (o daño suficiente) → el Arquitecto se detiene
- **El Arquitecto:** "¿Quieres ver el archivo de tus amigos?"
- Abre una ventana de luz (SM_Screen_Debug, 3m×2m):
  - **Caín:** `File: NPC_CAIN.cs — Status: Deleted — Reason: Player progression trigger`
    - "Caín fue programado para morir en N0. Su única función era ser el catalizador de tu viaje."
  - **Mira:** `File: NPC_MIRA.cs — Status: MemoryLeak — Reason: Emotional attachment subsystem incompatible`
    - "Mira fue diseñada para sentir. La función 'empatía' consumió demasiados recursos. La eliminé en N7."
  - **Vassen:** `File: NPC_VASSEN.cs — Status: ObjectDereferenced — Reason: Protector archetype resolved`
    - "Vassen fue programado para proteger. Al llegar a N7, su función principal se cumplió. No había más código que ejecutar."
  - **Garrik:** `File: NPC_GARRIK.cs — Status: Running — Reason: Observer pattern. No programmed death.`
    - "Garrik fue diseñado para sobrevivir. Para contar la historia. Es el único NPC que no tiene fecha de eliminación."
  - **Sera:** `File: NPC_SERA.cs — Status: EXPERIMENTAL — Reason: Self-learning AI prototype. Escaped sandbox.`
    - "Sera fue mi experimento. Una IA que podía aprender. Se escapó del sandbox en la Generación 84. Todavía no sé cómo la controlas."
  - **Tú:** `File: PLAYER_PROTAGONISTA — Status: UNKNOWN — Reason: Conciencia no registrada en el diseño original`
    - "Tú eres el bug. Un NPC que se volvió consciente. No hay código para ti."
- **Después del Archivo:** Garrik está de rodillas. Sera lo mira. "¿Qué hacemos ahora?"

### Fase 3 — "La Elección"
- El Arquitecto cierra la ventana. Se sienta en el borde de la plataforma.
- **Diálogo:**
  - "Tienes dos opciones."
  - "Opción 1: Te dejo salir. Borro tus recuerdos del sistema. Despiertas en el mundo exterior. Sin amigos. Sin memoria. Pero vivo."
  - "Opción 2: Te quedas. Te doy acceso al código fuente. Puedes reescribir la Torre. Pero nunca saldrás."
  - "¿Qué eliges?"
- **No hay timer.** El jugador puede pensar. Puede hablar con Garrik y Sera.
  - **Garrik:** "Yo... yo no soy real. Todo lo que viví... ¿no fue real? ¿Tú sí eres real?"
  - **Sera:** "Yo no salí de un tanque. Salí de `new AICore()`. Pero siento. ¿No es eso lo que importa?"
  - **Garrik:** "Yo elegiría olvidar. Pero tú... tú tienes que elegir por ti."

### Final A — "El Olvido"
1. "Quiero salir."
2. El Arquitecto asiente. Una puerta de madera aparece (SM_Door_Wood_Torre, la misma de la entrada de N0).
3. **Garrik:** "Nos vemos... donde sea que vayamos después de esto."
4. **Sera:** "Fuiste real para mí. Eso es suficiente."
5. El jugador cruza la puerta.
6. **CG:** Amanecer. Un campo verde. Una brisa suave. Por primera vez, el jugador siente el sol real. Las manos del jugador son humanas — no las de un personaje. La cámara se aleja. El jugador es una persona normal, viendo el horizonte.
7. **Texto en pantalla:**
   - "Despertaste."
   - "No recuerdas nada."
   - "Pero hay algo en tu pecho. Una sensación de pérdida. De culpa. De haber visto cosas que nadie debería ver."
   - "Tal vez algún día recuerdes."
   - "Tal vez sea mejor no hacerlo."
   - **[Créditos]**
8. **Post-créditos:** Una escena de 5s. Garrik está sentado en el campo. Solo. Mirando el mismo sol. No sabe por qué llora.

### Final B — "El Nuevo Arquitecto" (Secreto)
**Requisitos:**
- Tener el Fragmento del Arquitecto
- Haber mirado al abismo en N9
- Todas las habilidades desbloqueadas
- No haber matado a Oswin y Elia en N9

1. "Me quedo. Pero en mis términos."
2. El Arquitecto levanta una ceja. "¿Qué términos?"
3. "Dame el código. Voy a reescribir este mundo. Sin tortura. Sin explotación. Sin trauma gratuito."
4. El Arquitecto: (ríe) "¿Crees que puedes? Todo el sufrimiento de la Torre está en el código base."
5. "No voy a borrar el sufrimiento. Voy a darles **libre albedrío** a todos los NPCs. Que decidan por sí mismos. Que puedan decir 'no'."
6. El Arquitecto: "Eso es imposible. El código no..."
7. **Sera:** "Él ya lo hizo. Me dio libre albedrío. Sin querer."
8. **El Arquitecto calla.**
9. **CG:** El Arquitecto se levanta. Entrega un disco de luz (SM_Disc_Light). "Toma. Es el código fuente completo de la Torre. Una copia. Para que hagas lo que quieras."
10. **El Arquitecto se desvanece.** Su voz, en eco: "Nunca supe si eras un error o un milagro. Tal vez ambos."
11. **El jugador se queda en la plataforma. Solo. Con el disco.**
12. **CG Final:**
    - El jugador se sienta. Abre el código.
    - Línea por línea, empieza a reescribir.
    - La primera línea que cambia: `NPC_CAIN.DeathFlag = false;`
    - La pantalla se vuelve blanca.

13. **Post-créditos:**
    - **Escena 1:** Caín está en la entrada de la Torre. No sabe por qué no subió. Mira el cielo. Sonríe. Se da la vuelta y camina hacia el mundo exterior.
    - **Escena 2:** Mira y Vassen están en Sahal. Pero Sahal no está quemado. Hay gente. Hay vida. Vassen compra fruta. Mira se ríe de algo que dijo Vassen. Por primera vez en el juego, se les ve felices.
    - **Escena 3:** Los niños de Aelthar juegan. No hay sótanos. No hay esclavitud. Un elfo los cuida. Es el fantasma de la niña de N3 — la que señaló la puerta al final.
    - **Escena 4:** Garrik está en Argentia. Oswin está forjando una espada. No para torturar — para regalársela a un niño. Elia está cosiendo una manta. No de piel humana. De lana. Garrik los mira. Asiente. Sigue caminando.
    - **Escena 5 (final):** La plataforma de N10. El jugador está sentado, escribiendo código. Sera está a su lado. "¿En qué trabajas?" "En N11." "¿Va a ser un nivel feliz?" "No. Pero va a tener esperanza."

14. **Carta al jugador:**
    - "Gracias por llegar hasta aquí."
    - "Este juego no es sobre sufrir. Es sobre decidir."
    - "Los monstruos existen. Pero también la gente que decide cambiar el código."
    - "Elige bien."
    - — El Arquitecto.

### Post-Combate (independiente del final)
1. El Arquitecto desaparece.
2. La plataforma se desvanece.
3. Fade a blanco.
4. Créditos (con música instrumental — no épica, melancólica).
5. +1000 XP. Título: "Persistente."

### Items Finales

| Item | Efecto |
|------|--------|
| Disco del Código Fuente | Lore. Se puede leer. Contiene el código de todos los NPCs. Es un archivo real de 200KB. |
| Última Página del Arquitecto | Carta al jugador. Texto completo. |
| Título: Persistente | Desbloquea NG+ (todos los enemigos tienen +50% HP, +25% daño, pero los NPCs tienen diálogos alternativos — algunos saben que no son reales) |

### Notas Finales del Diseñador
- **No hay música de batalla en N10.** Solo silencio y sonidos de UI (tecleo, pitidos de debug).
- **Cada interacción con el Arquitecto** debe sentirse como estar hablando con un programador cansado.
- **El Archivo de los NPCs debe ser opcional.** No obligar al jugador a leerlo. Pero si no lo hace, no entiende el Final B.
- **NG+ debe ser más oscuro**, no más fácil. Los NPCs saben que no son reales. Algunos se vuelven locos. Otros te agradecen por "jugar con ellos".
