# N5 — Thornwall (Violación y Esclavitud Sexual) — Editor Setup
> Basado en: Design/Quests/N5_thornwall.md
> Código C++: Source/SOLOCore/Public/Character/N5/

---

## ⚠️ CONTENIDO SENSIBLE
Este piso trata violación, esclavitud sexual y abuso sistémico. No hay violencia gratuita — cada elemento sirve a la narrativa. El juego no se regodea; confronta.

---

## Resumen Temático
**TEMA: Esclavitud sexual y violación como sistema.** Una ciudad que es un burdel de 3 niveles. El Carnicero es el guardia/verdugo — más víctima que villano.

---

## C++ Classes

### ASOLON5_Carnicero (Jefe)
- `Public/Character/N5/ASOLON5_Carnicero.h`
- `Private/Character/N5/ASOLON5_Carnicero.cpp`
- 3 fases de combate directo.
- Fase 1: Ataques lentos, monólogos.
- Fase 2: Más rápido, revela su historia.
- Fase 3: Frenético, muestra vulnerabilidad.
- `OnDeathDialog()` → Últimas palabras. No pide perdón. Solo dice que no conoce otra vida.

### SOLON5_Constants.h
- IDs para N5.

---

## Blueprint Tasks

### 1. Nivel: L_N5_Thornwall
- **Tamaño:** ~150x150 unidades (ciudad vertical)
- **Atmósfera:** Siempre anocheciendo. Luces rojas/burdel en cada ventana. Música distorsionada.
- **3 Niveles Verticales:**
  - **Nivel Bajo (Calle):** Entrada de la ciudad. Calles sucias. Mujeres en ventanas. Hombres borrachos.
  - **Nivel Medio (Burdel Oficial):** Salones con cortinas rojas. Camas. El Carnicero está aquí inicialmente.
  - **Nivel Alto (Dueños):** Habitaciones de los clientes ricos. Alfombras. Decoración elegante. Más perturbador que la calle.

### 2. Ambientación
- **Mujeres en ventanas:** NPCs estáticas. No hablan si no se les paga. La mayoría tiene moretones visibles.
- **Hombre borracho:** NPC que se acerca. "¿Buscas compañía? Tienen de todo. Hasta de las que lloran."
- **La niña esclava (14-16 años):**
  - En una jaula en el Nivel Medio.
  - Visible: moretones en brazos y cuello, marcas de quemaduras de cigarro.
  - Cartel: "Virgen. 250 de oro."
  - **NO HAY OPCIÓN BUENA.**
    - Pagar 250 oro: la sacan de la jaula. La llevan a una habitación. Se escuchan gritos. La puerta no se abre.
    - Matar al guardia: la jaula no se abre — el dueño tiene la llave.
    - Romper la jaula: se asusta, sale corriendo, la agarran en la calle.
  - Si el jugador insiste mucho: habla. "No llores más, me dijo. Y no sé cómo se hace lo otro."

### 3. BP_Carnicero (Jefe)
- Hereda de ASOLON5_Carnicero
- Mesh: Humano grande (2.2x). Cuchillo de carnicero gigante en una mano. Mandil manchado.
- **Implementar:**
  - `AttackCleaverSwing` → Golpe lento pero masivo.
  - `AttackGrab` → Agarra al jugador, lo lanza.
  - `AttackEnrage` → Entra en furia al 30% HP.
  - `OnDeathDialog` → Se arrodilla. "Yo también fui vendido. Tenía 8. No conocía otro oficio."
  - `OnAttackLine` → Durante el combate: "Ellos pagan. Yo cobro. Simple."
  - También: Suelta frases que revelan que él mismo fue una víctima. "La primera vez que me tocaron, lloré. Dejé de llorar a los 12."

### 4. Secuencia
1. **Llegada:** El jugador entra a Thornwall. Un ambiente de feria de pueblo, pero la música está ligeramente distorsionada.
2. **Exploración:** Calles abajo, burdel arriba. NPCs ofrecen "servicios".
3. **La niña:** Encuentro inevitable (está en el camino central).
4. **Carnicero:** Al acercarse a los niveles superiores. Diálogo previo a la pelea.
5. **Combate:** Pelea dura. El Carnicero es fuerte pero no inteligente — patrones predecibles.
6. **Post-muerte:** La ciudad no se "libera". Los clientes se van. Las mujeres no saben qué hacer. Están atrapadas igual.
7. **Puerta a N6** en la habitación principal.

---

## Notas de Diseño
- El Carnicero NO es un monstruo. Es el resultado de un sistema que lleva generaciones. Sus últimas palabras deben ser incómodas, no triunfantes.
- NO mostrar violencia sexual directamente. Está implicada — verbal y contextualmente.
- La niña esclava es el momento más difícil del juego hasta N7. El jugador debe sentir que NO puede salvarla. Esa es la intención.
- **Post-N5:** Fragmento de Thornwall (item clave Ending B).
