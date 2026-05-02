// N7 — Sahal (Genocidio / Desmembramiento)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h"

/*
=== ENTRADA A SAHAL ===
Node: N7_Arrival
  Narration: "Sahal. La Ciudad de la Unión Eterna. Así la llamaban antes."
  Narration: "Antes de que alguien decidiera que la unión eterna significaba no poder separarse nunca."
  → Next: N7_City_Explore

=== MIRA — PRIMER ENCUENTRO ===
Node: N7_Mira_First
  Mira: "Llegaste. Sabía que lo harías."
  Player: "Mira. ¿Dónde está Vassen?"
  Mira: "La Costurera lo tiene. Me tiene a mí también. Pero hablamos por los hilos."
  Player: "¿Los hilos?"
  Mira: "Nos conecta. Siento su dolor. Él siente el mío. No podemos mentirnos."
  → Next: N7_Mira_Bound

=== VASSEN — ATRAPADO ===
Node: N7_Vassen_First
  Vassen: "No la escuches. No confíes en ella. La Costurera usa los hilos para hablar con su voz."
  Player: ¿Mira? ¿Eres tú?"
  Vassen: "No sé quién soy. A veces hablo. A veces ella habla por mí."
  → Next: N7_Costurera_Approach

=== LA COSTURERA — PRE-COMBATE ===
Node: N7_Costurera_PreCombat
  Costurera: "Ah. El viajero. Has llegado lejos."
  Costurera: "Los otros pisos son groseros. Violencia. Sangre. Yo hago arte."
  Player: "Suelta a mis amigos."
  Costurera: "¿Amigos? ¿Estos? Son material. Buena tela. Buena voz para gritar."
  → Next: N7_Combat_Phase1

=== POST-MUERTE DE MIRA Y VASSEN ===
Node: N7_AfterCutscene
  Narration: (silencio de 5 segundos. Sin música. Sin sonido.)
  Costurera: "Ahora sí. Todos cosidos."
  → Fase 3 comienza

=== COSTURERA — MUERTE ===
Node: N7_Costurera_Death
  Costurera: (cayendo) "Sahal era hermosa... antes del accidente."
  Player: "¿Qué accidente?"
  Costurera: "El que me hizo a mí."
  → El suelo tiembla. Las costuras se abren.
  → SetFlag: "N7_Costurera_Dead"
  → Reward: "Fragment_Sahal"

=== MARIONETAS (Enemies Ambient) ===
Node: N7_Marioneta_Dialog_1
  (Si el jugador se acerca a una marioneta todavía viva)
  Marioneta: "Hace frío."
  Player: "¿Puedes hablar?"
  Marioneta: "No debería. La Costurera no sabe que hablo. Si se entera, me vuelve a coser."
  → Next: N7_Marioneta_Info

=== ITEMS ===
Item: "Fragment_Sahal"
  Name: "Fragmento de Sahal"
  Description: "Un hilo ensangrentado. Al tacto, sientes el dolor de quien lo usó."
  Type: KeyItem

Item: "Carta_Vassen"
  Name: "Carta de Vassen (sin terminar)"
  Description: " 'Mira, si no salimos de aquí, quiero que sepas que—' La carta se corta. Firmado: 'Siempre tuyo.' "
  Type: Readable

=== QUESTS ===
Quest: "N7_Main"
  Name: "La Unión Eterna"
  Description: "Sahal está cosida a la fuerza. Literalmente."
  Floor: 7
  Objectives:
    1. "Encuentra a Mira"
    2. "Encuentra a Vassen"
    3. "Rompe los 6 hilos que los atan" (Fase 1)
    4. "Enfrenta a la Costurera" (Fase 3)
  Rewards: Fragment_Sahal, XP 1000

Quest: "N7_Optional_Marionetas"
  Name: "Voces Cosidas"
  Description: "Las marionetas de Sahal conservan fragmentos de conciencia."
  Floor: 7
  bIsHidden: true
  Objectives:
    1. "Habla con 3 marionetas diferentes"
    2. "Escucha sus historias"
  Rewards: "Blessing_Memoria" (bonus to final choice recognition)
*/
