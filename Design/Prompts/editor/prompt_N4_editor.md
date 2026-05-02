# N4 — Bosque Petrificado (Hambre) — Editor Setup
> Basado en: Design/Quests/N4_bosque_petrificado.md
> Código C++: Source/SOLOCore/Public/Character/N4/

---

## Resumen Temático
**TEMA: Inanición y canibalismo forzado.** Los árboles petrificados son personas. El hambre extrema lleva a actos desesperados. El boss (Coloso del Hambre) es una entidad que _chupa_ el hambre del jugador — no es un enemigo a vencer con fuerza bruta.

---

## C++ Classes Generadas

### ASOLON4_ColosoHambre (Jefe)
- `Public/Character/N4/ASOLON4_ColosoHambre.h`
- `Private/Character/N4/ASOLON4_ColosoHambre.cpp`
- **Un boss que se vence destruyendo su corazón (CorazonHambre).**
- Tiene aura de hambre — el jugador recibe daño por hambre al acercarse.
- Lanza ataques de área lentos pero devastadores.
- Llama a `OnDefeated()` cuando su corazón es destruido.

### ASOLON4_CorazonHambre (Objetivo real)
- `Public/Character/N4/ASOLON4_CorazonHambre.h`
- `Private/Character/N4/ASOLON4_CorazonHambre.cpp`
- El corazón late cada cierto intervalo, dañando en área.
- Protegido por hijos (fetus children) — destruirlos debilita el corazón.
- `HitsToDefeat` empieza en 8, baja a 5 cuando se destruyen suficientes hijos.
- `FetusChildren` colección de actores que lo protegen.

### ASOLON4_HungerComponent (Componente de jugador)
- `Public/World/N4/ASOLON4_HungerComponent.h`
- `Private/World/N4/ASOLON4_HungerComponent.cpp`
- Se attacha al jugador al entrar al nivel.
- Maneja nivel de hambre (0-100).
- Cuando hambre < 20, el jugador recibe penalizaciones de velocidad y daño.
- Drena hambre lentamente en el bosque. Se recupera al comer items especiales.
- Eventos: `OnHungerLow()`, `OnHungerCritical()`, `OnHungerDeath()`.

### SOLON4_Constants.h
- IDs de items, quests, NPCs para N4.

---

## Blueprint Tasks (Debes Crear)

### 1. Nivel: L_N4_BosquePetrificado
- **Tamaño:** ~300x300 unidades
- **Atmósfera:** Bosque gris sin color. Árboles con formas humanas retorcidas. Niebla baja. Silencio absoluto (sin música, solo sonido de viento).
- **Layout:**
  - **Entrada:** Puerta de la Torre se abre a un camino de tierra gris. Árbol petrificado al lado - al inspeccionarlo: "Este árbol tiene forma de mujer. Está abrazando a algo."
  - **Claro del Coloso:** Área amplia donde deambula el Coloso. No ataca inmediatamente.
  - **La Cabaña:** Edificio de madera podrida. Dentro: una familia (padre, madre, niño) sentada alrededor de una mesa. En la mesa: una pierna humana cocinada. Uno de ellos hablará con el jugador con normalidad.
  - **Cueva del Corazón:** Túnel subterráneo que lleva al CorazonHambre. Ambiente rojo/pulsante.
  - **Altar del Hambre:** Punto donde el jugador puede dejar comida para recuperar hambre (o sacrificar algo por poder).

### 2. Blueprint del Coloso (BP_ColosoHambre)
- Hereda de ASOLON4_ColosoHambre
- Mesh: humanoide enorme (escala 5x), cuerpo de corteza gris agrietada
- Boca abierta permanentemente (grito congelado)
- AnimBlueprint: movimientos lentos y pesados
- **Implementar:**
  - `AttackHungerWave` → Onda de hambre. El jugador pierde hambre al ser golpeado.
  - `AttackGroundSmash` → Golpe al piso, área grande, aturde.
  - `OnDeathDialog` → Al morir, no dice nada. Solo exhala. Su cuerpo se desmorona en polvo.

### 3. Blueprint del Corazón (BP_CorazonHambre)
- Hereda de ASOLON4_CorazonHambre
- Mesh: Órgano humanoide pulsante, suspendido en el aire por venas/petri
- Pulsa cada heartbeat (1.5s) — emite luz roja
- Al destruirse: las venas se rompen, el Coloso cae de rodillas
- **FetusChildren:** Mini-esferas/criaturas embrionarias que orbitan el corazón

### 4. Sistema de Hambre
- Attachar HungerComponent al ThirdPersonCharacter BP existente
- Crear UI para nivel de hambre (barra que se vacía)
- Items de comida:
  - **Fruta podrida** (restaura poco)
  - **Carne sospechosa** (restaura más pero daña PS)
  - **Pan duro** (restaura normal)
- Al llegar a hambre 0: muerte lenta

### 5. NPCs en Cabaña
- **Madre:** "¿Has comido? Te ves flaco." — Ofrece la pierna. No pregunta de dónde vino.
- **Padre:** "Aquí no crece nada. Tuvimos que... hacer sacrificios."
- **Niño:** No habla. Está masticando. Hay una mano pequeña en su plato.

### 6. Ambientación
- **Árboles-humano:** 30-40 árboles con formas humanas. Algunos: "Este árbol tiene la postura de alguien pidiendo clemencia."
- **Huesos:** Dispersos por todo el bosque. Algunos tienen ropa desgarrada.
- **Fogatas apagadas:** Restos de campamentos. Diarios de viajeros: "Día 7: Carl se fue a buscar comida. Día 12: Nos comimos a Carl."
- **El Altar:** Ofrecer comida restaura hambre al máximo. Ofrecer PS restaura hambre y da buff temporal.

---

## Secuencia de Eventos

1. **Salida:** El jugador emerge de la puerta de la Torre al bosque gris.
2. **Encuentro con el Coloso:** Lo ve de lejos. Es lento. Se puede esquivar fácilmente.
3. **La Cabaña:** El jugador encuentra la cabaña. Ve la escena. **Opción:** Hablar / Atacar / Salir.
   - Si ataca: La familia huye. No hay combate real.
   - Si habla: Diálogo incómodo donde ofrecen comida. "No es mucho, pero compartimos."
   - Si sale: El jugador escucha: "¿Se habrá ofendido?" desde adentro.
4. **Exploración:** El jugador busca el corazón. HungerComponent drena lentamente.
5. **Cueva del Corazón:** Encuentra el CorazonHambre.
6. **Combate:** Destruir fetos hijos + golpear el corazón.
7. **Coloso cae:** Cuando el corazón muere, el Coloso cae de rodillas, se desmorona en polvo.
8. **El bosque no cambia.** Los árboles siguen siendo personas. No hay "liberación".
9. **Puerta a N5 aparece** donde cayó el Coloso.

---

## Notas de Diseño

- No hay boss fight tradicional. El Coloso es un obstáculo, no un enemigo.
- El verdadero "enemigo" es el hambre y las decisiones que llevó.
- La familia en la cabaña no es mala. Son sobrevivientes. El jugador debe decidir si los juzga.
- El Corazón tiene caras de niños en su superficie. Al golpearlo: llora.
- **Post-N4:** El jugador obtiene **Fragmento de Hambre** (item clave para Ending B).
