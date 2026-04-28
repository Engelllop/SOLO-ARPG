# 05 — Folclore del Primer Ataque

> *"Quien clava la primera espada no es el más fuerte. Es el primero en mirar."*
> — Refrán de los gremios de aventureros, Octava Edad

---

## El sistema in-universe

El **Primer Ataque** es un fenómeno cósmico, no una mecánica de juego para los habitantes del mundo. Cuando un mortal mata por primera vez a un Jefe de Piso, **su nombre queda inscrito en el Tejido**. Persiste entre Edades. Es la única forma de inmortalidad que ni el Arquitecto puede borrar.

Esta es la única decisión del jugador que **trasciende la reescritura**. Y los habitantes lo saben — aunque no entiendan por qué.

---

## Manifestaciones físicas en el mundo

> *Nota de implementación:* este sistema debe sentirse vivo desde el día uno, incluso antes de que la Torre esté abierta. Los nombres de Edades pasadas ya están allí. Los nombres del jugador actual se añaden a lo largo del juego conforme conquista pisos.

### Las Estatuas del Umbral

En el **Refugio del Umbral** (campamento base de la Torre, ver [`03_la_torre.md`](03_la_torre.md)), una avenida central llamada **El Paseo de los Primeros** alberga **diez estatuas de piedra**. Cada una representa al Jefe de Piso correspondiente. En la base, una placa con:

```
PISO X — [Nombre del Jefe]
PRIMERO EN CAER ANTE: [Nombre del jugador / NPC]
EDAD: [Cuál]
```

**Comportamiento de las placas:**
- Al inicio de la Octava Edad (= inicio del juego), las placas tienen nombres de Edades pasadas. Algunos legendarios. Algunos olvidados. Algunos en lenguas muertas.
- Cuando el jugador (o cualquier jugador del servidor en multijugador) hace un Primer Ataque, **la placa cambia físicamente**. La piedra parece hervir un instante. Cuando se enfría, el nombre nuevo está grabado.
- Los nombres antiguos no se borran. Se desplazan hacia abajo, formando una columna histórica. Las estatuas más antiguas tienen columnas tan largas que llegan al suelo y desaparecen bajo el adoquín.

**Detalles ambientales:**
- La estatua del piso 60 (La Sombra Eterna) **siempre es ligeramente distinta** dependiendo de quién hizo el Primer Ataque, porque la Sombra adopta la forma del jugador.
- La estatua del piso 100 (El Arquitecto) está **vacía**. El pedestal existe; la figura no. *Aún no.* Cuando alguien complete el juego — en cualquier final — esa estatua aparece. La forma que toma depende del final elegido.

### Los Tablones de las Tabernas

Las **5 ciudades principales** tienen, cada una, una taberna emblemática. Y cada una tiene un **Tablón del Andar** — un panel de madera ennegrecido por el humo donde se anuncian:

- Misiones del gremio de aventureros (mecánica vanilla).
- **Crónicas de los Primeros** — pergaminos clavados con cuchillos pequeños, que cuentan la hazaña del último Primer Ataque del servidor / Edad.

**Comportamiento del tablón:**
- Cada vez que un jugador hace un Primer Ataque en cualquier piso, **un nuevo pergamino aparece** en los cinco tablones de las cinco ciudades, simultáneamente, sin que nadie lo haya colgado.
- El texto está escrito **en una caligrafía que cambia según el lugar de origen del jugador** (raza, ciudad). Detalle bonito que muestra que el Tejido conoce al héroe íntimamente.
- Los tabernarios y bardos no preguntan cómo aparecen. Asumen que algún viajero los trae.
- **Los pergaminos viejos no se quitan.** El tablón se hace cada vez más espeso. En las tabernas más antiguas, la pared entera ha desaparecido bajo capas de pergaminos clavados.

**Ejemplo de pergamino (piso 30, Rey Lich):**

```
╔══════════════════════════════════════╗
║  CRÓNICA DEL PASEO TREINTA            ║
║                                        ║
║  En la Octava Edad, en la luna del    ║
║  Águila, [NombreJugador] de [Ciudad]  ║
║  ascendió la Aguja hasta el piso      ║
║  treinta. Allí enfrentó al Rey de     ║
║  los Huesos Vacíos y le devolvió      ║
║  a su trono el silencio.               ║
║                                        ║
║  La Aguja recuerda.                    ║
╚══════════════════════════════════════╝
```

### Los Bardos Errantes

NPCs especiales — **bardos itinerantes** — viajan entre las cinco ciudades. Su función:

- Cantan canciones de los Primeros Ataques actuales y pasados.
- Si el jugador les paga unas monedas, le contarán la canción de un piso específico — incluyendo nombres de Edades anteriores que el jugador no conocería de otra forma.
- **Pueden cantar la canción del propio jugador** una vez que el jugador haya hecho un Primer Ataque. Es uno de los momentos cálidos del juego: un NPC anónimo recita tu hazaña a una taberna llena, y los bebedores levantan sus jarras hacia ti sin que tú hayas dicho una palabra.
- Los bardos olvidan. Si una Edad termina y empieza la siguiente, los bardos solo recuerdan las canciones más antiguas. Las recientes se pierden. **Solo las estatuas y los pergaminos persisten.**

### Las Marcas en las Armas Legendarias

Cuando un jugador hace un Primer Ataque, **el arma con la que asestó el golpe final cambia físicamente**:

- Una runa pequeña aparece grabada cerca de la guarda. La runa no estaba antes.
- Si el jugador examina el arma cuidadosamente, la descripción del ítem se actualiza con una línea nueva: *"Esta hoja silenció al Rey Lich, en la Octava Edad."*
- Si el arma se intercambia, vende, pierde o destruye, la runa permanece. **Los siguientes dueños del arma heredan el prestigio**, aunque ellos no hicieran el Primer Ataque. Esto es una mecánica social interesante: las armas con runas son codiciadas en el mercado.
- Las armas con cinco o más runas de Primer Ataque adquieren un nombre propio que aparece automáticamente. *El Tejido las nombra*.

### El Tablón Central del Refugio del Umbral

Aparte de las estatuas, el Refugio tiene un único tablón gigantesco — **El Diario del Umbral** — que registra **todos** los Primer Ataques de la Edad actual en orden cronológico. Un cronista NPC sin nombre escribe en él diariamente. **Nadie lo ha visto escribir nunca**. Las entradas aparecen al amanecer.

Cuando termina una Edad, el Diario se cierra y **es enterrado en una catacumba bajo el Refugio**. Las catacumbas — *Las Bóvedas del Cronista* — son una mazmorra opcional del juego donde el jugador puede leer Diarios de Edades anteriores y descubrir qué nombres ya estuvieron allí. Spoiler intencional: en algunos Diarios antiguos, el nombre del propio protagonista aparece. *Otra Edad. Otra cara. Lo mismo.*

---

## Reglas para escritores

1. **Los Primeros Ataques son honor sagrado, pero no veneración religiosa.** Los habitantes los respetan, los celebran, los recuerdan — pero no los adoran.
2. **Las canciones, pergaminos y placas nunca explican por qué los nombres persisten.** Solo lo afirman como un hecho. El misterio es parte del encanto. La verdad solo se revela en el Acto V.
3. **No hay rivalidad por hacer Primer Ataque.** Quien lo hace tiene el honor pero no la enemistad de los demás. El Tejido elige a su persistente; los demás aceptan.
4. **Los nombres in-game deben preservarse exactamente como los escribió el jugador.** Mayúsculas, minúsculas, símbolos. El Tejido no corrige. Esto puede dar momentos cómicos cuando un jugador con nombre ridículo aparece grabado en una estatua de mármol negro entre nombres heroicos. Ese contraste forma parte del encanto del sistema.
5. **El multijugador comparte el Tablón.** Aunque el juego sea principalmente single-player en FASE 0, el sistema de Primer Ataque debe diseñarse desde el principio asumiendo persistencia compartida. Es lo que hace que cuando ves "[OtroJugador] mató al Rey Lich" en una placa, sientas que estás en un mundo real.
6. **El nombre del propio jugador siempre aparece en cursiva azul** en su propio cliente, para diferenciarlo del resto. Los demás lo ven en la grafía estándar.

---

## Hooks narrativos para quests secundarias

- **"El Pergamino que Mencionaba a tu Padre"** — un viajero anciano dice haber visto el nombre del padre del protagonista en un pergamino antiguo del Tablón del Refugio. El padre nunca llegó tan lejos. ¿Es un error? ¿Es otra Edad? El protagonista puede investigar.
- **"La Estatua Cambiada"** — una jugadora reporta haber visto el rostro de la estatua del piso 60 cambiar mientras dormía cerca. El cambio sucedió cuando alguien lejano hizo Primer Ataque. Ella es la única que lo vio. Quest de investigación.
- **"El Bardo que se Calló"** — uno de los bardos errantes ha dejado de cantar. Solo bebe en silencio. Si el protagonista le pregunta y trae un objeto especial, le confiesa que **ha empezado a recordar canciones de Edades que nadie le ha enseñado**. Es un Persistente menor. No lo sabe. El protagonista puede decirle la verdad o no.
