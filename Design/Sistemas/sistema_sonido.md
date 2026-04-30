# Sistema de Sonido y Música de SOLO

> **SOLO (Sunlight of the Last Online)** — RPG medieval oscuro  
> Este documento especifica cómo debe sonar el juego. No es un documento técnico de audio — es la guía para que un compositor o Claude Code sepa qué se necesita para dar vida al mundo sonoro de SOLO. Mínimo 15KB.

---

## Índice

1. [Filosofía del Sonido](#1-filosofía-del-sonido)
2. [Música por Zona (Pistas Ambientales)](#2-música-por-zona-pistas-ambientales)
   - [N0 — Emberveil (Overworld)](#n0--emberveil-overworld)
   - [N1 — Velmar](#n1--velmar)
   - [N2 — Pastor de Cenizas](#n2--pastor-de-cenizas)
   - [N3 — Aelthar](#n3--aelthar)
   - [N4 — El Corredor de los Nombres](#n4--el-corredor-de-los-nombres)
   - [N5 — Thornwall](#n5--thornwall)
   - [N6 — Cámaras de Jade](#n6--cámaras-de-jade)
   - [N7 — Sahal](#n7--sahal)
   - [N8 — Jardines Flotantes](#n8--jardines-flotantes)
   - [N9 — Argentia](#n9--argentia)
   - [N10 — El Centinela](#n10--el-centinela)
3. [Leitmotivs por Personaje](#3-leitmotivs-por-personaje)
4. [Efectos de Sonido Clave](#4-efectos-de-sonido-clave)
5. [Silencios Importantes (Momentos sin Música)](#5-silencios-importantes-momentos-sin-música)
6. [Consideraciones Técnicas (para Claude Code)](#6-consideraciones-técnicas-para-claude-code)

---

## 1. Filosofía del Sonido

### Principios Fundamentales

El sonido en SOLO no es un acompañamiento — es un **personaje más**. No hay concesiones a lo épico gratuito. Cada nota, cada silencio, cada crujido existe porque tiene un propósito narrativo.

**Minimalista, no ausente.** La música no suena constantemente. Cuando suena, importa. Cuando se calla, importa aún más. El jugador debe notar la diferencia entre un mundo que respira y uno que guarda silencio.

**El silencio es un instrumento.** En SOLO, el silencio no es un error técnico ni un vacío que llenar. Es una herramienta deliberada de diseño emocional. Los momentos sin música pesan más que los que tienen música. El jugador debe sentir la ausencia como un duelo.

**Ambiente sobre melodía.** Los sonidos del mundo (viento, agua, pasos sobre distintos terrenos) son más importantes que la banda sonora. La atmósfera se construye desde abajo: primero el mundo suena real, luego la música lo realza. Si la música ahoga al ambiente, está mal diseñada.

**Los silencios duelen.** Después de eventos importantes (muerte de Caín, muerte de Mira o Vassen, el entierro), la música se detiene por completo durante minutos. No hay fade-out gradual que alivie el golpe. Es un corte seco. El jugador debe quedarse en silencio, procesando lo que acaba de pasar.

**El Tejido tiene su propio sonido.** La magia en SOLO no suena a fantasía genérica. El Tejido emite un zumbido de baja frecuencia, casi imperceptible, que solo se activa cuando el jugador usa habilidades mágicas o entra en lugares de poder. Es un sonido que el jugador aprende a reconocer instintivamente.

**Cada jefe tiene su tema.** No hay música de batalla genérica que se recicla para todos los encuentros. Cada combate importante tiene su propia identidad sonora — desde la percusión metálica del Carnicero hasta el zumbido de cristal del Coro de las Cámaras de Jade. La música de jefe no es relleno; es parte del diseño del encuentro.

**Transiciones emocionales.** La música no cambia solo por entrar a una zona — cambia por el **estado emocional** que debe transmitir la escena. Un mismo lugar puede sonar diferente según el momento de la historia.

**La rareza crea atención.** No todo merece un sonido. Los efectos se usan con moderación para que, cuando aparecen, el jugador los note. Un menú que hace clic con suavidad. Una habilidad que se desbloquea con un tono claro. Un quest completado con un acorde. Menos es más.

---

## 2. Música por Zona (Pistas Ambientales)

### N0 — Emberveil (Overworld)

La primera zona del juego. El jugador debe sentir que está en un lugar hermoso pero herido. La música establece la promesa de un mundo que merece ser salvado.

#### Día
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Cuerdas suaves (violín solo, sin acompañamiento orquestal). Viento leve. Pájaros distantes como textura de fondo. |
| **BPM** | 60 |
| **Tono** | Esperanza tranquila. No es alegre, pero tampoco triste. Es un lugar que aún tiene belleza. |
| **Duración de bucle** | 3-4 minutos. Debe ser lo suficientemente larga para no volverse repetitiva. |
| **Notas** | El violín debe sonar ligeramente desafinado en algunas notas — un imperceptible pero presente recordatorio de que algo no está bien en el mundo. |

#### Noche
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Cuerdas más graves (viola o cello, sin violín). Sin viento. Hay silencio entre las notas — pausas incómodas de 2-3 segundos. |
| **BPM** | 40 |
| **Tono** | Melancolía. No terror. El jugador debe sentir que el mundo descansa, no que está acechando. |
| **Duración de bucle** | 4-5 minutos. La noche debe sentirse larga y quieta. |
| **Notas** | Las pausas entre notas son intencionales. El jugador puede oír sus propios pasos y la respiración del personaje entre las notas de la viola. |

#### Bosque Vigilia
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Flauta de madera, percusión leve que imita ramas que crujen. Sin cuerdas. |
| **BPM** | 70 |
| **Tono** | Alerta pero en calma. El bosque está despierto. |
| **Notas** | La percusión debe sonar orgánica — como si el bosque mismo estuviera marcando el ritmo con ramas y hojas secas. |

#### Lago Negro
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Sin instrumentos. Solo sonidos de ambiente: agua quieta, niebla. |
| **BPM** | 0 — ambiente puro |
| **Tono** | Vacío. El lago no tiene emociones. |
| **Notas** | Este es el primer lugar donde el jugador experimenta la ausencia total de música en SOLO. Debe sentirse extraño. Incómodo. Como si el lugar estuviera muerto. |

#### Garganta del Errante
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Percusión grave (tambores profundos), redobles lentos. Sin melodía. |
| **BPM** | 50 |
| **Tono** | Tensión. Algo está mal aquí. |
| **Notas** | Los redobles deben ser irregulares — no un ritmo constante, sino golpes que llegan y se van sin patrón predecible. El jugador debe sentirse observado. |

---

### N1 — Velmar

La primera ciudad. Debe sentirse viva, pero empequeñecida por las murallas. La música refleja una comunidad que sobrevive, no que prospera.

#### Exploración (Calles)
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Campanas distantes (como las que suenan a la entrada de la ciudad), viento que pasa entre edificios, cuerdas que sostienen notas largas. |
| **BPM** | 65 |
| **Tono** | Cotidiano, con un trasfondo de tristeza contenida. La gente de Velmar ha perdido mucho. |
| **Duración de bucle** | 3-4 minutos. Suficiente para ambientar sin notarse. |

#### Mercado
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Gente murmurando (voces procesadas, no inteligibles). Pasos en madera. Campanas de tienda que tintinean al abrirse. Un laúd callejero que toca una melodía simple y ocasional. |
| **BPM** | Variable — no hay ritmo fijo. La vida del mercado marca el tempo. |
| **Tono** | Vitalidad frágil. La gente intenta seguir adelante. |
| **Notas** | El laúd callejero debe sonar solo algunas veces, no constantemente. Cuando deja de tocar, el vacío se nota. |

#### Entierro de Caín
| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — SILENCIO ABSOLUTO |
| **Sonido** | No hay música. Solo: pasos sobre tierra apisonada, una sola inhalación temblorosa (de alguien que contiene el llanto), tierra cayendo sobre madera. Nada más. |
| **Duración** | Desde que comienza la escena hasta que el jugador sale de la zona funeraria. |
| **Notas** | Este es uno de los momentos más importantes de sonido en todo el juego. El silencio aquí debe ser IMPECABLE — sin ruido de motor, sin zumbido eléctrico, sin ambiente de fondo. El jugador debe estar solo con el duelo. |

#### Combate — La Voz
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Percusión pesada + voces susurradas que se superponen. El agua del jefe (que ocupa el centro del arena) añade una capa de sonido líquido constante — chapoteo, gotas, oleaje suave. |
| **BPM** | 90 |
| **Tono** | Amenaza acuática. El jefe no es solo un enemigo — es una manifestación de algo más oscuro. |
| **Fases** | Fase 1: Percusión sola. Fase 2 (50% HP): Se añaden las voces susurradas. Las voces deben sonar como si vinieran de debajo del agua. Fase 3 (25% HP): El agua se agita violentamente. La percusión se acelera. |

---

### N2 — Pastor de Cenizas

Un mundo quemado. El silencio aquí es diferente al de Velmar — no es duelo, es desolación. No hay nada que llorar porque no queda nada.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Silencio + viento que arrastra ceniza. Crujido del suelo carbonizado bajo los pies del jugador. |
| **BPM** | 0-30 (el viento marca pulsos irregulares) |
| **Tono** | Desolación. No hay belleza aquí. Solo recuerdos de lo que fue. |
| **Duración de bucle** | Sin bucle — el ambiente es procedural según el viento. |
| **Notas** | La ceniza debe tener un sonido propio — no es nieve, no es arena. Es liviana, casi ingrávida. Cuando el viento la levanta, suena como papel quemado desintegrándose. |

#### Refugios Térmicos
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | Silencio, pero no el mismo silencio de afuera. Es un silencio de alivio. El fuego que calienta el refugio crepita suavemente. |
| **Tono** | Respiro. El único lugar seguro en un mundo que arde. |
| **Notas** | El contraste entre el exterior (viento de ceniza, silencio vacío) y el interior (crepitar del fuego, silencio cálido) debe ser dramático. El jugador debe sentir que está a salvo. |

#### Combate — Pastor de Cenizas
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Cencerros + percusión pesada que evoca un ritual funerario. Fuego crepitando constantemente como base rítmica. |
| **BPM** | 100 |
| **Tono** | Ritual de muerte. El Pastor no es un monstruo salvaje — es una ceremonia andante. |
| **Fases** | Fase 1: Cencerros + percusión + fuego. Fase 2 (60% HP): Se añaden cuerdas — disonantes, sin melodía clara. Fase 3 (30% HP): Se añade un coro bajo — voces sin palabras, como un canto gregoriano grabado al revés. El fuego se vuelve rugiente. |

---

### N3 — Aelthar

Las ruinas élficas. Aquí el sonido es más refinado que en cualquier otra zona. La civilización que construyó esto tenía oído para la belleza. Lo que queda es un eco de esa grandeza.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Agua goteando en piedra. Arpa élfica — notas claras y espaciadas. |
| **BPM** | 50 |
| **Tono** | Tranquilidad agridulce. Hay belleza, pero es una belleza que ya no pertenece a nadie. |
| **Duración de bucle** | 4-5 minutos. |
| **Notas** | Las gotas de agua deben ser parte del ritmo. No como metrónomo exacto — como un ritmo orgánico que el arpa a veces sigue y a veces ignora. |

#### Cerca de Hadda
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | La música ambiente se desvanece. El laúd de Hadda (su canción — ver Leitmotivs) reemplaza completamente el paisaje sonoro. |
| **Tono** | Calor humano en medio de la ruina. |
| **Notas** | Cuando el jugador se aleja de Hadda, la música ambiente regresa gradualmente, como emergiendo de un sueño. |

#### Combate — Jefe de Aelthar
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Cuerdas rápidas (violín en staccato) + percusión acuática (golpes que salpican). |
| **BPM** | 85 |
| **Tono** | Furia elegante. La batalla tiene gracia, no es brutal. |
| **Notas** | El agua aquí debe sonar diferente al agua de La Voz en Velmar. Allí era amenazante. Aquí es fluida, casi danzante. |

---

### N4 — El Corredor de los Nombres

El lugar más extraño del juego. Una galería infinita de estatuas de personas que el Tejido ha consumido. La ausencia total de sonido es el punto.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — SILENCIO ABSOLUTO |
| **Sonido** | No hay música. No hay ambiente. Solo los pasos del jugador sobre piedra pulida (que ecoan en la distancia) y su propia respiración. |
| **Tono** | Soledad. El jugador debe sentir que está completamente solo en el universo. |
| **Duración** | Todo el tiempo que dure la exploración del Corredor. |
| **Notas** | Esto no es un bug. No es un error de audio. Es diseño deliberado. El jugador debe preguntarse "¿se rompió el audio?" antes de darse cuenta de que no. El eco de los pasos es importante aquí — la habitación debe sonar enorme y vacía. La respiración del personaje debe amplificarse ligeramente para recordar al jugador que aún está vivo. |

#### Combate
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Percusión de piedra — golpes directos sobre roca. Sin melodía. Sin armonía. |
| **BPM** | 60 |
| **Tono** | Primitivo y frío. No hay arte aquí. Solo impacto. |
| **Notas** | Los golpes deben sonar secos y resonantes, como si el combate mismo estuviera rompiendo el silencio del Corredor. Cuando el combate termina, el silencio regresa inmediatamente — no hay fade-out, no hay resolución musical. |

---

### N5 — Thornwall

Una ciudad fortaleza que vive al borde del conflicto. El sonido aquí es más práctico — la gente trabaja, se prepara, sobrevive. No hay espacio para la poesía.

#### Exploración (Calles y Murallas)
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Viento silbando entre las almenas. Gente hablando (voces indistintas, ocupadas). Un herrero martillando metal a lo lejos. |
| **BPM** | 55 |
| **Tono** | Cotidiano beligerante. Thornwall no es acogedora — es funcional. |
| **Notas** | El martillo del herrero debe estar sincronizado de manera suelta con el ritmo de fondo. No nítido, sino orgánico — el sonido del trabajo real. |

#### Taberna
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Laúd desafinado tocado por alguien que ha bebido demasiado. Risas, vasos chocando, madera crujiendo. |
| **Tono** | Calor brusco. La única alegría que queda en Thornwall es la que se compra con monedas. |
| **Notas** | El laúd debe sonar claramente fuera de tono. No es música de calidad — es música de supervivencia. |

#### Alcantarillas
| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — sin música |
| **Sonido** | Agua sucia goteando y corriendo. Goteras. Ecos de tuberías. La respiración del jugador se escucha más fuerte aquí — el espacio es cerrado. |
| **Tono** | Repulsión. El jugador no debería querer estar aquí. |
| **Notas** | Contraste intencional con el Corredor de los Nombres: allí el silencio era solemne. Aquí el silencio es sucio. |

#### Combate — El Carnicero
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Percusión metálica — golpes que suenan a carne contra acero. Cuchillos, ganchos, cadenas. |
| **BPM** | 95 |
| **Tono** | Brutalidad mecánica. El Carnicero no pelea con arte — pelea con oficio. |
| **Notas** | Los sonidos deben ser viscerales. Golpes húmedos sobre carne, el rechinido de metal contra hueso, cadenas que se tensan y se aflojan. No hay melodía — solo ruido industrial y carnicero. |

---

### N6 — Cámaras de Jade

Una prisión viviente que respira. El sonido aquí es orgánico e incómodo — las paredes no están muertas.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Zumbido de cristales vibrando. Nota base: Si sostenido. El tono cambia según la sala — algunas cámaras son más agudas, otras más graves. |
| **BPM** | 30 (pulsos lentos del zumbido, como un latido) |
| **Tono** | Inquietud orgánica. La prisión está viva y sabe que estás aquí. |
| **Notas** | El zumbido debe tener micro-variaciones. No debe sonar como un tono de prueba — debe tener textura, como si el cristal respirara. En ciertas salas, el zumbido cambia de tonalidad para indicar peligro cercano o secretos ocultos. |

#### Alucinaciones
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | El zumbido base se distorsiona — se vuelve áspero, se quiebra. Se oyen voces que no existen: susurros que llaman al jugador por su nombre, fragmentos de conversaciones de otros prisioneros. |
| **Tono** | Paranoia. ¿Qué es real y qué no? |
| **Notas** | Las voces deben ser imposibles de localizar en el espacio de audio 3D. Vienen de todas partes y de ninguna. El jugador debe sentirse desorientado. |

#### Combate — El Coro
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | El zumbido de cristal se acelera progresivamente hasta convertirse en percusión. Ya no es ambiente — es ritmo. |
| **BPM** | 110 (aceleración progresiva desde 80 hasta 110) |
| **Tono** | Ataque de pánico sonoro. El Coro no es un enemigo que se pelea — es un enemigo que se sobrevive. |
| **Fases** | Fase 1: Zumbido acelerado (80 BPM). Fase 2: El zumbido se rompe en golpes percusivos (95 BPM). Fase 3: Percusión de cristal quebrado (110 BPM). El alarido del Coro debe sonar como vidrio rallado. |

---

### N7 — Sahal

La ciudad del desierto. El sonido aquí está dominado por el viento, la arena y el comercio. Es seco, cálido y eterno.

#### Exploración (Calles)
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Viento del desierto (cálido, no frío). Arena moviéndose — granos que chocan contra piedra. Campanas de viento de metal. |
| **BPM** | 45 |
| **Tono** | Paciencia antigua. Sahal ha estado aquí por siglos y estará aquí por siglos más. |
| **Duración de bucle** | 4-5 minutos. |
| **Notas** | Las campanas de viento deben afinarse a una escala pentatónica — evoca oriente sin ser literal. |

#### Bazar
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | Gente regateando, animales (camellos, pájaros), especias (sonido de telas al moverse, monedas cayendo, sacos de granos abriéndose). |
| **Tono** | Vida bulliciosa. Sahal es el lugar más vivo del juego. |
| **Notas** | El Bazar debe ser el lugar más ruidoso del juego — no en volumen, sino en densidad. Hay demasiadas cosas sonando a la vez. El jugador debe sentirse abrumado por la vida, no por el silencio. |

#### Templo (Antes de la Trampa)
| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — silencio |
| **Sonido** | Solo pasos en piedra. El eco de una sala grande y vacía. |
| **Tono** | Reverencia. El jugador está en un lugar sagrado. |
| **Notas** | El silencio aquí es diferente al del Corredor. Allí era soledad. Aquí es presencia — como si alguien o algo estuviera observando desde el silencio. El eco debe ser notable, como en una catedral vacía. |

#### Combate — La Costurera (Fase 1)
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Hilos tensándose — un sonido agudo, casi doloroso, como una cuerda de guitarra que se estira al límite. Cuerdas distorsionadas (violín procesado con efectos). |
| **BPM** | 80 |
| **Tono** | Marioneta. Algo está controlando a los enemigos, y los hilos se escuchan. |
| **Notas** | El sonido de los hilos debe ser físicamente incómodo. Debe hacer que el jugador quiera taparse los oídos. Cada vez que la Costurera controla a un enemigo, se oye un chasquido de hilo tensándose. |

#### Combate — La Costurera (Fase 2 — Mira/Vassen controladas)

Este es uno de los momentos más importantes de sonido en todo el juego.

| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — La música se detiene por completo |
| **Sonido** | Solo los sonidos de combate de Mira y Vassen (sus ataques, sus pasos, sus voces distorsionadas). El jugador debe oír a sus compañeros convertidos en enemigos. |
| **Tono** | Traición forzada. El jugador no debería querer pelear esta batalla. |
| **Duración** | Toda la duración de la Fase 2. |
| **Notas** | Cuando los hilos se tensan alrededor de Mira o Vassen, se oye un sonido de tela rasgándose. Cada golpe que el jugador les da debe sonar más fuerte de lo normal — como si el juego quisiera que el jugador NO quiera golpearlos. |

#### Post-muerte de la Costurera
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | Silencio total durante 30 segundos después de que la Costurera cae. Luego, un solo violín comienza a tocar un lamento — una melodía simple que se desvanece lentamente. |
| **Tono** | Alivio amargo. La Costurera ha muerto, pero Mira o Vassen también. |
| **Notas** | El lamento del violín debe ser la misma melodía que se oyó fragmentada durante la pelea, ahora completa y clara. Es el tema de la Costurera, pero liberado de la distorsión. |

---

### N8 — Jardines Flotantes

Un lugar que no debería existir. La belleza aquí es casi insoportable porque es efímera. El sonido debe reflejar eso.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Viento suave y constante. Flores que tintinean como campanas de cristal al moverse con la brisa. |
| **BPM** | 50 |
| **Tono** | Belleza frágil. Los Jardines son hermosos porque no durarán. |
| **Duración de bucle** | 5-6 minutos. |
| **Notas** | Cada flor debe tener un tono ligeramente diferente, como un carillón natural. El viento debe "tocar" los Jardines como un instrumento — a veces más fuerte, a veces más suave. |

#### Cerca de Sera
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | El zumbido del Tejido (baja frecuencia, casi imperceptible en volumen normal) se mezcla con el tintineo de las flores. Es sutil — el jugador no debería notarlo conscientemente al principio. |
| **Tono** | Misterio. Sera está conectada al Tejido de una manera que el jugador aún no comprende. |
| **Notas** | A medida que el jugador se acerca a Sera, el zumbido del Tejido se vuelve más audible. Cuando habla con ella, el zumbido se desvanece — como si ella lo estuviera conteniendo. |

#### Combate — Arpía de los Jardines
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Viento que se vuelve violento + percusión aérea (golpes que suenan como alas golpeando el aire). |
| **BPM** | 90 |
| **Tono** | Tormenta. La belleza de los Jardines se rompe en furia. |
| **Notas** | Durante el combate, el tintineo de las flores debe distorsionarse — las flores no están diseñadas para la violencia, y su sonido sufre. Algunas flores se rompen durante la pelea, y su tintineo se convierte en un crujido. |

---

### N9 — Argentia

La ciudad de plata. El último bastión de la civilización antes del enfrentamiento final. Todo aquí suena a metal — puro, frío, resonante.

#### Exploración
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Plata resonando — notas altas y claras que ecoan en calles de piedra y metal. Pasos que ecoan más de lo que deberían. |
| **BPM** | 45 |
| **Tono** | Determinación fría. Argentia no es cálida, pero es firme. |
| **Duración de bucle** | 4 minutos. |
| **Notas** | Cada superficie en Argentia debe sonar a metal. Los pasos del jugador tienen un leve eco metálico. Las puertas suenan como portones de plata. La ciudad entera es un instrumento de percusión. |

#### Preparación (Antes de la Batalla Final)
| Aspecto | Descripción |
|---------|-------------|
| **Sonido** | Herrero martillando con urgencia (más rápido que en Thornwall — aquí hay prisa). Gente seria que habla en voz baja. Pocos diálogos. El sonido del acero siendo templado. |
| **Tono** | Tensión contenida. Todos saben lo que se acerca. |
| **Notas** | El martilleo debe sonar más insistente que en Thornwall. Allí era trabajo diario. Aquí es trabajo de guerra. |

#### Combate — El Verdugo
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Metal contra metal — espadas, armaduras, escudos. Percusión militar (tambores de marcha). |
| **BPM** | 85 |
| **Tono** | Ejecución. El Verdugo no es un guerrero — es un castigo con piernas. |
| **Fases** | Fase 1: Tambores + metal. Fase 2 (50% HP): Se añade un cuerno de caza grave. Fase 3 (25% HP): Los metales se vuelven disonantes — el Verdugo está perdiendo el control. |

---

### N10 — El Centinela

El final del juego. Aquí la música cuenta la historia del enfrentamiento más importante de SOLO. No hay concesiones.

#### Antes de la Pelea
| Aspecto | Descripción |
|---------|-------------|
| **BPM** | 0 — SILENCIO ABSOLUTO |
| **Sonido** | Solo la respiración del personaje. El jugador debe sentir el peso del momento. No hay viento, no hay ambiente, no hay distancia. Solo la respiración y el silencio. |
| **Duración** | El tiempo que el jugador pase en la antesala. |
| **Notas** | Este es el silencio más importante del juego. La respiración debe ser audible pero no exagerada — el personaje está tranquilo porque ha aceptado lo que tiene que hacer. |

#### Centinela — Fase 1
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Percusión de roca — golpes sordos, profundos, que retumban en el pecho del jugador. |
| **BPM** | 60 |
| **Tono** | Imparable. El Centinela no se detendrá. |
| **Notas** | Los golpes deben sentirse físicos. Bajas frecuencias que hacen vibrar el subwoofer. Cada golpe del Centinela debe ser un impacto que el jugador siente en el cuerpo. |

#### Centinela — Fase 2 (Núcleo Expuesto)
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Zumbido del Tejido (baja frecuencia, casi imperceptible). La percusión se vuelve más espaciada. El núcleo del Centinela está expuesto y emite un sonido que no es de este mundo. |
| **BPM** | 40-50 (irregular, el zumbido pulsa sin ritmo fijo) |
| **Tono** | Vulnerabilidad. El Centinela ya no es invencible. |
| **Notas** | El zumbido debe mezclarse con latidos — como un corazón que late lentamente. El jugador debe preguntarse si el latido es del Centinela o suyo. |

#### El Arquitecto — Fase 3
| Aspecto | Descripción |
|---------|-------------|
| **Instrumentación** | Cuerdas minimalistas — una sola nota sostenida que cambia gradualmente de tonalidad a lo largo de la pelea. No es una melodía. Es una textura armónica que evoluciona. |
| **BPM** | 40 |
| **Tono** | Trascendencia. Esto no es una pelea de fuerza — es un baile. El Arquitecto no quiere matar al jugador. Quiere reemplazarlo. |
| **Notas** | Esto debe sentirse como una pelea de baile, no de fuerza. La nota sostenida de las cuerdas debe ser incómodamente larga — a veces el jugador se preguntará si el sonido ha cambiado o si es su percepción la que ha cambiado. El cambio de tonalidad debe ser casi imperceptible, como el lento avance de las manecillas de un reloj. |

---

## 3. Leitmotivs por Personaje

Cada personaje importante tiene su propia identidad musical, que aparece en momentos específicos. Estos temas no se tocan constantemente — aparecen en los momentos adecuados para recordar al jugador quién está en juego.

### Caín — Melodía Alegre de Flauta
- **Instrumento base:** Flauta de madera (dulce, cálida)
- **Aparece en:** N0 (camino a la Torre, momentos tranquilos), visiones de Hadda, recuerdos del pasado.
- **Después de su muerte:** Solo fragmentos distorsionados de la melodía original — como un recuerdo que se rompe. La flauta se sustituye por una versión procesada, con notas faltantes y pausas incómodas.
- **Significado:** Inocencia perdida. Caín era la esperanza del mundo. Su melodía es el sonido de lo que SOLO podría haber sido.
- **Uso narrativo:** Cada vez que el jugador escucha la melodía de Caín después de su muerte, es un recordatorio de por qué sigue adelante.

### Mira — Arco de Violín
- **Instrumento base:** Arco de violín (tenso, preciso, con ataques rápidos)
- **Aparece en:** Cuando Mira dispara flechas importantes, en momentos de peligro donde demuestra su precisión.
- **Significado:** Determinación afilada. El sonido de una voluntad que no se quiebra.
- **Notas:** No es una melodía completa — son frases cortas de 2-3 segundos que acompañan sus acciones más impactantes. Como el silbido de una flecha antes del impacto.

### Vassen — Percusión de Escudo
- **Instrumento base:** Golpes rítmicos sobre escudo de metal (sordos, protectores)
- **Aparece en:** Momentos donde Vassen protege al grupo, bloquea un atque importante o se planta frente al peligro.
- **Significado:** Protección. Su tema es el ritmo de alguien que se interpone entre el peligro y quienes ama.
- **Notas:** Tres golpes — pausa — tres golpes. Un ritmo militar, casi de marcha, pero más lento. Es el latido de alguien que no se rinde.

### Garrik — Laúd Rápido
- **Instrumento base:** Laúd (ágil, inquieto, con notas rápidas y saltarinas)
- **Aparece en:** Aelthar cuando el jugador se reencuentra con Garrik. También en escenas de sigilo o planes rápidos.
- **Significado:** Astucia. Garrik no pelea de frente — encuentra el camino más corto y lo toma.
- **Notas:** La melodía debe ser casi juguetona, con cambios de dirección impredecibles. Garrik es un pícaro, y su música debe reflejar eso.

### Sera — Voz Femenina (Canto sin Palabras)
- **Instrumento base:** Voz humana, etérea, procesada con reverberación. Sin letra — solo vocales sostenidas.
- **Aparece:** La primera vez en N8 (Jardines Flotantes). Luego en momentos de conexión con el Tejido.
- **Significado:** Misterio. Sera no es de este mundo, y su voz suena como si viniera de otro lugar. Es bella pero incómoda.
- **Notas:** El canto debe sonar a algo que el jugador reconoce como "humano" pero que está "mal" — como si la voz estuviera llegando desde muy lejos. El tratamiento de audio debe darle una calidad fantasmal.

### Hadda — Laúd de Hadda (La Canción de 7 Estrofas)
- **Instrumento base:** Laúd (cálido, humano, a veces con errores deliberados)
- **Aparece:** Solo en Aelthar, cuando el jugador está cerca de Hadda.
- **La Canción:** 7 estrofas que cuentan la historia de su pueblo. Cada estrofa tiene una melodía ligeramente diferente. La canción completa dura aproximadamente 3-4 minutos.
- **Significado:** Memoria. Hadda canta porque tiene miedo de olvidar. Su canción es un acto de resistencia contra el olvido.
- **Notas:** El laúd de Hadda debe sonar como el de alguien que aprendió a tocar por necesidad, no por entrenamiento. Hay pequeñas pausas donde duda, notas que no salen perfectas. Es humana. Cuando el jugador se aleja de Hadda, la canción se desvanece — no en fade-out, sino como si el viento se llevara el sonido.

### El Arquitecto — Cuerdas Electrónicas Distorsionadas
- **Instrumento base:** Cuerdas procesadas electrónicamente — no suena a medieval. Suena a OTRO MUNDO.
- **Aparece:** N10 (Fase 3 del Centinela), y en cualquier visión o comunicación del Arquitecto.
- **Significado:** Alienígena. El Arquitecto no pertenece a este mundo. Su sonido no debería existir en un mundo de flautas y laúdes. Debe ser incómodo.
- **Notas:** La textura debe ser evolutiva — un dron de cuerdas que cambia lentamente, pero con armónicos metálicos que no deberían estar ahí. Como si el sonido mismo estuviera corrupto.

### El Tejido — Zumbido de Baja Frecuencia
- **Instrumento base:** Síntesis de baja frecuencia, subconsciente.
- **Aparece:** Solo cuando se usa magia importante (habilidades del Tejido) o en lugares de poder (Árbol del Tejido, ciertas zonas de N6, N8 cerca de Sera, N10).
- **Significado:** El Tejido no es magia — es la realidad misma vibrando. El zumbido es el sonido del universo respirando.
- **Notas:** El zumbido debe estar en el límite de lo audible. El jugador no debería notarlo conscientmente al principio, pero debería sentir que algo está "diferente" en el aire. Con el tiempo, aprende a reconocerlo. Esta es la conexión más directa entre el diseño de sonido y la narrativa.

---

## 4. Efectos de Sonido Clave

### Pasos (10+ Tipos)
Cada tipo de superficie debe tener su propio sonido de paso, con variaciones para evitar repetición:
- **Tierra:** Paso seco, compacto. Baja frecuencia.
- **Piedra:** Paso firme, con eco (o sin él según la sala). Frecuencia media.
- **Madera:** Crujido leve, hueco. Tablones que se quejan.
- **Ceniza:** Paso blando, como pisar polvo muy fino. Un susurro apenas audible.
- **Arena:** Paso que se hunde ligeramente. Granos deslizándose.
- **Agua:** Chapoteo (profundidad variable — charco vs río).
- **Metal:** Resonancia metálica. El paso suena a que hay metal bajo el pie.
- **Nieve:** Crujido comprimido (no incluido en el juego base, pero listo para futuras zonas).
- **Hoja seca:** Crujido ligero, quebradizo.
- **Barro:** Paso que succiona, húmedo.

### Armas — Sonidos de Golpe por Material
Cada tipo de daño debe tener su propio sonido de impacto:
- **Carne:** Golpe húmedo, sordo o cortante según el arma (espada vs maza).
- **Armadura:** Metal resonando. El golpe suena a que algo se abolla.
- **Escudo:** Golpe seco y amortiguado. Como madera gruesa o metal recubierto.
- **Piedra:** Impacto duro, quebradizo. Esquirlas.
- **Hueso:** Crujido seco. Incómodo.

### Magia — Sonidos de Lanzamiento por Escuela
- **Fuego:** Chispa que crece hasta rugido. Encender un fósforo que se convierte en llama.
- **Hielo:** Crujido de congelación rápida. Agua que se solidifica en un instante.
- **Rayo:** Estática. Electricidad crepitando. Zumbido agudo antes del impacto.
- **Oscuridad:** Susurro. Como si el sonido mismo fuera absorbido.
- **Naturaleza:** Hojas moviéndose, madera creciendo, tierra removiéndose.
- **Voz:** Una sola nota musical sostenida, que vibra en el aire. Distinta a todos los demás — la Voz es una anomalía incluso dentro de la magia.

### Tejido — Sonido Característico
- **Zumbido base:** Baja frecuencia, casi imperceptible.
- **"Clic" de activación:** Un sonido corto, como un engranaje que encaja, cuando una habilidad del Tejido se activa. Este clic es la clave sonora — el jugador aprende a reconocerlo como señal de que algo importante está pasando.
- **Desactivación:** El zumbido se desvanece lentamente cuando la habilidad termina.

### Interfaz (UI) — Sonidos Mínimos
- **Clic de menú:** Un solo tono corto y suave. Madera o papel.
- **Notificación de quest:** Dos notas ascendentes (intervalo de quinta justa). Satisfactorio sin ser intrusivo.
- **Desbloqueo de habilidad:** Una tríada mayor que se construye nota por nota. Pequeña celebración.
- **Level up:** Acorde completo que resuena y se desvanece. Logro.
- **Error/Denegado:** Un solo tono grave, corto. Como una puerta que no se abre.
- **Diálogo importante:** Un sutil destello de campana para indicar que una línea de diálogo tiene peso narrativo.
- **Salvado automático:** Chasquido de tintero + pluma sobre pergamino.

### Ambiente — Variaciones
- **Viento (5 variaciones):**
  1. Viento suave (pradera, día claro) — susurro constante y bajo.
  2. Viento fuerte (tormenta, montaña) — silbido agudo y ráfagas.
  3. Viento entre edificios (ciudad) — que silba al pasar por callejones.
  4. Viento de ceniza (N2) — seco, áspero, arrastra partículas.
  5. Viento del desierto (N7) — cálido, cargado de arena, más grave.

- **Agua (5 variaciones):**
  1. Goteo — lento, rítmico, aleatorio.
  2. Río — corriente constante, burbujeo.
  3. Lago — agua quieta, oleaje mínimo contra la orilla.
  4. Lluvia — desde llovizna hasta tormenta.
  5. Oleaje — mar, olas rompiendo.

- **Fuego (2 variaciones contrastantes):**
  1. Fogata — crepitar suave, acogedor. Chispas ocasionales. Madera que se quema lentamente.
  2. Incendio — rugido constante, madera crujiendo violentamente, chispas que explotan. Amenazante.

---

## 5. Silencios Importantes (Momentos sin Música)

Los silencios en SOLO están tan diseñados como la música. Cada uno tiene un propósito narrativo y emocional específico. No son accidentes — son pistas de audio VACÍAS (ver sección técnica).

### 1. Muerte de Caín
- **Duración:** 0 BPM durante 2 minutos después del evento.
- **Sonido permitido:** Solo sonidos ambientales MÍNIMOS — el viento, un pájaro lejano. Nada más.
- **Propósito:** El jugador acaba de perder a un compañero. La música no debería estar ahí para "ayudar" a procesar el duelo. El jugador debe sentarse en el silencio y procesarlo solo.
- **Transición:** La música se corta en seco en el momento del impacto. No hay fade-out que suavice el golpe.

### 2. Entierro de Caín
- **Duración:** Toda la escena del entierro.
- **Sonido permitido:** Tierra cayendo sobre madera. Pasos al retirarse. Una respiración temblorosa.
- **Propósito:** El entierro es el momento más íntimo del juego para el protagonista. No hay público. No hay jugador que mire desde afuera. Solo el duelo.
- **Nota:** Este silencio debe ser impecable. Cualquier zumbido de fondo técnico arruina la escena.

### 3. Muerte de Mira/Vassen
- **Duración:** 0 BPM durante 30 segundos después del evento.
- **Sonido permitido:** Nada. Silencio absoluto.
- **Transición:** A los 30 segundos exactos, un violín solo comienza a tocar un lamento — la misma melodía que se escuchó durante la pelea, ahora pura y sin distorsión.
- **Propósito:** El contraste entre el silencio y la aparición del violín debe ser físico. El jugador debe sentir el alivio de que la música vuelva, pero también la tristeza de por qué se fue.

### 4. N4 — El Corredor de los Nombres
- **Duración:** Toda la exploración del Corredor.
- **Sonido permitido:** Solo pasos del jugador sobre piedra pulida y su propia respiración. El eco de una sala vacía.
- **Propósito:** El Corredor es un lugar de contemplación. Las estatuas de los consumidos por el Tejido no merecen banda sonora. Merecen silencio.
- **Nota:** El jugador debe preguntarse si el audio se rompió. Esa confusión inicial es intencional.

### 5. N10 — Antes del Arquitecto
- **Duración:** El tiempo que el jugador pase en la antesala antes de la pelea final.
- **Sonido permitido:** Solo la respiración del personaje.
- **Propósito:** El jugador debe sentir el peso del momento. Todo el juego ha llevado a esto. El silencio no es vacío — es expectativa.

### 6. Final A — El Escape
- **Sonido:** La música vuelve GRADUALMENTE mientras el grupo corre hacia la luz. No es una melodía completa al principio — son fragmentos que se van uniendo.
- **Transición:** Cuando salen al amanecer: solo viento + un solo violín. La misma nota del violín de N0 (Emberveil día), pero ahora transformada. Ya no es esperanza inocente — es esperanza ganada con sufrimiento.
- **Propósito:** El Final A es agridulce. La música refleja que el mundo se ha salvado, pero no sin costo.

### 7. Final B — El Nuevo Arquitecto
- **Duración:** El zumbido del Tejido baja gradualmente hasta silencio.
- **Sonido final:** Una sola nota de laúd — la nota final de la canción de Hadda.
- **Propósito:** El Final B es más oscuro. El jugador elige reemplazar al Arquitecto. El zumbido del Tejido se desvanece porque ahora el control es del jugador. La nota final de Hadda es un recordatorio de lo que se perdió para llegar aquí.

---

## 6. Consideraciones Técnicas (para Claude Code)

### Motor de Audio
- **Opción recomendada:** AkAudio (Wwise) o MetaSounds de Unreal Engine 5.
- **Wwise Advantage:** Ecosistema maduro, amplia documentación, herramientas de profiling, generación de SoundBanks optimizados.
- **MetaSounds Advantage:** Integración nativa con UE5, sistema procedural, sin dependencias externas.

### Sistema de Zonas de Audio (Audio Volumes)
- Cada zona del juego debe tener su propio Audio Volume en UE5.
- Los volúmenes deben superponerse con transiciones suaves (0.5-1 segundo de crossfade).
- Excepciones: Los cortes secos (muerte de Caín, cambios de fase en jefes) deben usar transiciones instantáneas.
- Implementar "Reverb Zones" que cambien la reverberación según el material de la sala (piedra, madera, metal, exterior, cueva).

### Música Dinámica por Capas
- Cada zona debe tener su música dividida en capas que se activan/desactivan según el estado del juego:
  - **Capa Base:** Ambiente de la zona (viento, agua, textura). Siempre activa.
  - **Capa Exploración:** Melodía principal de la zona.
  - **Capa Tensión:** Se activa cuando hay enemigos cerca pero no en combate.
  - **Capa Combate:** Se activa al entrar en combate. Reemplaza o se superpone a la capa de exploración.
  - **Capa Jefe:** Única por jefe. Reemplaza completamente las capas anteriores.
  - **Capa Resolución:** Se activa cuando el jefe cae o el evento termina. Transición de vuelta a exploración.
- Las transiciones entre capas deben ser suaves (1-2 segundos) excepto donde se indique lo contrario (cortes secos).

### Leitmotivs Modulares
- Cada leitmotiv debe componerse como una pieza MODULAR — capaz de tocarse en diferentes instrumentos según el contexto.
- Ejemplo: El tema de Caín se toca con flauta en N0, pero puede aparecer como cello en momentos tristes, o como campana distorsionada en pesadillas.
- Los módulos deben ser intercambiables: la melodía, la armonía y el ritmo deben poder separarse y recombinarse.
- Esto permite que el motor de audio "reorqueste" un tema según las condiciones del juego sin necesidad de pistas premezcladas para cada situación.

### Sistema de Stingers
- Los "stingers" son golpes musicales cortos que marcan momentos clave.
- **Muerte de jefe:** Acorde de resolución (generalmente mayor, con sustain).
- **Desbloqueo de habilidad:** Secuencia ascendente de 3 notas.
- **Revelación importante:** Golpe orquestal seco (todos los instrumentos al unísono).
- **Muerte de compañero:** Silencio + una sola nota grave que se desvanece.
- **Quest completado:** Dos notas, intervalo de quinta justa, como un "todo está bien".
- Los stingers no deben usarse en exceso. Máximo 1-2 por hora de juego en promedio.

### Implementación de Silencios
- **CRÍTICO:** Los silencios deben ser pistas de audio VACÍAS — no silencio técnico del motor.
- Esto significa crear archivos de audio de la duración necesaria que contengan SILENCIO ABSOLUTO (0 dB, sin ruido de piso).
- Razón: Si el motor de audio reproduce "silencio técnico" (canal abierto sin datos), puede introducir ruido de fondo, zumbido eléctrico, o artefactos de compresión.
- Una pista de silencio real elimina este problema porque el motor procesa datos (aunque sean ceros) en lugar de estar "en espera".
- El silencio de la pista vacía debe tener la misma ganancia y procesamiento que las pistas de música, para que la transición sea perfecta.

### Mezcla y Masterización
- **Rango dinámico:** Amplio. Los silencios deben ser SILENCIOS (-60 dB o menos), los golpes de jefe pueden llegar a -3 dB.
- **Frecuencias:** Priorizar medios y bajos. Evitar frecuencias agudas excesivas que causen fatiga auditiva.
- **Panorama:** Usar paneo 3D para ambiente (viento que rodea al jugador, agua que viene de una dirección específica).
- **Reverberación:** Diferente por zona (Ver tabla de zonas). El Corredor de los Nombres debe tener la reverberación más larga del juego.
- **Compresión:** Mínima. No comprimir el rango dinámico — los silencios deben ser silencios y los golpes deben doler.

### Optimización
- **Streaming:** Las pistas de ambiente largas (3-5 minutos) deben transmitirse en streaming desde disco, no cargarse en RAM.
- **Compresión:** Usar Ogg Vorbis (calidad 5-7) para música, WAV sin comprimir para efectos cortos (golpes, pasos, UI).
- **Memoria:** Los bancos de sonido de cada nivel deben cargarse en streaming según la zona. No cargar todo el audio del juego al inicio.
- **LOD de Audio:** Los sonidos lejanos deben tener versiones de menor calidad que se reproducen a distancia.

### Checklist de Implementación

- [ ] Crear directorio de assets de audio por zona (Assets/Audio/N0/, Assets/Audio/N1/, etc.)
- [ ] Configurar Wwise/MetaSounds como motor de audio
- [ ] Implementar Audio Volumes para cada zona
- [ ] Componer pistas base de ambiente para cada zona (ver sección 2)
- [ ] Componer temas de jefe para cada encuentro (ver secciones de combate)
- [ ] Componer leitmotivs de personajes (ver sección 3)
- [ ] Grabar y procesar efectos de sonido de pasos (10+ tipos, ver sección 4)
- [ ] Grabar y procesar efectos de sonido de armas/magia (ver sección 4)
- [ ] Implementar sistema de capas de música dinámica
- [ ] Implementar sistema de stingers
- [ ] Crear pistas de silencio vacías para momentos clave (ver sección 5)
- [ ] Implementar transiciones de zona (crossfade vs corte seco)
- [ ] Configurar reverberación por sala
- [ ] Prueba de integración: verificar que no haya zumbidos de fondo en los silencios
- [ ] Prueba de mezcla: verificar rango dinámico en auriculares y altavoces
- [ ] Prueba emocional: ¿los silencios duelen? ¿la música importa?

---

## Apéndice A — Mapa de Audio por Zona (Resumen Rápido)

| Zona | BPM Expl. | Instrumento Principal | Tono Emocional | ¿Tiene Jefe? | BPM Jefe |
|------|-----------|----------------------|----------------|-------------|----------|
| N0 Día | 60 | Violín solo | Esperanza tranquila | — | — |
| N0 Noche | 40 | Viola/Cello | Melancolía | — | — |
| N1 Velmar | 65 | Campanas + Cuerdas | Tristeza cotidiana | Sí (La Voz) | 90 |
| N2 Cenizas | 0-30 | Viento + Ceniza | Desolación | Sí (Pastor) | 100 |
| N3 Aelthar | 50 | Arpa élfica | Tranquilidad agridulce | Sí | 85 |
| N4 Corredor | 0 | SILENCIO | Soledad | Normal + Silent | 60 |
| N5 Thornwall | 55 | Viento + Herrero | Beligerancia | Sí (Carnicero) | 95 |
| N6 Jade | 30 | Cristales (Si♭) | Inquietud orgánica | Sí (Coro) | 110 |
| N7 Sahal | 45 | Viento desierto | Paciencia antigua | Sí (Costurera) | 80→0 |
| N8 Jardines | 50 | Flores de cristal | Belleza frágil | Sí (Arpía) | 90 |
| N9 Argentia | 45 | Plata resonante | Determinación fría | Sí (Verdugo) | 85 |
| N10 Centinela | 0→60→40 | Silencio→Piedra→Tejido | Trascendencia | Sí (3 fases) | 60→40→40 |

---

## Apéndice B — Banco de Referencias Sonoras (para el Compositor)

Estas referencias no son para copiar — son para establecer el rango emocional que SOLO necesita alcanzar.

- **Tono general (Emberveil):** "The Shire" de Howard Shore (LoTR), pero más triste y con menos instrumentos.
- **Melancolía nocturna:** Max Richter — "On the Nature of Daylight" (la textura, no la melodía).
- **Desolación (N2):** Colin Stetson — "The First Fire" (saxofón como textura de duelo).
- **Tensión (N0 Garganta, N6):** Hildur Guðnadóttir — banda sonora de Chernobyl (zumbidos industriales).
- **Combate (Carnicero):** Mick Gordon — DOOM (no la velocidad, sino la textura del metal).
- **Élfico (N3):** Two Steps From Hell — "Star Sky" (versión reducida, arpa sola).
- **Silencio + tensión (N4):** Ben Salisbury & Geoff Barrow — "The Alien" (Annihilation).
- **Trascendencia (N10):** Jóhann Jóhannsson — "The Theory of Everything" (minimalismo que crece).

---

*Fin del documento — Sistema de Sonido y Música de SOLO v1.0*