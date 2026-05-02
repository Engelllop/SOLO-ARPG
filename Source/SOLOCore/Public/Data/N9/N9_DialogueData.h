// N9 — Argentia (Tortura Política)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h";

/*
=== PUERTA DE ARGENTIA ===
Node: N9_Arrival
  Grabado en puerta: "ARGENTIA — JUSTICIA ETERNA"
  Debajo: "Población: 0."
  Narration: "La ciudad está vacía. Pero las herramientas no tienen polvo."
  → Next: N9_Explore

=== ELIA — TALLER DE LÁTIGOS ===
Node: N9_Elia_First
  Elia: "¿Eres el nuevo? No, qué digo. No hay nuevos. Todos se fueron."
  Player: "¿Tú por qué te quedaste?"
  Elia: "Este es mi taller. 60 años aquí. ¿Por qué me iría?"
  Player: "¿Sabes lo que se hacía aquí?"
  Elia: (ríe) "Claro. Hacía látigos. Buenos látigos. Mira este — 7 colas. 300 usos antes de reemplazar una."
  Player: "Eso es... horrible."
  Elia: "¿Horrible? Es un oficio. Yo no azotaba a nadie. Hacía herramientas."
  → Choices:
    - "Matarla" → N9_Elia_Killed (cierra Ending B)
    - "Dejarla" → N9_Elia_Spare

Node: N9_Elia_Spare
  Elia: "¿No vas a hacer nada? Raro. Los héroes siempre hacen algo."
  Player: "No soy un héroe."
  Elia: "Ah. El primero honesto."
  → SetFlag: "N9_Elia_Spared"

=== OSWIN — HERRERO ===
Node: N9_Oswin_First
  Oswin: (golpeando metal) "Un momento. Casi termino."
  Oswin: (pausa) "Listo. ¿Qué necesitas?"
  Player: "¿Eres Oswin?"
  Oswin: "El mismo. Herrero de Argentia. 47 años."
  Player: "¿Qué haces?"
  Oswin: "Instrumentos. De precisión. Mira esta sierra — 47 confesiones. Récord."
  Player: "Te refieres a tortura."
  Oswin: "Me refiero a eficiencia. El dolor saca la verdad más rápido que las palabras."
  → Choices:
    - "Matarlo" → N9_Oswin_Killed (cierra Ending B)
    - "Dejarlo" → N9_Oswin_Spare

Node: N9_Oswin_Spare
  Oswin: "Sabia decisión. Sin mí, no hay justicia."
  Player: "Sin ti no hay tortura."
  Oswin: "Misma moneda."
  → SetFlag: "N9_Oswin_Spared"

=== VERDUGO PLATA — COMBATE ===
Node: N9_Verdugo_PreCombat
  Verdugo: (sentado. Máscara en la mano. Cara descubierta.)
  Verdugo: "Llegaste. Te esperaba."
  Player: "¿Quién eres?"
  Verdugo: "Nadie. Soy el que ejecuta. Empecé a los 19."
  Player: "¿Y ahora?"
  Verdugo: (se pone la máscara) "Ahora termino."
  → Combate. Fase 1.

=== VERDUGO — FASE 2 (Máscara rota) ===
Node: N9_Verdugo_Phase2
  Verdugo: (máscara rota. Cara de niño.) "No recuerdo sus caras. Solo los números."
  Player: "¿Cuántos?"
  Verdugo: "¿Importa? Todos son el mismo después del primero."
  → Combate. Fase 2.

=== VERDUGO — MUERTE ===
Node: N9_Verdugo_Death
  Verdugo: (cae) "19... empecé a los 19..."
  → Ethereal: su espíritu habla
  VerdugoFantasma: "No era malo. Solo obedecía. ¿Eso me hace menos culpable?"
  → Choices:
    - "Sí" → N9_Verdugo_Forgive
    - "No" → N9_Verdugo_NoForgive

Node: N9_Verdugo_Forgive
  VerdugoFantasma: "Gracias. Eso es más de lo que me dieron a mí."
  → SetFlag: "N9_Forgave_Verdugo"

Node: N9_Verdugo_NoForgive
  VerdugoFantasma: "Tienes razón. Bueno... que te vaya bien."
  → SetFlag: "N9_NoForgave_Verdugo"

=== POZO DEL ABISMO (N9 opcional) ===
Node: N9_Abyss
  Narration: "En el centro de Argentia hay un pozo. No tiene fondo. O tal vez sí, pero no se ve."
  VozDelPozo: "Has visto mucho. Sangre. Niños. Cuerpos cosidos. ¿Y ahora?"
  VozDelPozo: "Mírame. Mírate. ¿Eres diferente a ellos?"
  Player:
    - "Sí" → N9_Abyss_No
    - "No" → N9_Abyss_Yes
    - (silencio) → N9_Abyss_Silence

Node: N9_Abyss_Yes
  VozDelPozo: "Bien. No es la respuesta correcta. Pero es honesta."
  → SetFlag: "N9_Looked_Into_Abyss" (requisito Ending B)

Node: N9_Abyss_No
  VozDelPozo: "Mentira. Todos son iguales. Pero gracias por jugar."
  → Nothing

Node: N9_Abyss_Silence
  VozDelPozo: "..."
  → SetFlag: "N9_Looked_Into_Abyss"

=== ITEMS ===
Item: "Mirada_Abismo"
  Name: "La Mirada del Abismo"
  Description: "No es un objeto. Es una comprensión. El abismo te vio a ti también."
  Type: KeyItem (invisible)

=== QUESTS ===
Quest: "N9_Main"
  Name: "Justicia Eterna"
  Description: "Argentia no juzga. Ejecuta."
  Floor: 9
  Objectives:
    1. "Explora Argentia"
    2. "Encuentra a Elia"
    3. "Encuentra a Oswin"
    4. "Enfrenta al Verdugo Plata"
  Rewards: Mirada_Abismo, XP 1500

Quest: "N9_Optional_Abyss"
  Name: "El Pozo"
  Description: "Hay algo en el centro de Argentia. Una pregunta que no tiene respuesta."
  Floor: 9
  bIsHidden: true
  Objectives:
    1. "Encuentra el pozo en el centro de la ciudad"
    2. "Responde con honestidad"
  Rewards: Ending B flag component
*/
