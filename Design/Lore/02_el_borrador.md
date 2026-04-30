# 02 â€” El Borrador

> *"Si miras un mapa el tiempo suficiente, los rÃ­os cambian de cauce. Eso no es magia. Eso es un dios distraÃ­do."*
> â€” RefrÃ¡n de los cartÃ³grafos del Valle Inicial

---

## El mundo como Borrador

El mundo de Solo es la **proyecciÃ³n fÃ­sica del PatrÃ³n** del Arquitecto sobre la realidad. **Es imperfecto por diseÃ±o:** una compilaciÃ³n nunca produce el binario ideal a la primera. Cada bioma, cada raza, cada ciudad â€” son **intentos** del Arquitecto por resolver un problema concreto. Algunos funcionaron. Otros se quedaron a medias.

Esta secciÃ³n define **quÃ© problemas estaba resolviendo el Arquitecto en cada componente del mundo** â€” y por tanto, quÃ© tipo de errores muestran.

---

## Los siete biomas como hipÃ³tesis del PatrÃ³n

| Bioma | Pregunta del Arquitecto | Estado de la hipÃ³tesis |
|---|---|---|
| **Valle Inicial** | *Â¿Puede una regiÃ³n sostener vida sin conflicto?* | Funciona. Es la Ãºnica zona estable de la Edad. Por eso es el "tutorial". |
| **Bosque Ã‰lfico** | *Â¿Puede la magia coexistir con la materia sin corromperla?* | Inestable. Funciona en los Ã¡rboles. No funciona en los habitantes (los elfos son frÃ¡giles, casi fluorescentes). |
| **Desierto Ardiente** | *Â¿CuÃ¡nto puede sufrir un ecosistema antes de colapsar?* | Resuelto, pero brutalmente. Los habitantes sobreviven a costa de toda blandura. |
| **Tundra Glacial** | *Â¿Puede la quietud ser una forma de orden?* | A medio resolver. La quietud aquÃ­ es el **silencio del olvido**, no la paz. |
| **Pantano Oscuro** | *Â¿Puede preservarse algo en el tiempo sin que se descomponga?* | FallÃ³. Lo que entra al pantano se preserva pero se pudre simultÃ¡neamente. |
| **MontaÃ±a del DragÃ³n** | *Â¿Puede el poder existir sin corromper a quien lo posee?* | Resuelto solo en los dragones. En los humanos siempre falla. |
| **Tierra Corrupta** | *Â¿Y si simplemente no aplico el PatrÃ³n aquÃ­?* | **Bug crÃ­tico no parcheado.** Ver mÃ¡s abajo. |
| **Isla de la Nada** | *Â¿DÃ³nde van las cosas que descarto?* | Ver [`04_isla_de_la_nada.md`](04_isla_de_la_nada.md). |

---

## La Tierra Corrupta â€” el bug que el Arquitecto dejÃ³ sin parchear

La Tierra Corrupta es la regiÃ³n del nivel 85+ del overworld. **No es una zona maldita por una guerra antigua, ni un campo de batalla de dioses.** Es, literalmente, **una regiÃ³n del mundo donde el PatrÃ³n nunca terminÃ³ de aplicarse**. Es una zona "sin compilar".

### SÃ­ntomas observables

- **GeometrÃ­a inestable:** un camino que ayer iba al norte hoy va al este. NingÃºn cartÃ³grafo de las ciudades vecinas ha podido mapearla.
- **Sombras sin objeto:** sombras proyectadas en el suelo cuyo origen no existe. Y al revÃ©s: objetos que no proyectan sombra.
- **Fauna sin clasificar:** criaturas que parecen partes de otros animales mal cosidas. No se reproducen. Aparecen y desaparecen.
- **Sonido sin fuente:** voces, campanas, llanto de niÃ±os. Nunca con causa identificable.
- **Tiempo desincronizado:** un viajero que pase tres horas dentro puede salir y descubrir que han pasado tres dÃ­as â€” o tres minutos.

### Por quÃ© nadie la "limpia"

Porque **el Arquitecto teme borrarla**. Sospecha que los Persistentes (como el protagonista) tienen relaciÃ³n con esta regiÃ³n. Si la sobrescribe, podrÃ­a perder el dato que lleva siglos rastreando. Por eso la Tierra Corrupta es la Ãºnica zona del mundo que **lleva exactamente igual durante las Ãºltimas tres Edades**. El resto del mundo se reescribe; ella permanece, exactamente igual de rota.

### Hook narrativo

El **arco final del juego antes de entrar a la Torre** deberÃ­a suceder aquÃ­. La Tierra Corrupta es la Ãºnica regiÃ³n del Borrador donde el velo es tan delgado que el protagonista puede empezar a *ver* el Tejido.

---

## Las razas como ramas

El Arquitecto ha experimentado con **siete linajes humanoides**. Cada uno es una respuesta a un problema distinto. Ninguno es "el correcto" â€” y esa es exactamente la razÃ³n por la que el mundo es plural.

| Raza | Problema que el Arquitecto intentaba resolver | Por quÃ© la mantiene |
|---|---|---|
| **Humano** | *La rama de control.* El intento mÃ¡s equilibrado, mÃ¡s promediado. Sin extremos. | Porque sigue funcionando. Adaptable. |
| **Elfo** | *Rama: longevidad.* Â¿Puede una conciencia durar siglos sin enloquecer? | Funciona, pero a costa de la fertilidad. La rama se estÃ¡ extinguiendo. |
| **Enano** | *Rama: paciencia.* Conciencias capaces de proyectos de mil aÃ±os. | Estable. Probablemente la mÃ¡s exitosa, aunque la menos numerosa. |
| **Medio-Orco** | *Rama: fuerza sin malicia.* Intento de separar el poder fÃ­sico de la crueldad. | FuncionÃ³ parcialmente. Por eso son fuertes pero socialmente marginados â€” su perfil emocional no encaja con las otras razas. |
| **Semihumano** | *Rama: fluidez.* Conciencias capaces de adaptarse a cualquier nicho. | Sigue activa porque rinde datos Ãºtiles, pero la propia fluidez los hace difÃ­ciles de "fijar". |
| **Demonio** | *Rama: el experimento abandonado.* El Arquitecto los creÃ³ cuando intentaba resolver "Â¿puede una rama tolerar la corrupciÃ³n del Tejido directamente?". | **No la mantiene voluntariamente.** Los demonios persisten porque, una vez creados, **resisten la reescritura**. El Arquitecto los marginÃ³ del mundo (vetados de ciudades) intentando que se extingan por sÃ­ solos. No lo han hecho. |
| **FeÃ©rico** | *Rama: ligereza.* Conciencias casi-puramente magia, mÃ­nima materia. | Funciona, pero es frÃ¡gil. Una rama elegante. El Arquitecto la conserva por estÃ©tica. |

### Implicaciones narrativas

- **Los demonios saben mÃ¡s de lo que dicen.** Su persistencia ante la reescritura los convierte, biolÃ³gicamente, en archivistas inconscientes de Edades pasadas. Hay lÃ­neas de sangre demonÃ­acas con recuerdos genÃ©ticos de tres iteraciones atrÃ¡s.
- **Los enanos preservan secretos por costumbre.** Su paciencia centenaria significa que sus archivos contienen detalles que el resto del mundo olvidÃ³ hace cinco generaciones humanas. Las forjas mÃ¡s antiguas tienen documentos en lenguas extintas.
- **Los semihumanos son los mÃ¡s vulnerables a la siguiente compilaciÃ³n.** Su fluidez los hace fÃ¡ciles de reescribir. Los sabios semihumanos lo intuyen y muchos viven con melancolÃ­a existencial.
- **Los feÃ©ricos no creen en la muerte.** Para ellos, *Ser Recogido* es solo regresar al hilo. Su cultura no entiende el luto.

---

## Las cinco ciudades

Las cinco ciudades principales del overworld son **nÃºcleos donde el PatrÃ³n estÃ¡ mÃ¡s estabilizado**. Cada una protege un equilibrio distinto.

| Ciudad | Bioma | Cultura dominante | FunciÃ³n simbÃ³lica |
|---|---|---|---|
| **Velmar** | Valle Inicial | Humanos, multirracial | El hogar. Donde el PatrÃ³n es mÃ¡s limpio. |
| **Aelthar** | Bosque Ã‰lfico | Elfos, fae | La biblioteca. Cada calle es una pÃ¡gina viva. |
| **Khorr-DÃ»m** | MontaÃ±a del DragÃ³n | Enanos | La forja. La memoria material. |
| **Sahal** | Desierto Ardiente | Mestizo, mucha presencia medio-orca | La frontera. Donde el orden negocia con lo salvaje. |
| **Nynth** | Tundra Glacial | Recluidos, monÃ¡sticos | El silencio. Donde se estudia el Tejido en secreto. |

Los demonios no tienen ciudad. Sus comunidades viven en los mÃ¡rgenes â€” cuevas, ruinas, los bordes de la Tierra Corrupta. Esto es deliberado del Arquitecto.

---

## Spawn por raza (sistema multiplayer)

El **primer respawn** de un jugador nuevo ocurre en la ciudad que corresponde a su raza. Esto no es una restricciÃ³n permanente â€” el jugador puede moverse libremente. Pero su punto de entrada al mundo es la ciudad de su linaje.

| Raza | Ciudad de spawn | RazÃ³n in-universe |
|---|---|---|
| **Humano** | Velmar | Ciudad mÃ¡s acogedora, sin restricciones raciales. Punto neutral. |
| **Elfo** | Aelthar | Los elfos tratan al jugador elfo como *pariente lejano*. Acceso a la Biblioteca desde el dÃ­a 1. |
| **Enano** | Khorr-DÃ»m | La Forja reconoce a sus hijos. Archivos viejos desbloqueados por defecto. |
| **Medio-Orco** | Sahal | La ciudad fronteriza donde la reputaciÃ³n se gana con hechos, no con linaje. |
| **Semihumano** | Velmar | Ciudad neutral, dado que los semihumanos no tienen ciudad propia. |
| **Demonio** | MÃ¡rgenes de la Tierra Corrupta | Los demonios no pueden entrar en ciudades principales de noche (excepciÃ³n: Velmar, solo de noche). Spawn en el asentamiento demoniaco mÃ¡s cercano. |
| **FeÃ©rico** | Aelthar | La ciudad mÃ¡gica es la mÃ¡s tolerante con los feÃ©ricos. Pero incluso aquÃ­ los tratan como visitantes, no como residentes. |

> **Nota de diseÃ±o:** el spawn no es solo flavor. La ciudad inicial determina las primeras quests disponibles, la reputaciÃ³n de inicio y los NPCs que reconocen al jugador. Ver [`06_el_protagonista.md`](06_el_protagonista.md) para los hooks narrativos por raza.

---

## Errores que el jugador puede observar (sembrar como detalles)

Estos detalles son **bugs del Borrador** y deben aparecer dispersos en quests, descripciones de Ã­tems, diÃ¡logos de NPCs sin importancia. Son las pistas para que el jugador atento intuya la verdad antes del Acto V.

- Un campesino que cuenta una historia ligeramente distinta cada vez que el jugador habla con Ã©l. Nunca recuerda haberla cambiado.
- Una taberna donde, en cierto rincÃ³n, las velas no se apagan al soplarlas. Nadie le da importancia.
- Un rÃ­o que tiene tres nombres distintos segÃºn en quÃ© orilla preguntes.
- Un libro que cada vez que se abre tiene una pÃ¡gina menos. Nadie sabe si la pierden o si nunca estuvo.
- Un perro callejero que aparece en cuatro ciudades distintas. IdÃ©ntico. Sin explicaciÃ³n.
- Un dÃ­a al aÃ±o en que **nadie sueÃ±a**. Lo notan al despertar.
- Un sonido bajo y constante, justo en el lÃ­mite de la audiciÃ³n, que cualquier personaje puede confirmar si se le pregunta â€” pero del que nadie hablaba antes.

> Estos detalles **nunca se resuelven dentro del juego**. Son ambientales. El jugador los conecta solo. Esa es la magia.