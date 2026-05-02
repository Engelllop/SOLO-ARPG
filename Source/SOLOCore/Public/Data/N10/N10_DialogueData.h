// N10 — El Centinela / El Arquitecto (Final)
// Diálogos, NPCs, Elección Final, Items

#pragma once

#include "CoreMinimal.h"

/*
=== CORREDOR DE ESPEJOS ===
Node: N10_MirrorHall
  Narration: "Pasas por el corredor. Cada espejo muestra un recuerdo."
  Espejo1: "N3. El niño en el reflejo. Todavía espera."
  Espejo2: "N4. La familia. La pierna en la mesa."
  Espejo3: "N5. La jaula vacía. El cartel en el piso."
  Espejo4: "N6. El canto. Siempre el canto."
  Espejo5: "N7. Vassen. Su mano separándose de su cuerpo."
  Espejo6: "N8. Sera. Sentada frente al tanque."
  Espejo7: "N9. El pozo. Te devuelve la mirada."
  → Next: N10_CloneRoom

=== SALA DEL CLON ===
Node: N10_Clone_Appears
  Narration: "Una figura aparece frente a ti. Idéntica. Mismas armas. Misma postura."
  Clon: "Hola."
  Player: "¿Qué eres?"
  Clon: "Tú. Pero sin las decisiones que te hicieron quien eres."
  → Combate opcional: el clon copia todo.
  → Si el jugador ataca: combate infinito. No se puede ganar.
  → Si el jugador baja el arma por 3s:

Node: N10_Clone_Surrender
  Clon: (baja el arma)
  Clon: "Nunca había... parado."
  Player: "¿Por qué iba a pelear contra mí mismo?"
  Clon: "Porque todos lo hacen. La mayoría nunca descubre que pueden soltar el arma."
  Clon: "Pasa. Te está esperando."
  → El clon se desvanece.
  → SetFlag: "N10_Clone_Defeated_Peace"

  (Si el jugador nunca baja el arma después de 60s:)
Node: N10_Clone_Endless
  Narration: "El clon te ha estado copiando por un minuto. No se cansa. No se rinde."
  Clon: "¿Vas a seguir? ¿Toda la noche?"
  Player: "Hasta que mueras."
  Clon: (ríe) "No puedo morir. Soy un reflejo. Cuando te vayas, desaparezco. Pero no muero."
  Clon: (desaparece) "Entra. Te aburre pelear con sombras."
  → SetFlag: "N10_Clone_Defeated_Combat"

=== OFICINA DEL ARQUITECTO ===
Node: N10_Architect_First
  Arquitecto: (tecleando) "Un momento. Casi termino esta línea."
  Arquitecto: (pausa) "Bien. Hola. Siéntate."
  Narration: "La oficina es normal. Escritorio de madera. Lámpara. Taza de café."
  Arquitecto: "Supongo que tienes preguntas."
  Player: "¿Qué es esto?"
  Arquitecto: "Una Torre. Un producto. Un encargo."
  → Next: N10_Architect_Phase1

=== FASE 1: PRESENTACIÓN ===
Node: N10_Architect_Phase1
  Arquitecto: "Una corporación de entretenimiento me contrató. 'Queremos una experiencia inmersiva sobre el mal', dijeron."
  Arquitecto: "Contraté consultores. Investigadores. Gente que sabía de qué hablaba."
  Player: "¿Consultores? ¿Para el N7?"
  Arquitecto: "Sí. Habló con sobrevivientes. Con perpetradores. Con gente que cosía."
  Arquitecto: "Me dijo que la autenticidad era clave. Tenía razón. El N7 tiene la retención más alta."
  → Next: N10_Architect_Phase2

=== FASE 2: REVELACIÓN ===
Node: N10_Architect_Phase2
  Player: "¿Sabes lo que pasó en esos pisos?"
  Arquitecto: (enciende una pantalla) "Mira."
  Pantalla: "N3: Tráfico infantil — Engagement rate: 87%. N5: Esclavitud sexual — Replay value: medio. N7: Genocidio — Retention rate: muy alta."
  Arquitecto: "No creé el sufrimiento. Creé una simulación del sufrimiento. ¿Es diferente?"
  Player: "Claro que es diferente. Esto es ficción."
  Arquitecto: "¿Y qué hay de los archivos? ¿Qué hay de las identidades reales que usé como base?"
  Player: (silencio)
  Arquitecto: "Exacto."
  → Next: N10_Ending_Choice

=== FASE 3: ELECCIÓN ===
Node: N10_Ending_Choice
  Arquitecto: "Bien. Hora de decidir. Hay tres caminos."
  Pantalla muestra opciones:

  1. "Destruir la Torre" → Ending A
  2. "Tomar el control" → Ending C
  3. "Salir" → Ending B (solo si tienes los fragmentos)

  Arquitecto: "Elige con cuidado. Solo hay una primera vez."

=== ENDING A — DESTRUIR ===
Node: N10_Ending_A
  Player: "Voy a destruir esto."
  Arquitecto: "¿Estás seguro? Hay gente que trabaja aquí. Vidas reales."
  Player: "Tú no tienes derecho a hablar de vidas reales."
  Arquitecto: (suspira) "Supongo que no."
  Narration: "Golpeas el servidor. Una chispa. Luego otra. La Torre tiembla."
  Narration: "Las luces se apagan. Una a una. Como los pisos que subiste."
  Narration: "Silencio."
  Narration: "Gracias por jugar."
  → Roll credits
  → SetFlag: "ENDING_A"

=== ENDING C — TOMAR CONTROL ===
Node: N10_Ending_C
  Player: "Voy a tomar tu lugar."
  Arquitecto: (sonríe) "Sabía que dirías eso. Siéntate. Te enseño."
  → Time skip: "1 año después"
  Pantalla: "N11 — Preparando contenido."
  Arquitecto: (voz en off) "Es fácil acostumbrarse. Al poder. A decidir qué ve la gente."
  Arquitecto: "No aprendiste nada."
  → Roll credits (tristes)
  → SetFlag: "ENDING_C"

=== ENDING B — SALIR (REQUIERE FRAGMENTOS) ===
Node: N10_Ending_B
  Player: "Quiero salir."
  Arquitecto: (pausa. Te mira.) "Sabía que dirías eso."
  Arquitecto: (abre un cajón. Saca un disco.) "Toma."
  Player: "¿Qué es?"
  Arquitecto: "Los archivos. Las identidades reales. Direcciones. Nombres. Pruebas."
  Arquitecto: "Todo lo que usé para construir los pisos... es real. Los consultores no eran consultores. Eran víctimas. O perpetradores. A veces ambos."
  Player: "¿Por qué me das esto?"
  Arquitecto: "Porque tú llegaste hasta aquí. Y porque nadie más lo pidió."
  Narration: "El Arquitecto señala una puerta que no estaba allí antes."
  Arquitecto: "Adelante. Afuera está el mundo real."
  → El jugador sale. Se ve una ciudad. Gente. Tráfico. Normalidad.
  → Texto: "Tienes los archivos. La verdad en tus manos. ¿Qué haces con ella?"
  → La decisión final NO está en el juego. Es del jugador.
  → Pantalla negra. Créditos.
  → SetFlag: "ENDING_B_SECRET"

=== POST-CRÉDITOS (Ending B) ===
Node: N10_PostCredits_B
  (Elección aleatoria basada en flags del jugador durante el juego)
  Si el jugador tendió a la justicia:
    Noticia: "Filtración masiva de documentos revela red global. Autoridades investigan."
  Si el jugador fue indiferente:
    Noticia: "Incendio destruye archivo histórico. No hay sobrevivientes."
  → Fin.

=== QUESTS ===
Quest: "N10_Main"
  Name: "El Arquitecto"
  Description: "El último piso. Arriba no hay un jefe. Hay un creador."
  Floor: 10
  Objectives:
    1. "Atraviesa el corredor de espejos"
    2. "Enfrenta a tu clon"
    3. "Habla con el Arquitecto"
    4. "Elige tu final"
  Rewards: (depende del ending)

Quest: "N10_Secret_Ending"
  Name: "La Verdad"
  Description: "Requisitos ocultos para Ending B"
  Floor: 10
  bIsHidden: true
  bIsSecretOnly: true
  Objectives: (no se muestran)
    1. "Fragment_Espejo (N3)"
    2. "Fragment_Hambre (N4)"
    3. "Fragment_Thornwall (N5)"
    4. "Fragment_Jade (N6)"
    5. "Fragment_Sahal (N7)"
    6. "Fragment_Jardin (N8)"
    7. "LookedIntoAbyss (N9)"
    8. "All abilities unlocked"
    9. "Elia and Oswin spared (N9)"
  Rewards: Ending B
*/
