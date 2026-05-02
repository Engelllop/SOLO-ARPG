// N7 — Sahal (Genocidio / Desmembramiento) v3
// Dialogos, NPCs, Quests, Items, Iteracion 87

#pragma once

#include "CoreMinimal.h"

/*
=== ENTRADA A SAHAL ===
Node: N7_Arrival
  Narration: "Sahal. La Ciudad de la Union Eterna. Asi la llamaban antes."
  Narration: "Antes de que alguien decidiera que la union eterna significaba no poder separarse nunca."
  → Next: N7_City_Explore

=== MIRA — DIARIO ENCONTRABLE (v3) ===
Node: N7_Mira_Diary
  Narration: "En la habitacion que Mira ocupo la noche anterior, un cuaderno abierto sobre la cama."
  Diario: "Creo que el protagonista es real."
  Diario: "No en el sentido de carne y hueso — en el sentido de que elige."
  Diario: "No como yo elijo, que a veces no se si elijo o solo sigo el mapa."
  Diario: "El... el se detiene. Mira el suelo. Y entonces decide."
  Diario: "Eso no esta en ningun mapa que yo conozca."
  Diario: "Si esto sigue adelante y yo no, que quede registrado: estuvo aqui. Cain Ardeth, N1-K7. Y el protagonista que no tiene coordenada porque no estaba en el plano original."
  Diario: "No se como se llama. Nunca se lo pregunte. Asi que en el mapa va a poner: N0-A1. El primer punto. El origen."
  → SetItem: "N7_DiarioMira"
  → SetFlag: "N7.DiaryRead"

=== MIRA — PRIMER ENCUENTRO (v3) ===
Node: N7_Mira_First
  Mira: "Llegaste. Sabia que lo harias."
  Player: "Mira. Donde esta Vassen?"
  Mira: "La Costurera lo tiene. Me tiene a mi tambien. Pero hablamos por los hilos."
  Player: "Los hilos?"
  Mira: "Nos conecta. Siento su dolor. El siente el mio. No podemos mentirnos."
  → Next: N7_Mira_Bound

=== VASSEN — ATRAPADO (v3) ===
Node: N7_Vassen_First
  Vassen: "No la escuches. No confies en ella. La Costurera usa los hilos para hablar con su voz."
  Player: "Mira? Eres tu?"
  Vassen: "No se quien soy. A veces hablo. A veces ella habla por mi."
  → Next: N7_Costurera_Approach

=== LA COSTURERA — PRE-COMBATE (v3) ===
Node: N7_Costurera_PreCombat
  Costurera: "Ah. El viajero. Has llegado lejos."
  Costurera: "Los otros pisos son groseros. Violencia. Sangre. Yo hago arte."
  Player: "Suelta a mis amigos."
  Costurera: "Amigos? Estos? Son material. Buena tela. Buena voz para gritar."
  → Next: N7_Combat_Phase1

=== COMBATE — FASE MARIONETAS (v3) ===
Node: N7_Combat_Phase1
  Narration: "Mira y Vassen atacan controlados por hilos de plata."
  Mira (marioneta): "No quiero... no quiero hacer esto..."
  Vassen (marioneta): "Corre... corre mientras puedas..."
  Narration: "No se puede atacar a Mira o Vassen sin hacerles dano real."
  → Duracion: 40 segundos hasta el evento de liberacion.

=== MUERTE DE VASSEN (v3) ===
Node: N7_Vassen_Death
  Narration: "Vassen se libera del control. Corre hacia la Costurera. La apunala."
  Narration: "Ella lo agarra del cuello y lo descuartiza en camara lenta."
  Narration: "Primero el brazo derecho. Luego izquierdo. Luego piernas."
  Narration: "Vassen sigue vivo 2 segundos despues del ultimo corte."
  Vassen: "(sonriendo) Encontre algo en que creer. Nada grande. Solo alguien que se detiene y mira el suelo antes de decidir."
  → Vassen muere.
  → SetItem: "N7_EscudoVassen"

=== MUERTE DE MIRA (v3) ===
Node: N7_Mira_Death
  Narration: "Mira grita. El control se rompe."
  Narration: "Corre hacia el telar y ve lo que le espera: ser cosida viva."
  Narration: "No duda. Se corta la garganta con un hilo de plata."
  Mira: "(ultimo susurro) Ya estas en el mapa."
  → Mira muere.
  → SetItem: "N7_UltimaFlechaMira" (se puede recuperar)

=== ITERACION 87 — ARCHIVO DEL ARQUITECTO (v3) ===
Node: N7_Iteracion87
  Narration: "Al derrotar a la Costurera, el telar se deshace. Entre los hilos, un fragmento de papel impreso."
  Texto: "[ARCHIVO DEL ARQUITECTO — Iteracion 87]"
  Texto: "Iteracion 87: Cartografa: eliminada. Guardian: eliminado. Funciones cumplidas."
  Texto: "Tiempo de proceso: 47 segundos."
  Texto: "Nota personal: llevo veinte minutos mirando la ultima entrada del diario de la cartografa en el monitor."
  Texto: "No la cierro."
  Texto: "No se por que no la cierro."
  → SetItem: "N7_ArchivoIteracion87"
  → SetFlag: "N7.Iteracion87Read"
  → (Silencio de 5 segundos en el juego)

Node: N7_Post_I87
  Narration: "Sera aparece por primera vez al borde de la torre. No mira. Solo esta ahi."
  Garrik: "Los voy a enterrar. Tu sigue. Ve. Termina lo que empezamos."

=== COSTURERA — MUERTE ===
Node: N7_Costurera_Death
  Costurera: (cayendo) "Sahal era hermosa... antes del accidente."
  Player: "Que accidente?"
  Costurera: "El que me hizo a mi."
  → El suelo tiembla. Las costuras se abren.
  → SetFlag: "N7.CostureraDead"

=== MARIONETAS (Enemies Ambient) ===
Node: N7_Marioneta_Dialog_1
  Marioneta: "Hace frio."
  Player: "Puedes hablar?"
  Marioneta: "No deberia. La Costurera no sabe que hablo. Si se entera, me vuelve a coser."
  → Next: N7_Marioneta_Info

=== ITEMS (v3) ===
Items:
  Diario de Mira — "No se como se llama. Asi que en el mapa va a poner: N0-A1. El origen."
  Archivo Iteracion 87 — "No la cierro. No se por que no la cierro." (primer crack del Arquitecto)
  Ultima Flecha de Mira — Se puede recuperar del suelo
  Escudo de Vassen — Roto por la mitad
  Hilo de Plata — Fragmento usado por la Costurera
  Fragment_Sahal — Hilo ensangrentado

=== QUESTS (v3) ===
Quest: "N7_Main"
  Name: "La Union Eterna"
  Description: "Sahal esta cosida a la fuerza."
  Floor: 7
  Objectives:
    1. "Encuentra a Mira"
    2. "Encuentra a Vassen"
    3. "Encuentra el diario de Mira" (v3 — opcional, recomendado)
    4. "Rompe los hilos que los atan"
    5. "Enfrenta a la Costurera"
  Rewards: Fragment_Sahal, Archivo I87

Quest: "N7_Optional_Marionetas"
  Name: "Voces Cosidas"
  Description: "Las marionetas conservan fragmentos de conciencia."
  Floor: 7
  bIsHidden: true
  Objectives:
    1. "Habla con 3 marionetas diferentes"
    2. "Escucha sus historias"
  Rewards: "Blessing_Memoria" (bonus to final choice recognition)
*/
