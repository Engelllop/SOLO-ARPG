# N8 — Los Jardines de Carne
## Tema: Experimentación Humana y Eugenesia

### Contexto
Ruinas de una "ciudad" que no es una ciudad. Es un **laboratorio de eugenesia élfica**. Los elfos de la Tercera Edad intentaron crear la "raza perfecta" mediante experimentación genética. Lo que queda son jardines colgantes de carne, plataformas donde crecen cuerpos como si fueran plantas. Brazos que brotan del suelo como raíces. Rostros en las paredes que hablan.

### La Verdad
Los Jardines no son naturales. Son el resultado de siglos de **cruza forzada, inseminación artificial y modificación genética**. Los elfos querían crear soldados perfectos. Crearon **monstruos con conciencia** — seres que saben que son errores, que sienten dolor, que quieren morir pero no pueden.

### Señales para el jugador

1. **El suelo respira** — SM_Floor_Flesh (textura de piel humana, sube y baja 1cm cada 3s). Si te quedas quieto, escuchas un latido.
2. **Caras en las paredes** — SM_Wall_Faces (bajorrelieves de rostros humanos. Todos diferentes. Todos con expresión de terror congelado). Si te acercas, parpadean.
3. **El viento huele a sangre y formaldehído**
4. **Los "árboles" son cuerpos** — SM_Tree_Body (tronco humanoide, 6m, brazos extendidos como ramas, dedos como hojas)
   - Si golpeas un árbol: grita
5. **Las "flores" son fetos** — SM_Flower_Fetus (flores con forma de feto, abiertas, embriones visibles)
   - Algunos están vivos. Se mueven.

### Mecánica Especial — El "Indice de Pureza"
- **UI:** Barra de 4 segmentos que mide qué tan "puro" eres según los estándares élficos
- **Sube por:** Matar a los mutantes (cada mutante muerto +1)
- **Baja por:** Ayudar a los mutantes / Negarte a matar / Mostrar empatía
- **Al llegar a 4:** Los elfos (si quedan) te consideran "puro". Te dan acceso al laboratorio principal.
- **Al llegar a 0:** Los mutantes no te atacan. Te reconocen como uno de ellos.

### Zonas del Nivel

1. **El Vivero** (entrada) — Plataforma de 20m×20m llena de cuerpos en diferentes etapas de desarrollo:
   - BP_Fetus_Tank ×12 (tanques de vidrio SM_Tank_Glass, 2m alto, fetos humanos de diferentes tamaños flotando en líquido verde)
   - Algunos tanques están rotos — el feto está en el suelo, moviéndose (BP_Fetus_Crawling, 5 HP, no ataca, solo se arrastra)
   - **Diálogo (Sera, si está en el grupo):** "Yo salí de uno de esos tanques. Lo recuerdo."

2. **El Jardín de los Brazos** — El suelo está cubierto de brazos humanos que brotan como hierba (BP_ArmGrass ×∞, 3 HP cada uno, te agarran el tobillo si pasas, reducen velocidad 20%)
   - Para liberarte: matarlos o que Sera los corte

3. **La Cámara de las Madres** — 4 mujeres (BP_SurrogateMother ×4) conectadas a máquinas (SM_Machine_Insemination). Están vivas. Conscientes. Sus vientres están abiertos — se pueden ver los fetos dentro.
   - **Opción A:** Matar a las madres (misericordia). Los fetos mueren con ellas.
   - **Opción B:** Liberar a las madres (desconectar máquinas). Los fetos sobreviven. Las madres te miran con odio. "Nos quitaste nuestro propósito."
   - **No hay opción buena.**
   - **Diálogo (una madre, si la liberas):** "Teníamos un propósito. Ahora solo somos carne vacía. Gracias."

4. **El Archivo Genético** — BP_Computer_Elven (pantalla de luz élfica, interactivable). Contiene:
   - **Registros de la Tercera Edad:**
     - "Proyecto Soldado Perfecto — Generación 47: 79% de sujetos murieron. 12% desarrollaron conciencia. 9% se volvieron inútiles (sentían empatía)."
     - "Problema de la Conciencia: los sujetos conciente lloran. Preguntan 'por qué'. Hay que reiniciar el proyecto."
   - **Nota al margen:** "La Generación 84 produjo un espécimen notable — alta inteligencia, cero empatía, regeneración completa. Prototipo nombrado 'Sera'. Se escapó."

5. **La Fuente de Carne** (arena del jefe) — Plataforma de 30m diámetro. En el centro: un pozo (SM_Well_Flesh, 4m diámetro). Del pozo brota carne líquida que se solidifica en formas humanoides.

### La Arpía de Bronce — Reescrita

**La Arpía no es una arpía.** Es la **criada del laboratorio** — la primera "éxito" del proyecto de eugenesia. Un ser alado de bronce que los elfos usaban para "podar" los especímenes defectuosos. Ha estado viva 500 años, sirviendo a un laboratorio que ya no existe. No sabe hacer otra cosa.

**Fase 1 — La Criada**
- BP_CriadaBronce (ASOLOEnemyCharacter):
  - Mesh: SM_Humanoid_BronzeMaiden (mujer de bronce, 3m, alas de plumas de bronce, rostro sin expresión)
  - HP: 250
  - **Ataques:**
    - **Podar:** Barrido con garras de bronce, 20 daño, sangra
    - **Vuelo de Inspección:** Vuela sobre el jugador, suelta "semillas" (SM_Seed_Bronze que explotan en AOE 3m, 15 daño)
    - **Clasificación:** Agarra al jugador, lo examina 2s ("Defectuoso."), lo tira al suelo (25 daño)
  - **Diálogo (mecánico, sin emoción):**
    - "Sujeto no registrado. Clasificar."
    - "Procesando... defectuoso. Eliminar."
    - "¿Por qué... por qué sientes tanto?"
    - **Última línea (30% HP):** "Nunca... nunca me pregunté por qué."

**Fase 2 — La Grieta**
- Trigger: 150 HP perdidos
- La armadura de bronce se agrieta. Debajo: **carne viva**. La Criada es un ser biológico envuelto en metal.
- **Nuevos ataques:**
  - **Grito Biométrico:** AOE 6m, 20 daño + miedo 2s. Su voz suena a humana por primera vez.
  - **Carne Expuesta:** Invoca 3 tentáculos (BP_Tentacle_Flesh, 30 HP cada uno, golpe 10 daño)
- **Mecánica "El Recuerdo":** Cada 15s, la Criada se detiene. Su rostro cambia de seco a confundido.
  - "¿Por qué... por qué obedecía?"
  - "Los elfos... se fueron... ¿por qué sigo aquí?"
  - "Puedo... puedo elegir?"

**Fase 3 — La Elección**
- Trigger: muerte de la Criada
- Antes de morir, la Criada cae al suelo. Su armadura se rompe completamente. Es una mujer joven, desnuda, deforme. **Pero sonríe.**
- **Diálogo:** "Gracias... por dejarme elegir... por primera vez."
  - (pausa) "Nunca tuve un nombre. Dame uno."
- **Opción:** El jugador puede nombrarla (input de texto). El nombre aparece en una lápida (SM_Tombstone) que Sera coloca.
  - Si no le pones nombre: la Criada muere con lágrimas. "Ni siquiera... al final..."
- **CG:** Sera entierra el cuerpo. Coloca una flor (SM_Flower_SilverBloom) sobre la tumba.

### Post-Combate
1. La Criada muere. El laboratorio deja de palpitar.
2. Los mutantes de los jardines (los que sobrevivieron) se reúnen alrededor de la tumba. No atacan. Se sientan en silencio.
3. **Uno se acerca al protagonista.** Le toca la mano. **No tiene boca.** Pero su expresión dice todo.
4. **Sera:** "Ellos... nos están agradeciendo."
5. **Transición:** Un mutante señala una puerta de luz (SM_Door_LightBio) → N9
6. Recompensa: 350 XP, 180 oro, **Carta Genética** (item, +20% daño a jefes élficos)

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Pluma de la Criada | +10% velocidad. "Todavía tiene su sangre." |
| Carta Genética | +20% daño a enemigos élficos |
| Frasco de Embrión | Consumible. Restaura 30 HP. Descripción: "No preguntes." |
| Registro del Laboratorio | Lore completo del proyecto eugenesia |
