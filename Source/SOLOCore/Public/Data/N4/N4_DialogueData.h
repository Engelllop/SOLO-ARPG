// N4 — Bosque Petrificado (Hambre) — v3
// Dialogos, NPCs, Quests, Items, Esquirlas Explicacion, Codex

#pragma once

#include "CoreMinimal.h"

/*
=== FAMILIA EN CABAÑA ===
Node: N4_Cabin_Enter
  Madre: "¿Has comido? Te ves flaco. Siéntate."
  Player: "¿De dónde sacaron comida?"
  Madre: "Del bosque. Da lo que puede."
  Padre: (señala la pierna en la mesa) "Caza. Caza buena."
  Player: "Eso no es de animal."
  Madre: (pausa) "Comes o no comes. No hay tercera opción."
  → Choices:
    - "Me siento" → N4_Cabin_Eat
    - "Me voy" → N4_Cabin_Leave
    - "Los voy a matar" → N4_Cabin_Attack

Node: N4_Cabin_Eat
  Madre: "Bueno. No es mucho, pero compartimos."
  Narration: "La carne tiene un sabor que no reconoces. El niño te mira sin parpadear mientras comes."
  Player: (en silencio)
  Madre: "No hay un 'antes', ¿sabes? Solo hay ahora. Y ahora hay esto."
  → Heal: +40HP
  → SetFlag: "N4_Ate_With_Family"
  → Narration: "Mira no come. Se queda mirando el plato."
  → Narration: "Se levanta y sale. La encuentras afuera, vomitando."
  → (Si Mira presente: relacion -5)
  → Next: N4_Continue

Node: N4_Cabin_Leave
  Madre: "Como quieras. La puerta está abierta."
  Padre: (murmura) "No va a durar una noche ahí fuera."
  → SetFlag: "N4_Left_Cabin"

Node: N4_Cabin_Attack
  Madre: (sin miedo) "Ah. ¿Un héroe? El bosque está lleno de héroes. Todos muertos."
  → Combat: Family (weak, 3x30HP)
  → SetFlag: "N4_Killed_Family"

=== COLO DEL HAMBRE (Encuentro) ===
Node: N4_Coloso_FirstSight
  Narration: "Lo ves entre los árboles. Alto como una casa. No camina. Se arrastra. De su boca abierta sale un sonido — no un rugido, sino un suspiro largo."
  Coloso: (no habla. Solo respira.)
  → Next: N4_Explore_Forest

=== CORAZÓN DEL HAMBRE ===
Node: N4_Heart_Discovery
  Narration: "En la cueva, un corazón late. No es de piedra ni de carne. Es hambre hecha forma. A su alrededor, formas embrionarias flotan."
  Voz: "No... no lo destruyas."
  Player: "¿Quién habla?"
  Voz: "El bosque. Lo que queda de él. Sin el corazón... el hambre se esparcirá. Sin control."
  Player: "¿Y con él? Sigue matando."
  Voz: "Sí. Pero es lento. Da tiempo. Tiempo para decir adiós."
  → Choices:
    - "Destruir el corazón" → N4_Heart_Destroy
    - "Dejarlo" → N4_Heart_Leave (boss respawns)

Node: N4_Heart_Destroy
  Narration: "El corazón se rompe. El bosque tiembla. El Coloso cae. Los árboles... no vuelven a ser personas."
  → SetFlag: "N4_Heart_Destroyed"
  → Reward: "Fragment_Hambre"

=== ESCENA — ESQUIRLAS (v3, despues de la cueva) ===
Node: N4_Esquirlas_Scene
  Mira: "Garrik... que es una Esquirla de la 6ta Edad?"
  Garrik: "Cada 'Edad' es una version de la Torre. La primera fue de piedra y fuego. La sexta fue la ultima antes de la actual."
  Garrik: "En la 6ta Edad hubo una guerra entre los pisos. Todos contra todos."
  Garrik: "Cuando termino, los que sobrevivieron quedaron atrapados en el codigo de la nueva version. Fragmentos de algo anterior que no fue limpiado."
  Garrik: "Eso soy yo. No deberia estar aqui. Mi codigo pertenece a otra compilacion."
  Garrik: "Pero no me eliminaron porque sirvo para algo."
  Mira: "Para que?"
  Garrik: "(pausa larga) Para recordar que ya hubo un apocalipsis aqui. Que el mundo termino antes. Y que siguio igual."
  Player: "(internamente) Cada version termina. Y empieza otra. Que soy yo entonces?"
  → Next: N4_Heart_Discovery

=== NPC: PEREGRINO MUERTO ===
Node: N4_DeadPilgrim
  Narration: "Un cuerpo en el suelo. Todavía tiene las manos extendidas, como si buscara algo."
  Diario: "Día 14: Comí tierra. Día 16: Comí mi cinturón. Día 18: Vi una luz. Día 19: Era otro árbol. Día 20: Adiós."
  → Item: "Diario_Peregrino" (readable)

=== ITEMS ===
Item: "Fragment_Hambre"
  Name: "Fragmento de Hambre"
  Description: "Un trozo de corazón petrificado. Cuando lo tocas, sientes un vacío que no se llena."
  Type: KeyItem
  bIsStackable: false

Item: "Fruta_Podrida"
  Name: "Fruta Podrida"
  Description: "No deberías comer esto. Pero tienes hambre."
  Type: Food
  HungerRestore: 15
  HPCost: 5

Item: "Carne_Sospechosa"
  Name: "Carne Ahumada"
  Description: "Sabes exactamente qué es. Comes igual."
  Type: Food
  HungerRestore: 40
  HPCost: 10

Item: "Pan_Duro"
  Name: "Pan Duro"
  Description: "Duro como piedra. Pero es comida."
  Type: Food
  HungerRestore: 20

=== QUESTS ===
Quest: "N4_Main"
  Name: "El Hambre"
  Description: "Atraviesa el Bosque Petrificado. No confíes en lo que te ofrezcan."
  Floor: 4
  Objectives:
    1. "Explora el bosque"
    2. "Encuentra la cabaña"
    3. "Llega al corazón del hambre"
    4. "Decide el destino del bosque"
  Rewards: Fragment_Hambre, XP 600, Gold 150

Quest: "N4_Optional_Family"
  Name: "Lo que Queda"
  Description: "La familia en la cabaña no es mala. Solo sobrevive."
  Floor: 4
  bIsHidden: true
  Objectives:
    1. "Habla con la familia (no ataques)"
    2. "Decide si compartes tu comida con ellos"
  Rewards: "Blessing_Sobreviviente" (buff)
*/
