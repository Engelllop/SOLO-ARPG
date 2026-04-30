# Sistema de Clima y Ciclo Día/Noche — SOLO

> *"La Torre no olvida el sol. Pero el mundo de afuera sí."*
> — Anónimo, habitante de Emberveil

---

## 1. Ciclo Día/Noche

SOLO cuenta con un ciclo día/noche persistente que afecta al overworld (N0) y a algunos pisos de la Torre. El ciclo es global: todos los jugadores en una misma instancia de mundo comparten el mismo tiempo.

### 1.1 Duración del Ciclo

| Fase | Duración (tiempo real) | % del Ciclo |
|------|----------------------|-------------|
| **Día** | 30 minutos | 68.2% |
| **Atardecer** | 2 minutos | 4.5% |
| **Noche** | 10 minutos | 22.7% |
| **Amanecer** | 2 minutos | 4.5% |
| **Total** | **44 minutos** | **100%** |

### 1.2 Transiciones

#### Atardecer (Día → Noche)
- **Minuto 0:** El sol comienza a descender. La luz se vuelve naranja/cobriza.
- **Minuto 1:** El sol toca el horizonte. Las sombras se alargan. Primeras estrellas visibles.
- **Minuto 2:** Oscuridad total. El ciclo nocturno comienza.

#### Amanecer (Noche → Día)
- **Minuto 0:** Primer resplandor en el horizonte. Las estrellas se desvanecen.
- **Minuto 1:** El sol asoma. La luz cambia de azul profundo a dorado suave. Los enemigos nocturnos comienzan a retirarse.
- **Minuto 2:** Día completo. Todos los efectos nocturnos cesan.

> **Nota de diseño:** Las transiciones no solo son visuales. Durante el atardecer y amanecer, algunos enemigos están en *estado de transición*: no son agresivos de inmediato (están buscando refugio o despertando), lo que permite al jugador pasar de un ciclo a otro sin ser emboscado instantáneamente.

### 1.3 Dónde Aplica el Ciclo

| Zona | Ciclo Día/Noche | Notas |
|------|----------------|-------|
| **Overworld (N0)** | ✅ Sí | Ciclo completo de 44 min |
| **Emberveil** | ✅ Sí | La ciudad tiene faroles que se encienden al atardecer |
| **N1 Velmar** | ❌ No | Siempre día artificial. Luz emitida por cristales del techo |
| **N2 Pastor de Cenizas** | ❌ No | Sin sol. Atmósfera gris perpetua |
| **N3 Aelthar** | ❌ No | Cielo de cueva, bioluminiscencia constante |
| **N4 Bosque Petrificado** | ❌ No | Sin tiempo, sin luz natural, solo silencio |
| **N5 Thornwall** | ✅ Sí (modificado) | Ciclo 20 min día / 10 min noche |
| **N6 Cámaras de Jade** | ❌ No | Zumbido cíclico de cristales cada 15 min |
| **N7 Sahal** | ✅ Sí (extendido) | Ciclo 40 min día / 15 min noche |
| **N8 Jardines Flotantes** | ❌ No | Luz perpetua de flores bioluminiscentes |
| **N9 Argentia** | ✅ Sí | Ciclo normal (30/10). Nieve ocasional |
| **N10** | ❌ No | Vacío. No hay tiempo ni clima |

---

## 2. Efectos del Día

Durante el día, el mundo de SOLO es más seguro y acogedor. Los NPCs están activos, los caminos son transitables y la visibilidad es completa.

### 2.1 Efectos de Combate

| Efecto | Valor | Explicación |
|--------|-------|-------------|
| Visibilidad | **Máxima** (100m en exteriores) | Sin restricción de rango de detección |
| Detección de enemigos bestia | **-10%** probabilidad de detectarte | Las bestias tienen más dificultad para percibir al jugador bajo luz directa |
| Probabilidad de crítico (exteriores) | **+5%** | La luz solar da confianza y precisión |
| Efectividad de pociones curativas | **+10%** | La luz natural potencia los ingredientes herbales |
| Daño sagrado | **Estándar** | Sin bonificación ni penalización |
| Daño oscuro | **Estándar** | Sin bonificación ni penalización |

### 2.2 NPCs y Misiones Exclusivos del Día

| NPC | Ubicación | Propósito |
|-----|-----------|-----------|
| **Herrero Olaf** | Emberveil, forja central | Solo vende armas de rango D y superiores durante el día |
| **Maestra de Hierbas Elara** | Camino de los Prados (N0) | Enseña recetas de pociones. Desaparece al atardecer |
| **Correo Real** | Emberveil, plaza | Reparte misiones secundarias. Solo de día |
| **Niño de las Flores** | Puente de Emberveil | Quest oculta: encuentra 5 flores raras. Solo interactuable de día |
| **Capitán de la Guardia** | Emberveil, cuartel | Da misiones de exterminio de bestias. No disponible de noche |

### 2.3 Misiones Solo de Día

- **"Luz en los Prados"** (Rango E): Eliminar bestias en los Prados de Emberveil. Los enemigos tienen -10% detección.
- **"El Correo Perdido"** (Rango E): Recuperar una carta real de un mensajero caído. El cadáver solo es visible de día.
- **"Hierbas del Mediodía"** (Rango D): Recolectar 10 Hierba Solar. Solo crece bajo el sol directo.
- **"El Puente del Alba"** (Rango D): Cruzar el Puente de los Suspiros antes del mediodía. Las sombras del puente ocultan pasajes solo visibles con luz cenital.

### 2.4 Beneficios de Exploración Diurna

- Los cofres en exteriores tienen **+15% probabilidad** de no estar atrapados (se ven los mecanismos con luz)
- Los atajos y caminos secundarios son claramente visibles
- Los puentes de cuerda y plataformas frágiles se inspeccionan visualmente sin necesidad de herramientas
- Las plantas medicinales rinden **+1 unidad** al ser recolectadas (la luz preserva su frescura)

---

## 3. Efectos de la Noche

La noche transforma SOLO. El mundo se vuelve hostil, pero también revela secretos que el día oculta.

### 3.1 Efectos de Combate

| Efecto | Valor | Explicación |
|--------|-------|-------------|
| Visibilidad | **Reducida** (15-20m sin antorcha, 30m con antorcha) | Las sombras consumen el paisaje |
| Enemigos nocturnos | **Aparecen** | Murciélagos, espectros, sombras, aparecidos |
| Enemigos bestia | **+20% daño, +20% velocidad** | La oscuridad los enfurece o libera su instinto |
| Probabilidad de crítico (exteriores) | **-10%** | Difícil apuntar con precisión en la oscuridad |
| Daño oscuro | **+10%** | La noche potencia la magia oscura |
| Daño sagrado | **-10%** | La luz divina es más débil sin sol |
| Detección de sigilo | **+15%** | Las sombras favorecen al jugador que se oculta |

### 3.2 Enemigos Nocturnos Exclusivos

| Enemigo | Rango Recomendado | Comportamiento |
|---------|------------------|----------------|
| **Murciélago de las Sombras** | E | Ataca en enjambres. Débil, pero molesto. Puede robar objetos pequeños |
| **Espectro Errante** | D | Inmune a daño físico sin encantamiento. Se desvanece al amanecer |
| **Sombra Profunda** | D | Se oculta en tu propia sombra. Ataca por sorpresa. Solo visible con luz |
| **Aparecido del Camino** | C | Copia tu equipo base. Pelea con tu propio reflejo |
| **Cazador Nocturno** | C | Jefe de campo. Patrulla caminos. Extremadamente agresivo. Solo aparece si el jugador ha matado 20+ bestias ese día |
| **El Mercader de Recuerdos** | — | NPC no hostil. Aparece en cruces de caminos. Vende recuerdos de personajes muertos. Solo de noche |

### 3.3 Jefes Opcionales Nocturnos

| Jefe | Piso | Condición | Recompensa |
|------|------|-----------|------------|
| **La Bestia Sin Ojos** | N0 (Bosque Dormido) | Solo entre 00:00 y 03:00 del ciclo nocturno | Colgante de la Bestia (+20% daño nocturno) |
| **El Caballero de la Luna** | N0 (Ruinas de la Colina) | Derrotar 3 espectros en la misma noche sin morir | Espada Lunar (daño sagrado + oscuro, 50/50) |
| **El Coro de Sombras** | N5 (Murallas exteriores) | Noche cerrada + viento fuerte | Fragmento de Eternidad (objeto de quest N10) |

### 3.4 NPCs Nocturnos

| NPC | Ubicación | Ofrece |
|-----|-----------|--------|
| **Borrachín del Puente** | Emberveil, puente sur | Misiones de borrachos, información sobre contrabando, acceso al mercado negro |
| **Fugitivo de las Cenizas** | Caminos de N0 | Quest para escapar de N0. Lore sobre N2 |
| **El Mercader de Recuerdos** | Cruces de caminos aleatorios | Vende recuerdos consumibles: revive un momento de un NPC muerto. 100 oro c/u |
| **Anciana de las Sombras** | Emberveil, callejón oeste | Vende pociones prohibidas, maldiciones, información sobre la Torre |

### 3.5 El Trueque del Tejido (Efecto Visual Especial)

Durante la noche despejada, un evento visual único ocurre en el cielo de N0, N5, N7 y N9.

- **Qué se ve:** Hilos brillantes de luz cruzan el cielo nocturno, como si alguien estuviera tejiendo el universo.
- **Cuándo:** Solo en noches sin nubes (soleado o nublado, nunca con lluvia, tormenta o niebla).
- **Duración:** Toda la noche.
- **Efecto de juego:** Durante el Trueque, las habilidades de Tejido (si el jugador las tiene) tienen **+25% efectividad**. Los Espectros del Tejido **no atacan** mientras el Trueque está visible.
- **Lore:** El Tejido es la estructura subyacente de la realidad en SOLO. Ver el Trueque es ver los hilos que conectan todos los mundos. Solo visible de noche porque la luz del sol oculta los hilos.

> *"Cuando las estrellas se vuelven hilos, y los hilos se vuelven caminos... ahí es donde la Torre realmente comienza."*
> — El Mercader de Recuerdos

---

## 4. Sistema de Clima

El clima afecta al overworld (N0) y a ciertos pisos de la Torre. Cada zona tiene su propio clima predominante.

### 4.1 Probabilidades en el Overworld (N0)

| Clima | Probabilidad | Duración (min) | Efectos Principales |
|-------|-------------|----------------|---------------------|
| **Soleado** | 60% | 10-30 min | Sin efectos |
| **Nublado** | 25% | 8-20 min | Sin efectos mecánicos. Cambio visual |
| **Lluvia** | 10% | 5-15 min | -30% visibilidad, +10% daño eléctrico, -15m detección enemigos |
| **Tormenta** | 4% | 3-8 min | -50% visibilidad, +30% daño eléctrico, rayos aleatorios |
| **Niebla** | 1% | 5-10 min | Visibilidad 10m máxima. Espectros del Tejido aparecen |

> **Nota:** El clima se recalcula cada 10 minutos de tiempo real. Puede cambiar gradualmente (ej: soleado → nublado → lluvia) o de forma abrupta (tormenta o niebla).

### 4.2 Efectos Detallados por Clima

#### Soleado ☀️
- **Visibilidad:** 100m
- **Sin efectos mecánicos**
- Los NPCs están más animados (diálogos alternativos)
- Las plantas medicinales rinden el máximo (3 unidades por recolección)

#### Nublado ☁️
- **Visibilidad:** 70m
- **Sin efectos mecánicos de combate**
- Efecto visual: luz difusa, sombras suaves
- Los enemigos bestia están ligeramente más irritables (diálogos de agresión, pero sin cambio estadístico)
- Algunos NPCs cambian sus diálogos ("Va a llover... lo siento en los huesos")

#### Lluvia 🌧️
- **Visibilidad:** -30% (máximo 40m)
- **Daño eléctrico:** +10% (afecta armas encantadas con rayo y hechizos eléctricos)
- **Detección enemiga:** Los enemigos detectan 15m menos (el sonido de la lluvia enmascara tus pasos)
- **Barro:** El terreno sin pavimentar ralentiza -10% movimiento. Caminos de piedra sin efecto
- **Duración:** 5-15 minutos de tiempo real
- **Efecto visual:** Lluvia ligera a moderada. Charcos en el suelo. Los NPCs buscan refugio

#### Tormenta ⛈️
- **Visibilidad:** -50% (máximo 20m, destellos de rayos permiten ver momentáneamente a 50m)
- **Daño eléctrico:** +30%
- **Daño de viento/hielo:** +20% (el viento de la tormenta potencia estos elementos)
- **Rayos aleatorios:** Cada 15-30 segundos, un rayo cae en una ubicación aleatoria del mapa. Si golpea al jugador: daño letal (muerte instantánea, sin importar HP). Indicador visual: el cielo se ilumina 2 segundos antes del impacto. ¡Corre!
- **Duración:** 3-8 minutos de tiempo real
- **Efecto visual:** Oscuridad casi total salvo por relámpagos. Viento sacudiendo árboles. Sonido ensordecedor
- **Recomendación:** Buscar refugio inmediatamente. Las tormentas no duran mucho

#### Niebla 🌫️
- **Visibilidad:** 10m máximo. Literalmente. No ves más allá de 10 metros en ninguna dirección
- **Detección mutua:** Los enemigos no te ven hasta que estás a 10m de ellos. Y viceversa. Esto puede ser ventajoso o mortal
- **Espectros del Tejido:** Enemigos especiales que SOLO aparecen durante la niebla. Son criaturas etéreas que deambulan sin rumbo. No agresivos a menos que los ataques. Al derrotarlos sueltan Fragmentos de Tejido (material de crafting rarísimo)
- **Duración:** 5-10 minutos de tiempo real
- **Efecto visual:** Blanco absoluto. Solo se ven siluetas a 5m. El sonido se amortigua. Sensación de aislamiento total
- **Lore:** La niebla ocurre cuando el Tejido se rasga y el vacío entre mundos se filtra al plano físico. Los Espectros del Tejido son almas atrapadas en esas rasgaduras

> *"La niebla no está hecha de agua. Está hecha de todo lo que la Torre se ha tragado."*
> — Archivero de Velmar

### 4.3 Transiciones de Clima

El clima no cambia instantáneamente (excepto en casos de evento). Las transiciones duran entre 30 y 60 segundos:

- **Soleado → Nublado:** Las nubes se acumulan gradualmente. La luz se atenúa en 30s.
- **Nublado → Lluvia:** Primero unas gotas, luego lluvia constante en 45s.
- **Nublado → Tormenta:** El viento se levanta, el cielo se oscurece, primeros relámpagos a los 30s.
- **Cualquiera → Niebla:** La niebla surge del suelo en 20s. Rápida y silenciosa.
- **Lluvia → Soleado:** La lluvia cesa gradualmente, las nubes se disipan en 60s.
- **Tormenta → Lluvia:** Los rayos cesan, la lluvia se vuelve constante y luego se calma en 60s.

---

## 5. Clima por Piso de la Torre

Cada piso de la Torre tiene su propio clima y condiciones ambientales únicas. Algunos pisos ignoran completamente el ciclo día/noche.

### N0 — Overworld (Emberveil y alrededores)
- **Ciclo:** Completo (30/2/10/2 min)
- **Clima:** Variable (ver tabla de probabilidades arriba)
- **Particularidades:** El clima aquí es natural, gobernado por el mundo real. Las tormentas pueden ser mortales para jugadores desprevenidos

### N1 — Velmar, La Ciudad de las Máquinas
- **Ciclo:** Siempre día artificial
- **Clima:** Sin clima natural. Lluvia perpetua solo en el Borde (piso inferior, zona de mantenimiento)
- **Particularidades:** La luz proviene de enormes cristales incandescentes en el techo. Nunca se apagan. El Borde tiene lluvia constante que cae de las tuberías superiores. Las nubes de vapor cubren el abismo — caerse significa muerte segura. El sonido de la lluvia en el Borde es mecánico, no natural

### N2 — Pastor de Cenizas
- **Ciclo:** Sin ciclo (siempre el mismo estado)
- **Clima:** Gris perpetuo. Calor constante (alrededor de 40°C)
- **Particularidades:** El sol no existe aquí. El cielo es una capa de cenizas suspendidas. Nunca llueve. Nunca hay viento. El calor es sofocante — sin armadura adecuada, el jugador recibe daño de calor cada 60s (1% HP por tick). Los Lobo de Ceniza están adaptados a este ambiente. El silencio del clima es opresivo

### N3 — Aelthar, El Lago de los Elfos
- **Ciclo:** Sin ciclo (siempre el mismo estado)
- **Clima:** Bioluminiscencia perpetua
- **Particularidades:** El "cielo" es el techo de una caverna colosal. El lago Aelthar refleja este techo, creando la ilusión de un cielo estrellado perpetuo. No hay día ni noche — la luz proviene de hongos y criaturas bioluminiscentes. La temperatura es constante (20°C). La humedad es alta pero sin lluvia. Los sonidos del agua y la vida subterránea crean una atmósfera de ensueño

### N4 — Bosque Petrificado
- **Ciclo:** Sin ciclo (no aplica)
- **Clima:** No hay clima
- **Particularidades:** Silencio absoluto. No hay viento. No hay sonido. No hay clima. Los árboles de piedra se alzan en un silencio eterno. Cualquier sonido del jugador se escucha a 3x la distancia normal. No hay pájaros, no hay insectos. No hay nada. El vacío climático es un personaje más del piso. Algunos enemigos usan este silencio para emboscar

### N5 — Thornwall, La Muralla de Espinas
- **Ciclo:** Día 20 min / Noche 10 min
- **Clima:** Viento constante en las murallas exteriores
- **Particularidades:** El viento nunca cesa en las murallas exteriores. Reduce la efectividad de proyectiles enemigos (-15% precisión) pero también los tuyos (-10%). El interior de las fortificaciones está protegido del viento. La noche es particularmente peligrosa aquí porque el viento enmascara los sonidos de los enemigos. Las antorchas en las murallas se apagan con el viento — hay que reencenderlas manualmente

### N6 — Cámaras de Jade
- **Ciclo:** Sin día/noche
- **Clima:** Sin clima. Zumbido cíclico de cristales
- **Particularidades:** No hay cambios climáticos tradicionales. En su lugar, los cristales de jade que cubren las paredes emiten un zumbido que cambia de tono cada 15 minutos. El zumbido afecta a los enemigos: en tono bajo, los enemigos están lentos (-20% velocidad); en tono alto, están agresivos (+20% daño). Los jugadores pueden usar el zumbido para sincronizar sus ataques. El tono se anuncia con un brillo pulsante de los cristales

### N7 — Sahal, El Desierto de Cristal
- **Ciclo:** Día 40 min / Noche 15 min
- **Clima:** Calor/frío extremos
- **Particularidades:**
  - **Día:** El medidor de calor se activa. Sin protección, el jugador recibe daño progresivo. A los 30s sin sombra: 5% HP cada 10s. A los 60s: 10% HP cada 10s. Las pociones curativas restauran 50% menos (el calor las evapora). Las armaduras pesadas son perjudiciales: +50% velocidad de acumulación de calor.
  - **Noche:** Hace un frío extremo. Sin abrigo: daño de frío progresivo. A los 30s sin fuente de calor: 5% HP cada 10s. Movimiento ralentizado (-20%). Las pociones de fuego son necesarias.
  - **Transición:** El cambio de día a noche y viceversa es rápido (1 minuto real). La temperatura cambia drásticamente en minutos.

### N8 — Jardines Flutantes
- **Ciclo:** Sin noche (luz perpetua de las flores)
- **Clima:** Viento suave constante. Sin precipitación
- **Particularidades:** Las flores bioluminiscentes iluminan todo el piso. Nunca hay oscuridad completa. El viento suave es constante pero no afecta el combate. La temperatura es perfecta (22°C). Este es el único piso con clima "perfecto". El viento mueve las flores y crea patrones de luz cambiantes. Es un piso para descansar... o para despedirse

### N9 — Argentia, La Ciudad Plateada
- **Ciclo:** Normal (30/2/10/2 min)
- **Clima:** Nieve ocasional. Frío constante
- **Particularidades:** La nieve no es tan intensa como en el N7 nocturno, pero es constante. La visibilidad en tormenta de nieve se reduce a 30m. La nieve en el suelo ralentiza el movimiento (-15%) a menos que tengas Botas de Nieve. Los NPCs usan abrigos. El sonido de la nieve amortigua los pasos (+20% sigilo en exteriores). Las fogatas y fuentes de calor son refugios seguros. La luna en Argentia siempre es llena (efecto visual)

### N10 — El Vacío
- **Ciclo:** No hay ciclo
- **Clima:** No hay clima. No hay tiempo
- **Particularidades:** No hay cielo. No hay suelo. No hay temperatura. No hay sonido. El Vacío es literalmente la ausencia de todo. Los efectos climáticos no aplican. Los efectos de día/noche no aplican. El jugador está suspendido en la nada. Cualquier habilidad relacionada con clima, luz u oscuridad no funciona. Es el último piso. No hay vuelta atrás

---

## 6. Equipo y Protección Contra el Clima

Para sobrevivir a los climas extremos, el jugador necesita equipo especializado.

### 6.1 Equipo para Lluvia y Tormenta

| Objeto | Costo | Efecto | Dónde Conseguirlo |
|--------|-------|--------|-------------------|
| **Capa Impermeable** | 200 oro | Resiste lluvia: elimina penalización de visibilidad por lluvia | Emberveil, Tienda General (rango E+) |
| **Capa Impermeable Reforzada** | 500 oro | Resiste lluvia + tormenta: elimina penalización de visibilidad por tormenta, +10% resistencia a daño eléctrico | Emberveil, Tienda General (rango D+) |
| **Botas de Barro** | 150 oro | Ignora ralentización por barro en lluvia | Emberveil, Zapatero |
| **Pararrayos Portátil** | 300 oro | Reduce 80% la probabilidad de ser golpeado por un rayo en tormenta | N5 Thornwall, Mercader |
| **Aceite de Silencio** | 50 oro (consumible, 10 min) | Añade +5m de reducción de detección durante lluvia | Emberveil, Apotecaria |
| **Túnica Aislante** | 800 oro | Inmunidad total a daño eléctrico durante tormentas. No evita rayos directos | N6 Cámaras de Jade (crafting) |

### 6.2 Equipo para Noche

| Objeto | Costo | Efecto | Dónde Conseguirlo |
|--------|-------|--------|-------------------|
| **Antorcha** | 10 oro | Ilumina 10m alrededor. Reduce penalización nocturna 50%. Se apaga con lluvia/tormenta (solo con viento fuerte) | Emberveil, cualquier tienda |
| **Antorcha Eterna** | 300 oro | Nunca se apaga. Ilumina 12m. Reduce penalización nocturna 50% | N1 Velmar, Mercader de Luz |
| **Linterna** | 80 oro | Ilumina 10m en cono frontal. Reduce penalización nocturna 50%. No se apaga con viento/lluvia | Emberveil, Herrero |
| **Linterna de Cristal** | 400 oro | Ilumina 15m. Reduce penalización nocturna 60%. Resistente a clima. Recargable con cristales de luz | N3 Aelthar, Artesano Elfo |
| **Amuleto de Visión Nocturna** | 600 oro | Visión nocturna completa (100% visibilidad de noche). No ocupa manos. Duradero | N1 Velmar (quest) |
| **Capucha de Sombras** | 350 oro | +20% sigilo de noche. +5% daño oscuro | Mercado Negro (noche, N0) |
| **Vela de Bolsillo** | 5 oro | Ilumina 3m. Dura 30 min de juego. Se apaga con viento | Cualquier tienda |
| **Baston de Luz** (Mago/Clérigo) | — | Ilumina 8m alrededor. Gratis. Dura mientras tenga SP | Habilidad de clase |

### 6.3 Equipo para Calor Extremo (N7 día)

| Objeto | Costo | Efecto | Notas |
|--------|-------|--------|-------|
| **Túnica de Lino** | 100 oro | Reduce acumulación de calor 30% | Básica, se consigue en Emberveil antes de N7 |
| **Sombrero de Alá** | 150 oro | Reduce acumulación de calor 20% | Se apila con túnica |
| **Capacete de Sombra** | 400 oro | Reduce acumulación de calor 50%. Protege la cabeza | N7 Sahal (comprado a nómadas) |
| **Abrigo de Nieve** | 500 oro | Resistencia al frío extremo (esencial para N7 noche). También protege del calor en N7 día (aislante térmico, 20%) | Argentia (N9) |
| **Poción Refrescante** | 30 oro | Elimina toda acumulación de calor por 5 minutos | Apotecaria. Craftable con Agua de Manantial + Hielo de N9 |
| **Parasol de Viaje** | 200 oro | Crea sombra portátil (3m de radio). Reduce acumulación de calor 40% mientras esté abierto | N5 Thornwall |
| **Armadura de Arena** | 700 oro | Inmunidad completa al calor. Pero -20% velocidad y -10% defensa contra daño de agua/hielo | N7 Sahal (crafting con Cristales de Desierto) |

### 6.4 Equipo para Frío Extremo (N7 noche, N9)

| Objeto | Costo | Efecto | Notas |
|--------|-------|--------|-------|
| **Abrigo de Nieve** | 500 oro | Resistencia al frío extremo. +10% defensa contra daño de hielo | Argentia (N9) |
| **Abrigo de Nieve Mejorado** | 900 oro | Inmunidad al frío extremo. +20% defensa contra daño de hielo. +5% velocidad en nieve | Argentia (N9), quest |
| **Botas de Nieve** | 200 oro | Ignora ralentización por nieve. +10% velocidad en terreno nevado | Argentia (N9) |
| **Guantes Térmicos** | 150 oro | Permite usar habilidades sin penalización por frío | Argentia (N9) |
| **Poción de Fuego Interno** | 40 oro | Inmunidad al frío por 8 minutos | Craftable con Esencia de Fuego + Cristal de Sahal |
| **Fogata Portátil** | 100 oro | Crea un punto de calor de 5m de radio. Dura 3 minutos. Consumible | Emberveil, Cazador |
| **Capa de Lobo** | 350 oro | Resistencia al frío 50%. +5% sigilo en nieve | N2 Pastor de Cenizas (crafting con piel de Lobo) |
| **Coraza de Hielo** (Clérigo/Paladín) | — | Habilidad: crea una barrera que protege del frío 30s. 30 SP | Habilidad de clase |

### 6.5 Equipo para Niebla

| Objeto | Costo | Efecto | Notas |
|--------|-------|--------|-------|
| **Brújula de Tejido** | 500 oro | Muestra la dirección del jugador incluso en niebla máxima | N1 Velmar (quest) |
| **Campana de Niebla** | 200 oro | Al tocarla, los Espectros del Tejido huyen 20s. 5 usos | Emberveil, Tienda de Rarezas |
| **Lentes de Niebla** | 300 oro | Aumenta visibilidad en niebla a 15m (+5m) | N3 Aelthar |
| **Fragmento de Tejido** | — | Al activarlo, revela la ubicación de todos los Espectros del Tejido en 30m a la redonda. Dura 10s | Drop de Espectros del Tejido |
| **Vela de Almas** | 150 oro | Los espectros no te atacan mientras la vela esté encendida. Dura 5 min. Consumible | El Mercader de Recuerdos (solo de noche) |

---

## 7. Eventos Climáticos Especiales

Más allá del clima regular, SOLO cuenta con eventos climáticos raros que pueden ocurrir bajo condiciones específicas.

### Lluvia de Estrellas (Evento Nocturno)
- **Condición:** Noche despejada + sin luna (fase lunar específica)
- **Frecuencia:** Una vez cada 7 ciclos nocturnos (~cada 5 horas reales)
- **Efecto:** Todos los fragmentos de Tejido en el mapa brillan. Se pueden recolectar sin combatir Espectros. +50% EXP de cualquier enemigo derrotado durante el evento
- **Duración:** Toda la noche (10 min reales)

### Tormenta de Cenizas (N2)
- **Condición:** Evento aleatorio en N2
- **Frecuencia:** 5% probabilidad por hora en N2
- **Efecto:** Visibilidad 5m. Daño de calor +50%. Los Lobo de Ceniza se vuelven hostiles incluso si están domados. Los enemigos no pueden detectarte por olfato (solo por visión)
- **Duración:** 5-8 minutos

### Niebla Carmesí (N0, evento rarísimo)
- **Condición:** Niebla + luna roja (fase lunar específica + 1% de probabilidad adicional)
- **Frecuencia:** Extremadamente rara. Puede no ocurrir en toda una partida
- **Efecto:** Todos los enemigos en N0 se vuelven de nivel +5. Aparecen jefes únicos. Los cofres se convierten en cofres de jefe. Los Fragmentos de Tejido caen en grupos de 3-5
- **Duración:** 10 minutos reales
- **Advertencia:** No apto para personajes de bajo nivel. La Niebla Carmesí es un evento de alto riesgo y alta recompensa

### Silencio del Tejido (N4)
- **Condición:** Sin condición especial