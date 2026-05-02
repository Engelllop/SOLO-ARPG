// N6 — Cámaras de Jade (Explotación Infantil Industrial)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h"

/*
=== ENTRADA ===
Node: N6_Arrival
  Narration: "El eco del canto llega antes que la luz. Voces de niños. Cientos. Una canción que no conoces pero que tu cuerpo reconoce."
  → Next: N6_First_Chamber

=== PRIMERA CÁMARA DE CANTO ===
Node: N6_First_Chamber
  Narration: "La sala está iluminada por jade. Cinco niños, sentados en círculo, cantan. Sus ojos están abiertos pero no te ven."
  Player: (intenta hablarles)
  Narration: "No responden. Siguen cantando. El jade en las paredes pulsa con cada nota."
  → Next: N6_Explore

=== CHILD MINER ===
Node: N6_Miner_First
  Niño: (golpea la pared con un pico pequeño)
  Player: "¿Cuánto tiempo llevas aquí?"
  Niño: (sigue golpeando) "No sé. Ayer saqué 48 kilos. Hoy van 30."
  Player: "¿Cuántos años tienes?"
  Niño: (pausa) "Mi mamá decía que 10. Pero eso fue antes."
  Player: "Antes de qué."
  Niño: "Antes de que me vendieran. ¿Tienes pan?"
  → Choices:
    - "Dar pan" → N6_Miner_Feed
    - "No tengo" → N6_Miner_NoFood
  → SetFlag: "N6_Miner_Met"

Node: N6_Miner_Feed
  Niño: (toma el pan. No come. Lo guarda.) "Gracias. Para mi hermana. Canta en la cámara 3."
  → SetFlag: "N6_Miner_Fed"
  → PTSDComponent: -5 trauma

=== CAPATAZ — VASSEN REACCIONA (v3) ===
Node: N6_Foreman_VassenReacts
  Capataz: "Son huérfanos. Les damos techo y comida. Es un negocio."
  Capataz: (se ríe) "Además, nadie los extraña. Nadie vino a buscarlos."
  Narration: "Vassen se queda quieto 3 segundos."
  Narration: "Luego golpea al capataz en la cara sin avisar."
  Vassen: "(respirando fuerte) No debiste decir eso."
  Mira: "Vassen!"
  Vassen: "(al capataz, en el suelo) Hay 47 niños aqui. 12 muertos. Y dices que nadie los extraña?"
  Capataz: (sangrando) "Vas a... vas a pagar por esto-"
  Vassen: "(se inclina) Ya pague. Baje lo que baje en esta Torre, ya pague."
  → Combate contra el capataz comienza.
  → SetFlag: "N6.VassenHitForeman"

=== CORO DE JADE (Capataz — Combate) ===
Node: N6_Foreman_Combat_Start
  Capataz: "Sabía que ibas a ser problema. PURA VIDA — CANTEN MÁS FUERTE."
  → Los ChildSingers aumentan daño
  → Combate comienza

=== POST-CAPATAZ ===
Node: N6_PostForeman
  Capataz: (herido) "Yo también... empecé cantando."
  Player: "¿Qué?"
  Capataz: "A los 6. Después crecí. Me dejaron ser capataz. Porque obedecía."
  Capataz: "Y ahora... ¿qué van a hacer ellos? Sin mí, sin orden... van a cantar hasta morir."
  → Next: N6_Departure

=== POST-LIBERACIÓN (niños) ===
Node: N6_Aftermath
  Narration: "Sin el capataz, los niños dejan de cantar. Pero no se mueven. Se sientan. Esperan."
  NiñoMiner: "¿Y ahora qué hacemos?"
  Narration: "Uno empieza a cantar otra vez. Solo. Los demás lo siguen. No saben hacer otra cosa."
  → SetFlag: "N6_Children_Freed"
  → El ciclo no se rompe realmente

=== ITEMS ===
Item: "Fragment_Jade"
  Name: "Fragmento de Jade"
  Description: "Una gema de jade puro. Cuando la tocas, oyes un canto lejano."
  Type: KeyItem

Item: "Jade_Crudo"
  Name: "Jade Crudo"
  Description: "Jade sin pulir. Vale 10 de oro cada pieza. Útil para comerciar."
  Type: Resource
  Value: 10

Item: "Pan_Viejo"
  Name: "Pan Duro de Fábrica"
  Description: "El pan que les dan a los niños. Huele a metal."
  Type: Food
  HungerRestore: 10
  HPCost: 2

=== QUESTS ===
Quest: "N6_Main"
  Name: "El Canto del Jade"
  Description: "La fábrica de jade funciona con lágrimas. Decubre cómo detenerla."
  Floor: 6
  Objectives:
    1. "Explora las Cámaras de Jade"
    2. "Habla con un ChildMiner"
    3. "Enfrenta al Capataz"
  Rewards: Fragment_Jade, XP 800, Gold 200

Quest: "N6_Optional_Sister"
  Name: "La Hermana que Canta"
  Description: "El ChildMiner te pidió que busques a su hermana."
  Floor: 6
  bIsHidden: true
  Objectives:
    1. "Encuentra a la hermana en la Cámara 3"
    2. "Dile que su hermano la está esperando"
  Rewards: "Blessing_Hermandad" (heal companion)
  Requires: "N6_Miner_Fed"
*/
