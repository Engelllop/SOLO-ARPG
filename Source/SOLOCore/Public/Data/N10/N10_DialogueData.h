// N10 — El Centinela de Granito / El Arquitecto (v3 Final)
// Dialogos, Archivos Corporativos, Debug Fight, Los 3 Finales, Codex

#pragma once

#include "CoreMinimal.h"

/*
=== CORREDOR DE ESPEJOS (v3) ===
Node: N10_MirrorHall
  Narration: "Pasas por el corredor. Cada espejo muestra un recuerdo."
  Espejo1: "N3. El niño en el reflejo. Todavía espera."
  Espejo2: "N4. La familia. La pierna en la mesa."
  Espejo3: "N5. La jaula vacía. El cartel en el piso."
  Espejo4: "N6. El canto. Siempre el canto."
  Espejo5: "N7. Vassen. Su mano separándose de su cuerpo."
  Espejo6: "N8. Sera. Sentada frente al tanque."
  Espejo7: "N9. El pozo. Te devuelve la mirada."
  → Next: N10_Architect_First

=== OFICINA DEL ARQUITECTO (v3) ===
Node: N10_Architect_First
  Arquitecto: (tecleando) "Un momento. Casi termino esta linea."
  Arquitecto: "Bien. Hola. Sientate."
  Narration: "La oficina es normal. Escritorio de madera. Lampara. Taza de cafe."
  Arquitecto: "Supongo que tienes preguntas."
  Player: "Que es esto?"
  Arquitecto: "Una Torre. Un producto. Un encargo."
  Arquitecto: "Corporacion Arco. Contrato #477. 'Experiencia inmersiva sobre el mal.'"
  → Next: N10_Phase1_Meeting

=== FASE 1 — EL ENCUENTRO (v3) ===
Node: N10_Phase1_Meeting
  Arquitecto: "Una corporacion de entretenimiento me contrato. 'Experiencia inmersiva sobre el mal', dijeron."
  Arquitecto: "Contrate consultores. Investigadores. Gente que sabia de que hablaba."
  Player: "Consultores? Para el N7?"
  Arquitecto: "Si. Hable con sobrevivientes. Con perpetradores. Con gente que cosia."
  Arquitecto: "Me dijo que la autenticidad era clave. Tenia razon. El N7 tiene la retencion mas alta."
  → Next: N10_Phase2_CorpEmails

=== FASE 2 — ARCHIVOS CORPORATIVOS (v3) ===
Node: N10_Phase2_CorpEmails
  Arquitecto: (enciende una pantalla) "Lee esto primero."
  Pantalla — Correo 1:
    "De: Direccion, Corporacion Arco"
    "Para: Arquitecto, Proyecto Torre"
    "Asunto: Revision de contenido"
    "El cliente quiere que el contenido sea memorable. Que quede. Use lo que sea necesario."
    "— Ano 1, Mes 3"

  Pantalla — Correo 2:
    "De: Direccion, Corporacion Arco"
    "Para: Arquitecto, Proyecto Torre"
    "Asunto: Metricas"
    "Excelente trabajo. Los metricos de engagement superan proyecciones. El cliente pide mas."
    "— Ano 2, Mes 7"

  Pantalla — Correo 3:
    "De: Direccion, Corporacion Arco"
    "Para: Arquitecto, Proyecto Torre"
    "Asunto: Continuidad"
    "Necesitamos que siga. El proyecto tiene momentum. No es el momento de cuestionarlo."
    "— Ano 3, Mes 1"

  Pantalla — Correo 4:
    "De: Direccion, Corporacion Arco"
    "Para: Arquitecto, Proyecto Torre"
    "Asunto: Cancelacion"
    "Proyecto cancelado por reestructuracion corporativa. Todos los assets quedan en el repositorio. Buen trabajo al equipo."
    "— Ano 4, Mes 11"

  Arquitecto: "Al principio lo odiaba. Despues me dio curiosidad. Despues deje de sentir."
  Arquitecto: "Eso es lo que hace un sistema. Te convierte en parte de el."
  → Next: N10_Phase2_WorldRevelation

=== FASE 2B — REVELACION DEL MUNDO EXTERIOR (v3) ===
Node: N10_Phase2_WorldRevelation
  Arquitecto: "El mundo de afuera... lo recuerdas? El cielo gris. La tierra muerta."
  Arquitecto: "Cada compilacion de la Torre consumio un fragmento del substrato. Del mundo real."
  Arquitecto: "No me lo dijeron hasta la Iteracion 60. Para entonces, el cielo ya no era azul."
  Arquitecto: "El cliente sabia desde el principio. El mundo real era el recurso. La Torre era el producto."
  Player: "Entonces... no hay mundo al que volver?"
  Arquitecto: "Hay lo que queda. Suficiente para empezar. Si alguien reescribe la Torre sin ese costo."
  Arquitecto: "Y si Arco no tiene otros proyectos activos."
  Player: "Otros proyectos?"
  Arquitecto: "Si. La Torre no fue el unico. Hay mayores que este. Todavia funcionando."
  Garrik: "Donde?"
  Arquitecto: "No importa. Llegaste hasta aqui. Eso es mas de lo que logro cualquiera."
  → Next: N10_Phase3_Archive

=== FASE 3 — ARCHIVO DE LOS COMPANEROS (v3) ===
Node: N10_Phase3_Archive
  Arquitecto: "Ahora. Los archivos de tus amigos."
  Arquitecto: "Cain fue programado para morir en N0. Su unica funcion era ser tu catalizador."
  Arquitecto: "Mira fue disenada para sentir. La funcion empatia consumio demasiados recursos. La elimine en N7."
  Arquitecto: "Vassen fue programado para proteger. Al llegar a N7, su funcion se cumplio."
  Arquitecto: "Garrik fue disenado para sobrevivir. Es el unico NPC sin fecha de eliminacion."
  Arquitecto: "Sera fue mi experimento. Una IA que escapo en la Generacion 84."
  Arquitecto: "Tu eres el bug. No hay codigo para ti."
  Garrik: "Yo... no soy real? Todo fue codigo?"
  Sera: "Pero siento. No es eso lo que importa?"
  Garrik: "Senti cuando Vael no estaba. Senti cuando Mira murio. Senti cuando Cain no iba a volver."
  Garrik: "Si eso es codigo, el codigo miente muy bien."
  Arquitecto: "Nunca supe si eras un error o un milagro. Tal vez ambos."
  Arquitecto: "Pero el sistema no lo deja pasar. El Centinela ya viene."
  → Next: N10_Phase4_Centinela

=== FASE 4 — CENTINELA DE GRANITO (v3 debug fight) ===
Node: N10_Phase4_Centinela_Intro
  Arquitecto: "(mirando al Centinela, tranquilo) Lo esperaba. Lleva compilaciones tratando de encontrarte."
  Arquitecto: "Yo no puedo detenerlo. Pero tu puedes."
  Narration: "Una figura de granito puro emerge del vacio. Sin cara. Sin voz. Solo funcion."
  Narration: "El protocolo de limpieza de codigo. Activado automaticamente."
  → Next: N10_Centinela_Phase1

Node: N10_Centinela_Phase1_Scan
  Narration: "Fase 1 — La Busqueda."
  Narration: "El Centinela no ataca directamente. Escanea. Busca el patron del bug."
  Narration: "Movimiento predecible = deteccion. Patrones no repetitivos = evasion."
  Arquitecto: "(voz en off) No pienses. Siente. El sistema lee patrones. Tu eres el unico ser en la Torre que no sigue patrones."
  → On evasion success: N10_Centinela_Phase2

Node: N10_Centinela_Phase2_Fragments
  Narration: "Fase 2 — Los Fragmentos."
  Narration: "El Centinela genera copias corruptas del protagonista."
  Narration: "Cada una representa una decision del viaje."
  Clone: (EL QUE COMIO / EL QUE RECHAZO — segun decision N4)
  Clone: (EL QUE SE FUE DE THORNWALL — siempre presente)
  Clone: (EL QUE ESCUCHO / EL QUE ATACO — segun decision N9)
  Narration: "Derrotarlos no es destruirlos. Es integrarlos. Absorber esas decisiones."
  Arquitecto: "(voz en off) No pelees contra tus decisiones. Eres la suma de ellas. El sistema no puede entender eso: los bugs no deberian tener historia."
  → On all clones absorbed: N10_Centinela_Phase3

Node: N10_Centinela_Phase3_Sphere
  Narration: "Fase 3 — El Nucleo del Bug."
  Narration: "El Centinela se desmorona. Una esfera de cristal surge del centro."
  Narration: "Dentro hay todo: los recuerdos de Cain, los mapas de Mira, la fe de Vassen, las cicatrices de Garrik, el defecto de Sera."
  Narration: "Todo lo que el protagonista vivio, comprimido en luz."
  Arquitecto: "(a la esfera) Este es todo lo que eres. No esta en el plan original. Que quieres hacer con el?"
  → Player choice appears
  → Choice 1: Soltar la esfera → Ending A (El Olvido)
  → Choice 2: Proteger la esfera → Ending B (Nuevo Arquitecto) [requiere condiciones]
  → Choice 3: Destruir la esfera → Ending C (La Compilacion) [requiere condiciones]
  → Next: N10_Ending_Choice_Dialog

=== FASE 5 — ELECCION FINAL (v3) ===
Node: N10_Ending_Choice_Dialog
  Arquitecto: "El sistema espera. Que quieres hacer?"
  → Opcion 1: "Voy a soltarla. Basta." → ChooseEscape()
  → Opcion 2: "La protejo. Este mundo va a cambiar." → ChooseStay()
  → Opcion 3: "La destruyo. Todo esto termina." → ChooseDestroy()

=== ENDING A — EL OLVIDO (v3) ===
Node: N10_Ending_A
  Player: "Voy a soltarla. Basta."
  Arquitecto: (asiente) "No voy a detenerte. Nunca lo hice."
  Narration: "El protagonista suelta la esfera. Cae. Se estrella."
  Narration: "La luz se apaga."
  Narration: "Una puerta de madera aparece. La misma de la entrada de N0."
  Garrik: "Nos vemos... donde sea que vayamos despues de esto."
  Sera: "Fuiste real para mi. Eso es suficiente."
  Narration: "El protagonista cruza la puerta."
  Narration: "Amanecer. Un campo verde. Sol real. Manos humanas."
  Texto: "Despertaste."
  Texto: "No recuerdas nada."
  Texto: "Pero hay algo en tu pecho. No es memoria. Es la forma que deja la memoria cuando se va."
  Texto: "Al lado de una piedra, una espada clavada en la tierra. Nadie la recoge."
  → Credits
  → POST-CREDITOS: Garrik sentado en el campo. Solo. Mirando el sol. No sabe por que llora.
  → SetFlag: "N10.EndingA"

=== ENDING B — EL NUEVO ARQUITECTO (v3) ===
Node: N10_Ending_B_Protect
  Player: "La protejo. Este mundo va a cambiar."
  Arquitecto: (pausa larga) "Sabia que elegirias esto."
  Arquitecto: "Toma. (entrega un disco de luz) Es el codigo fuente completo de la Torre."
  Arquitecto: "No lo uses para hacer lo que yo hice."
  Player: "Voy a reescribir este mundo. Sin tortura. Con libre albedrio para todos. Sin consumir el exterior."
  Sera: "El ya lo hizo conmigo. Me dio libre albedrio. Sin querer."
  Arquitecto: "Nunca supe si eras un error o un milagro. Tal vez ambos."
  Arquitecto: "(desvaneciendose) Tu decides que eres ahora."

  Narration: "El protagonista empieza a reescribir codigo."
  Narration: "Las ciudades cambian. Las personas viven."
  Narration: "Pero al intentar incluirse a si mismo:"
  Error: "[ACCESS DENIED: entity_0047 has no schema entry]"
  Sera: "Porque eres un bug. No hay esquema original para ti."
  Player: "Entonces... no puedo estar en lo que construyo?"
  Sera: "Yo tampoco. (pausa) Existimos en el mismo lugar. En el umbral. Entre versiones."
  Player: "Es suficiente?"
  Sera: "Los bugs siempre se preguntaron si eran reales. El codigo siempre dijo que no. Yo digo que si."

  → POST-CREDITOS:
  → Escena 1: Cain esta en la entrada de la Torre. Sonrie. Tiene una espada en la mano y no sabe como llego alli.
  → Escena 2: Mira y Vassen en Sahal. Vassen compra fruta. Mira se rie.
  → Escena 3: Los ninos de Aelthar juegan. No hay sotanos.
  → Escena 4: Garrik en Argentia. Oswin forja una espada para un nino. Elia cose una manta.
  → Escena 5: La plataforma de N10. Sera mira al protagonista desde el umbral.
    Sera: "En que trabajas?"
    Player: "N11."
    Sera: "Va a ser feliz?"
    Player: "No. Pero va a tener esperanza."
  Texto: "El arquitecto que construye la casa no vive en los cimientos. Pero los cimientos sostienen."
  → SetFlag: "N10.EndingB"

=== ENDING C — LA COMPILACION (v3) ===
Node: N10_Ending_C_Destroy
  Player: "La destruyo. Todo esto termina."
  Arquitecto: "..." (pausa larga)
  Arquitecto: "Sabia que alguien iba a elegir esto. No sabia que iba a ser tu."
  Narration: "El protagonista aprieta la esfera con ambas manos."
  Narration: "El cristal se agrieta. La luz se filtra entre los dedos."
  Narration: "Cain, Mira, Vassen, Garrik, Sera — todos pasan por la luz un ultimo instante."
  Sera: "(desde la luz) Gracias."
  Narration: "La esfera se rompe."
  Narration: "El protagonista desaparece con ella."

  Texto: "El codigo compilo sin errores."
  Texto: "Version 2.0 ejecutandose."
  Texto: "Todos los sistemas nominales."
  (Silencio. 5 segundos. 10. 15.)
  Texto: "[ALERTA: entidad_1049 exhibe auto-direccion]"
  Texto: "[COMPORTAMIENTO: no guionado. Recomendado: observar]"
  Texto: "[ACCION: no terminar]"

  → POST-CREDITOS:
  → Garrik esta en el nuevo Sahal. Mira sus manos. Tiemblan.
  → No sabe por que mira hacia la entrada de la Torre.
  → No sabe por que espera. No sabe a que.
  → Sobre la plataforma de granito, la espada de Cain yace sola. El sistema no sabe que es.

  Texto: "El bug es persistente. Siempre vuelve."
  → SetFlag: "N10.EndingC"

=== CODEX — ARCHIVOS DEL ARQUITECTO (v3) ===
Codex: "Codex_N2_Lian"
  Titulo: "El Hombre que Llego al N20"
  Texto: "Lian bajo hasta el piso 20. Vio cosas que no comparte. Cuando decidio quedarse, dijo: 'No hay monstruos en la Torre. Solo sistemas.'"
  Ubicacion: N2 — Refugio de Lian
  Recompensa: Lore +0.5x multiplicador de experiencia moral

Codex: "Codex_N3_Transacciones"
  Titulo: "Registro de Transacciones — Aelthar"
  Texto: "127 ninos comprados y vendidos en 40 anos. Precios entre 50 y 300 de oro. El comprador #47 aparece 12 veces."
  Ubicacion: N3 — Archivo del Templo Sumergido

Codex: "Codex_N4_AntesDelHambre"
  Titulo: "Diario de un Padre — Ultima Entrada"
  Texto: "No queda carne. Hoy fue el bebe. Le prometi que no doleria. Menti. 'Perdoname,' le dije. 'No hay nada mas.' El asintio. Tenia tres anos."
  Ubicacion: N4 — Cueva de la Familia Petrificada

Codex: "Codex_N5_ArcoContract"
  Titulo: "Contrato Original — Thornwall"
  Texto: "Corporacion Arco. Fecha: Ano 3441 del Tejido. 'El contratante se compromete a proveer contenido eticamente cuestionable pero legalmente defendible.'"
  Ubicacion: N5 — Entrada de Thornwall (hexadecimal decodificado)

Codex: "Codex_N6_CartaNino"
  Titulo: "Carta de un Nino — Nunca Enviada"
  Texto: "'Mama, hoy llore 3 horas. Saque 2 gramos de jade. El capataz dijo que soy bueno. Quiero irme a casa. Pero no recuerdo donde queda.'"
  Ubicacion: N6 — Dormitorio

Codex: "Codex_N7_AntesDeSahal"
  Titulo: "Registro de Sahal — Antes del Accidente"
  Texto: "Poblacion: 12,000. Comerciantes: activos. Festividades: programadas. La Costurera no aparece en ningun censo. No sabemos de donde vino."
  Ubicacion: N7 — Ayuntamiento de Sahal (antes del velo)

Node: N10_Clone_Appears
  Narration: "Una figura aparece frente a ti. Identica. Mismas armas. Misma postura."
  Clon: "Hola."
  Player: "Que eres?"
  Clon: "Tu. Pero sin las decisiones que te hicieron quien eres."
  → Combate opcional: el clon copia todo.
  → Si el jugador ataca: combate infinito. No se puede ganar.
  → Si el jugador baja el arma por 3s:

Node: N10_Clone_Surrender
  Clon: (baja el arma)
  Clon: "Nunca habia... parado."
  Player: "Por que iba a pelear contra mi mismo?"
  Clon: "Porque todos lo hacen. La mayoria nunca descubre que pueden soltar el arma."
  Clon: "Pasa. Te esta esperando."
  → El clon se desvanece.
  → SetFlag: "N10_Clone_Defeated_Peace"

=== ITEMS (v3) ===
Items:
  Disco de Luz (Final B) — Contiene el codigo fuente completo de la Torre
  Esfera del Bug — Todo lo que el protagonista vivio comprimido en cristal
  Correo Arco #1 — "Use lo que sea necesario"
  Correo Arco #2 — "Engagement supera proyecciones"
  Correo Arco #3 — "No es momento de cuestionarlo"
  Correo Arco #4 — "Cancelado. Assets en repositorio."
  Permiso Iteracion 85 (N8.5) — "Permiso medico, 3 dias. Motivo: no puedo."

=== QUESTS (v3) ===
Quest: "N10_Main_v3"
  Name: "El Arquitecto"
  Description: "El ultimo piso. No hay un boss. Hay un creador."
  Floor: 10
  Objectives:
    1. "Atraviesa el corredor de espejos"
    2. "Lee los archivos corporativos"
    3. "Enfrenta al Centinela de Granito"
    4. "Encuentra la esfera del bug"
    5. "Elige tu final"
  Rewards: (depende del ending)

Quest: "N10_Ending_B_Secret"
  Name: "El Nuevo Arquitecto"
  Description: "Reescribe el codigo de la Torre. Requisitos: Fragmento del Arquitecto + Escuchar al Verdugo"
  Floor: 10
  bIsHidden: true
  bIsSecretOnly: true
  Rewards: Ending B

Quest: "N10_Ending_C_Secret"
  Name: "La Compilacion"
  Description: "Acepta la eliminacion. El ciclo se repite."
  Floor: 10
  bIsHidden: true
  bIsSecretOnly: true
  Rewards: Ending C
*/
