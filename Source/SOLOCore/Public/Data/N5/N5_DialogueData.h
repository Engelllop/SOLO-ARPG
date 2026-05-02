// N5 — Thornwall (Esclavitud Sexual) — v3
// Dialogos, NPCs, Quests, Items — SIN BOSS FINAL

#pragma once

#include "CoreMinimal.h"

/*
=== LLEGADA A THORNWALL (v3) ===
Node: N5_Arrival
  Narration: "Las puertas de Thornwall estan abiertas. No hay guardias. No hay bienvenida. Solo musica que viene de adentro."
  Voz: "(desde la calle) Eh, viajero! Buscas diversion? Aqui hay de todo."
  ◈ [GLITCH] En la entrada, un cartel con texto decorativo en la parte inferior. Mira lo lee en voz alta.
  Mira: "No es un idioma. Son numeros y letras. 'S-U-F-R-I-M-I-E-N-T-O-v2-7.' Que significa? 'cliente: CORPORACION ARCO.'"
  Vassen: "...Que es una corporacion? Y por que su nombre esta en un burdel?"
  → Next: N5_Street_Explore

=== CALLE PRINCIPAL (v3 — NO BOSS) ===
Node: N5_Street_Explore
  Narration: "Por las calles caminan mujeres con vestidos caros. Collares con nombres grabados."
  Narration: "Esclavas. No hay cadenas — la deuda es la cadena."
  Mujer: "(sin mirarte) Si no te vas hoy, no te vas nunca. No es amenaza. Es como funciona."
  → Prueba de la Condicion:
  → La puerta de salida NO se abre peleando.
  → La puerta se abre cuando el grupo acepta irse.
  
=== CONVERSACION CLAVE — GARRIK THORNWALL (v3) ===
Node: N5_Garrik_Confession
  Narration: "Garrik se detiene en medio de la calle. No mira a nadie."
  Garrik: "La primera vez que pase por aqui... compre a alguien."
  Mira: "(se congela)"
  Garrik: "No sabia que mas hacer. Estaba solo. Asustado. Y ella estaba ahi."
  Garrik: "Le pague 100 de oro. Le dije que se fuera. Le di una carta para que la llevara a un lugar seguro."
  Garrik: "No se si llego. Nunca supe su nombre."
  Garrik: "(reanuda la marcha) Por eso volvi. Para averiguar si existe algo como la redencion."
  Vassen: "...Y existe?"
  Garrik: "Todavia no lo se."
  → Next: N5_Street_Continue

=== LA NIÑA EN LA JAULA (v3) ===
Node: N5_CagedGirl_First
  Narration: "En una esquina del segundo nivel, hay una jaula de oro. Las barras estan manchadas."
  Narration: "Dentro hay una niña. Tendra 14, 16 años. Desnuda. Moretones. Marcas de mordidas."
  Cartel: "Virgen. 250 de oro."
  Niña: "(no habla. Mira al piso.)"
  Player: "Como te llamas?"
  Niña: "(levanta la cabeza. Tarda en responder.) No... me acuerdo."
  Mira: "(susurra) Dios... que le hicieron?"
  → Opciones:
    - Pagar 250 de oro → se ahorca despues (N5_Girl_Paid)
    - Matar a Torben (el guardia) → el collar explota (N5_Girl_KillGuard)
    - No hacer nada → es subastada (N5_Girl_Ignored)
  → No hay opcion buena.

=== GARRIK — CARTA DE LA CHICA (v3) ===
Node: N5_Garrik_Letter
  Narration: "En una habitacion vacia del segundo piso, Garrik encuentra algo."
  Garrik: "(recogiendo un papel doblado) Es la carta. La que le di a la chica."
  Garrik: "Nunca la entrego. Estaba aqui todo este tiempo."
  Narration: "La carta esta abierta. Alguien la leyo."
  → SetItem: "N5_CartaNoEntregada"
  → SetFlag: "N5.LetterFound"

=== LA SALIDA (v3) ===
Node: N5_Departure
  Narration: "El grupo se reune en la entrada."
  Mira: "(en voz baja) No podemos llevarnos a todos. No podemos matarlos a todos."
  Vassen: "Entonces... esto sigue funcionando. Cuando nos vamos."
  Garrik: "Si. Thornwall va a seguir aqui manana. Y pasado. Y dentro de cien años."
  Garrik: "Eso tambien es parte del mensaje."
  Mira: "Que mensaje?"
  Garrik: "Que algunas ciudades no se derrotan con una espada."
  Narration: "La puerta se abre. No porque alguien la abrio. Porque el sistema detecta que el grupo acepto seguir."
  → SetFlag: "N5_Left_Thornwall"
  → Salida disponible. La ciudad sigue en pie.

=== CODEX (v3) ===
Codex: "N5_ArcoContract"
  Titulo: "Contrato Original — Thornwall"
  Texto: "Corporacion Arco. Fecha: Ano 3441 del Tejido. 'El contratante se compromete a proveer contenido eticamente cuestionable pero legalmente defendible.'"
  Ubicacion: N5 — Entrada (hexadecimal decodificado)

=== ITEMS (v3) ===
Item: "Fragment_Thornwall"
  Name: "Fragmento de Thornwall"
  Description: "Un cartel doblado. 'Virgen. 250 de oro.' Las letras estan manchadas."
  Type: KeyItem

Item: "N5_CartaNoEntregada"
  Name: "Carta de Garrik — Nunca Entregada"
  Description: "'No se tu nombre. Pero decidi que vas a estar bien. Vas a salir de ahi. Te espero en la entrada.' La carta nunca salio de Thornwall."

=== QUESTS (v3) ===
Quest: "N5_Main_v3"
  Name: "Thornwall"
  Description: "Thornwall no se derrota. Se abandona."
  Floor: 5
  Objectives:
    1. "Explora Thornwall"
    2. "Encuentra a la niña en la jaula"
    3. "Decide que hacer"
    4. "Acepta que no puedes salvar a todos"
    5. "Vete"
  Rewards: Fragment_Thornwall, XP 700, Gold 300

Quest: "N5_Optional_GarrikLetter"
  Name: "La Carta"
  Description: "Garrik dejo algo aqui hace un año."
  Floor: 5
  bIsHidden: true
  Objectives:
    1. "Encuentra la carta en el segundo piso"
  Rewards: "Character development — Garrik"
*/
