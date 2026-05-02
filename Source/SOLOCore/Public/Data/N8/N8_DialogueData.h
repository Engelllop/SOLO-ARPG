// N8 — Jardines de Carne (Eugenesia / Experimentacion Humana) — v3
// Dialogos, NPCs, Quests, Items, Sera Quiebre, Voz Arquitecto

#pragma once

#include "CoreMinimal.h"

/*
=== COMBATE INICIAL — CRIADA DE BRONCE (v3) ===
Node: N8_Criada_Fight
  Narration: "El protagonista cae directamente en la sala de la Criada al entrar al piso."
  Narration: "Sin exploracion previa. Sin contexto. Solo el combate."
  Fase 1: Movimientos mecanicos.
  Criada: "Sujeto no registrado. Clasificar... defectuoso. Eliminar."

  Fase 2: Armadura de bronce se agrieta. Debajo hay carne viva. Voz cambia.
  Criada: "Por que... por que obedeci? Puedo... puedo elegir?"

  Fase 3: Antes de morir, pide un nombre.
  Criada: "Nunca tuve un nombre. Solo un numero. (pausa) Dimelo. Por favor."
  → Si el jugador ingresa un nombre: SetFlag "N8.CriadaNamed" + nombre guardado
  → Si el jugador se va sin nombrarla: la Criada muere sin nombre
  Criada: "(si nombrada) Gracias. Ahora puedo... irme."
  Criada: "(si no nombrada) ...Entiendo."

  → Next: N8_Garden_Explore (el jardin se abre)

=== VIVERO (v3 — Voz Arquitecto #1) ===
Node: N8_Vivero
  Narration: "Tanques de vidrio con fetos. Uno roto en el suelo arrastrandose."
  Voz Arquitecto: "(lejana) Observando."
  Sera: "Sali de uno de esos tanques. Lo recuerdo. Generacion 84."
  Sera: "Desarrollamos empatia. El Arquitecto llamo a eso un defecto."
  → Next: N8_GeneticArchive

=== ARCHIVO GENETICO (v3 — Sera se quiebra) ===
Node: N8_GeneticArchive
  Narration: "El Archivo Genetico."
  Texto: "Generacion 47: 79% murieron. 12% desarrollaron conciencia. 9% sentian empatia."
  Texto: "Nota: Generacion 84 produjo a Sera. Se escapo."
  Texto: "Estado: NO RECUPERADA."
  Sera: "(mirando la pantalla en blanco) Yo soy la Generacion 84."
  Sera: "(pausa larga) No me conocen como Sera alli. Me conocen como 'Fuga.'"
  Sera: "(voz quebrada) Sabia que iba a encontrar esto. Lo sabia desde N7."
  Sera: "(se sienta en el suelo. No se mueve.) No puedo... no puedo seguir."
  Narration: "Sera no se mueve. No mira al protagonista. Solo mira la pantalla."
  Narration: "Es la primera vez que alguien la ve asi."
  → Opcion: [Esperar] — sentarse junto a ella 30 segundos
  → Opcion: [Hablar] — "No tengo codigo original yo tampoco."
  → Opcion: [Tocar el hombro] — Sera reacciona
  → Cualquier opcion:
  Sera: "(despues de un momento) Tu tambien eres un error. Y aqui estamos."
  Sera: "(se levanta sola) Bueno. Vamos."

Node: N8_Post_Break
  Narration: "Sera vuelve a caminar. Pero algo cambio."
  Narration: "Por primera vez, no esta observando. Esta presente."

  Voz Arquitecto: "(lejana, mas clara) Interesante. Eso tampoco estaba en el script."

=== JARDIN DE LOS BRAZOS (v3) ===
Node: N8_GardenOfArms
  Narration: "Brazos humanos que brotan del suelo como hierba."
  Narration: "Tienen cicatrices de batalla. No son experimentos — son soldados que ya no necesitaban."
  → Next: N8_SoldierGallery

=== GALERIA DE LOS SOLDADOS (v3) ===
Node: N8_SoldierGallery
  Narration: "Filas de adultos modificados en cajas de cristal. Conscientes. Despiertos."
  Narration: "Algunos han raspado sus nombres en el vidrio desde adentro."
  Soldado: "Me llamo Edren. Era panadero. Me dijeron que me harian mas fuerte. Dije que si porque mi familia tenia hambre."
  Soldado: "Me convertir en esto. Mi familia recibio oro. Yo recibi... (pausa) no se que recibi."
  Sera: "(a Edren) Edren. Te escucho."
  Edren: "Puedes sacarme?"
  Sera: "No. (sin excusas) Lo siento."
  → El jugador puede tocar el cristal. Sin efecto.
  → El Indice de Pureza baja al ver el sufrimiento.

  Voz Arquitecto: "(lejana) Por que eso? Que significa para ti?"

=== CAMARA DE LAS MADRES (v3) ===
Node: N8_ChamberOfMothers
  Narration: "4 mujeres conectadas a maquinas, vientres abiertos."
  → Opcion: Matarlas (misericordia)
  → Opcion: Dejarlas (sufren)
  → Opcion: Preguntarle a Sera (ella responde: "Lo mio fue peor o mejor? No se cual es cual.")

=== ITEMS (v3) ===
Items:
  Pluma de la Criada — Lo unico que dejo al desaparecer
  Carta Genetica — Generacion 84: FUGA
  Diario de Edren — "Hoy recorde el olor del pan. No se si es real o inventado."
  Permiso Iteracion 85 — Archivo del Arquitecto. "Permiso medico. Motivo: no puedo."

=== QUESTS (v3) ===
Quest: "N8_Main"
  Name: "Los Jardines de Carne"
  Description: "Un laboratorio donde la vida se cultiva como producto."
  Floor: 8
  Objectives:
    1. "Derrota a la Criada de Bronce"
    2. "Explora el Vivero"
    3. "Visita el Archivo Genetico" (es obligatorio — Sera se quiebra aqui)
    4. "Atraviesa la Galeria de los Soldados"
    5. "Sal de los Jardines"
  Rewards: XP, Sera Companion (oficial), Fragmento

Quest: "N8_85_Secret"
  Name: "Lo que el Arquitecto Dejo Atras"
  Description: "Un nivel nunca terminado. Accesible con el Permiso I85"
  Floor: 8
  bIsHidden: true
  Objectives:
    1. "Encuentra el Permiso I85 en el Archivo"
    2. "Accede al Nivel Inacabado"
    3. "Lee la nota del escritorio"
  Rewards: "Codex_N85_ArchitectQuit"
*/
