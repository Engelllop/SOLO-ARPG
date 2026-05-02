// N3 — Aelthar (Ciudad Espejo)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h"

// ==============================
// DIALOGUE NODES (for DataTable)
// ==============================

/*
=== GARRIK — LLEGADA ===
Node: N3_Garrik_Arrival
  Garrik: "Aelthar. La ciudad espejo. ¿Has visto algo igual?"
  Player: "Es hermosa."
  Garrik: "Lo es. Por eso vinimos. Dicen que aquí se negocia la paz entre los reinos."
  Player: "¿Entre elfos y humanos?"
  Garrik: "Eso dicen. Pero no me fío de ciudades que brillan tanto."
  → Next: N3_Garrik_Explore

Node: N3_Garrik_Explore
  Garrik: "Voy a hablar con los comerciantes. Busca pistas. Nos vemos en la fuente."
  Player: "¿Qué tipo de pistas?"
  Garrik: "No sé. Algo que no cuadre. Siempre hay algo que no cuadra."
  → Next: N3_Fountain_Reflection

=== EL REFLEJO (trigger en fuente) ===
Node: N3_Fountain_Reflection
  Narration: "Te inclinas sobre la fuente. Tu reflejo te devuelve la mirada. Pero al lado... hay otro rostro."
  NiñoReflejo: "No confíes en los espejos."
  Player: "¿Quién eres?"
  NiñoReflejo: "No hay tiempo. Los espejos no reflejan. Muestran. Ventanas a otra parte."
  Player: "¿Dónde estás?"
  NiñoReflejo: "Abajo. Siempre abajo."
  → Next: N3_Fountain_Vanishes
  → SetFlag: "N3_Reflection_Seen"

=== GARRIK — REUNIÓN EN FUENTE ===
Node: N3_Garrik_Reunion
  Garrik: "¿Encontraste algo?"
  Player: "Vi a un niño. En el reflejo."
  Garrik: "¿Un niño? ¿Estás bien?"
  Player: "No era una ilusión. Dijo que estaba abajo."
  Garrik: "...Mierda. Vamos a investigar."
  → Next: N3_Basement_Entrance

=== SÓTANO — DESCUBRIMIENTO ===
Node: N3_Basement_Discovery
  Narration: "Detrás del altar del templo, una puerta disimulada. Escalones que bajan."
  Garrik: "Esto es... una celda."
  NiñoEnCelda: (no habla. Mira fijamente.)
  Garrik: "Hay más. Todos niños."
  Player: "¿Cuántos?"
  Garrik: "No sé. Demasiados."
  → Next: N3_Garrik_Truth
  → SetFlag: "N3_Basement_Found"

Node: N3_Garrik_Truth
  Garrik: "Los espejos. No eran decoración. Eran ventanas. Para que los clientes..."
  Player: "Vieran lo que compraban."
  Garrik: (silencio) "Voy a abrir las celdas. Tú ve al templo. Hay algo más grande allí."
  Player: "¿El Leviatán?"
  Garrik: "El que sea. Acaba con esto."
  → Next: N3_Leviathan_Approach

=== POST-LEVIATHAN ===
Node: N3_PostLeviatan
  Garrik: "Los niños... no quieren salir."
  Player: "No saben qué hay fuera."
  Garrik: "Me quedo. Alguien tiene que enseñarles que el mundo no es solo esto."
  Player: "¿Estás seguro?"
  Garrik: "No. Pero es lo correcto."
  → SetFlag: "N3_Garrik_Stays"
  → Reward: "Fragment_Espejo"
*/

// ==============================
// NPC DATA (for DataTable)
// ==============================

/*
RowName: "N3_Garrik"
  Name: "Garrik"
  Title: "Guerrero Errante"
  Floor: 3
  bIsCompanion: true
  bCanBeAttacked: false
  InitialDialogue: "N3_Garrik_Arrival"
  MeshPath: "/Game/Characters/N3/Garrik/Garrik_Base"

RowName: "N3_Senescal"
  Name: "Senescal Vael"
  Title: "Guardián de Aelthar"
  Floor: 3
  bIsCompanion: false
  bCanBeAttacked: true  // but drops nothing special
  InitialDialogue: "N3_Senescal_Welcome"

RowName: "N3_NinoReflejo"
  Name: "???"
  Title: "Reflejo"
  Floor: 3
  bIsCompanion: false
  bCanBeAttacked: false
  InitialDialogue: "N3_Fountain_Reflection"
  bIsTriggerOnly: true
*/

// ==============================
// QUEST DATA
// ==============================

/*
=== Quest 1: El Espejo Roto ===
QuestID: "N3_Main"
  Name: "El Espejo Roto"
  Description: "Aelthar esconde algo detrás de su brillo. Descubre la verdad."
  Floor: 3
  Objectives:
    1. "Explora Aelthar" (optional)
    2. "Ve el reflejo en la fuente" → hidden
    3. "Encuentra el sótano"
    4. "Derrota al Leviatán"
  Rewards:
    - Item: "Fragment_Espejo" (Ending B key)
    - XP: 500
    - Gold: 200
  Prerequisites: "N2_Main" (or N0_Complete)

=== Quest 2: El Precio del Silencio (Opcional) ===
QuestID: "N3_Optional_Children"
  Name: "El Precio del Silencio"
  Description: "Los niños de Aelthar necesitan más que ser liberados."
  Floor: 3
  bIsHidden: true
  Objectives:
    1. "Habla con tres niños en las celdas"
    2. "Dale comida a los que tengan hambre"
    3. "Decide qué hacer con el templo"
  Rewards:
    - Item: "Blessing_Aelthar" (healing boost N4)
  Prerequisites: "N3_Main" (active)
*/

// ==============================
// ITEM DATA
// ==============================

/*
=== Fragmento de Espejo (Key Item) ===
RowName: "Fragment_Espejo"
  Name: "Fragmento de Espejo"
  Description: "Un pedazo de espejo roto. Cuando lo miras, ves una versión de ti que tomó otra decisión."
  Type: KeyItem
  bIsStackable: false
  bIsQuestItem: true
  Icon: "/Game/UI/Icons/KeyItems/Fragment_Espejo"

=== Bendición de Aelthar (Consumable) ===
RowName: "Blessing_Aelthar"
  Name: "Bendición de Aelthar"
  Description: "Un niño te tomó la mano antes de irse. Todavía sientes el calor."
  Type: Consumable
  Effect: "HealOverTime 30s + 50HP"
  bIsStackable: false
  Icon: "/Game/UI/Icons/Items/Blessing_Aelthar"
*/
