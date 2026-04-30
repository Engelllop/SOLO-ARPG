# Sistema de Reputación — Facciones de SOLO

> **Sunlight of the Last Online** — RPG medieval oscuro
> Documento de diseño v1.0

---

## Índice

1. [Introducción](#1-introducción)
2. [Mecánica General de Reputación](#2-mecánica-general-de-reputación)
3. [Gremio de Aventureros (Emberveil — N0)](#3-gremio-de-aventureros-emberveil--n0)
4. [Elfos de Aelthar (N3)](#4-elfos-de-aelthar-n3)
5. [Thornwall (N5)](#5-thornwall-n5)
6. [Argentia (N9)](#6-argentia-n9)
7. [Mercaderes Viajeros (Transversal)](#7-mercaderes-viajeros-transversal)
8. [Interacciones entre Facciones](#8-interacciones-entre-facciones)
9. [Consecuencias Globales](#9-consecuencias-globales)
10. [Diálogos Dinámicos](#10-diálogos-dinámicos)
11. [Implementación Técnica](#11-implementación-técnica)
12. [Tabla Resumen](#12-tabla-resumen)
13. [Notas de Diseño](#13-notas-de-diseño)

---

## 1. Introducción

La Torre de SOLO no es un lugar vacío. En cada piso habitable, los sobrevivientes han formado comunidades, fortalezas y facciones que luchan por mantener lo que queda de civilización. El jugador no es un héroe predestinado — es un extraño que llega a un ecosistema de relaciones ya existentes. Su reputación determina si será bienvenido, tolerado, respetado o expulsado.

Este sistema está diseñado para que las decisiones del jugador tengan peso narrativo y mecánico. Cada facción ofrece recompensas exclusivas, pero ganar la confianza de una puede significar cerrar puertas en otra. No se puede complacer a todos.

---

## 2. Mecánica General de Reputación

### 2.1. Valores Fundamentales

- La reputación es un **valor entero** que cada facción guarda sobre el jugador.
- Rango general: **-100 a 300** (varía ligeramente por facción).
- Se almacena en el archivo `SaveGame` y persiste entre sesiones.
- **No se muestra como número al jugador.** En lugar de eso, la UI muestra:
  - Una **barra cualitativa** (vacía, un cuarto, media, tres cuartos, llena).
  - Un **texto descriptivo** del nivel actual (Hostil, Neutral, Amistoso, etc.).
  - Un **tooltip** con pistas narrativas: "Aldric frunce el ceño cuando pasas" / "Los guardias te saludan al verte".

### 2.2. Umbrales de Reputación

Cada facción tiene sus propios umbrales, pero todos siguen esta estructura general:

| Rango | Efecto General |
|-------|----------------|
| Hostil | Puertas cerradas. NPCs clave no te hablan. Misiones bloqueadas. Precios inflados (si es que te venden). |
| Neutral | Estado por defecto al llegar. Comercio básico, misiones iniciales. NPCs desconfiados pero profesionales. |
| Amistoso / Confiable | Descuentos. Nuevas misiones. Acceso a áreas restringidas. Lore adicional. |
| Máximo (Aliado / Héroe / Hermano) | Recompensas únicas. Items legendarios. Diálogos especiales. Influencia en el final del juego. |

### 2.3. Cómo se Modifica la Reputación

**Ganar reputación:**
- Completar misiones para la facción.
- Realizar acciones que beneficien a la facción (llevar recursos, curar miembros, defender el asentamiento).
- Diálogos respetuosos o que demuestren lealtad.

**Perder reputación:**
- Fallar misiones aceptadas.
- Atacar a miembros de la facción.
- Robar en territorio de la facción.
- Traicionar a la facción (aliarse con sus enemigos, revelar secretos).
- Negarse a ayudar en eventos críticos.

### 2.4. Retroalimentación al Jugador

Cuando la reputación cambia, el jugador recibe retroalimentación inmediata:

- **Notificación visual:** Un icono de la facción aparece brevemente en pantalla con el cambio (+20 Gremio, -10 Thornwall).
- **Notificación narrativa:** Un mensaje contextual: "Aldric asiente con respeto" / "Los elfos murmuran a tu paso".
- **Cambio de umbral:** Cuando cruzas un umbral (de Neutral a Amistoso, por ejemplo), el juego lo anuncia con un mensaje más grande y una pausa dramática.

### 2.5. Límites y Saturación

- No se puede superar el máximo de 300 en ninguna facción.
- Una vez en el nivel máximo, las acciones que normalmente darían reputación ya no suman (aunque pueden generar otros beneficios, como objetos o lore).
- Si llegas a -100, la facción se vuelve **hostil permanente** a menos que realices una misión de redención específica (solo disponible para algunas facciones).

---

## 3. Gremio de Aventureros (Emberveil — N0)

### 3.1. Identidad

El Gremio de Aventureros es la primera facción que el jugador encuentra. Con base en Emberveil (N0), es la organización más antigua y extendida de la Torre. Aldric, su fundador, entrena a los novatos y los envía a los pisos superiores. El gremio es el punto de entrada natural a la historia principal.

> "El gremio no es una familia. Es una herramienta. Úsala bien y llegarás lejos. Úsala mal y te quedas en el lodo con el resto." — Aldric

### 3.2. Datos Básicos

| Atributo | Valor |
|----------|-------|
| **Líder** | Aldric (Maestro del Gremio) |
| **Base** | Emberveil, edificio del gremio (plaza central) |
| **Alcance** | N0 (principal), influencia hasta N5 |
| **Miembros notables** | Lila (recepcionista), Borg (herrero), Miren (archivera) |
| **Monstruo asociado** | El Errante (evento N0) |

### 3.3. Niveles de Reputación

| Nivel | Rango | Efectos |
|-------|-------|---------|
| **Hostil** | -50 o menos | Expulsado del gremio. No puedes aceptar misiones oficiales. Aldric no te habla. Puedes recuperar reputación haciendo misiones por fuera (cazando monstruos, llevando recursos). |
| **Neutral** | 0 a 49 | Estado inicial. Misiones de rango E y D disponibles. Precios normales en herrería y tienda. Aldric te trata con cortesía profesional. |
| **Amistoso** | 50 a 149 | Misiones de rango C disponibles. 10% descuento en herrería de Borg. Acceso al archivo del gremio (lore sobre la Torre, mapas antiguos, diarios de aventureros caídos). |
| **Aliado** | 150 a 299 | Misiones de rango B disponibles. 20% descuento. Puedes solicitar escolta del gremio para un piso (un miembro experto te acompaña hasta N3). |
| **Héroe** | 300+ | Aldric comparte información personal sobre su pasado y por qué fundó el gremio. Te entrega un item único. Desbloquea diálogo especial en el final del juego. |

### 3.4. Cómo Ganar Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Completar misión rango E | +10 | Misiones iniciales de exploración |
| Completar misión rango D | +20 | Cacerías y rescates sencillos |
| Completar misión rango C | +30 | Incursiones a pisos superiores |
| Completar misión rango B | +40 | Misiones de alto riesgo |
| Completar misión rango A (si aplica) | +50 | Eventos únicos |
| Llevar recursos a Emberveil desde pisos superiores | +5 por recurso | Madera, mineral, textiles, etc. |
| Salvar a miembros del gremio en la Torre | +20 | Eventos de rescate aleatorios |
| Matar monstruos cerca de Emberveil | +1 cada 5 | Solo aplica en N0 |
| Donar objetos de valor al gremio | +2 por objeto | Items raros o antiguos |

### 3.5. Cómo Perder Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Fallar misión del gremio | -20 | Sin importar el rango |
| Atacar a miembros del gremio | -50 | Por agresión confirmada |
| Robar en Emberveil | -10 por robo | Incluye tiendas y casas |
| Negarse a ayudar en el ataque del Errante | -100 | Evento crítico de N0, solo si huyes |
| Insultar a Aldric en su cara | -30 | Opción de diálogo específica |
| Revelar secretos del gremio a otra facción | -40 | Si el gremio se entera |

### 3.6. Recompensas Exclusivas

| Nivel | Recompensa | Efecto |
|-------|------------|--------|
| **Amistoso** | Anillo del Gremio | +5% XP obtenida de misiones. Item equipable (anillo). |
| **Aliado** | Carta de Recomendación | Otorga 15% de descuento inicial en Thornwall (se suma al descuento normal). Consumible de una vez. |
| **Héroe** | El Último Favor de Aldric | Item único narrativo. En el combate final de N10, el Arquitecto duda antes de atacar al portador. No es invencibilidad, es un respiro que puede cambiar el curso de la batalla. |

### 3.7. Diálogos Clave por Nivel

**Neutral:**
- Aldric: "Eres nuevo. Todos lo fuimos. Demuestra que vales algo."
- Lila: "¿Misión? Toma el tablón. Escoge una que no te mate."

**Amistoso:**
- Aldric: "Sigues vivo. Eso ya es más de lo que logra la mayoría."
- Miren: "Tienes acceso al archivo ahora. No lo ensucies."

**Aliado:**
- Aldric: (asintiendo) "Podrías haber muerto veinte veces ya. Y aquí estás."
- Borg: "Para ti, descuento. Has ganado suficiente sangre en mi yunque."

**Héroe:**
- Aldric: (voz baja) "Yo también perdí a alguien en la Torre. Por eso construí esto. No para que subieran. Para que volvieran."
- Aldric te entrega el relicario. "Esto era de mi hermano. Llegó a N10. No volvió. Tal vez a ti te vaya mejor."

---

## 4. Elfos de Aelthar (N3)

### 4.1. Identidad

Aelthar es un asentamiento élfico construido alrededor de un lago cristalino en N3. Los elfos de Aelthar son los guardianes de la memoria de su pueblo, que existía antes de que la Torre fuera construida. Son reservados, longevos y desconfían de los humanos, pero respetan a quienes demuestran valía.

> "El lago recuerda todo. Nosotros solo somos sus guardianes." — Thalia

### 4.2. Datos Básicos

| Atributo | Valor |
|----------|-------|
| **Líder** | Thalia (Guardián del Lago) |
| **Base** | Aelthar, orilla del lago en N3 |
| **Alcance** | N3-N5 (comerciantes elfos llegan hasta Thornwall) |
| **Miembros notables** | Hadda (cantora, guardiana del Fragmento), Kaelen (herrero élfico), Lian (guardabosques) |
| **Lugar sagrado** | El Lago de los Susurros |

### 4.3. Niveles de Reputación

| Nivel | Rango | Efectos |
|-------|-------|---------|
| **Hostil** | -40 o menos | Expulsado de Aelthar. No puedes comprar ni descansar. Hadda se niega a hablarte (no obtienes el Fragmento por esta vía). Los elfos te atacan a la vista. |
| **Neutral** | 0 a 39 | Estado inicial. Precios normales. Thalia te observa con desconfianza. Puedes comerciar pero no accedes a zonas restringidas. |
| **Amistoso** | 40 a 99 | Hadda comparte una estrofa adicional de su canción (lore sobre el Fragmento). 15% descuento en herrería élfica de Kaelen. Acceso al claro de los cantos. |
| **Hermano de Aelthar** | 100+ | Acceso al Jardín Hundido (item único). Kaelen forja un arma personalizada según tu estilo de juego. Hadda te entrega el Fragmento sin necesidad de convencerla. |

### 4.4. Cómo Ganar Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Completar misiones de elfos | +15 a +30 | Cazar bestias corruptas, recuperar reliquias |
| No robar en Aelthar | (implícito) | No pierdes reputación |
| Respetar el lago | (implícito) | No contaminar, no pescar en exceso |
| Ayudar a elfos heridos | +15 | Eventos aleatorios en N3-N5 |
| Devolver objetos élficos perdidos | +10 por objeto | Reliquias, armas antiguas |
| Compartir información sobre la Torre | +5 | Si la información es útil y nueva |
| Defender Aelthar de una invasión | +40 | Evento único si ocurre |

### 4.5. Cómo Perder Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Robar en Aelthar | -30 | Por cada robo confirmado |
| Matar elfos | -50 por cada uno | Incluye accidentes "no intencionales" |
| Contaminar el lago | -60 | Tirar venenos, cadáveres o basura al agua |
| Dañar el Jardín Hundido | -40 | Si logras acceder sin permiso |
| Romper un juramento hecho a un elfo | -80 | Traición directa |

### 4.6. Recompensas Exclusivas

| Nivel | Recompensa | Efecto |
|-------|------------|--------|
| **Amistoso** | Estrofa de la Canción Olvidada | Lore importante sobre el origen de la Torre y el Fragmento. |
| **Hermano de Aelthar** | Llave del Jardín Hundido | Acceso a un área oculta con un item único (reliquia élfica que otorga resistencia al daño de N10). |
| **Hermano de Aelthar** | Arma Forjada por Kaelen | Arma personalizada que escala con el nivel del jugador. Se elige tipo en el momento de la forja. |

### 4.7. Diálogos Clave por Nivel

**Neutral:**
- Thalia: "Otro forastero. El lago te observa. No hagas nada que lamentes."
- Hadda: (canta en élfico, te ignora)

**Amistoso:**
- Hadda: "Escucha. Esta parte de la canción habla de un sol que nunca se apagó."
- Kaelen: "Tus manos conocen el acero. Te haré descuento."

**Hermano de Aelthar:**
- Thalia: (sonrisa leve) "Pocos humanos entienden el lago. Tú... casi lo logras."
- Hadda: (entregando el Fragmento) "Tómalo. La canción dice que es tuyo. No preguntes por qué."
- Kaelen: "Dime qué arma usas. Haré una que recuerde tu nombre."

---

## 5. Thornwall (N5)

### 5.1. Identidad

Thornwall es una fortaleza militar construida en N5, el primer puesto de avanzada serio de la humanidad en la Torre. Aquí viven soldados, cazadores y refugiados que han jurado no retroceder. Es una facción dura, práctica y desconfiada. No les importa tu pasado — solo importa si puedes pelear.

> "Aquí no hay héroes. Hay soldados vivos y soldados muertos. Escoge tu bando rápido." — Comandante Roderic

### 5.2. Datos Básicos

| Atributo | Valor |
|----------|-------|
| **Líder** | Comandante Roderic (o Capitana Drenna, si Roderic cae) |
| **Base** | Thornwall — fortaleza militar en N5 |
| **Alcance** | N5-N7 (patrullas llegan hasta la entrada de N8) |
| **Miembros notables** | Capitana Drenna (estratega), Ferrus (herrero de guerra), Iris (cartógrafa), Vigía Markus |
| **Amenaza constante** | La infección (cosa que corrompe a los caídos) |

### 5.3. Niveles de Reputación

| Nivel | Rango | Efectos |
|-------|-------|---------|
| **Hostil** | -60 o menos | Thornwall cierra sus puertas. No puedes entrar por la entrada principal. Debes usar las alcantarillas (riesgo de infección). Los guardias te atacan a la vista si te ven. |
| **Neutral** | 0 a 59 | Estado inicial. Puedes comprar en el mercado. No accedes al arsenal ni a las zonas militares. Roderic te evalúa. |
| **Confiable** | 60 a 149 | Acceso al arsenal militar (armas y armaduras de calidad superior). Misiones de caza mayor disponibles. Drenna te da información táctica. |
| **Hermano de Armas** | 150+ | Escolta militar disponible hasta N7 (dos soldados te acompañan por un piso). Drenna comparte consejos específicos sobre cómo enfrentar a la Costurera (jefe de N8). |

### 5.4. Cómo Ganar Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Misiones de caza en N5-N6 | +15 a +25 | Matar monstruos que amenazan rutas |
| Ayudar a guardias en apuros | +15 | Eventos aleatorios |
| Curar la infección en heridos | +10 por soldado | Usando hierbas o conocimientos médicos |
| Entregar recursos estratégicos | +5 por recurso | Armas, armaduras, suministros médicos |
| Defender Thornwall de un asalto | +50 | Evento único de defensa masiva |
| Entrenar con los soldados | +2 por sesión | Minijuego de entrenamiento |
| Completar misiones de exploración | +20 | Mapear zonas peligrosas |

### 5.5. Cómo Perder Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Matar guardias de Thornwall | -70 por guardia | Incluye "accidentes" |
| Robar armas del arsenal | -40 | Si te descubren |
| Negarse a ayudar en defensa | -60 | Cuando Thornwall es atacada |
| Colaborar con enemigos de Thornwall | -80 | Si se descubre tu traición |
| Robar en el mercado | -15 por robo |
| Provocar peleas en taberna | -10 | Desorden público |

### 5.6. Recompensas Exclusivas

| Nivel | Recompensa | Efecto |
|-------|------------|--------|
| **Confiable** | Pase de Arsenal | Compra armas y armaduras de grado militar. Algunas son exclusivas de Thornwall. |
| **Hermano de Armas** | Escolta Militar | Dos soldados de élite te acompañan por un piso completo (N5-N7). No pueden subir más allá. |
| **Hermano de Armas** | Consejo de Drenna sobre la Costurera | Información que revela una debilidad específica del jefe de N8, haciendo el combate significativamente más fácil. |

### 5.7. Diálogos Clave por Nivel

**Neutral:**
- Roderic: "No te conozco. No confío en ti. Demuéstrame que no soy idiota por dejarte entrar."
- Drenna: (mirando un mapa) "Si vas a morir, hazlo lejos de aquí. No tengo tiempo para funerales."

**Confiable:**
- Roderic: "Has matado cosas. Bueno. Sigue así."
- Drenna: "El arsenal está abierto. No me hagas arrepentirme."
- Ferrus: "Esto es acero de verdad, no las latas que venden abajo."

**Hermano de Armas:**
- Roderic: (palma en tu hombro) "Eres de los nuestros. Si caes, caerás hacia adelante."
- Drenna: (voz baja) "La Costurera... no le gusta el fuego. Recuérdalo cuando la enfrentes."
- Drenna: "Y otra cosa. No confíes en lo que ves en N8. Ella te muestra lo que quieres ver."

---

## 6. Argentia (N9)

### 6.1. Identidad

Argentia es la última ciudad antes de N10. No es un asentamiento de paso — es una fortaleza de los que han llegado más lejos que nadie. Aquí viven los supervivientes que han visto el techo de la Torre y han regresado (o decidieron quedarse). Argentia es seria, estoica y no tiene tiempo para novatos. Si llegas aquí, ya has demostrado que eres alguien.

> "Bienvenido al final del camino. Si llegaste hasta aquí, sabes que no hay vuelta atrás." — Capitana Elia

### 6.2. Datos Básicos

| Atributo | Valor |
|----------|-------|
| **Líder** | Capitana Elia |
| **Base** | Argentia — última ciudad antes de N10 |
| **Alcance** | N9-N10 (expediciones a N10) |
| **Miembros notables** | Ferrus (herrero legendario), Iris (cartógrafa de N10), El Caballero Caído (misterio), Hakeem (si llegó antes) |
| **Propósito** | Preparar al jugador para N10 |

### 6.3. Niveles de Reputación

| Nivel | Rango | Efectos |
|-------|-------|---------|
| **Neutral** | 0 a 49 | Estado inicial al llegar. Puedes comprar lo básico. Elia te evalúa. Información limitada. |
| **Respetado** | 50 a 99 | Ferrus acepta mejorar tu equipo a calidad Legendaria. Iris comparte el mapa detallado de N10 (rutas, peligros, puntos de interés). |
| **Confianza Total** | 100+ | Elia revela que sabe más de N10 de lo que admite. Te da el Fragmento del Arquitecto (si no lo conseguiste en Aelthar). Te entrega una poción especial que revive a un compañero caído **una sola vez** en N10. |

### 6.4. Cómo Ganar Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Misiones en Argentia | +15 a +30 | Explorar, cazar, recuperar suministros |
| Vencer al Verdugo | +40 | Jefe opcional cerca de Argentia |
| Hablar con el Caballero Caído | +10 | Completar su diálogo, ayudarlo a descansar |
| Compartir información sobre pisos inferiores | +5 por dato útil | Mapas, lore, experiencia |
| Ayudar en la defensa de Argentia | +35 | Evento de invasión |
| Curar a los heridos de expediciones | +8 por soldado |
| Entregar objetos de N10 (si bajas) | +20 por objeto | Solo aplica si sobrevives N10 y vuelves |

### 6.5. Cómo Perder Reputación

| Acción | Reputación | Notas |
|--------|------------|-------|
| Robar en Argentia | -25 por robo | Seguridad alta |
| Atacar NPCs | -60 por NPC | Incluye "pruebas de fuerza" no autorizadas |
| Rechazar misiones de Elia repetidamente | -15 por rechazo | Después del segundo rechazo |
| Traicionar a Argentia | -100 | Aliarse con el Arquitecto |
| Causar caos en la ciudad | -30 | Destrucción de propiedad |

### 6.6. Recompensas Exclusivas

| Nivel | Recompensa | Efecto |
|-------|------------|--------|
| **Respetado** | Mejora Legendaria por Ferrus | Cualquier arma o armadura puede ser mejorada a calidad Legendaria (la más alta del juego). |
| **Respetado** | Mapa de N10 (Iris) | Revela todas las salas, atajos y peligros de N10. No revela secretos ni jefes, pero evita que te pierdas. |
| **Confianza Total** | Fragmento del Arquitecto | Necesario para ciertos finales. Si ya lo tienes de Aelthar, Elia te da información adicional sobre su uso. |
| **Confianza Total** | Poción de la Última Voluntad | Revive a un compañero caído una sola vez en N10. Consumible, desaparece tras usarse. Solo funciona en N10. |

### 6.7. Diálogos Clave por Nivel

**Neutral:**
- Elia: "Llegaste lejos. Felicidades. Ahora demuestra que no fue suerte."
- Ferrus: (sin mirarte) "¿Eso es lo que usas para pelear? Tráeme algo mejor y hablamos."

**Respetado:**
- Elia: (asintiendo) "Los de abajo hablan bien de ti. Eso no es común."
- Ferrus: "Tu equipo tiene historia. La respetaré. Ven mañana."
- Iris: (extendiendo un mapa) "N10 es un laberinto. Esto te mantendrá vivo unos minutos más."

**Confianza Total:**
- Elia: (voz seria) "Te mentí. No sé todo lo que hay en N10... pero sé más de lo que dije. El Arquitecto no es lo que crees."
- Elia: "Toma. Un Fragmento. Y esto — úsalo solo si alguien cae. En N10, los muertos no vuelven."
- Elia: "Cuando enfrentes al Arquitecto... recuerda que la luz también ciega."

---

## 7. Mercaderes Viajeros (Transversal)

### 7.1. Identidad

Los Mercaderes Viajeros no son una facción en el sentido tradicional — no tienen territorio, no tienen ejército, no te piden lealtad. Son una caravana comercial que recorre la Torre, apareciendo siempre un piso antes que el jugador. Su líder, Hakeem, sabe más de lo que dice y siempre aparece cuando más lo necesitas.

> "No tengo facción. Tengo negocios. Y tú, amigo, eres un buen negocio." — Hakeem

### 7.2. Datos Básicos

| Atributo | Valor |
|----------|-------|
| **Líder** | Hakeem (jefe de la caravana) |
| **Base** | Móvil (caravana) |
| **Puntos de aparición** | N2, N5, N7, N9 (siempre un piso antes que el jugador por primera vez) |
| **Tipo de facción** | Comercial / Neutral |
| **Peligro** | Si matas a Hakeem, la caravana desaparece permanentemente |

### 7.3. Mecánica de Precios

Los Mercaderes Viajeros no usan niveles de reputación. En su lugar, usan un sistema de **fidelidad comercial**:

- Cada transacción (compra o venta) suma un punto de fidelidad.
- Por cada punto, obtienes **+2% de descuento** en todas las compras futuras.
- El descuento máximo es **30%** (15 transacciones).
- Los precios de venta también mejoran: +1% por cada punto de fidelidad (máx 15% más por tus objetos).

### 7.4. Riesgos y Consecuencias

| Acción | Consecuencia |
|--------|--------------|
| Matar a Hakeem | La caravana desaparece permanentemente. No vuelve a aparecer en ningún piso. Todos los beneficios de fidelidad se pierden. |
| Robar a la caravana | Hakeem desaparece por 3 encuentros (se salta las siguientes 3 apariciones). Al volver, los precios están inflados +20% como penalización. |
| Salvar a Hakeem de un ataque | +5 puntos de fidelidad inmediatos (10% de descuento). |

### 7.5. Diálogos Clave

**Primer encuentro:**
- Hakeem: "Ah, carne fresca. No, no te ofendas — es jerga de mercader. Tengo cosas que te interesan."

**Después de varias compras:**
- Hakeem: "Ya eres cliente habitual. Te guardo lo mejor. Y lo mejor no está en el mostrador."

**Si mataste a Hakeem (nunca vuelve a aparecer):**
- Mensaje persistente en el diario: "La caravana de Hakeem ya no viaja por la Torre. Nadie sabe qué pasó."

---

## 8. Interacciones entre Facciones

### 8.1. Bonificaciones Cruzadas

La reputación en una facción puede influir en tu posición inicial en otra:

| Condición | Efecto |
|-----------|--------|
| Aliado o Héroe del Gremio | +10 de reputación inicial con Thornwall ("Viene recomendado por Aldric") |
| Hermano de Aelthar | +15 de reputación inicial con Argentia ("Los elfos rara vez se equivocan con la gente") |
| Hermano de Armas (Thornwall) | +20 de reputación inicial con Argentia ("Luchó junto a los nuestros") |
| Hostil con el Gremio | -10 de reputación inicial con Thornwall ("Aldric no lo quiere. Algo hizo.") |
| Hostil con Aelthar | -5 de reputación inicial con Argentia (indiferencia, pero los elfos se quejan) |

### 8.2. Penalizaciones Cruzadas

| Acción | Penalización |
|--------|-------------|
| Ser aliado de una facción enemiga de otra | No hay penalización directa, pero ciertos diálogos se bloquean |
| Matar a un miembro de Thornwall | Los elfos de Aelthar te miran con recelo (-5 si se enteran) |
| Ayudar a la infección a propagarse (N5) | Todas las facciones te penalizan (-10 cada una) |

### 8.3. Sinergias Máximas

Si logras reputación máxima en múltiples facciones, el juego lo reconoce:

- **Héroe del Gremio + Hermano de Aelthar + Hermano de Armas + Confianza Total en Argentia:**
  - Diálogo especial de Elia en Argentia: "Los de abajo hablan bien de ti. Eso es raro. Muy raro. ¿Quién eres realmente?"
  - Opción de diálogo desbloqueada: "Solo alguien que quiere llegar a N10."
  - Elia responde: "No. Alguien a quien todos recuerdan. Eso es más raro todavía."

- **Héroe del Gremio + Hermano de Aelthar:**
  - Aldric y Thalia tienen un intercambio de mensajes (lore adicional sobre el pasado del gremio).
  - Hadda canta una versión extendida de su canción si sabe que Aldric te respalda.

---

## 9. Consecuencias Globales

### 9.1. Impacto en la Historia Principal

| Escenario | Consecuencia |
|-----------|-------------|
| **Hostil con el Gremio** | No puedes empezar las misiones principales por la vía normal. No tienes acceso al tablón de misiones. Debes colarte en la Torre por la ruta de las alcantarillas (N0 alternativo). La historia sigue siendo jugable, pero es más difícil y solitaria. |
| **Hostil con Aelthar** | Hadda no te da el Fragmento. El final B (revelación del origen de la Torre) queda bloqueado — **a menos que** obtengas el Fragmento en Argentia (si logras Confianza Total). Es más difícil, pero no imposible. |
| **Hostil con Thornwall** | Entrada restringida. Debes usar las alcantarillas de N5 (peligrosas). No obtienes el consejo de Drenna sobre la Costurera (N8 es más difícil). No tienes escolta para N7. |
| **Hostil con Argentia** | No obtienes la mejora Legendaria. No obtienes el mapa de N10. No obtienes la poción de resurrección. N10 es significativamente más difícil. |

### 9.2. El Camino del Rechazado

Si **todas las facciones te odian** simultáneamente:

- No hay NPCs que te ayuden en ningún piso habitable.
- No puedes comprar en Emberveil, Aelthar, Thornwall ni Argentia.
- Tu única fuente de comercio es Hakeem (si no lo mataste). Los finales del juego siguen siendo accesibles, pero son significativamente más difíciles: sin ayuda de NPCs, sin items especiales, sin descanso seguro. Es el modo difícil definitivo.
### 9.3. Rutas Alternativas

| Ruta | Requisito | Dificultad |
|------|-----------|------------|
| Ruta del Héroe | Reputación alta en 2+ facciones | Normal |
| Ruta del Mercenario | Solo comercio con Hakeem, facciones hostiles | Alta |
| Ruta del Fantasma | Todas las facciones hostiles, Hakeem muerto | Extremo |
| Ruta del Redimido | Reputación hostil → redención mediante misiones especiales | Variable |

---

## 10. Diálogos Dinámicos

### 10.1. Sistema de Diálogo por Reputación

Los NPCs cambian su comportamiento según la reputación del jugador. Esto no es solo texto decorativo — afecta qué información está disponible, qué misiones se activan y cómo los NPCs reaccionan ante las decisiones del jugador.

### 10.2. Ejemplos de Diálogo por Nivel de Reputación

**Aldric (Gremio) según reputación:**

- **Hostil (-50):** "No tienes nada que hacer aquí. Largo."
- **Neutral (0-49):** "Misiones en el tablón. No las desperdicies."
- **Amistoso (50-149):** "Has hecho buen trabajo. Sigue así."
- **Aliado (150-299):** "Puedes pedir escolta. Escoge bien a quién llevas."
- **Héroe (300+):** (pausa) "Mi hermano murió en N10. Si llegas allí... dile que lo siento."

**Thalia (Aelthar) según reputación:**

- **Hostil (-40):** "Aléjate del lago. No eres bienvenido."
- **Neutral (0-39):** "El lago te tolera. No abuses de su paciencia."
- **Amistoso (40-99):** "Hadda quiere hablarte. Ve con ella."
- **Hermano (100+):** "Eres de Aelthar ahora. El lago te reconoce."

**Vendedores según reputación:**

- **Hostil:** "No te vendo nada. Largo." (no abre el menú de comercio)
- **Neutral:** Precios base. "Escoge rápido."
- **Amistoso:** 10-15% descuento. "Para ti, precio especial."
- **Aliado/Máximo:** 20%+ descuento. "Toma, es tuyo. Ojalá vuelvas."

### 10.3. Diálogos Especiales por Combinación de Facciones

**Héroe del Gremio + Hermano de Aelthar + llegada a Argentia:**

- Guardia de Argentia: "¿El del gremio? ¿El que los elfos dejaron entrar al Jardín? Pase, capitana Elia quiere verlo."
- Elia: "Tres facciones te respaldan. Eso no es casualidad. Siéntate. Hablemos."

**Hostil en Gremio pero Hermano de Armas en Thornwall:**

- Roderic: "El gremio te odia. Aquí nos importa poco. Lo que importa es lo que matas."

**Hermano de Aelthar pero Hostil en Thornwall:**

- Los elfos comerciantes en Thornwall te tratan bien, pero los guardias de Thornwall te miran con sospecha por asociación.

---

## 11. Implementación Técnica

### 11.1. Estructura de Datos (SaveGame)

`json
{
  "reputation": {
    "guild": {
      "value": 45,
      "flags": ["met_aldric", "completed_first_mission", "saved_member"]
    },
    "aelthar": {
      "value": 0,
      "flags": ["discovered_aelthar"]
    },
    "thornwall": {
      "value": -10,
      "flags": []
    },
    "argentia": {
      "value": 0,
      "flags": []
    },
    "merchants": {
      "transactions": 3,
      "hakeem_alive": true,
      "last_encounter_floor": 2
    }
  }
}
`

### 11.2. Flags Asociados

Los flags son booleanos que registran eventos importantes para cada facción:

| Flag | Facción | Evento |
|------|---------|--------|
| met_aldric | Gremio | Primera conversación con Aldric |
| completed_first_mission | Gremio | Primera misión completada |
| saved_member | Gremio | Rescató a un miembro en la Torre |
| betrayed_guild | Gremio | Traicionó activamente al gremio |
| discovered_aelthar | Aelthar | Encontró Aelthar |
| lake_contaminated | Aelthar | Contaminó el lago |
| hadda_fragment | Aelthar | Hadda entregó el Fragmento |
| entered_thornwall | Thornwall | Entró a Thornwall por primera vez |
| cure_infection | Thornwall | Curó la infección en un soldado |
| defended_thornwall | Thornwall | Participó en la defensa |
| met_elia | Argentia | Primera conversación con Elia |
| beat_executioner | Argentia | Venció al Verdugo |
| elia_fragment | Argentia | Elia entregó el Fragmento |
| hakeem_dead | Mercaderes | Hakeem fue asesinado |

### 11.3. Persistencia y Carga

- La reputación se guarda en el archivo de SaveGame junto con el progreso del jugador.
- Al cargar una partida, todos los NPCs verifican la reputación actual antes de cada interacción.
- No hay "decaimiento" de reputación con el tiempo — solo cambia por acciones del jugador.

### 11.4. UI y Feedback

**Elementos visuales:**

1. **Pantalla de personaje:** Pestaña de "Relaciones" muestra las facciones conocidas con barra cualitativa.
2. **Notificaciones:** Icono de facción + valor numérico (ocultable en opciones).
3. **Tooltips:** Al pasar sobre un NPC, un icono pequeño muestra tu reputación con su facción.
4. **Mapa:** Las zonas de facciones hostiles se marcan en rojo en el minimapa.

**Sonido:**

- Subir de nivel: sonido ascendente positivo.
- Bajar de nivel: sonido grave descendente.
- Cruce de umbral: efecto sonoro especial (campanas para amistoso, redoble para hostil).

### 11.5. Sistema de Misiones con Reputación

Cada misión tiene requisitos de reputación:

| Rango | Reputación Mínima | Ejemplo |
|-------|-------------------|---------|
| E | 0 (Neutral) | "Limpia de ratas el sótano" |
| D | 0 (Neutral) | "Explora el borde de N1" |
| C | 50 (Amistoso) | "Investiga la cueva de N2" |
| B | 150 (Aliado) | "Asalta el nido de N4" |
| A | 250+ (Héroe) | "Enfrenta al Errante de N0" |

---

## 12. Tabla Resumen

### 12.1. Umbrales de Reputación por Facción

| Facción | Base | Hostil | Neutro | Bueno | Máximo |
|---------|------|--------|--------|-------|--------|
| Gremio de Aventureros | N0 | -50 o menos | 0 a 49 | 50 a 149 (Amistoso) / 150 a 299 (Aliado) | 300+ (Héroe) |
| Elfos de Aelthar | N3 | -40 o menos | 0 a 39 | 40 a 99 (Amistoso) | 100+ (Hermano) |
| Thornwall | N5 | -60 o menos | 0 a 59 | 60 a 149 (Confiable) | 150+ (Hermano de Armas) |
| Argentia | N9 | N/A (no hay hostil) | 0 a 49 | 50 a 99 (Respetado) | 100+ (Confianza Total) |
| Mercaderes Viajeros | Global | N/A | 0 transacciones | — | 30% descuento máximo |

### 12.2. Recompensas por Facción y Nivel

| Facción | Nivel Bueno | Nivel Máximo |
|---------|-------------|--------------|
| **Gremio** | Anillo del Gremio (+5% XP), misiones C, descuento 10%, acceso a archivo | Carta de Recomendación, misiones B, descuento 20%, escolta, El Último Favor de Aldric |
| **Aelthar** | Estrofa adicional de Hadda, descuento 15% en herrería élfica | Jardín Hundido (item único), arma forjada por Kaelen, Fragmento directo |
| **Thornwall** | Acceso al arsenal, misiones de caza mayor | Escolta militar hasta N7, consejo de Drenna sobre la Costurera |
| **Argentia** | Mejora Legendaria por Ferrus, mapa de N10 | Fragmento (si falta), Poción de la Última Voluntad, lore de Elia |
| **Mercaderes** | Descuento progresivo +2% por transacción | Máximo 30% descuento, 15% bonus en ventas |

### 12.3. Penalizaciones por Facción

| Acción | Gremio | Aelthar | Thornwall | Argentia | Mercaderes |
|--------|--------|---------|-----------|----------|------------|
| Fallar misión | -20 | -15 | -25 | -20 | N/A |
| Atacar miembro | -50 | -50 | -70 | -60 | N/A (muerte permanente) |
| Robar | -10/robo | -30/robo | -15/robo | -25/robo | Pierde 3 apariciones |
| Traicionar | -40 | -80 | -80 | -100 | N/A |
| Negar ayuda crítica | -100 | -40 | -60 | -15/rechazo | N/A |

---

## 13. Notas de Diseño

### 13.1. Filosofía del Sistema

El sistema de reputación de SOLO no está diseñado para ser un checklist de "completa todas las misiones para maximizar". Está diseñado para **obligar al jugador a tomar decisiones difíciles**.

- No hay suficiente tiempo/recursos para maximizar todas las facciones en una sola partida.
- Algunas decisiones que benefician a una facción perjudican a otra.
- El juego es completable incluso con reputación mínima — es más difícil, pero no imposible.
- Las recompensas exclusivas están diseñadas para que cada partida se sienta diferente según las alianzas elegidas.

### 13.2. Rejugabilidad

El sistema de reputación es un pilar de rejugabilidad en SOLO:

| Partida | Enfoque | Experiencia |
|---------|---------|-------------|
| 1ª partida | Buscar ayuda de todos (neutral-general) | Experiencia base, final neutral |
| 2ª partida | Máxima reputación con Gremio + Thornwall | Ruta militar, final de fuerza |
| 3ª partida | Máxima reputación con Aelthar + Argentia | Ruta mística, final de conocimiento |
| 4ª partida | Todas las facciones hostiles | Ruta del rechazado, final extremo |
| 5ª partida | Equilibrio cuidadoso | Diálogos especiales, final oculto (si existe) |

### 13.3. Notas de Balance

- **El descuento máximo acumulado** (Gremio 20% + Thornwall + Aelthar 15% + Hakeem 30%) puede hacer que ciertos objetos sean muy baratos. Considerar un límite de descuento global (ej: 50% máximo).
- **La Poción de la Última Voluntad** es extremadamente poderosa. Debe ser un consumible de una sola vez y solo funcional en N10.
- **El Último Favor de Aldric** no debe hacer invencible al jugador. El Arquitecto duda un turno (o su equivalente), no perdona la vida automáticamente.
- **El arma forjada por Kaelen** debe ser única y no mejorable (o mejorable solo por Ferrus en Argentia, creando una conexión entre facciones).
- **Hakeem muerto = pérdida permanente.** Sin posibilidad de revivir. Esto debe quedar muy claro al jugador antes de actuar.

### 13.4. Consideraciones de Narrativa

- La reputación no es solo mecánica — es narrativa. Cada subida y bajada debe sentirse justificada por las acciones del jugador.
- Los NPCs deben reaccionar de forma creíble. Un mercader no debería tratarte igual después de que mataste a su sobrino.
- El diálogo de "Héroe + Hermano + Hermano de Armas + Confianza Total" debe sentirse como un logro narrativo, no solo una suma de números.
- La ruta del rechazado debe tener su propia narrativa satisfactoria, no ser un castigo.

### 13.5. Posibles Expansiones

- **Misiones de redención:** Si llegas a Hostil en una facción, podría existir una misión especial (una sola vez) para recuperar reputación hasta Neutral. Costosa en tiempo y recursos.
- **Facción secreta:** Una sexta facción oculta que solo se descubre con combinaciones específicas de reputación.
- **Sistema de aliados:** Más allá de la escolta, tener alta reputación podría permitir reclutar NPCs como compañeros temporales.
- **Eventos de facción:** Asaltos coordinados entre facciones, cambios de liderazgo si el líder muere, etc.

---

## Apéndice A: Flujo del Jugador por Reputación

### A.1. Flujo Recomendado (Primera Partida)

1. **N0 — Gremio:** Completa misiones E y D. Llega a Amistoso (50+). Consigue el Anillo del Gremio.
2. **N3 — Aelthar:** Sé respetuoso. No robes. Llega a Amistoso (40+). Obtén la estrofa de Hadda.
3. **N5 — Thornwall:** Ayuda en cacerías. Llega a Confiable (60+). Accede al arsenal.
4. **N7 — Preparación:** Usa la escolta de Thornwall si es necesario.
5. **N8 — La Costurera:** Usa el consejo de Drenna.
6. **N9 — Argentia:** Sé útil. Llega a Respetado (50+). Mejora tu equipo a Legendario. Consigue el mapa de N10.
7. **N10 — El Arquitecto:** Enfrenta el final con las herramientas de tus alianzas.

### A.2. Flujo Alternativo (Ruta Solitaria)

1. Ignora al Gremio. Entra a la Torre por las alcantarillas.
2. Evita Aelthar. Toma rutas alternativas en N3-N4.
3. Entra a Thornwall por las alcantarillas de N5.
4. Llega a Argentia sin aliados.
5. Intenta ganar reputación en Argentia directamente.
6. Enfrenta N10 sin ayuda externa.

---

## Apéndice B: Registro de Cambios

| Versión | Fecha | Cambios |
|---------|-------|---------|
| v1.0 | 2026-04-29 | Documento inicial. Sistema de reputación completo con 5 facciones, mecánicas, diálogos y consecuencias globales. |

---

> *"La Torre no olvida. Tampoco los que viven en ella."*
> — Proverbio de Emberveil
