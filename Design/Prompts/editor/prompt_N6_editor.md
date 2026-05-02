# N6 — Cámaras de Jade (Explotación Infantil Industrial) — Editor Setup
> Basado en: Design/Quests/N6_camaras_de_jade.md
> Código C++: Source/SOLOCore/Public/Character/N6/

---

## Resumen Temático
**TEMA: Explotación infantil industrial.** El jade es lágrimas cristalizadas de niños que lloran forzadamente en una fábrica. El boss es un capataz que también fue niño en la fábrica.

---

## C++ Classes

### ASOLON6_CoroDeJade (Jefe)
- `Public/Character/N6/ASOLON6_CoroDeJade.h`
- `Private/Character/N6/ASOLON6_CoroDeJade.cpp`
- **No es un jefe de combate.** El Coro es un colectivo de niños cantores que producen jade.
- Si el jugador los ataca: reciben daño, pero los ChildSingers dejan de cantar → la producción de jade se detiene.
- El capataz (CoroDeJade) ataca si el jugador interfiere.
- 2 fases: Fase 1 (capataz pelea mientras niños cantan), Fase 2 (niños huyen, capataz entra en furia).

### ASOLON6_ChildSinger (World Actor)
- `Public/World/N6/ASOLON6_ChildSinger.h`
- `Private/World/N6/ASOLON6_ChildSinger.cpp`
- Niño/a que canta. Daña al jugador con ondas sónicas si está cantando.
- No se puede matar — si recibe suficiente daño, deja de cantar y huye.
- `StopSinging()` → llama a `OnFreed()` BlueprintImplementableEvent.

### ASOLON6_ChildMiner (World Actor)
- `Public/World/N6/ASOLON6_ChildMiner.h`
- `Private/World/N6/ASOLON6_ChildMiner.cpp`
- Niño minero que extrae jade de las paredes.
- No ataca. Si el jugador se acerca demasiado: se encoge, deja caer jade, se va.
- Si el jugador lo golpea: llora, suelta más jade, se va corriendo.

### ASOLON6_PTSDComponent (Component de jugador)
- `Public/World/N6/ASOLON6_PTSDComponent.h`
- `Private/World/N6/ASOLON6_PTSDComponent.cpp`
- Se attacha al entrar a N6.
- Acumula "trauma" cada vez que un niño huye.
- A alto trauma: la visión del jugador se distorsiona, aparecen alucinaciones de niños.
- `BreakMentally()` → paraliza al jugador brevemente (flashback).

### SOLON6_Constants.h
- IDs items, quests, diálogos.

---

## Blueprint Tasks

### 1. Nivel: L_N6_CamarasDeJade
- **Tamaño:** ~250x150 unidades
- **Atmósfera:** Cavernas de jade verde brillante. Frío. Eco de niños cantando siempre presente.
- **Layout:**
  - **Entrada:** Garganta estrecha de jade. Música infantil distorsionada desde lejos.
  - **Cámaras de Canto:** Salas donde 3-5 ChildSingers están sentados en círculo, cantando. Producen jade que flota hacia arriba.
  - **Corredores de Jade:** Paredes de jade puro. ChildMiners golpean con picos diminutos. Si se les habla: "Tengo que sacar 50 kilos hoy. Ayer saqué 48 y el capataz me pegó."
  - **Cámara del Capataz:** Oficina de ASOLON6_CoroDeJade. Escritorio con látigo. Retratos de la "familia de la fábrica".
  - **Salida/Balcón:** Puerto de carga donde bajan el jade. Hacia N7.

### 2. BP_CoroDeJade (Capataz)
- Mesh: Hombre adulto (1.8x). Ropa de capataz industrial. Cinturón de herramientas.
- Cicatrices en la cara. Manos callosas.
- Diálogo: "¿Viniste a joderme el turno? Estos mocosos tienen que producir. Yo producía a su edad."
- **Implementar:**
  - `AttackLash` → Látigo de cuero. Rango medio.
  - `AttackCrystalThrow` → Tira fragmentos de jade.
  - `OnPhase2Dialog` → "Yo también lloraba. Pero aprendes a tragar. A tragar todo."
  - `OnDeathDialog` → "No hay afuera. Esto es todo. Para ellos. Para mí."

### 3. BP_ChildSinger
- Mesh: Niño/a (escala 0.6x). Ropa raída. Ojos vidriosos.
- Animación de balanceo rítmico. Boca abierta en "a" — canto.
- `OnFreed`: Deja de cantar. Se queda mirando al vacío. "¿Y ahora qué voy a hacer?" Luego huye lentamente.

### 4. BP_ChildMiner
- Mesh: Niño/a (escala 0.6x). Pico pequeño en mano. Ropa/trapos.
- Animación de golpe repetitivo a la pared.
- Si el jugador se acerca mucho: pausa, mira, deja caer jade, camina a otra veta.
- Si el jugador lo golpea: grito, cae, se levanta y huye cojeando.

### 5. Sistema PTSDComponent
- Attachar al ThirdPersonCharacter al entrar a N6
- UI: Indicador de trauma (esquinas rojas, pulsan)
- Efectos:
  - 40% trauma: sombras en bordes de pantalla
  - 60% trauma: susurros
  - 80% trauma: los ChildSingers se ven momentáneamente como monstruos (ilusión)
  - 100% trauma: BreakMentally — pantalla negra, 2s de flashback auditivo

### 6. Ambientación
- **Niño en esquina (inicial):** El primero que el jugador ve. Sentado, abrazando sus rodillas. Llora en silencio. No responde.
- **Comedor de la fábrica:** Plato de gachas grises. "Los niños que producen bien comen doble."
- **El molino:** Donde el jade se muele. Hay manchas de sangre. Un ChildMiner con un brazo vendado — "me atrapó la máquina anteayer".
- **Retrato del fundador:** Elfos sonrientes en la pared. "Fundado en el Año de la Abundancia. 43 generaciones."

---

## Secuencia
1. **Entrada:** El jugador escucha el canto antes de ver a los niños.
2. **Primera cámara:** Ve a los ChildSingers. El PTSDComponent se activa.
3. **Exploración:** ChildMiners en los corredores. Comedor. Molino.
4. **Capataz:** Cuando el jugador interfiere significativamente (salva a un minero o detiene a un cantante).
5. **Combate:** ChildSingers dañan con canto. Si el jugador los toca, huyen (trauma +++++).
6. **Derrota del capataz:** Los niños no saben qué hacer. Se quedan en las cámaras. Algunos siguen cantando por inercia.
7. **Puerta a N7** en el balcón de carga.

---

## Notas de Diseño
- NO matar niños. ChildSinger y ChildMiner no attackeables directamente — solo "liberables" o "asustables".
- El PTSDComponent asegura que "salvar" a los niños tiene costo emocional.
- El capataz no es un monstruo. Fue niño aquí. Repite el ciclo. No lo sabe.
- **Post-N6:** Fragmento de Jade (item clave Ending B).
