# N10 — El Centinela / El Arquitecto (El Mal como Sistema) — Editor Setup
> Basado en: Design/Quests/N10_centinela_de_granito.md
> Código C++: Source/SOLOCore/Public/Character/N10/

---

## ⚠️ PISO FINAL — NO HAY BOSS FIGHT

---

## Resumen Temático
**TEMA: La indiferencia de Dios. El mal como sistema.** El Arquitecto no es un demonio ni un dios. Es un programador. La Torre es un producto. Los pisos son niveles. Los NPCs son scripts. El jugador elige: destruir el sistema (Ending A) o revelar la verdad (Ending B).

---

## C++ Classes

### ASOLON10_Arquitecto (NPC/Jefe No-Combatiente)
- `Public/Character/N10/ASOLON10_Arquitecto.h`
- `Private/Character/N10/ASOLON10_Arquitecto.cpp`
- **NO ATACA. NO PUEDE SER ATACADO.** 
- Es un humano normal en una oficina normal.
- 3 fases (de diálogo):
  - **Fase 1:** Presentación. "Hola. Llegaste. Siéntate." Muestra pantallas con los pisos. Explica que la Torre es un producto.
  - **Fase 2:** Revelación. "Cada piso es un tema. Trabajé con consultores para la autenticidad. El N7 fue mi favorito."
  - **Fase 3:** Elección. El jugador elige: Destruir / Liberar / Saber la verdad.
- `AdvanceLocalPhase()` → avanza diálogo.
- `OnDefeated()` → no aplica. Llama `ChooseEscape()` directamente.

### ASOLON10_PlayerClone (Enemigo Reflejo)
- `Public/World/N10/ASOLON10_PlayerClone.h`
- `Private/World/N10/ASOLON10_PlayerClone.cpp`
- Antes de llegar al Arquitecto, el jugador pelea contra... sí mismo.
- Clon perfecto que copia movimientos, armas, habilidades.
- El daño recibido es el mismo que el jugador inflige (1:1).
- Para vencer: el jugador debe descubrir que el clon no puede *dejarse* ganar — es un espejo.
  - Solución: dejar de pelear. Bajar el arma. El clon baja el arma también.
  - Si el jugador insiste en pelear: nunca muere. Huye infinitamente.
  - Mecánica: After 30 segundos sin atacar: "Eres... ¿real?" Luego se desvanece.

### SOLON10_Constants.h

---

## Blueprint Tasks

### 1. Nivel: L_N10_Centinel
- **Tamaño:** Variable (secciones pequeñas)
- **Atmósfera:**
  - **Entrada:** Corredor de espejos. El jugador ve su propio reflejo infinitamente. Pisos anteriores destellan en los espejos al pasar.
  - **Sala del Clon:** Arena circular blanca. Sin texturas. Sin sonido. Piso, paredes, techo — todo blanco infinito.
  - **Oficina del Arquitecto:** Una oficina común. Escritorio de madera. Computadora. Lámpara. Taza de café. Ventana que da a... código. Líneas y líneas de código color verde sobre negro.
  - **El Servidor:** Backend de la Torre. Luces parpadeantes. Tubos de conexión. "¿Y si todo esto solo corre en un servidor en alguna parte?"

### 2. BP_Arquitecto (NPC Final)
- Mesh: Humano normal (1.7x). Camisa arremangada. Lentes. Barba de 3 días. Taza de café en mano.
- **NO tiene animaciones de combate.**
- **Implementar:**
  - `OnPlayerArrived` → "Oh, llegaste. Más rápido de lo que esperaba. O más lento. No sé, perdí la noción."
  - `OnPhase1` → Presenta la Torre. "Es un producto, ¿sabes? Encargado por una corporación de entretenimiento. Los pisos representan males sociales. Es... inmersivo."
  - `OnPhase2` → Muestra estadísticas. "N7 tiene la tasa de retención más alta. La gente vuelve a jugarlo. ¿Sabes por qué? Porque es ficción. Pueden sentir lástima sin sentirse culpables."
  - `OnPhase3` → Elección. Tres opciones que se muestran en pantalla:
    1. **Destruir** — Termina la Torre. Todos los NPCs mueren para siempre. Final A.
    2. **Tomar el control** — El jugador se sienta en la silla. Sigue haciendo niveles. El ciclo continúa. Final C (secret fail).
    3. **Salir** — El jugador camina hacia una puerta que no existía. "¿Qué hay afuera?" "Nada. El vacío. Pero es tuyo." → Final B.

### 3. BP_PlayerClone (Enemigo)
- Mesh: Copia exacta del personaje del jugador.
- Animaciones: exactamente las mismas que el jugador.
- **IA:**
  - Copia inputs del jugador con 0.5s de retraso.
  - Si el jugador ataca: el clon ataca. Daño mutuo.
  - Si el jugador recibe daño: el clon también (copy).
  - Si el jugador baja el arma 3 segundos: el clon baja el arma. Destello. "No puedo... lastimarte. Eres yo." Desaparece.
- Diálogo (texto flotante):
  - "¿Por qué luchas contra ti mismo?"
  - "¿Sabes cuántas veces he intentado matarte?"
  - "Eres tan terco como yo."
  - "Si de verdad quieres ganar... deja de pelear."

### 4. Ambientación
- **Corredor de espejos:** Cada espejo muestra un recuerdo de piso anterior. Al pasar por uno de N7: "¿Vassen y Mira?" — los espejos se empañan.
- **La computadora:** Pantalla con código. Al interactuar: se ve el archivo "player.py". Función: `def make_choice(player):`. Incompleto.
- **Correo corporativo:** "Para: DevTeam. Asunto: Feedback N7. 'Los jugadores reportan incomodidad. Excelente trabajo. La retention rate subió 12%. Próximo sprint: N11—Sistema de Castas.'"
- **Diagrama de la Torre:** En la pared. "N3: Tráfico infantil — Engagement rate: alta." "N5: Esclavitud sexual — Replay value: medio." "N7: Genocidio — retention rate: muy alta."

### 5. Secuencia
1. **Corredor de espejos:** Flashbacks. Música: versión distorsionada de las canciones de cada piso.
2. **Sala del Clon:** Combate/no-combate. Prueba final.
3. **Oficina del Arquitecto:** El Arquitecto recibe al jugador informalmente.
4. **Diálogo final:** Fases 1-2-3. El Arquitecto no es malvado. Es indiferente. "No creé el sufrimiento. Creé una simulación del sufrimiento. ¿Es diferente?"
5. **Elección:**
   - **Ending A (Destruir):** El jugador rompe la computadora. La Torre tiembla. Pantalla blanca. "Gracias por jugar." Créditos.
   - **Ending B (Salir):** Requisitos: Fragmento de Espejo (N3) + Fragmento de Hambre (N4) + Fragmento de Thornwall (N5) + Fragmento de Jade (N6) + Fragmento de Sahal (N7) + Fragmento de Jardín (N8) + Mirar el Abismo (N9) + Todas las habilidades + NO matar Oswin/Elia (N9).
     
     El jugador elige "Salir". El Arquitecto: "Sabía que elegirías esa. Aquí." Dale un disco. "Los archivos. Las identidades reales. Los lugares. Todo es real, ¿sabes? Los pisos no son ficción. Son documentación."
     
     La puerta se abre. El jugador sale. Ve un mundo real. Gente real. La Torre era una instalación. El jugador decide: ¿Publica los archivos? ¿Los quema? ¿Se sienta y llora? **La decisión final no está en el juego. Está en el jugador. Negra. Créditos.**

   - **Ending C (Tomar control):** El jugador se sienta. "Bien. Te enseño." Un año después. Una pantalla: "N11 — Preparando contenido." El ciclo continúa. **"No aprendiste nada."** Créditos tristes.

---

## Notas de Diseño Finales

- **El Arquitecto no es un villano.** Es un desarrollador de videojuegos que hizo lo que le pidieron. "Si no lo hacía yo, lo hacía otro. Al menos yo puse algún arte."
- **La Torre es un producto.** El mal es sistémico. Los individuos son piezas reemplazables.
- **El jugador no tiene poder real.** No puede "matar al jefe final y arreglar todo". Elige cómo termina su historia.
- **Ending B no es "feliz".** Es "verdadero". El jugador enfrenta que el sufrimiento en la Torre pasó en el mundo real. Los archivos son evidencia. ¿Qué hace con eso?
- **Ending C es el fracaso.** El jugador se convierte en el Arquitecto. "Espero que disfrutes tu nuevo trabajo. El café es malo."
- **Post-créditos (Ending B):** Una noticia. "Filtración masiva de documentos revela operación de tráfico a nivel global. Autoridades investigan." O: "Incendio destruye archivos. No hay sobrevivientes." Depende de la elección final del jugador.

---

# Fin del Viaje.
Gracias por llegar hasta aquí. Ahora ponlo en el editor.
