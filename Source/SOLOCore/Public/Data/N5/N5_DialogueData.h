// N5 — Thornwall (Esclavitud Sexual)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h"

/*
=== LLEGADA A THORNWALL ===
Node: N5_Arrival
  Narration: "Las puertas de Thornwall están abiertas. No hay guardias. No hay bienvenida. Solo música que viene de adentro."
  Voz: (desde la calle) "¡Eh, viajero! ¿Buscas diversión? Aquí hay de todo."
  → Next: N5_Street_Explore

=== HOMBRE BORRACHO ===
Node: N5_DrunkMan_First
  Borracho: "Bonito lugar, ¿eh? Ven por el vino, te quedas por... bueno, todo."
  Player: "¿Qué clase de lugar es este?"
  Borracho: "¿Qué clase? Un burdel. El más grande del reino. Tres pisos. Tres sabores."
  Player: "¿Y la gente que vive aquí?"
  Borracho: (ríe) "Vivir? Nadie vive aquí. Todos estamos de paso. Unos pagan, otros cobran."
  → Next: N5_Street_Continue

=== LA NIÑA EN LA JAULA ===
Node: N5_CagedGirl_First
  Narration: "En una esquina del segundo nivel, hay una jaula. Dentro, una niña. Tendrá 14, 16 años. No sabes cuánto tiempo lleva aquí."
  Cartel: "Virgen. 250 de oro."
  Niña: (no habla. Mira al piso.)
  Player: "¿Cómo te llamas?"
  Niña: (levanta la cabeza. Tarda en responder.) "No... me acuerdo."
  → Choices:
    - "Pagar 250 de oro" → N5_Girl_Paid
    - "Romper la jaula" → N5_Girl_Break
    - "Matar al guardia" → N5_Girl_KillGuard

Node: N5_Girl_Paid
  Guardia: (toma el oro. Abre la jaula.) "Bien. Disfruta."
  Narration: "Un hombre toma a la niña del brazo. La lleva por un pasillo. Una puerta se cierra."
  Sonido: (golpes. Silencio.)
  Narration: "La niña no vuelve a salir."
  → SetFlag: "N5_Paid_For_Girl"

Node: N5_Girl_Break
  Narration: "Golpeas la jaula. Se abre. La niña sale corriendo. Llega a la calle. Dos hombres la agarran."
  Niña: (grita una vez. Luego calla.)
  → SetFlag: "N5_Broke_Cage"

Node: N5_Girl_KillGuard
  Narration: "El guardia cae. Tiene la llave en el cinturón."
  Narration: "La abres. La niña no se mueve."
  Niña: "¿Y ahora?"
  Player: "Eres libre."
  Niña: "¿Libre para qué? No conozco nada fuera de esta ciudad."
  → SetFlag: "N5_Freed_Girl"
  → Pero: no hay opción buena. Se queda sentada en la jaula abierta.

=== CARNICERO (Pre-Combat) ===
Node: N5_Carnicero_PreCombat
  Carnicero: "Ah. El nuevo. Oí que andas preguntando."
  Player: "¿Tú eres quien cuida este lugar?"
  Carnicero: "Cuido? No. Trabajo aquí. Como todos."
  Player: "¿Incluso los niños?"
  Carnicero: (pausa) "Empecé a los 8. Limpiando. Después aprendí el oficio."
  Player: "¿El oficio de qué?"
  Carnicero: "De hacer lo que me dicen. Como tú. Como todos."
  → Next: N5_Carnicero_Combat_Start

=== POST-CARNICERO ===
Node: N5_PostCarnicero
  Carnicero: (agonizando) "Mi mamá me vendió. Dijo que aquí iba a estar mejor. Mentira."
  Player: "..."
  Carnicero: "Pero qué sabes tú. Tú viniste, mataste al malo, te fuiste. Así funcionan las historias."
  → Next: N5_Departure

=== ITEMS ===
Item: "Fragment_Thornwall"
  Name: "Fragmento de Thornwall"
  Description: "Un cartel doblado. 'Virgen. 250 de oro.' Las letras están manchadas."
  Type: KeyItem

Item: "Llave_Thornwall"
  Name: "Llave del Burdel"
  Description: "Abre la mayoría de las puertas en Thornwall. No todas las que deberías abrir."

=== QUESTS ===
Quest: "N5_Main"
  Name: "El Precio del Placer"
  Description: "Thornwall es un negocio. Descubre qué se vende realmente."
  Floor: 5
  Objectives:
    1. "Explora Thornwall"
    2. "Encuentra a la niña en la jaula"
    3. "Decide qué hacer"
    4. "Enfrenta al Carnicero"
  Rewards: Fragment_Thornwall, XP 700, Gold 300

Quest: "N5_Optional_CarniceroHistory"
  Name: "El Pasado del Carnicero"
  Description: "El Carnicero no siempre fue así. Busca pistas sobre su infancia."
  Floor: 5
  bIsHidden: true
  Objectives:
    1. "Encuentra la celda donde vivió el Carnicero de niño"
    2. "Lee sus diarios de entrenamiento"
  Rewards: "Blessing_Perdon" (unique buff)
*/
