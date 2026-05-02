// N9 — Argentia (Tortura Politica) — v3
// Dialogos, NPCs, Quests, Items — Verdugo no quiere pelear, Flashbacks personalizados, Codex

#pragma once

#include "CoreMinimal.h"

/*
=== PUERTA DE ARGENTIA (v3) ===
Node: N9_Arrival
  Grabado en puerta: "ARGENTIA — JUSTICIA ETERNA"
  Debajo: "Poblacion: 0."
  Narration: "La ciudad esta vacia. Pero las herramientas no tienen polvo."

  [ Codigo hexadecimal en el marco de la puerta ]
  Sera: "(leyendo) Es... codigo. Codigo fuente."
  Sera: "Dice: 'Player_Protagonista. Status: UNKNOWN. Reason: Conciencia no registrada en el diseno original.'"
  Sera: "(pausa) Arquitecto. El Arquitecto sabia de ti desde el principio."
  → Next: N9_Explore

=== MECANICA — FLASHBACKS PERSONALIZADOS (v3) ===
Node: N9_Flashback_System
  Narration: "Al entrar a cada sala de interrogacion, el sistema detecta la memoria del jugador."
  Narration: "Las victimas en los flashbacks no son genericas: son caras que el jugador ya vio."
  → Flashback 1 (si N5 visitado): Edren, la mujer de la jaula de N5, atada a una mesa.
  → Flashback 2 (si N6 visitado): Uno de los ninos de las Camaras de Jade, gritando.
  → Flashback 3 (si N4 visitado): La familia de la cabana, siendo interrogada.
  Narration: "No se puede saltar. El jugador debe ver hasta el final cada vez."
  → SetFlag: "N9.FlashbacksSeen"
  → Next: N9_Explore_Deep

=== ELIA — SALA DE INTERROGACION (v3) ===
Node: N9_Elia_First
  Elia: "(sentada en una silla, sin mirarte) Llegaste. Sabia que alguien iba a llegar."
  Elia: "(se levanta) 12,847 personas pase por esta sala. Algunas se fueron. La mayoria no."
  Player: "12,847? Como recuerdas el numero exacto?"
  Elia: "(sonrisa) Llevo la cuenta. Es lo unico que me queda."
  Elia: "La gente cree que la tortura es emocion. Gritos. Sangre. No. Es paciencia. Es esperar a que la persona entienda que no hay salida."
  Elia: "La mayoria confesaba. No porque fuera cierto. Porque querian morir."
  Elia: "Eso es poder."
  → Dialogo opcional — si el jugador carga el Fragmento del Arquitecto:
  Elia: "(viendo el fragmento) Eso... (pausa) Eso es del Arquitecto."
  Elia: "El venia aqui. Se sentaba donde estas tu. Veia las sesiones."
  Elia: "Nunca tomo notas. Nunca pregunto nada. Solo miraba."
  Elia: "Yo creia que le importaba el proceso. Ahora creo que le importaba otra cosa."
  Player: "Que?"
  Elia: "Confirmar que la gente podia ser rota. Porque el tambien estaba siendo roto."
  → Opcion [Matar] — cierra Final B
  → Opcion [Dejar vivir] — Elia entrega el Fragmento del Arquitecto (item clave N10)
  Elia: "(si la dejas vivir) Toma. Es lo unico que no he vendido. (te da el Fragmento) Te va a servir alla abajo."
  → SetFlag: "N9.EliaSpared"

=== OSWIN — FORJA (v3) ===
Node: N9_Oswin_First
  Oswin: (golpeando metal) "Un momento. Casi termino."
  Oswin: "Listo. Que necesitas?"
  Player: "Eres Oswin?"
  Oswin: "El mismo. Herrero de Argentia. 47 años."
  Player: "Que haces?"
  Oswin: "Instrumentos. De precision. Mira esta sierra — 47 confesiones. Record."
  Player: "Te refieres a tortura."
  Oswin: "Me refiero a eficiencia. El dolor saca la verdad mas rapido que las palabras."
  → Opcion si el jugador tiene Hueso (N4) + Lagrima (N6) + Sangre (N7):
  Oswin: "Esos materiales... (los toma) Voy a hacer algo con esto. No preguntes que."
  → Forja la Espada del Verdugo.
  → SetItem: "N9_EspadaVerdugo"

  → Opcion [Matar]
  → Opcion [Dejar vivir]

=== VERDUGO DE PLATA — SE SIENTA (v3) ===
Node: N9_Verdugo_Sits
  Narration: "El Verdugo de Plata esta sentado en el centro de la plataforma. La mascara en el suelo."
  Narration: "No tiene cara de monstruo. Tiene cara de quien tuvo 19 años hace mucho tiempo."
  Verdugo: "(sin mirarte) Se lo que viniste a hacer. Y no voy a pelear."
  Verdugo: "Si quieres matarme, hazlo. Pero no voy a darte el combate que buscas."
  → Opcion [Escuchar] — El Verdugo habla
  → Opcion [Atacar] — Forzar el combate

Node: N9_Verdugo_Listen (v3)
  Narration: "El jugador se sienta frente al Verdugo."
  Verdugo: "Tenía 19 años cuando empecé. Me dijeron que era por la seguridad de la ciudad."
  Verdugo: "La primera vez... vomité."
  Verdugo: "La centésima... ya no sentía nada."
  Verdugo: "La milésima... empecé a disfrutarlo."
  Verdugo: "(pausa larga) No me perdones. Solo... recuérdame como fui antes."
  Verdugo: "Antes de que esto fuera un trabajo. Antes de que 19 fuera una persona que todavía podía llorar."
  Narration: "El Verdugo se queda en silencio. La plataforma se abre detrás de él."
  Verdugo: "Pasa. Yo me quedo aquí."
  → El jugador puede pasar sin pelear.
  → SetFlag: "N9.VerdugoListened" (requisito Finales B y C)

Node: N9_Verdugo_ForcedCombat (v3)
  Narration: "El jugador levanta el arma."
  Verdugo: "(suspira) Como todos. Nadie quiere escuchar. Todos quieren pelear."
  Verdugo: "(se pone la máscara) Bien. Dame lo que viniste a dar."
  → Combate forzado. El jugador se convierte en lo que el piso critica.
  → Fase 1: Patrones rituales. Ventana de ataque 4s.
  → Fase 2: Cerebro de plata expuesto. 12 gritos diferentes.
  → Fase 3: Forma original del Verdugo (soldado de 19 años).
  → SetFlag: "N9.VerdugoForced"
  
=== CODEX (v3) ===
Codex: "N9_Archivo_112"
  Titulo: "Archivo del Arquitecto — Iteracion 112"
  Texto: "'Elia tenia tres hijos. Dejo de verlos cuando empezo a trabajar aqui. Uno le escribio una carta. 'Mama, se que me olvidaste. Pero yo no te olvido.' Elia la tiro a la basura frente a mi. Yo la recogi. La tengo en mi escritorio.'"
  Ubicacion: N9 — Escritorio de Elia

=== ITEMS (v3) ===
Item: "Fragment_Arquitecto"
  Name: "Fragmento del Arquitecto"
  Description: "Un disco de datos. Contiene registros del comportamiento del Arquitecto durante las sesiones."
  Type: KeyItem (requisito Final B)

Item: "N9_EspadaVerdugo"
  Name: "Espada del Verdugo"
  Description: "Forjada con los restos de cuatro pisos. Pesa como una culpa."

Item: "N9_PergaminoCodigo"
  Name: "Pergamino del Codigo Fuente"
  Description: "Player_Protagonista. Status: UNKNOWN. Conciencia no registrada."

=== QUESTS (v3) ===
Quest: "N9_Main_v3"
  Name: "Justicia Eterna"
  Description: "Argentia no juzga. Ejecuta. O escucha."
  Floor: 9
  Objectives:
    1. "Explora Argentia"
    2. "Sobrevive a los flashbacks"
    3. "Encuentra a Elia"
    4. "Encuentra a Oswin"
    5. "Enfrenta al Verdugo de Plata"
    6. "Decide si escuchas o atacas"
  Rewards: XP 1500, Fragmento del Arquitecto (condicional)

Quest: "N9_Optional_Abyss_v3"
  Name: "El Pozo"
  Description: "Tienes una pregunta que responder."
  Floor: 9
  bIsHidden: true
  Objectives:
    1. "Encuentra el pozo en el centro de Argentia"
    2. "Responde con honestidad"
  Rewards: Ending B flag component
*/
