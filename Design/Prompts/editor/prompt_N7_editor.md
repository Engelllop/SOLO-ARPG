# N7 — Sahal (Genocidio y Destripamiento) — Editor Setup
> Basado en: Design/Quests/N7_sahal.md
> Código C++: Source/SOLOCore/Public/Character/N7/

---

## ⚠️ PISO CLAVE
**Este es el punto de no retorno emocional.** Mira y Vassen NO pueden ser salvados. Su muerte es obligatoria para avanzar. No hay sorpresa, no hay "rescate de último segundo". El juego es honesto con esto.

---

## Resumen Temático
**TEMA: Genocidio y desmembramiento.** Ciudad entera hecha de cadáveres cosidos. La Costurera usa hilos de agujas para unir cuerpos. El jugador debe matar a sus compañeros (Mira y Vassen) para avanzar.

---

## C++ Classes

### ASOLON7_Costurera (Jefe Final)
- `Public/Character/N7/ASOLON7_Costurera.h`
- `Private/Character/N7/ASOLON7_Costurera.cpp`
- **3 fases:**
  - **Fase 1:** Mira y Vassen están atrapados en hilos. La Costurera no es atacable. El jugador debe romper 6 hilos (RegisterThreadBreak) para liberarlos.
  - **Fase 2:** Los compañeros liberados... la Costurera los mata en cutscene no interactiva.
    - Vassen es desmembrado (brazos primero, luego piernas, luego torso — 4 estocadas)
    - Mira se corta la garganta (suicidio) para no ser torturada
  - **Fase 3:** Combate directo. Costurera intangible periódicamente (2s cada 15s). ToggleIntangible(). Ataca con agujas, hilos y "Danza de Parches".
- `OnDefeated()` → cae. "Sahal era hermosa antes del 'accidente'." Implica que ella también fue víctima de algo.

### ASOLON7_MarionetaComponent
- `Public/World/N7/ASOLON7_MarionetaComponent.h`
- `Private/World/N7/ASOLON7_MarionetaComponent.cpp`
- Componente para los enemigos menores (cadáveres cosidos).
- Controla movimiento con hilos invisibles.
- Al destruir al enemigo: el cuerpo se desarma en partes separadas.

### SOLON7_Constants.h

---

## Blueprint Tasks

### 1. Nivel: L_N7_Sahal
- **Tamaño:** ~200x200 unidades
- **Atmósfera:** Ciudad en ruinas. Todo está cosido. Paredes de piel humana. Suelo de huesos. El cielo es una sábana gris cosida al horizonte.
- **Layout:**
  - **Entrada:** Puerta de N6 se abre a las afueras. Cartel: "Sahal — Ciudad de la Unión Eterna". Debajo, escrito con sangre: "No entres".
  - **Plaza de las Marionetas:** 10-15 MarionetaComponent enemies que deambulan. Todos son cadáveres reanimados. Algunos tienen caras reconocibles (sufrimiento congelado).
  - **Taller de la Costurera:** Sala gigante con cuerpos colgando del techo como ropa. Maniquíes de carne. Agujas del tamaño de espadas clavadas en el piso.
  - **Altar del Hilo:** Donde Mira y Vassen están atados. Hilos de aguja atraviesan sus muñecas, tobillos, cuello.
  - **Salón de los Recuerdos:** Habitación con diarios de víctimas. Fotos de familia cosidas en las paredes.

### 2. BP_Costurera (Jefe)
- Hereda de ASOLON7_Costurera
- Mesh: Mujer anciana (1.5x). Ropa hecha de parches de piel humana. Aguja gigante como bastón. Una cara de Vassen cosida a su hombro.
- **Implementar:**
  - `OnMiraFreed` / `OnVassenFreed` → Los compañeros caen al suelo, libres.
  - `OnMiraDeath` / `OnVassenDeath` → Cutscene BPI: cámara fija, sin control de jugador.
  - `OnCompanionsDeadCG` → La Costurera ríe. "Ahora sí, todos cosidos."
  - `AttackNeedle` → Estocada con aguja.
  - `AttackThread` → Látigo de hilo que jala al jugador.
  - `AttackDanceOfPatches` → Ataque en área, cuerpos cosidos vuelan.
  - `AttackSummonPainMarionette` → Spawnea enemigo Marioneta.

### 3. Las Muertes (Cutscene — CRÍTICO)
- **Vassen:** La Costurera lo agarra. Hilo alrededor de su muñeca derecha. Tira. "No—" Corte seco. Muñeca derecha separada. Grita. Muñeca izquierda. "¡MIRA—" Garganta. No termina la frase. Pausa. El torso cae. Cuatro estocadas en total. La pantalla no corta. El jugador ve todo.
- **Mira:** Vassen muerto. La Costurera se acerca a ella. Mira la mira. "No. Yo elijo." Saca un cuchillo escondido. Se lo clava en la garganta. Corte de lado a lado. Cae. Sangre en el piso. La Costurera: "Qué desperdicio. Era bonita."

### 4. Post-Muerte
- No hay respawn. No hay checkpoint entre Fase 1 y 2.
- Después de la escena, la Costurera se vuelve atacable.
- El combate es catártico. El jugador debe ganar.
- Al morir la Costurera: su último hilo se rompe. Todo Sahal se descos literalmente. Las paredes caen. Los cuerpos se separan. El cielo se rasga.

### 5. Ambientación
- **Marionetas:** Cadáveres cosidos que caminan. Cabezas giradas. Algunos tienen múltiples brazos de diferentes personas.
- **Diario de la Costurera:** "Hoy encontré un ojo azul. Lo puse en el hombro del general. Me quedó torcido. Lo voy a reemplazar."
- **Carta de Vassen (antes de morir):** "Mira, si no salimos de aquí, quiero que sepas que—" La carta se corta. Firmada: "Siempre tuyo."
- **Fotos de Sahal antes:** Una ciudad vibrante. La gente sonríe. No se ve la Costurera en ninguna foto.

---

## Secuencia
1. **Entrada:** Jugador llega. Descubre que no puede salir. La puerta se cosió sola.
2. **Marionetas:** Pelea por la ciudad.
3. **Taller:** Encuentra a Mira y Vassen atados.
4. **Fase 1:** Romper 6 hilos. Cada hilo roto = un compañero liberado gradualmente.
5. **Cutscene.** Muertes. **NO SE PUEDE EVITAR.**
6. **Fase 3:** Combate contra la Costurera.
7. **Muerte de Costurera:** Sahal se descos. El jugador está de pie entre restos de cuerpos.
8. **Puerta a N8:** Hecha de huesos verticales. "Sigue subiendo. Siempre hay más."

---

## Notas de Diseño

- NO poner opciones. NO poner QTE para salvar. La muerte es obligatoria.
- La cutscene de muerte debe ser **incómodamente larga**. 15-20 segundos. Sin música. Solo sonidos: hilo tensándose, carne rompiéndose, tos ahogada.
- Después de la escena, darle al jugador 5 segundos de silencio absoluto antes de que la Costurera hable.
- Vassen y Mira no deben tener "death quotes" heroicas. Vassen grita. Mira calla. Así de real.
- El jugador ODIA a la Costurera después de esto. Eso es intencional.
- **Post-N7:** Fragmento de Sahal (item clave Ending B).
