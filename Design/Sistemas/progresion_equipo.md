# Sistema de Progresión de Equipo — SOLO (Sunlight of the Last Online)

> **Versión:** 1.0  
> **Motor:** Unreal Engine 5.7  
> **Género:** RPG medieval 3D para un jugador  
> **Propósito:** El equipo es el motor de progresión horizontal junto con los niveles del personaje. Cada piso ofrece mejores armaduras, armas y accesorios, creando una escalada de poder tangible y gratificante.

---

## Índice

1. [Sistema de Rarezas](#1-sistema-de-rarezas)
2. [Sets de Equipo por Piso](#2-sets-de-equipo-por-piso)
3. [Estadísticas de Equipo](#3-estadísticas-de-equipo)
4. [Mejora de Equipo (Forja)](#4-mejora-de-equipo-forja)
5. [Progresión de Poder por Rango](#5-progresión-de-poder-por-rango)
6. [Items Únicos (No duplicables)](#6-items-únicos-no-duplicables)
7. [Generación de Loot](#7-generación-de-loot)
8. [Sistema de Runas](#8-sistema-de-runas)
9. [Sistema de Transmog](#9-sistema-de-transmog)
10. [Sistema de Vinculación](#10-sistema-de-vinculación)
11. [Economía de Equipo](#11-economía-de-equipo)
12. [Durabilidad y Degradación](#12-durabilidad-y-degradación)
13. [Glosario](#13-glosario)

---

## 1. Sistema de Rarezas

El equipo en SOLO se divide en 6 niveles de rareza. Cada nivel incrementa las estadísticas base sobre el anterior siguiendo un multiplicador progresivo.

### Tabla de Rarezas

| Rareza | Color | Bonificación | Drop desde |
|--------|-------|:-----------:|-----------|
| **Común** | Gris | ×1.0 | Enemigos básicos, cofres básicos, loot de mundo abierto |
| **Poco Común** | Verde | ×1.15 | Enemigos élite, cofres de madera, misiones secundarias |
| **Raro** | Azul | ×1.35 | Mini-jefes, cofres de hierro, eventos de mundo, quests principales |
| **Épico** | Púrpura | ×1.6 | Jefes de piso, cofres de plata, recompensas de pactos |
| **Legendario** | Naranja | ×2.0 | Jefes de rango A+, cofres de oro, misiones especiales, combinaciones de crafteo |
| **Mítico** | Rojo | ×2.5 | Crafteo con materiales únicos, jefes legendarios, Final B |

### Sistema de Nomenclatura

Los items cambian de nombre según su rareza para reflejar su poder creciente. Esto aplica a todas las categorías de equipo.

**Ejemplo de progresión de nombre — Espada Larga:**

| Rareza | Nombre |
|--------|--------|
| Común | Espada Larga de Hierro |
| Poco Común | Espada Larga Templada |
| Raro | Hoja de Fuego |
| Épico | Filo de la Llanura |
| Legendario | Cortacielos |
| Mítico | Último Rayo de Sol |

**Ejemplo de progresión de nombre — Armadura de Pecho:**

| Rareza | Nombre |
|--------|--------|
| Común | Cota de Malla de Hierro |
| Poco Común | Cota de Malla Reforzada |
| Raro | Coraza del Soldado |
| Épico | Peto del Alba |
| Legendario | Coraza del Sol Naciente |
| Mítico | Vientre del Sol |

**Ejemplo de progresión de nombre — Anillo:**

| Rareza | Nombre |
|--------|--------|
| Común | Anillo de Latón |
| Poco Común | Anillo de Plata Simple |
| Raro | Anillo del Cazador |
| Épico | Sortija de la Luna |
| Legendario | Anillo del Eclipse |
| Mítico | Grito del Vacío |

### Efectos Visuales por Rareza

Cada rareza tiene un efecto visual distintivo que ayuda al jugador a identificar rápidamente la calidad del loot:

- **Común:** Sin brillo. Apariencia desgastada.
- **Poco Común:** Brillo tenue verde. Pequeño destello al rotar el item en el inventario.
- **Raro:** Brillo azul suave. Partículas sutiles flotando alrededor del item.
- **Épico:** Brillo púrpura intenso. Partículas ascendentes. Pulso de luz cada 3 segundos.
- **Legendario:** Brillo naranja con chispas doradas. Efecto de "llama fría". Sonido ambiental al abrir el inventario.
- **Mítico:** Brillo rojo con ondas de energía. Partículas de fuego negro. El item "respira" con un latido visible.

### Porcentajes de Drop por Rareza

Los porcentajes base de drop varían según el tipo de enemigo:

| Tipo de Enemigo | Común | Poco Común | Raro | Épico | Legendario | Mítico |
|:---------------:|:-----:|:----------:|:----:|:----:|:----------:|:------:|
| Enemigo básico | 70% | 25% | 4% | 1% | 0.05% | — |
| Élite | 30% | 40% | 20% | 8% | 2% | 0.01% |
| Mini-jefe | 10% | 25% | 35% | 22% | 7% | 1% |
| Jefe de piso | 5% | 15% | 30% | 30% | 15% | 5% |
| Jefe de rango A+ | 2% | 8% | 20% | 30% | 30% | 10% |
| Jefe legendario | — | 5% | 10% | 20% | 35% | 30% |

> **Nota:** Los porcentajes se modifican por el modificador de "Suerte" del personaje y por el nivel de dificultad. En dificultad "Cenizas" (NG+), los porcentajes de rareza alta se duplican.

---

## 2. Sets de Equipo por Piso

Cada piso tiene al menos un set de equipo asociado, con bonificaciones por piezas equipadas. Los sets están diseñados para potenciar builds específicas y ofrecer decisiones tácticas al jugador.

### Reglas Generales de Sets

- **2 piezas:** Otorgan un bonus menor o situacional.
- **3 piezas (si el set tiene 3):** Otorgan un bonus mediano o mecánico.
- **4 piezas:** Otorgan un bonus mayor que cambia la forma de jugar.
- Los sets no tienen un bonus fijo para 3 piezas si están diseñados para 4 — se usa la nomenclatura "Bonus 2" y "Bonus 4".
- Los sets de 2 piezas solo tienen "Bonus 2".
- Las piezas del set se identifican con un color de borde especial en el inventario y un icono de "vinculación de set".
- Los sets no se pueden mezclar entre pisos; cada set es completo.

### Sets hasta N10

---

#### N0 — Emberveil (Ruinas del Inicio)

El piso de aprendizaje. Sets básicos que enseñan al jugador el valor de las bonificaciones por set.

**Set del Errante** *(Rango E)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Pecho, Botas, Guantes |
| Rareza base | Común |
| Drop | Enemigos en Emberveil, cofres básicos |
| Estilo visual | Telas raídas, cuero remendado, colores tierra |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +10% velocidad de movimiento |
| 3 piezas | +10% defensa contra bestias |

*Nota de diseño:* Set tutorial. Enseña al jugador a buscar sets completos.

---

**Set del Guardián** *(Rango D)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Casco, Pecho, Botas, Brazales |
| Rareza base | Poco Común |
| Drop | Guardián de la Garganta (jefe de N0) |
| Estilo visual | Metal oxidado con remaches, hombreras anchas, aspecto robusto |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +15% defensa |
| 4 piezas | Al recibir daño letal, sobrevives con 1 PV (1 vez por pelea). Activa un efecto visual de "último aliento" con partículas doradas. |

*Nota de diseño:* Primer set defensivo real. La bonificación de 4 piezas da seguridad al jugador para enfrentar desafíos mayores.

---

#### N1 — Velmar (Ciudad de las Campanas)

Piso de transición. Primer set orientado a sigilo y precisión.

**Set del Colgante** *(Rango D+)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Capa, Anillo, Amuleto |
| Rareza base | Poco Común |
| Drop | La Voz (jefe de Velmar), cofres de madera en Velmar |
| Estilo visual | Capa gris con bordados plateados, joyería sencilla pero elegante |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +10% daño crítico |
| 3 piezas | Los ataques tienen 5% de probabilidad de no hacer sonido (no alerta a enemigos cercanos). El jugador ve un indicador visual cuando el efecto se activa. |

*Nota de diseño:* Introduce la mecánica de sigilo y crítico. Buen set para jugadores que prefieren evitar combates directos.

---

#### N2 — Pastor de Cenizas (Llanuras Quemadas)

Piso temático de fuego. Sets que otorgan resistencia y potencian daño ígneo.

**Set del Calcinado** *(Rango D+)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Pecho, Capa, Botas |
| Rareza base | Raro |
| Drop | Enemigos quemados (variante de enemigos en N2), Pastor de Cenizas (jefe de N2) |
| Estilo visual | Armadura carbonizada con vetas de brasa, capa desgarrada con bordes ardientes |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | Resistencia al calor +50% (reduce la velocidad de llenado del medidor de calor) |
| 3 piezas | Ataques de fuego +20% de daño |

*Nota de diseño:* Set temático de N2. Esencial para sobrevivir en zonas de calor extremo. Sin este set, el medidor de calor se llena 2× más rápido.

---

#### N3 — Aelthar (Ciudad del Lago)

Piso de magia y pactos. Sets con resistencia mágica y apoyo a invocaciones.

**Set del Lago** *(Rango C)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Pecho, Casco, Guantes, Botas |
| Rareza base | Raro |
| Drop | NPCs de Aelthar (comerciante de armaduras), jefe del lago, cofres de hierro |
| Estilo visual | Azul profundo con incrustaciones de cristal de agua, aspecto fluido y elegante |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +15% resistencia mágica |
| 4 piezas | Al recibir daño mágico, 25% de probabilidad de curarte 10% de tu PV máximo. El efecto tiene un cooldown interno de 3 segundos. |

*Nota de diseño:* Set antimagia. Vital para el jefe de N3 que usa ataques mágicos pesados.

---

**Set del Errante de Aelthar** *(Rango C)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 2: Brazales, Amuleto |
| Rareza base | Raro |
| Drop | Garrik (PNJ, recompensa de quest) |
| Estilo visual | Brazales de cuero con runas brillantes, amuleto con una gema azul pulsante |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +1 invocación máxima (Nigromante/Druida). Aumenta el límite de esqueletos/espíritus invocados simultáneamente. |

*Nota de diseño:* Set de apoyo a clases de invocación. Permite builds de "ejército" más viables.

---

#### N4 — El Corredor de los Nombres (Catacumbas Olvidadas)

Piso de defensa y resistencia a estados alterados.

**Set Petrificado** *(Rango C)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Pecho, Botas, Brazales |
| Rareza base | Raro |
| Drop | Guardián Petrificado (mini-jefe de N4), cofres de hierro en catacumbas |
| Estilo visual | Armadura de piedra gris con vetas de mármol, aspecto pesado y monolítico |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +20% defensa, pero -10% velocidad de movimiento. El personaje se ve más lento y pesado. |
| 3 piezas | Inmune a petrificación. El jugador ya no puede ser convertido en piedra por los Gorgonops del piso. |

*Nota de diseño:* Set situacional. La penalización de velocidad hace que el jugador decida si vale la pena el trade-off. Contra el jefe de N4, es casi obligatorio.

---

#### N5 — Thornwall (Muralla de Espinas)

Piso de combate cuerpo a cuerpo intenso. Sets ofensivos.

**Set del Cazador** *(Rango C+)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Capa, Guantes, Botas |
| Rareza base | Épico |
| Drop | Mercader de Thornwall (comerciante de objetos especiales), cofres de plata |
| Estilo visual | Cuero marrón oscuro con adornos de hueso, capa de piel de bestia |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +20% daño contra monstruos |
| 3 piezas | +15% velocidad de movimiento fuera de combate |

*Nota de diseño:* Set de "granjeo". Perfecto para farmear pisos anteriores o explorar Thornwall eficientemente.

---

**Set del Carnicero** *(Rango B-)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Pecho, Casco, Guantes, Botas |
| Rareza base | Épico |
| Drop | El Carnicero (jefe de N5) |
| Estilo visual | Armadura de acero manchada de sangre, hombreras con púas, casco con visera de cráneo |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +25% daño físico |
| 4 piezas | Los golpes fuertes (cargados o críticos) regeneran 3% de tu PV máximo. El efecto tiene un cooldown de 1.5 segundos. |

*Nota de diseño:* Set ofensivo puro. El bonus de 4 piezas da sostenibilidad en peleas largas. Excelente para builds de Tanque o Guerrero.

---

#### N6 — Cámaras de Jade (Templo Elemental)

Piso de daño elemental y magia.

**Set de Jade** *(Rango C+)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Brazales, Amuleto, Anillo, Capa |
| Rareza base | Épico |
| Drop | Constructos de Jade (enemigos élite), Coro de Jade (jefe de N6) |
| Estilo visual | Jade translúcido con incrustaciones de oro, aspecto ceremonial y antiguo |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +15% daño elemental (fuego, hielo, eléctrico, oscuro, sagrado) |
| 4 piezas | Los ataques elementales tienen 15% de probabilidad de aplicar un segundo efecto del mismo tipo (ej: fuego aplica quemadura extra, hielo aplica congelación adicional). |

*Nota de diseño:* Set de builds elementales. El "segundo efecto" puede stackearse con efectos existentes para builds de alto daño por segundo.

---

#### N7 — Sahal (Desierto de los Susurros)

Piso de supervivencia en condiciones extremas.

**Set de Arena** *(Rango B-)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Pecho, Capa, Botas |
| Rareza base | Épico |
| Drop | Enemigos de Sahal (todos los tipos), cofres de plata |
| Estilo visual | Tela beige desgastada, capa que imita las dunas, botas con polainas de cuero |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +20% resistencia al calor, +10% sigilo en desierto. El sigilo adicional solo aplica en bioma de desierto. |
| 3 piezas | Los ataques que matan a un enemigo regeneran 15% de tu PV máximo. No aplica a estructuras u objetos. |

*Nota de diseño:* Set de sustento. Permite al jugador explorar Sahal sin preocuparse tanto por recursos de curación.

---

**Set de la Costurera** *(Rango B)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Guantes, Amuleto, Anillo |
| Rareza base | Épico |
| Drop | La Costurera (jefe de N7, opcional) |
| Estilo visual | Guantes de encaje negro con hilos de seda roja, joyería de obsidiana |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +20% daño contra humanos/humanoides |
| 3 piezas | Los ataques que matan a un enemigo humanoide lo reaniman como aliado por 10 segundos. El aliado tiene 50% de su PV original y ataca a otros enemigos. |

*Nota de diseño:* Set temático oscuro. El reanimar enemigos es situacional pero muy poderoso contra grupos grandes.

---

#### N8 — Jardines Flotantes (Islas Celestiales)

Piso de verticalidad y combate aéreo.

**Set Flotante** *(Rango B)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 3: Botas, Guantes, Casco |
| Rareza base | Legendario |
| Drop | Gárgolas (enemigos élite voladores), Arpía de Bronce (jefe de N8) |
| Estilo visual | Armadura ligera de bronce y plumas, casco con alas, aspecto etéreo |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | Caída lenta (como pluma). El jugador desciende suavemente, similar a tener alas. |
| 3 piezas | +20% velocidad de ataque mientras estás en el aire (saltando, cayendo o después de un dash aéreo). |

*Nota de diseño:* Set que cambia la exploración vertical. Fundamental para navegar N8 sin recibir daño de caída.

---

#### N9 — Argentia (Ciudad de Plata)

Piso de reflexión y equilibrio.

**Set de Plata** *(Rango B)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Casco, Pecho, Guantes, Botas — set completo |
| Rareza base | Legendario |
| Drop | Verdugo de Plata (jefe de N9), comerciante de Argentia (PNJ vendedor) |
| Estilo visual | Armadura de plata pulida con filigranas doradas, aspecto real y majestuoso |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +20% defensa, +20% resistencia mágica |
| 4 piezas | Al recibir daño, 30% de probabilidad de reflejar 50% del daño recibido al atacante. El reflejo ignora la defensa del atacante. |

*Nota de diseño:* Set defensivo por excelencia. La bonificación de 4 piezas castiga a los enemigos que atacan rápido y con mucho daño.

---

#### N10 — El Centinela (Fortaleza del Fin)

El desafío final. Un solo set, el más poderoso del juego base.

**Armadura del Centinela** *(Rango A, Legendario)*

| Propiedad | Valor |
|-----------|-------|
| Piezas | 4: Casco, Pecho, Guantes, Botas |
| Rareza base | Legendario |
| Drop | Centinela de Granito (fase 1 de N10) |
| Estilo visual | Armadura de piedra viva con vetas de energía dorada, aspecto imponente y ancestral |

| Bonus | Efecto |
|:----:|--------|
| 2 piezas | +30% defensa contra jefes |
| 4 piezas | Inmune a muerte instantánea. Efectos que matarían al jugador de un golpe (como ciertos ataques del Centinela) en su lugar dejan al jugador con 10% de PV. |

*Nota de diseño:* Set de "boss killer". La inmunidad a muerte instantánea es clave para la pelea final contra el Centinela, que tiene varios ataques de un solo golpe.

---

### Items Únicos por Final

Estos items solo se obtienen en momentos específicos de la narrativa y dependen de la elección del final.

**Medalla del Sol** *(Rango A, Legendario)* — Final A

| Propiedad | Valor |
|-----------|-------|
| Tipo | Amuleto |
| Rareza | Legendario |
| Efecto | +25% a todas las estadísticas mientras haya luz natural (exterior, día). En interiores o de noche, el bonus se reduce a +10%. |
| Cómo obtenerlo | Recogido durante el amanecer en el Final A, justo antes del enfrentamiento final. Solo disponible en esa ventana. |

*Nota de diseño:* Recompensa por elegir el "final heroico". El bonus variable incentiva al jugador a planificar sus enfrentamientos según el ciclo día/noche.

---

**Corona del Arquitecto** *(Rango A+, Mítico)* — Final B

| Propiedad | Valor |
|-----------|-------|
| Tipo | Casco |
| Rareza | Mítico |
| Efecto | Permite ver enemigos invisibles. +1 a todas las estadísticas (Fuerza, Destreza, Inteligencia, Fe, Suerte) por cada jefe muerto en toda la partida. |
| Cómo obtenerlo | Al aceptar el puesto de Nuevo Arquitecto al final del Final B. |

*Nota de diseño:* Item de escalado infinito. Recompensa a los jugadores que han derrotado a todos los jefes opcionales. Potencialmente el item más poderoso del juego si se completa al 100%.

---

## 3. Estadísticas de Equipo

### 3.1 Armas

Cada arma en SOLO tiene un conjunto de estadísticas base que definen su comportamiento en combate.

#### Estadísticas Base de Arma

| Estadística | Descripción |
|-------------|-------------|
| **Daño base** | Daño antes de modificadores. Escala con la rareza: Común ×1.0 → Mítico ×2.5 |
| **Multiplicador de golpe** | Modificador según categoría de velocidad: Rápido 0.8×, Normal 1.0×, Lento 1.5× |
| **Tipo de daño** | Cortante / Contundente / Perforante. Puede tener un elemento附加 (fuego, hielo, eléctrico, oscuro, sagrado) |
| **Velocidad de ataque** | Rápida / Normal / Lenta / Muy rápida / Muy lenta. Determina la animación y el frame data |
| **Requisito de clase** | Algunas armas solo pueden ser equipadas por clases específicas |
| **Slot de runa** | Número de slots disponibles para insertar runas (0-3 según rareza y tipo) |
| **Alcance** | Cuerpo a cuerpo (corto/medio/largo) o distancia (corto/medio/largo) |
| **Consumo de SP** | Stamina o maná gastado por ataque |

#### Catálogo de Armas

**Espada Corta**
- Velocidad: Rápida
- Tipo de daño: Cortante
- Multiplicador de daño: 0.9×
- Alcance: Cuerpo a cuerpo (corto)
- Clases: Guerrero, Pícaro, Mercenario
- Notas: Buena para combos rápidos y acumular golpes.

**Espada Larga**
- Velocidad: Normal
- Tipo de daño: Cortante
- Multiplicador de daño: 1.0×
- Alcance: Cuerpo a cuerpo (medio)
- Clases: Guerrero, Paladín, Centinela
- Notas: Arma versátil, sin debilidades significativas.

**Mandoble**
- Velocidad: Lenta
- Tipo de daño: Cortante
- Multiplicador de daño: 1.4×
- Alcance: Cuerpo a cuerpo (largo), AOE frontal
- Clases: Guerrero, Centinela
- Notas: Golpea en un cono frente al jugador. Bueno para multitudes.

**Hacha**
- Velocidad: Lenta
- Tipo de daño: Contundente
- Multiplicador de daño: 1.3×
- Alcance: Cuerpo a cuerpo (medio)
- Clases: Guerrero, Bárbaro
- Notas: Rompe postura de enemigos más rápido que otras armas. Ideal contra escudos.

**Hacha Doble**
- Velocidad: Muy lenta
- Tipo de daño: Contundente
- Multiplicador de daño: 1.6× (dos golpes en la animación, 0.8× cada uno)
- Alcance: Cuerpo a cuerpo (medio)
- Clases: Bárbaro
- Notas: Dos impactos por ataque. Alto daño total pero muy lenta. Deja expuesto al jugador.

**Martillo de Guerra**
- Velocidad: Muy lenta
- Tipo de daño: Contundente
- Multiplicador de daño: 1.8×
- Alcance: Cuerpo a cuerpo (medio)
- Clases: Paladín, Centinela
- Notas: Derriba enemigos pequeños. Gasta mucha stamina. Alto riesgo, alta recompensa.

**Daga**
- Velocidad: Muy rápida
- Tipo de daño: Perforante
- Multiplicador de daño: 0.6×
- Alcance: Cuerpo a cuerpo (muy corto)
- Clases: Pícaro, Nigromante (como arma secundaria)
- Notas: +10% de probabilidad de crítico. Se puede usar en la mano izquierda (dual wield).

**Arco**
- Velocidad: Normal
- Tipo de daño: Perforante (a distancia)
- Multiplicador de daño: 1.0×
- Alcance: Distancia (medio)
- Clases: Cazador, Pícaro
- Notas: Requiere munición (flechas). La distancia reduce el daño. Daño completo a corta distancia.

**Ballesta**
- Velocidad: Lenta
- Tipo de daño: Perforante (a distancia)
- Multiplicador de daño: 1.5×
- Alcance: Distancia (largo)
- Clases: Cazador, Mercenario
- Notas: Perfora hasta 2 objetivos en línea recta. Recarga lenta. Alto daño por disparo.

**Bastón / Báculo**
- Velocidad: Normal
- Tipo de daño: Contundente (cuerpo a cuerpo) + Mágico (a distancia/hechizos)
- Multiplicador de daño: 1.0× (+ poder mágico adicional)
- Alcance: Cuerpo a cuerpo (medio) / Distancia variable según hechizo
- Clases: Nigromante, Druida, Paladín (como foco)
- Notas: Multiplica el poder de los hechizos. El daño cuerpo a cuerpo es secundario.

**Katana**
- Velocidad: Normal
- Tipo de daño: Cortante
- Multiplicador de daño: 1.0×
- Alcance: Cuerpo a cuerpo (medio)
- Clases: Cualquiera (requiere entrenamiento: "Estilo del Este" — habilidad desbloqueable en N7)
- Notas: +5% de probabilidad de crítico. Se puede desenvainar y envainar rápidamente (ataque Iai). No disponible al inicio.

**Hojas Gemelas** *(Exclusivo de Garrik)*
- Velocidad: Rápida
- Tipo de daño: Cortante
- Multiplicador de daño: 0.8× cada hoja (total 1.6× por ataque completo)
- Alcance: Cuerpo a cuerpo (corto)
- Clases: Garrik (PNJ acompañante)
- Notas: Dos golpes por ataque. No disponible para el jugador. Garrik las usa exclusivamente.

#### Tabla de Referencia Rápida de Armas

| Arma | Velocidad | Daño × | Tipo | Alcance | Clases |
|:----:|:---------:|:------:|:----:|:-------:|:------:|
| Espada Corta | Rápida | 0.9× | Cortante | Cuerpo a cuerpo (corto) | Guerrero, Pícaro, Mercenario |
| Espada Larga | Normal | 1.0× | Cortante | Cuerpo a cuerpo (medio) | Guerrero, Paladín, Centinela |
| Mandoble | Lenta | 1.4× | Cortante | Cuerpo a cuerpo (largo, AOE) | Guerrero, Centinela |
| Hacha | Lenta | 1.3× | Contundente | Cuerpo a cuerpo (medio) | Guerrero, Bárbaro |
| Hacha Doble | Muy lenta | 1.6× | Contundente | Cuerpo a cuerpo (medio, 2 golpes) | Bárbaro |
| Martillo | Muy lenta | 1.8× | Contundente | Cuerpo a cuerpo (medio) | Paladín, Centinela |
| Daga | Muy rápida | 0.6× | Perforante | Cuerpo a cuerpo (muy corto) | Pícaro, Nigromante |
| Arco | Normal | 1.0× | Perforante | Distancia (medio) | Cazador, Pícaro |
| Ballesta | Lenta | 1.5× | Perforante | Distancia (largo) | Cazador, Mercenario |
| Bastón/Báculo | Normal | 1.0×+ | Contundente/Mágico | Variable | Nigromante, Druida, Paladín |
| Katana | Normal | 1.0× | Cortante | Cuerpo a cuerpo (medio) | Cualquiera (con entrenamiento) |
| Hojas Gemelas | Rápida | 0.8× c/u | Cortante | Cuerpo a cuerpo (corto) | Garrik (PNJ) |

### 3.2 Armadura

Cada pieza de armadura contribuye a la defensa total del personaje y puede tener resistencias elementales y bonificaciones secundarias.

#### Estadísticas Base de Armadura

| Estadística | Descripción |
|-------------|-------------|
| **Defensa base** | Reduce el daño físico recibido. Escala con rareza |
| **Peso** | Ligera / Media / Pesada. Afecta velocidad de movimiento y consumo de stamina |
| **Resistencias elementales** | Fuego, Hielo, Eléctrico, Oscuro, Sagrado. Valores de 0-100 |
| **Bonificaciones** | Atributos secundarios aleatorios (en Raro+). Pueden incluir aumento de PV, SP, regeneración, etc. |

#### Pesos de Armadura

| Peso | Velocidad | Stamina | Defensa | Recomendado para |
|:----:|:---------:|:-------:|:-------:|:----------------:|
| Ligera | +10% | -10% consumo | -20% | Pícaro, Cazador, Nigromante |
| Media | 0% | 0% | 0% | Guerrero, Mercenario, Druida |
| Pesada | -15% | +25% consumo | +30% | Paladín, Centinela, Bárbaro |

#### Piezas de Armadura

| Pieza | Slot | Función principal | Peso ligero | Peso medio | Peso pesado |
|:-----:|:----:|:-----------------:|:-----------:|:----------:|:-----------:|
| **Casco / Yelmo** | Cabeza | Defensa, resistencia a críticos en la cabeza | Gorro de cuero | Yelmo de acero | Gran yelmo |
| **Pecho / Cota** | Torso | Defensa principal, resistencia elemental | Túnica acolchada | Cota de malla | Coraza de placas |
| **Guantes / Brazales** | Manos | Defensa, bonificación a daño de armas | Guantes de cuero | Brazales de metal | Manoplas de acero |
| **Botas / Grebas** | Pies | Defensa, velocidad de movimiento | Botas de piel | Grebas de cuero tachonado | Grebas de placas |
| **Capa** | Espalda | Resistencia elemental, bonificaciones secundarias | Capa de lana | Capa de seda | Capa de piel de bestia |

#### Rangos de Defensa Base por Rareza (Pecho)| Rareza | Ligera | Media | Pesada |
|:------:|:------:|:-----:|:------:|
| Común | 8-12 | 12-18 | 18-25 |
| Poco Común | 10-15 | 15-22 | 22-30 |
| Raro | 14-20 | 20-28 | 28-38 |
| Épico | 18-26 | 26-36 | 36-48 |
| Legendario | 24-34 | 34-46 | 46-60 |
| Mítico | 30-42 | 42-56 | 56-75 |

*Los valores son para la pieza de Pecho. Otras piezas tienen valores proporcionales: Casco 70%, Guantes 50%, Botas 60%, Capa 40%.*

### 3.3 Accesorios (4 Slots)

Los accesorios proporcionan bonificaciones pasivas y activas sin defensa física directa.

| Slot | Cantidad | Función | Ejemplos |
|:----:|:--------:|---------|----------|
| **Anillo** | 2 | Efectos de combate | +daño, +crítico, +velocidad de ataque, robo de vida |
| **Amuleto** | 1 | Efectos pasivos | +PV, +SP, +regeneración, resistencia a estados |
| **Cinturón / Báculo** | 1 | Slot único de utilidad | Bolsas extra (inventario), pociones automáticas, faro que revela tesoros |

### 3.4 Escudos (Exclusivo Centinela/Paladín)

Los escudos son equipo de mano izquierda exclusivo para las clases Centinela y Paladín.

| Estadística | Descripción |
|-------------|-------------|
| **Defensa adicional** | Se suma a la defensa total cuando el escudo está equipado (aunque no esté bloqueando) |
| **Capacidad de bloqueo** | Reduce el daño recibido al bloquear: 50-80% según el escudo y su rareza |
| **Gasto de SP** | El bloqueo consume SP. Escudos mejores reducen este gasto |
| **Encantamiento** | Los escudos pueden tener encantamientos (reflejo, resistencia elemental, etc.) |

**Tipos de Escudo:**

| Escudo | Bloqueo | Defensa | SP gasto | Peso | Clase |
|:------:|:-------:|:-------:|:--------:|:----:|:-----:|
| Escudo de Madera | 50% | 5-10 | 8 SP/s | Ligero | Centinela, Paladín |
| Escudo de Hierro | 60% | 10-18 | 6 SP/s | Medio | Centinela, Paladín |
| Escudo de Acero | 70% | 18-28 | 4 SP/s | Medio | Paladín |
| Escudo Torre | 80% | 25-40 | 3 SP/s | Pesado | Centinela |
| Escudo de Plata | 75% | 20-35 | 3 SP/s | Medio | Paladín (N9+) |

---

## 4. Mejora de Equipo (Forja)

En ciudades con herrero (N1 — Velmar, N3 — Aelthar, N5 — Thornwall, N7 — Sahal, N9 — Argentia), el jugador puede mejorar, encantar y reparar su equipo.

### 4.1 Mejora de Rareza

Subir la rareza de un item incrementa todas sus estadísticas base al multiplicador de la siguiente rareza.

| Mejora | Costo Oro | Materiales | Requisito |
|--------|:---------:|------------|:---------:|
| Común → Poco Común | 100 | 2 Fragmentos de Hierro | N0+ |
| Poco Común → Raro | 300 | 3 Hierro + 1 Esencia de Fuego | N3+ |
| Raro → Épico | 800 | 2 Acero + 2 Esencia Elemental | N5+ |
| Épico → Legendario | 2000 | 3 Plata + 1 Corazón de Monstruo | N7+ |
| Legendario → Mítico | — | No se mejora — solo se craftea o dropea | — |

> **Nota:** Al mejorar de rareza, el item conserva sus bonificaciones secundarias pero estas se escalan al nuevo nivel. No se pueden perder bonificaciones al mejorar.

### 4.2 Encantamientos

Los encantamientos añaden efectos elementales o resistencias al equipo.

| Encantamiento | Costo Oro | Materiales | Efecto |
|:-------------:|:---------:|------------|--------|
| Fuego (arma) | 500 | 2 Esencia de Fuego | Añade daño de fuego al arma |
| Hielo (arma) | 500 | 2 Esencia de Hielo | Añade daño de hielo al arma |
| Eléctrico (arma) | 500 | 2 Esencia Eléctrica | Añade daño eléctrico al arma |
| Resistencia (armadura) | 400 | 1 Esencia Elemental + 2 Mineral | Añade +15 resistencia elemental |
| Slot de runa | 1000 | 1 Material Raro + 500 oro | Añade un slot de runa al item |

### 4.3 Reparación

Cada muerte o daño recibido desgasta el equipo.

| Mecánica | Detalle |
|----------|---------|
| Pérdida por muerte | -5% durabilidad en todas las piezas equipadas |
| Penalización | Cada 5 muertes sin reparar → equipo pierde 10% de efectividad (acumulable hasta -50%) |
| Costo de reparación | 10% del valor del item por cada 10% de durabilidad perdida |
| Reparación en fogata | Descansando en una fogata, se puede reparar 20% de durabilidad gratis (1 vez por fogata) |

### 4.4 Desbloqueo de Clases de Armas

Ciertas armas requieren entrenamiento previo para ser equipadas.

| Entrenamiento | Costo | Requisito | Desbloquea |
|:-------------:|:-----:|:---------:|:----------:|
| Estilo del Este | 1500 oro + quest | N7+ | Katana |
| Maestría en Mandoble | 1000 oro | N3+ | Mandoble |
| Forja Mejorada | 2000 oro + quest | N5+ | Mejora a Épico |

---

## 5. Progresión de Poder por Rango

Esta tabla indica el poder mínimo esperado para sobrevivir cómodamente en cada piso. Por debajo de estos valores, el jugador encontrará el piso extremadamente difícil.

| Piso | Rango Mínimo | Arma Esperada | Armadura Esperada | PV Mínimo |
|:----:|:------------:|:-------------:|:-----------------:|:---------:|
| N0 — Emberveil | E | Común | Común | 150 |
| Guardián | D | Poco Común | Común | 200 |
| N1 — Velmar | D+ | Poco Común | Poco Común | 250 |
| N2 — Pastor | D+ | Raro | Poco Común | 350 |
| N3 — Aelthar | C | Raro | Raro | 450 |
| N4 — Corredor | C | Raro+ | Raro | 550 |
| N5 — Thornwall | C | Épico | Raro+ | 650 |
| N6 — Jade | C+ | Épico | Épico | 750 |
| N7 — Sahal | C+ | Épico+ | Épico | 850 |
| N8 — Jardines | B- | Legendario | Épico+ | 1000 |
| N9 — Argentia | B | Legendario | Legendario | 1200 |
| N10 — Centinela | B+ | Legendario+ | Legendario+ | 1500 |

> **Nota:** "Raro+" significa que al menos una pieza de equipo debería ser de rareza Raro o superior. "Legendario+" indica que al menos una pieza debería ser del piso actual o superior.

---

## 6. Items Únicos (No duplicables)

Estos items solo se pueden obtener UNA VEZ por partida. Si se venden o destruyen, no hay forma de recuperarlos. Representan decisiones importantes y exploración cuidadosa.

| Item | Piso | Tipo | Efecto | Notas |
|:----:|:----:|:----:|--------|-------|
| **Anillo de las Cenizas** | N2 | Anillo | +20% daño de fuego, +20% resistencia al calor | Missable si no exploras lo suficiente. Escondido en una cueva detrás de una cascada de lava. |
| **Lágrima de Elfa** | N3 | Consumible | Revive al último compañero caído (excluye muertes de historia). 1 uso. | Entregado por una PNJ élfica en Aelthar si completas su misión secundaria. |
| **Piedra de Toque** | N5 | Consumible | Permite mejorar una pieza de equipo a la siguiente rareza sin materiales (excepto Mítico). 1 uso. | Recompensa por derrotar al Carnicero sin usar curaciones. |
| **Fragmento del Arquitecto** | N9 | Quest item | Requerido para acceder al Final B. Debe estar en el inventario al enfrentar al Centinela. | Obtenido en la misión de Elia en Argentia. No se puede vender ni tirar. |
| **Ojo del Centinela** | N9 | Quest item | Revela la verdadera forma del Arquitecto en N10, reduciendo su daño en 15% durante toda la pelea. | Quest secundaria en Argentia. Muy fácil de perder si no hablas con el PNJ correcto. |
| **Último Aliento de Caín** | N0→N1 | Amuleto | Mientras lo tengas equipado, el espíritu de Caín aparece ocasionalmente para dar un golpe extra (10% de probabilidad por ataque). El golpe hace 50% del daño del arma como daño de oscuro. | Loot del Guardián de la Garganta. Solo disponible si Caín murió en tu partida. |

---

## 7. Generación de Loot

### 7.1 Algoritmo de Drop

Cuando un enemigo muere, el juego calcula el loot en este orden:

1. **Tabla de drop del enemigo:** Cada enemigo tiene una tabla de loot específica con items fijos y aleatorios.
2. **Rareza:** Se tira el porcentaje de rareza (ver sección 1). La Suerte del jugador modifica este valor.
3. **Cantidad de piezas:** Se genera 1-3 items según el tipo de enemigo (básico: 1, élite: 1-2, jefe: 2-3).
4. **Bonificaciones secundarias:** Si el item es Raro+, se generan 1-3 bonificaciones secundarias aleatorias de un pool según el tipo de item.
5. **Rodar duplicados:** Si el jugador ya tiene el item único en el inventario, no se genera de nuevo.

### 7.2 Modificador de Suerte

La estadística de Suerte afecta la calidad del loot:

| Suerte | Efecto en drops |
|:------:|:---------------:|
| 0-5 | Sin modificación |
| 6-10 | +5% a probabilidad de rareza superior |
| 11-15 | +10% a probabilidad de rareza superior |
| 16-20 | +15% a probabilidad de rareza superior, +1 item adicional en jefes |
| 21+ | +20% a probabilidad de rareza superior, +1 item adicional en élites+ |

### 7.3 Cofres

Los cofres tienen garantizado un mínimo de rareza:

| Tipo de Cofre | Rareza Mínima | Items | Probabilidad de rareza extra |
|:-------------:|:-------------:|:-----:|:----------------------------:|
| Básico (madera) | Común | 1-2 | 10% de ser Poco Común |
| Madera (Velmar+) | Poco Común | 2-3 | 20% de ser Raro |
| Hierro (N3+) | Raro | 2-3 | 15% de ser Épico |
| Plata (N5+) | Épico | 2-4 | 10% de ser Legendario |
| Oro (N8+) | Legendario | 3-4 | 5% de ser Mítico |

---

## 8. Sistema de Runas

Las armas y armaduras pueden tener slots de runa. Las runas son modificadores adicionales que se pueden insertar y extraer libremente.

### 8.1 Tipos de Runa

| Runa | Efecto | Slot | Rareza |
|:----:|--------|:----:|:------:|
| Runa de Fuerza | +3 Fuerza | Arma/Armadura | Raro |
| Runa de Destreza | +3 Destreza | Arma/Armadura | Raro |
| Runa de Vitalidad | +20 PV | Armadura | Raro |
| Runa de Velocidad | +5% velocidad de ataque | Arma | Épico |
| Runa de Crítico | +3% probabilidad de crítico | Arma | Épico |
| Runa de Robo de Vida | 2% de daño como PV | Arma | Legendario |
| Runa de Resistencia | +10 resistencia a un elemento | Armadura | Épico |
| Runa de Penetración | Ignora 5% de defensa enemiga | Arma | Legendario |

### 8.2 Extracción de Runas

Las runas insertadas se pueden extraer en cualquier herrero por 100 oro + 1 Fragmento de Hierro. No hay riesgo de perder la runa al extraerla.

---

## 9. Sistema de Transmog (Apariencia)

El jugador puede cambiar la apariencia de su equipo por la de cualquier otro item que haya encontrado antes.

| Mecánica | Detalle |
|----------|---------|
| Desbloqueo | Al recoger un item por primera vez, su apariencia se desbloquea permanentemente para transmog |
| Costo | 200 oro por pieza |
| Restricción | Solo se puede transmoggear dentro del mismo tipo de pieza (casco → casco, pecho → pecho) |
| Visual | El item mantiene sus estadísticas pero cambia su modelo y textura |
| NPC | El herrero de cualquier ciudad ofrece transmog |

---

## 10. Sistema de Vinculación

Algunos items se vinculan al personaje y no pueden ser comerciados ni vendidos.

| Tipo de Vinculación | Efecto |
|:-------------------:|--------|
| **Vinculado al recoger (BoP)** | Se vincula al personaje en cuanto se recoge. No se puede vender a otros jugadores (en caso de multijugador) ni intercambiar. Solo se puede vender a NPCs. |
| **Vinculado al equipar (BoE)** | Se puede comerciar libremente hasta que se equipa. Al equiparlo, se vuelve BoP. |
| **No vinculado** | Se puede comerciar, vender y equipar libremente. |

Items que son BoP:
- Todos los items únicos (sección 6)
- Items de sets completos (sección 2)
- Items Míticos
- Items de final (Medalla del Sol, Corona del Arquitecto)

Items que son BoE:
- Items Legendarios de drop normal
- Items Épicos de jefes

Items no vinculados:
- Items Comunes y Poco Comunes
- Items Raros de enemigos básicos

---

## 11. Economía de Equipo

### 11.1 Valor de Venta

| Rareza | % del valor de compra |
|:------:|:---------------------:|
| Común | 20% |
| Poco Común | 25% |
| Raro | 30% |
| Épico | 40% |
| Legendario | 50% |
| Mítico | 60% |

### 11.2 Valor de Compra Base

| Tipo de Item | Precio Base (Común) | Escala por Rareza |
|:------------:|:-------------------:|:-----------------:|
| Arma | 50-200 oro | ×1 (Común) → ×4 (Mítico) |
| Armadura (pecho) | 80-300 oro | ×1 → ×4 |
| Armadura (casco) | 50-200 oro | ×1 → ×4 |
| Armadura (guantes) | 30-100 oro | ×1 → ×4 |
| Armadura (botas) | 40-150 oro | ×1 → ×4 |
| Capa | 60-250 oro | ×1 → ×4 |
| Anillo | 100-400 oro | ×1 → ×4 |
| Amuleto | 150-500 oro | ×1 → ×4 |
| Cinturón | 80-300 oro | ×1 → ×4 |

### 11.3 Materiales de Forja

| Material | Rareza | Cómo obtenerlo | Precio de venta |
|:--------:|:------:|:--------------:|:---------------:|
| Fragmento de Hierro | Común | Rocas, enemigos de piedra, cofres básicos | 5 oro |
| Hierro | Poco Común | Mineral de hierro (N2+), enemigos metálicos | 15 oro |
| Acero | Raro | Mineral de acero (N4+), golems | 40 oro |
| Plata | Épico | Mineral de plata (N6+), Verdugo de Plata | 100 oro |
| Oro | Legendario | Mineral de oro (N8+), cofres de oro | 250 oro |
| Esencia de Fuego | Raro | Enemigos de fuego, hornos | 50 oro |
| Esencia de Hielo | Raro | Enemigos de hielo, zonas congeladas | 50 oro |
| Esencia Eléctrica | Raro | Enemigos eléctricos, tormentas | 50 oro |
| Esencia Elemental | Épico | Elementales, jefes elementales | 120 oro |
| Corazón de Monstruo | Legendario | Jefes de piso (N6+) | 300 oro |

---

## 12. Durabilidad y Degradación

### 12.1 Sistema de Durabilidad

Cada pieza de equipo tiene una durabilidad máxima que se reduce con el uso y las muertes.

| Rareza | Durabilidad Base |
|:------:|:----------------:|
| Común | 50 |
| Poco Común | 75 |
| Raro | 100 |
| Épico | 150 |
| Legendario | 200 |
| Mítico | 300 |

### 12.2 Pérdida de Durabilidad

| Acción | Durabilidad perdida |
|--------|:-------------------:|
| Recibir un golpe | 1 |
| Ser derribado | 3 |
| Morir | 5 (todas las piezas) |
| Usar un ataque especial | 0.5 por pieza de armadura (desgaste por movimiento) |
| Bloquear un golpe (escudo) | 2 |

### 12.3 Efectos de Durabilidad Baja

| Durabilidad restante | Efecto |
|:--------------------:|--------|
| 50-100% | Normal |
| 25-50% | -10% a estadísticas del item |
| 10-25% | -25% a estadísticas del item |
| 1-10% | -50% a estadísticas del item |
| 0% | El item no funciona. No da estadísticas. Debe repararse. |

### 12.4 Flags de Reparación

| Estado | Indicador visual en el inventario |
|:------:|:---------------------------------:|
| 100-75% | Sin indicador |
| 75-50% | Icono de herramienta pequeño (gris) |
| 50-25% | Icono de herramienta (amarillo) |
| 25-1% | Icono de herramienta (rojo, parpadeante) |
| 0% | Icono de "roto" (tachado) |

---

## 13. Glosario

| Término | Definición |
|---------|------------|
| **PV** | Puntos de Vida / Health Points (HP) |
| **SP** | Puntos de Stamina / Stamina Points. Se usan para atacar, bloquear y esprintar |
| **BoP** | Bind on Pickup — Vinculado al recoger |
| **BoE** | Bind on Equip — Vinculado al equipar |
| **Set** | Colección de piezas de equipo que otorgan bonificaciones adicionales al equipar múltiples piezas |
| **Rango** | Clasificación de poder del personaje (E, D, C, B, A, S). Determina qué pisos puedes enfrentar |
| **Slot de runa** | Ranura en un item donde se puede insertar una runa para obtener bonificaciones adicionales |
| **Transmog** | Transmogrification — Cambiar la apariencia de un item por otra sin cambiar sus estadísticas |
| **Durabilidad** | Medida de cuánto daño puede soportar un item antes de necesitar reparación |
| **Missable** | Item o contenido que se puede perder permanentemente si no se obtiene en el momento correcto |
| **NG+** | New Game Plus — Nueva partida con mayor dificultad y mejores drops |
| **Crafteo** | Creación de items usando materiales recolectados |
| **Forja** | Sistema de mejora, encantamiento y reparación de equipo |
| **Peso (armadura)** | Clasificación de armadura (Ligera/Media/Pesada) que afecta velocidad y consumo de stamina |
| **Final A** | Final heroico — El jugador derrota al Arquitecto y restaura el ciclo del sol |
| **Final B** | Final del Arquitecto — El jugador reemplaza al Arquitecto y mantiene el orden actual |
| **Elemento** | Tipo de daño mágico (Fuego, Hielo, Eléctrico, Oscuro, Sagrado) que puede añadirse a armas y resistirse con armadura |

---

> **Documento actualizado:** Abril 2026  
> **Próximas revisiones:** Sets post-N10 (contenido descargable), sistema de gemas para accesorios, sistema de encantamientos avanzados.
