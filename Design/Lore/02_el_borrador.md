# 02 — El Borrador

> *"Si miras un mapa el tiempo suficiente, los ríos cambian de cauce. Eso no es magia. Eso es un dios distraído."*
> — Refrán de los cartógrafos del Valle Inicial

---

## El mundo como Borrador

El mundo de EMBERVEIL es la **proyección física del Patrón** del Arquitecto sobre la realidad. **Es imperfecto por diseño:** una compilación nunca produce el binario ideal a la primera. Cada bioma, cada raza, cada ciudad — son **intentos** del Arquitecto por resolver un problema concreto. Algunos funcionaron. Otros se quedaron a medias.

Esta sección define **qué problemas estaba resolviendo el Arquitecto en cada componente del mundo** — y por tanto, qué tipo de errores muestran.

---

## Los siete biomas como hipótesis del Patrón

| Bioma | Pregunta del Arquitecto | Estado de la hipótesis |
|---|---|---|
| **Valle Inicial** | *¿Puede una región sostener vida sin conflicto?* | Funciona. Es la única zona estable de la Edad. Por eso es el "tutorial". |
| **Bosque Élfico** | *¿Puede la magia coexistir con la materia sin corromperla?* | Inestable. Funciona en los árboles. No funciona en los habitantes (los elfos son frágiles, casi fluorescentes). |
| **Desierto Ardiente** | *¿Cuánto puede sufrir un ecosistema antes de colapsar?* | Resuelto, pero brutalmente. Los habitantes sobreviven a costa de toda blandura. |
| **Tundra Glacial** | *¿Puede la quietud ser una forma de orden?* | A medio resolver. La quietud aquí es el **silencio del olvido**, no la paz. |
| **Pantano Oscuro** | *¿Puede preservarse algo en el tiempo sin que se descomponga?* | Falló. Lo que entra al pantano se preserva pero se pudre simultáneamente. |
| **Montaña del Dragón** | *¿Puede el poder existir sin corromper a quien lo posee?* | Resuelto solo en los dragones. En los humanos siempre falla. |
| **Tierra Corrupta** | *¿Y si simplemente no aplico el Patrón aquí?* | **Bug crítico no parcheado.** Ver más abajo. |
| **Isla de la Nada** | *¿Dónde van las cosas que descarto?* | Ver [`04_isla_de_la_nada.md`](04_isla_de_la_nada.md). |

---

## La Tierra Corrupta — el bug que el Arquitecto dejó sin parchear

La Tierra Corrupta es la región del nivel 85+ del overworld. **No es una zona maldita por una guerra antigua, ni un campo de batalla de dioses.** Es, literalmente, **una región del mundo donde el Patrón nunca terminó de aplicarse**. Es una zona "sin compilar".

### Síntomas observables

- **Geometría inestable:** un camino que ayer iba al norte hoy va al este. Ningún cartógrafo de las ciudades vecinas ha podido mapearla.
- **Sombras sin objeto:** sombras proyectadas en el suelo cuyo origen no existe. Y al revés: objetos que no proyectan sombra.
- **Fauna sin clasificar:** criaturas que parecen partes de otros animales mal cosidas. No se reproducen. Aparecen y desaparecen.
- **Sonido sin fuente:** voces, campanas, llanto de niños. Nunca con causa identificable.
- **Tiempo desincronizado:** un viajero que pase tres horas dentro puede salir y descubrir que han pasado tres días — o tres minutos.

### Por qué nadie la "limpia"

Porque **el Arquitecto teme borrarla**. Sospecha que los Persistentes (como el protagonista) tienen relación con esta región. Si la sobrescribe, podría perder el dato que lleva siglos rastreando. Por eso la Tierra Corrupta es la única zona del mundo que **lleva exactamente igual durante las últimas tres Edades**. El resto del mundo se reescribe; ella permanece, exactamente igual de rota.

### Hook narrativo

El **arco final del juego antes de entrar a la Torre** debería suceder aquí. La Tierra Corrupta es la única región del Borrador donde el velo es tan delgado que el protagonista puede empezar a *ver* el Tejido.

---

## Las razas como ramas

El Arquitecto ha experimentado con **siete linajes humanoides**. Cada uno es una respuesta a un problema distinto. Ninguno es "el correcto" — y esa es exactamente la razón por la que el mundo es plural.

| Raza | Problema que el Arquitecto intentaba resolver | Por qué la mantiene |
|---|---|---|
| **Humano** | *La rama de control.* El intento más equilibrado, más promediado. Sin extremos. | Porque sigue funcionando. Adaptable. |
| **Elfo** | *Rama: longevidad.* ¿Puede una conciencia durar siglos sin enloquecer? | Funciona, pero a costa de la fertilidad. La rama se está extinguiendo. |
| **Enano** | *Rama: paciencia.* Conciencias capaces de proyectos de mil años. | Estable. Probablemente la más exitosa, aunque la menos numerosa. |
| **Medio-Orco** | *Rama: fuerza sin malicia.* Intento de separar el poder físico de la crueldad. | Funcionó parcialmente. Por eso son fuertes pero socialmente marginados — su perfil emocional no encaja con las otras razas. |
| **Semihumano** | *Rama: fluidez.* Conciencias capaces de adaptarse a cualquier nicho. | Sigue activa porque rinde datos útiles, pero la propia fluidez los hace difíciles de "fijar". |
| **Demonio** | *Rama: el experimento abandonado.* El Arquitecto los creó cuando intentaba resolver "¿puede una rama tolerar la corrupción del Tejido directamente?". | **No la mantiene voluntariamente.** Los demonios persisten porque, una vez creados, **resisten la reescritura**. El Arquitecto los marginó del mundo (vetados de ciudades) intentando que se extingan por sí solos. No lo han hecho. |
| **Feérico** | *Rama: ligereza.* Conciencias casi-puramente magia, mínima materia. | Funciona, pero es frágil. Una rama elegante. El Arquitecto la conserva por estética. |

### Implicaciones narrativas

- **Los demonios saben más de lo que dicen.** Su persistencia ante la reescritura los convierte, biológicamente, en archivistas inconscientes de Edades pasadas. Hay líneas de sangre demoníacas con recuerdos genéticos de tres iteraciones atrás.
- **Los enanos preservan secretos por costumbre.** Su paciencia centenaria significa que sus archivos contienen detalles que el resto del mundo olvidó hace cinco generaciones humanas. Las forjas más antiguas tienen documentos en lenguas extintas.
- **Los semihumanos son los más vulnerables a la siguiente compilación.** Su fluidez los hace fáciles de reescribir. Los sabios semihumanos lo intuyen y muchos viven con melancolía existencial.
- **Los feéricos no creen en la muerte.** Para ellos, *Ser Recogido* es solo regresar al hilo. Su cultura no entiende el luto.

---

## Las cinco ciudades

Las cinco ciudades principales del overworld son **núcleos donde el Patrón está más estabilizado**. Cada una protege un equilibrio distinto.

| Ciudad | Bioma | Cultura dominante | Función simbólica |
|---|---|---|---|
| **Velmar** | Valle Inicial | Humanos, multirracial | El hogar. Donde el Patrón es más limpio. |
| **Aelthar** | Bosque Élfico | Elfos, fae | La biblioteca. Cada calle es una página viva. |
| **Khorr-Dûm** | Montaña del Dragón | Enanos | La forja. La memoria material. |
| **Sahal** | Desierto Ardiente | Mestizo, mucha presencia medio-orca | La frontera. Donde el orden negocia con lo salvaje. |
| **Nynth** | Tundra Glacial | Recluidos, monásticos | El silencio. Donde se estudia el Tejido en secreto. |

Los demonios no tienen ciudad. Sus comunidades viven en los márgenes — cuevas, ruinas, los bordes de la Tierra Corrupta. Esto es deliberado del Arquitecto.

---

## Spawn por raza (sistema multiplayer)

El **primer respawn** de un jugador nuevo ocurre en la ciudad que corresponde a su raza. Esto no es una restricción permanente — el jugador puede moverse libremente. Pero su punto de entrada al mundo es la ciudad de su linaje.

| Raza | Ciudad de spawn | Razón in-universe |
|---|---|---|
| **Humano** | Velmar | Ciudad más acogedora, sin restricciones raciales. Punto neutral. |
| **Elfo** | Aelthar | Los elfos tratan al jugador elfo como *pariente lejano*. Acceso a la Biblioteca desde el día 1. |
| **Enano** | Khorr-Dûm | La Forja reconoce a sus hijos. Archivos viejos desbloqueados por defecto. |
| **Medio-Orco** | Sahal | La ciudad fronteriza donde la reputación se gana con hechos, no con linaje. |
| **Semihumano** | Velmar | Ciudad neutral, dado que los semihumanos no tienen ciudad propia. |
| **Demonio** | Márgenes de la Tierra Corrupta | Los demonios no pueden entrar en ciudades principales de noche (excepción: Velmar, solo de noche). Spawn en el asentamiento demoniaco más cercano. |
| **Feérico** | Aelthar | La ciudad mágica es la más tolerante con los feéricos. Pero incluso aquí los tratan como visitantes, no como residentes. |

> **Nota de diseño:** el spawn no es solo flavor. La ciudad inicial determina las primeras quests disponibles, la reputación de inicio y los NPCs que reconocen al jugador. Ver [`06_el_protagonista.md`](06_el_protagonista.md) para los hooks narrativos por raza.

---

## Errores que el jugador puede observar (sembrar como detalles)

Estos detalles son **bugs del Borrador** y deben aparecer dispersos en quests, descripciones de ítems, diálogos de NPCs sin importancia. Son las pistas para que el jugador atento intuya la verdad antes del Acto V.

- Un campesino que cuenta una historia ligeramente distinta cada vez que el jugador habla con él. Nunca recuerda haberla cambiado.
- Una taberna donde, en cierto rincón, las velas no se apagan al soplarlas. Nadie le da importancia.
- Un río que tiene tres nombres distintos según en qué orilla preguntes.
- Un libro que cada vez que se abre tiene una página menos. Nadie sabe si la pierden o si nunca estuvo.
- Un perro callejero que aparece en cuatro ciudades distintas. Idéntico. Sin explicación.
- Un día al año en que **nadie sueña**. Lo notan al despertar.
- Un sonido bajo y constante, justo en el límite de la audición, que cualquier personaje puede confirmar si se le pregunta — pero del que nadie hablaba antes.

> Estos detalles **nunca se resuelven dentro del juego**. Son ambientales. El jugador los conecta solo. Esa es la magia.
