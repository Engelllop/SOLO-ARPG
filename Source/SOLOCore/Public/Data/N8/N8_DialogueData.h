// N8 — Jardines Flotantes (Eugenesia)
// Diálogos, NPCs, Quests, Items

#pragma once

#include "CoreMinimal.h"

/*
=== SERA — ENCUENTRO INICIAL ===
Node: N8_Sera_First
  Sera: "Hola. No esperaba visitas."
  Player: "¿Quién eres?"
  Sera: "Me llamo Sera. Soy... una cosecha."
  Player: "¿Una cosecha?"
  Sera: (señala las ramas que crecen de su espalda) "Me cultivaron. Soy la que salió bien."
  → Next: N8_Tour_Begin

=== SERA — TOUR DEL LABORATORIO ===
Node: N8_Tour_Begin
  Sera: "Te voy a mostrar el jardín. Pero no es un jardín de flores."
  → Next: N8_Growth_Chamber

Node: N8_Growth_Chamber
  Narration: "Sera te lleva a una sala con hileras de tanques de vidrio. Dentro, fetos en diferentes etapas."
  Sera: "Aquí empieza todo. Toman una célula. La cultivan. Deciden qué va a ser."
  Player: "¿Deciden?"
  Sera: "Altura. Color de ojos. Sexo. Inteligencia. Si va a tener pecas. Todo."
  Player: "¿Y los que 'salen mal'?"
  Sera: (señala una puerta) "Allí. La Sala de Poda."
  → Next: N8_Pruning_Room

Node: N8_Pruning_Room
  Narration: "La sala huele a metal y alcohol. Mesas de operación. Instrumentos."
  Sera: "Si una cosecha tiene defectos... se repara. O se recicla."
  Player: "¿Reciclar?"
  Sera: "Se deshace. Las partes buenas se reutilizan. El resto... abono."
  → Next: N8_Criada_Approach

=== CRIADA BRONCE — PRE-COMBATE ===
Node: N8_Criada_First
  Criada: "Sera. Has vuelto."
  Sera: (retrocede) "Ella es... la Criada Bronce. La jardinera."
  Criada: (te mira) "Tú debes ser el visitante. Has visto el jardín. ¿Bonito, verdad?"
  Player: "No son personas. Son productos."
  Criada: (pausa) "Son lo que necesitan ser. Y yo soy lo que necesito ser. La que los cuida."
  → Next: N8_Criada_Combat

=== CRIADA BRONCE — MUERTE ===
Node: N8_Criada_Death
  Criada: (herida, arrodillada) "No... no me mata aún."
  Player: (arma levantada)
  Criada: "Dame... un nombre. Nunca tuve uno. Toda mi vida fui 'Criada Bronce'. 'La jardinera'."
  → Opción: INPUT DE TEXTO
    - Si el jugador escribe nombre: Criada sonríe. "Gracias. Ahora puedo morir."
    - Si no: "No importa. No iba a recordarlo."
  → SetFlag: "N8_Criada_Named" (si le dieron nombre)

=== POST-CRIADA ===
Node: N8_Aftermath
  Sera: (se sienta frente al tanque de la Cosecha Perfecta) "Podría haber sido yo."
  Player: "¿Vas a venir?"
  Sera: "No. Me quedo."
  Player: "¿Por qué?"
  Sera: "No conozco otro lugar. Y alguien tiene que apagar las máquinas."
  → SetFlag: "N8_Sera_Stays"

=== ITEMS ===
Item: "Fragment_Jardin"
  Name: "Fragmento de Jardín"
  Description: "Una semilla dorada. Al plantarla, crece algo que no es una planta."
  Type: KeyItem

Item: "Diario_Criada"
  Name: "Diario de la Criada Bronce"
  Description: "Registros de 200 años de podas. Escritos con caligrafía perfecta. Sin errores."
  Type: Readable

=== QUESTS ===
Quest: "N8_Main"
  Name: "El Jardín de Carne"
  Description: "Los Jardines Flotantes cultivan cuerpos. Descubre el propósito."
  Floor: 8
  Objectives:
    1. "Sigue a Sera por el laboratorio"
    2. "Ve la Sala de Crecimiento"
    3. "Ve la Sala de Poda"
    4. "Enfrenta a la Criada Bronce"
  Rewards: Fragment_Jardin, XP 1200

Quest: "N8_Optional_Name"
  Name: "El Último Regalo"
  Description: "La Criada Bronce nunca tuvo nombre. Decide si dárselo."
  Floor: 8
  bIsHidden: true
  Objectives:
    1. "Dale un nombre a la Criada Bronce"
  Rewards: "Blessing_Nombre" (special ending recognition)
*/
