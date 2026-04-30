# Sistema de Combate — SOLO (Sunlight of the Last Online)

> **Versión:** 1.0  
> **Motor:** Unreal Engine 5.7  
> **Género:** RPG medieval 3D en tiempo real  
> **Filosofía de combate:** Táctico pero fluido. Punto medio entre soulslike (demasiado castigador) y hack-and-slash puro (demasiado caótico). El jugador debe pensar antes de actuar, pero sin sentirse penalizado excesivamente por errores. Se premia la lectura de patrones, el posicionamiento y la gestión de recursos (SP/MP) por encima de los reflejos puros.

---

## Índice

1. [Fundamentos](#1-fundamentos)
2. [Sistema de Daño](#2-sistema-de-daño)
3. [Sistema de Postura (Romper Defensas)](#3-sistema-de-postura-romper-defensas)
4. [Combos y Encadenamientos](#4-combos-y-encadenamientos)
5. [Habilidades de Clase en Combate](#5-habilidades-de-clase-en-combate)
6. [Compañeros en Combate (AI)](#6-compañeros-en-combate-ai)
7. [Sistema de Jefe](#7-sistema-de-jefe)
8. [Estado y Buffs durante Combate](#8-estado-y-buffs-durante-combate)
9. [Sistema de Muerte y Respawn](#9-sistema-de-muerte-y-respawn)
10. [Apéndice: Tablas de Referencia Rápida](#10-apéndice-tablas-de-referencia-rápida)

---

## 1. Fundamentos

### 1.1 Filosofía General

SOLO busca un combate **deliberado pero ágil**. El jugador dispone de tres recursos principales (HP, MP, SP) que debe gestionar activamente. No hay "combo infinito" ni dependencia exclusiva de reflejos. El énfasis está en:

- **Lectura del enemigo:** Identificar ataques telegrafiados y elegir entre bloquear, esquivar, parry o contraatacar.
- **Gestión de Aguante (SP):** Gastar SP para atacar, bloquear y esquivar significa que el jugador nunca puede hacerlo todo a la vez. Atacar agresivamente deja sin defensa. Defenderse pasivamente impide generar daño.
- **Posicionamiento táctico:** Flanqueos, retrocesos medidos, aprovechar el entorno.
- **Coordinación con compañeros:** Dar órdenes, aprovechar sinergias, cubrir debilidades mutuas.
- **Ritmo de batalla:** Alternar entre fases ofensivas (gastar SP para romper postura) y defensivas (regenerar SP, esperar opening).

### 1.2 Estadísticas Base del Personaje

| Estadística | Fórmula / Valor Base | Notas |
|---|---|---|
| **Salud (HP)** | 100 + (Nivel × 20) | 0 HP = caída. Recuperación pasiva fuera de combate. |
| **Maná (MP)** | 50 + (Nivel × 10) | Recurso mágico. Clases no-mágicas tienen 0 MP base. |
| **Aguante (SP)** | 100 base (no escala por nivel) | Recurso universal para ataques físicos, bloqueo y esquiva. |
| **Daño Físico** | Fuerza × Multiplicador de arma | El arma define el multiplicador base. |
| **Daño Mágico** | Inteligencia × Multiplicador de catalizador | Varita, bastón o foco mágico. |
| **Defensa Física** | Armadura × (1 + Aguante/200) | Aguante contribuye un 0.5% de defensa adicional por punto. |
| **Defensa Mágica** | Resistencia × (1 + Inteligencia/200) | Inteligencia aporta resistencia mágica pasiva. |
| **Velocidad de Ataque** | Base de arma / (1 + Destreza/300) | Destreza acelera el animation speed del arma. |
| **Velocidad de Movimiento** | 100% base + modificador de clase + botas/sets | Rango típico: 90%-125%. |

#### 1.2.1 Desglose de Atributos

- **Fuerza (FUE):** Aumenta daño físico. 1 punto = +2 daño base físico. Requisito para armas pesadas.
- **Destreza (DES):** Aumenta velocidad de ataque, probabilidad crítica leve (0.1% por punto), precisión. Requisito para armas ligeras y arcos.
- **Inteligencia (INT):** Aumenta daño mágico (1 punto = +3 daño mágico base). Aporta defensa mágica pasiva. Requisito para hechizos y catalizadores.
- **Aguante (AGU):** Aumenta SP total (1 punto = +5 SP máximo), defensa física pasiva. Reduce costo de SP en acciones.
- **Resistencia (RES):** Aumenta HP (1 punto = +10 HP adicional), defensa mágica. Reduce duración de estados negativos.

#### 1.2.2 Progresión por Nivel

- Subir de nivel otorga: +20 HP, +10 MP, +5 SP, +2 puntos de atributo para distribuir.
- Atributos disponibles: Fuerza, Destreza, Inteligencia, Aguante, Resistencia.
- **Nivel máximo:** ∞ (sin techo de nivel). Sin embargo, los rangos E-SSS de equipo y habilidades crean una progresión vertical real.
- Cada 10 niveles se desbloquea un espacio de habilidad adicional.
- Cada 25 niveles se desbloquea un espacio de "maestría" pasiva.

#### 1.2.3 Recuperación de Recursos

| Recurso | Recuperación Pasiva | Condiciones Especiales |
|---|---|---|
| **HP** | 1% del HP máximo por segundo | Solo fuera de combate. 5 segundos después del último daño recibido. |
| **HP (combate)** | 0% (no se regenera en combate) | Solo por habilidades, pociones o compañeros. |
| **MP** | 2% del MP máximo por segundo constante | Nunca se detiene. Se reduce a 1%/s si el medidor de MP está por debajo del 10%. |
| **SP** | 3% del SP máximo por segundo constante | Aumenta a 5%/s si no se ha atacado, bloqueado o esquivado en los últimos 2 segundos. |
| **Pociones** | Instantáneo o gradual según tipo | Ver sección de objetos de combate. |

**Nota de diseño:** El SP es el recurso central de toma de decisiones. Se regenera rápido para fomentar acción constante, pero se vacía igual de rápido si se abusa del bloqueo o los ataques fuertes. El jugador debe alternar momentos ofensivos y defensivos para optimizar su uso.

---

## 2. Sistema de Daño

### 2.1 Fórmulas de Daño

#### Daño Físico

```
Daño_Físico = (Ataque_base × Multiplicador_golpe) 
              × (1 - Defensa_enemigo / (Defensa_enemigo + 300)) 
              × Critico 
              × Bonuses
```

Donde:
- **Ataque_base =** Fuerza × Multiplicador de arma.
- **Multiplicador_golpe =** Varía según el tipo de ataque: débil (0.8×), combo (1.0×-1.5× según cuándo en la cadena), fuerte (1.8×-2.5×), cargado (3.0×-5.0×).
- **Defensa_enemigo =** Defensa física del objetivo.
- **Critico =** 1.0 si no es crítico, 1.5+ si es crítico.
- **Bonuses =** Buffs, debuffs elementales, sinergias (ej. ×1.25 por vulnerabilidad elemental).

La fórmula de reducción `Defensa / (Defensa + 300)` produce rendimientos decrecientes: 100 de defensa = 25% reducción, 300 = 50%, 900 = 75%. Así, invertir en defensa nunca vuelve inmune al jugador pero da un beneficio tangible.

#### Daño Mágico

```
Daño_Mágico = (Poder_mágico × Multiplicador_hechizo) 
              × (1 - Res_mágica_enemigo / (Res_mágica + 300)) 
              × Bonuses_elementales
```

Donde:
- **Poder_mágico =** Inteligencia × Multiplicador de catalizador.
- **Multiplicador_hechizo =** Varía por hechizo (ej. Bola de Fuego = 2.5×, Chispa = 0.8×, Meteoro = 6.0×).
- **Res_mágica_enemigo =** Resistencia mágica del objetivo.
- **Bonuses_elementales =** Multiplicador por debilidad o resistencia elemental del objetivo.

#### Daño de Estado (DoT)

```
Daño_DoT = Daño_base_del_estado × (1 + Poder_aplicador/100) × Cargas_activas
```

Los DoT (Damage over Time) se calculan con su propia fórmula separada para evitar doble-dipping con defensas. Solo se ven afectados por el poder del aplicador y el número de cargas.

### 2.2 Tipos de Daño

SOLO cuenta con 10 tipos de daño, divididos en físicos, elementales y especiales.

#### 2.2.1 Daño Físico

| Tipo | Armas Principales | Efectivo Contra | Débil Contra | Notas |
|---|---|---|---|---|
| **Cortante** | Espadas, dagas, katanas, hachas de una mano | Carne, monstruos orgánicos | Armadura pesada, esqueletos | El tipo más común. |
| **Contundente** | Martillos, mazas, hachas grandes, puños | Armadura, constructos, roca | Carne, bestias ágiles | Rompe postura 20% más efectivo. |
| **Perforante** | Flechas, lanzas, estocadas, ballestas | Piel dura, escamas | Escudos grandes, armadura reflectante | 1.0× efectividad contra todo (versátil). |

**Sobretipo Físico — Cortante vs Armadura:** Cuando un ataque cortante golpea una armadura con valor mayor a 50, su daño se reduce un 30% adicional (simulando que el filo no penetra bien el metal). Los ataques perforantes no sufren esta penalización, y los contundentes ganan +15% de daño contra armadura.

#### 2.2.2 Daño Elemental

| Tipo | Efectivo Contra | Débil Contra | Efecto Secundario |
|---|---|---|---|
| **Fuego** | Carne, vegetación, hielo | Fuego, constructos, demonios de fuego | Quemado (DoT). |
| **Hielo** | Fuego, bestias de sangre caliente | Hielo, elementales de hielo | Congelado (ralentiza). |
| **Eléctrico** | Metal, agua, constructos mecánicos | Eléctrico, bestias de tormenta | Daño adicional +50% si el enemigo está en agua/metal conductivo. |
| **Veneno** | Carne, bestias, humanos | Veneno, no-muertos, constructos | Envenenado (DoT + acumulación). |

**Interacciones Elementales:**

- **Fuego + Agua:** Si un enemigo está mojado (bajo agua, bajo lluvia, o mojado por habilidad), el daño de fuego se reduce 40%, pero se genera vapor que ciega (-30% precisión por 3s).
- **Hielo + Agua:** Mojado + Hielo = Congelado instantáneo (3s, inmovilización total).
- **Eléctrico + Agua:** Mojado + Eléctrico = daño eléctrico ×2.0 y stun 1s adicional.
- **Fuego + Veneno:** Quemado + Envenenado = Combustión (daño adicional ×1.5 cada tick, ambas cargas se consumen).
- **Hielo + Eléctrico:** Congelado + Eléctrico = Sobrecarga (explosión AoE que daña a enemigos cercanos, consume ambos estados).

#### 2.2.3 Daño Especial

| Tipo | Efectivo Contra | Débil Contra | Notas |
|---|---|---|---|
| **Sagrado** | No-muertos, sombras, demonios | Vivos, bestias sagradas | Cura a aliados con afinidad sagrada en lugar de dañar. |
| **Oscuro** | Vivos, curanderos, ángeles | Oscuridad, no-muertos altos | Ignora 30% de defensa mágica. |
| **Tejido** | Todo (ignora defensas) | Escudos mágicos (barrera) | Solo habilidades del Tejido (rama especial de habilidades). No escala con atributos — daño fijo por nivel de habilidad. |

### 2.3 Ataque Crítico

- **Probabilidad base:** 5% (sin modificadores).
- **Daño crítico base:** 1.5× (multiplica el daño final después de defensa).
- **Modificadores:**
  - Clase Asesino: hasta 25% prob., 3.0× daño.
  - Equipo: bufandas, anillos, armas con bonus de crítico.
  - Buffs: habilidades que aumentan prob. crítica temporalmente.
  - Atributo: Destreza aporta +0.1% prob. crítica por punto.

- **Crítico garantizado cuando:**
  - Golpe por detrás (ángulo de 120° detrás del enemigo).
  - Enemigo aturdido o postura rota.
  - Ventana de contraataque tras parry exitoso.
  - Habilidad específica lo declara (ej. "Golpe Certero" del Asesino).

- **Crítico de Habilidad:** Las habilidades pueden tener su propio multiplicador de crítico (ej. una habilidad definitiva puede hacer crítico a 2.5× en lugar de 1.5×).

### 2.4 Golpes Débiles y Fuertes

#### Golpe Débil (Botón izquierdo / R1)
- Rápido, bajo daño, bajo costo de SP (5 SP por golpe).
- Sirve para acumular combo (ver sección 4).
- Se puede encadenar en secuencias de 3-5 golpes según el arma.
- Ideal para: probar defensas, acumular postura, mantener presión, enemigos débiles.

#### Golpe Fuerte (Botón derecho / R2)
- Lento, alto daño, alto costo de SP (20 SP por golpe).
- Rompe postura significativamente (+50% más daño a postura que golpe débil).
- Se puede mantener para cargar (ver abajo).
- Ideal para: romper defensas, castigar openings grandes, enemigos con escudo.

#### Carga (Mantener Golpe Fuerte)
- Mecánica: Mantener el botón de golpe fuerte inicia una carga que consume SP gradualmente (5 SP por segundo cargando).
- Al soltar, se libera un golpe potenciado.
- Daño escala con el tiempo de carga:
  - Carga parcial (0.5-1s): 1.5× daño, gasta 25 SP.
  - Carga media (1-2s): 2.5× daño, gasta 40 SP.
  - Carga completa (2-3s): 4.0× daño, gasta 60 SP. Derriba enemigos medianos o menores.
  - Sobrecarga (>3s): El arma "chisporrotea" visualmente. A los 4s el golpe se cancela automáticamente y el SP gastado no se reembolsa.
- **Movimiento durante carga:** 50% de velocidad de movimiento. No se puede esquivar mientras se carga (el jugador se "compromete").
- **Interrupción:** Si recibes daño mientras cargas, la carga se interrumpe y pierdes el SP acumulado.

### 2.5 Golpe Descargado (Aturdimiento / Knockback)

Cada arma tiene un "golpe descargado" especial que se activa cuando:
- El enemigo está con postura rota + realizas un golpe fuerte.
- Realizas un ataque cargado completo contra un enemigo de tamaño menor o igual.

El golpe descargado varía por arma:
- **Espada:** Estocada que empala al enemigo, lo levanta y lo lanza hacia atrás.
- **Martillo:** Golpe descendente que entierra al enemigo en el suelo (noqueado 3s).
- **Lanza:** Barrido que derriba a todos los enemigos en un cono frontal.
- **Daga:** Salto y puñalada descendente, clavando al enemigo al suelo (garantiza crítico).
- **Arco:** Flecha cuerda completa que atraviesa al enemigo y daña a los que están detrás.

---

## 3. Sistema de Postura (Romper Defensas)

### 3.1 Concepto Base

Cada entidad combativa (jugador, compañero, enemigo, jefe) posee una **Barra de Postura**, representada visualmente como una barra de color sobre la barra de HP (estilo _Sekiro_ pero más sutil — no domina la interfaz).

| Estado de Postura | Efecto |
|---|---|
| **Postura llena (100%)** | Puede bloquear, parry y esquivar normalmente. |
| **Postura parcial (1-99%)** | El jugador recibe 2% más de daño por cada 10% de postura perdida, hasta 20% adicional. |
| **Postura rota (0%)** | Recibe 50% más de daño. No puede bloquear ni parry. Vulnerable a derribo. 50% de probabilidad de que el próximo golpe fuerte lo derribe. |
| **Recuperando postura (0-100%)** | La postura se regenera al 10%/s si no se recibe daño en 3 segundos consecutivos. |

#### Postura de Jefes
- Los jefes tienen postura 2× a 4× mayor que un enemigo normal de su nivel.
- Regeneración de postura más rápida: 15%/s en fase 1, 10%/s en fase 2, 5%/s en fase de enrage.
- Romper la postura de un jefe otorga una **ventana de daño masivo de 5 segundos** donde no puede atacar, bloquear ni moverse. Indicador visual: brillo dorado en el jefe y sonido de "break".

### 3.2 Bloqueo

- **Activación:** Mantener presionado el botón de bloqueo (L1 / LB / Shift derecho).
- **Reducción de daño:** 60% de daño físico reducido. 20% de daño mágico reducido.
- **Costo de SP:** 5 SP por segundo mientras se mantiene el bloqueo.
- **Postura:** Mientras bloqueas, tu postura no se regenera.
- **Postura Rota durante Bloqueo:** Si SP llega a 0 mientras bloqueas, tu postura se rompe automáticamente y quedas aturdido 2s (la guardia literalmente "revienta").
- **Ángulo de bloqueo:** Solo bloquea ataques que vienen del frente (cono de 180°). Ataques por detrás no se bloquean.
- **Ataques Imbloqueables:** Indicados con color rojo (ver sección 7.3). No se pueden bloquear. Ignoran el 60% de reducción.

### 3.3 Parry / Contraataque

El Parry es la mecánica de mayor riesgo/recompensa del combate.

- **Activación:** Presionar bloqueo en el momento exacto en que el ataque enemigo conectaría (ventana de 200ms).
- **Efectos inmediatos del Parry exitoso:**
  1. Reembolsa el SP gastado (el bloqueo no cuesta SP si es parry).
  2. Aturde al enemigo por 2 segundos.
  3. El enemigo queda vulnerable a un **contraataque garantizado** (golpe automático o habilidad asignada al botón de ataque durante la ventana de 1s posterior al parry).
  4. El contraataque posterior hace daño crítico garantizado (×1.5 mínimo).
- **Limitaciones del Parry:**
  - No funciona contra ataques de área (AoE) — el personaje no puede parry una explosión.
  - No funciona contra ataques de jefe marcados como "imparables" (indicador púrpura con marco negro).
  - Tiene un enfriamiento interno de 1s (no se puede parry dos veces seguidas al mismo enemigo).
  - No se puede mantener el bloqueo y parry al mismo tiempo — es una acción de timing, no de mantenimiento.
- **Parry Mágico:** Algunas clases (Paladín, Hechicero de Batalla) tienen la habilidad de parry hechizos. Funciona igual pero con ventana más reducida (150ms).

### 3.4 Esquiva

- **Activación:** Botón de esquiva (círculo / B / Space).
- **Tipo:** Dash corto en la dirección del input (4 direcciones + diagonales). Distancia: 4-6 metros.
- **Frames de invencibilidad (I-frames):** 0.2 segundos al inicio de la animación de esquiva.
  - El resto de la animación (0.3-0.5s) no tiene invencibilidad. El jugador puede ser golpeado durante el final de la esquiva.
  - **Backstep:** Doble tap hacia atrás realiza un backstep más corto (2m) pero con I-frames extendidos a 0.35s.
- **Costo:** 15 SP por esquiva.
- **Enfriamiento:** 0.5 segundos entre esquivas consecutivas.
- **Esquivas de Clase:**
  - Asesino: Esquiva sin costo de SP. I-frames 0.3s.
  - Centinela: Esquiva más lenta (menos distancia) pero con superarmadura durante la animación.
  - Mago: Teletransporte corto (6m) en lugar de dash. Gasta MP en lugar de SP. I-frames 0.4s pero enfriamiento de 2s.

### 3.5 Estrategias de Defensa

| Situación | Mejor Opción | Segunda Opción | No Recomendado |
|---|---|---|---|
| Ataque enemigo lento, telegrafiado | Parry | Esquiva lateral | Bloquear (gasta SP innecesario) |
| Ataque rápido, poco daño | Bloquear | Esquiva hacia atrás | Parry (ventana muy ajustada) |
| Ataque imbloqueable (rojo) | Esquivar | Interrumpir con ataque fuerte si es posible | Bloquear (no funciona) |
| Ataque de área grande | Esquiva con backstep | Salto + esquiva aérea (gana altura) | Parry (no funciona contra AoE) |
| Lluvia de ataques (ráfaga) | Bloquear los primeros, esquivar el último | Retroceder y esperar | Parry secuencial (demasiado riesgo) |
| Jefe en fase de enrage | Combinar bloqueo + esquiva según patrón | Usar superarmadura + atacar agresivamente | Solo defender (el jefe romperá tu postura) |

---

## 4. Combos y Encadenamientos

### 4.1 Sistema de Combos Ligeros

Cada arma tiene una secuencia de 3-5 golpes que se ejecutan presionando repetidamente el botón de golpe débil.

| Número de Golpes | Armas |
|---|---|
| 3 golpes | Martillos, hachas grandes, lanzas |
| 4 golpes | Espadas, mazas, arcos |
| 5 golpes | Dagas, katanas, puños (sin armas) |

**Reglas del Combo Ligero:**
- **Progresión de daño:** 1.0× → 1.0× → 1.2× → 1.5× (si aplica) → 2.0× (último golpe).
- **Costo de SP:** Cada golpe cuesta 5 SP. El último golpe cuesta 10 SP adicionales.
- **Cancelación:** Se puede cancelar la cadena en cualquier momento con esquiva, bloqueo o parry.
- **Commit:** El último golpe de la cadena NO se puede cancelar — una vez iniciado, el jugador debe completar la animación. El diseño fuerza decisiones: ¿sacrifico el último golpe (alto daño) por seguridad?
- **Reset:** Si el combo se cancela (esquiva o bloqueo), el contador de combo se reinicia.
- **Tiempo de input:** Hay una ventana de 0.4s entre golpes para presionar el siguiente botón. Si se excede, el combo termina y vuelve al estado neutral.

### 4.2 Combos Pesados

Secuencia de 2-3 golpes usando el botón de golpe fuerte.

| Número de Golpes | Armas |
|---|---|
| 2 golpes | Armas rápidas (dagas, katanas, espadas) |
| 3 golpes | Armas lentas (martillos, hachas, lanzas) |

**Reglas del Combo Pesado:**
- **Daño por postura:** Cada golpe pesado hace ×1.5 daño a postura en lugar de ×1.0. El tercer golpe hace ×2.5 daño a postura.
- **Costo de SP:** Primer golpe 20 SP, segundo 25 SP, tercero 30 SP.
- **Derribo:** El último golpe de un combo pesado derriba automáticamente a enemigos de tamaño mediano o menor.
- **No cancelable:** Un combo pesado no se puede cancelar una vez iniciado — es un compromiso total.
- **Especial:** Si el combo pesado se conecta completamente (todos los golpes), el enemigo recibe -30% de postura adicional como bonus.

### 4.3 Ataques en Salto

| Ataque | Input | Daño | Efecto | Coste SP |
|---|---|---|---|---|
| **Golpe en salto** | Salto + golpe débil | 1.3× | Derriba enemigos pequeños | 10 SP |
| **Ataque descendente** | Salto + golpe débil (en el aire, apuntando abajo) | 1.5× | Stun 1s | 15 SP |
| **Patada aérea** | Salto + golpe fuerte | 1.0× (daño fijo) | Empuja al enemigo 5m hacia atrás | 10 SP |
| **Barrido aéreo** | Salto + ataque hacia abajo con arma larga | 1.2× (cono) | Derriba enemigos en cono frontal | 20 SP |

**Notas de movimiento aéreo:** Mientras estás en el aire, no puedes bloquear ni esquivar. Eres completamente vulnerable. Los ataques aéreos son de alto riesgo y deben usarse con cuidado, especialmente contra jefes.

### 4.4 Ataque Cargado Corriendo

- **Input:** Correr (mantener hacia adelante + botón de correr) + golpe fuerte.
- **Efecto:** Carga frontal a máxima velocidad, cubriendo 8-12 metros. Derriba a todos los enemigos en el camino (no aplica a jefes, solo los detiene).
- **Daño:** 1.5× del daño base del arma.
- **Costo:** 30 SP (se gasta al iniciar la carga, no al conectar).
- **Paredes:** Si chocas contra una pared durante la carga, recibes un stun propio de 1s (el personaje "rebota"). No hay daño colateral.
- **Usos tácticos:**
  - Atravesar grupos de enemigos pequeños para aislar al jefe.
  - Interrumpir hechizos de enemigos canalizadores.
  - Escapar de un cerco (la carga tiene I-frames parciales durante los primeros 0.3s).
  - Abrir combate con ventaja posicional.

### 4.5 Ataque de Salto (Enganche / Grapple)

Algunas clases y armas tienen acceso a un **ataque de salto-enganche**:

- **Input:** Botón interactuar + objetivo en rango (8-12m).
- **Efecto:** El personaje se lanza hacia el enemigo, aterrizando con un golpe. No hace daño, pero pone al jugador cuerpo a cuerpo instantáneamente.
- **Costo:** 5 SP.
- **Uso:** Cerrar distancia rápidamente contra arqueros o magos. Reubicación táctica.

### 4.6 Cancelaciones Avanzadas

Técnicas opcionales para jugadores avanzados (no requeridas para completar el juego, pero útiles para optimización):

| Técnica | Input | Efecto | Coste |
|---|---|---|---|
| **Wave Cancel** | Ataque → esquiva en el frame exacto de impacto | Cancelas el recovery del ataque pero el daño aún aplica. Permite encadenar ataques más rápido. | Doble SP del ataque original (el ataque + la esquiva) |
| **Jump Cancel** | Ataque → salto en el frame exacto | Similar a wave cancel pero en vertical. Permite iniciar ataques aéreos más rápido. | Coste del salto + ataque |
| **Parry Cancel** | Ataque → parry | Cancelas un ataque para hacer un parry instantáneo. Útil si leíste mal al enemigo. | Coste del ataque original (se desperdicia) + parry |
| **Guard Cancel** | Bloqueo → ataque en el primer frame de bloqueo | Cancelas la animación de bloqueo en un ataque rápido. | Coste de ataque + 1s de bloqueo (5 SP) |
| **Sprint Cancel** | Último golpe de combo → correr inmediatamente | Eliminas el recovery del último golpe con una animación de carrera. | 10 SP extra |

**Nota de diseño:** Las cancelaciones avanzadas son intencionalmente costosas en SP para evitar que el combate se convierta en un juego de ritmo puro. El SP es el limitador: puedes hacer movimientos increíblemente rápidos, pero te quedarás sin defensa inmediatamente después.

---

## 5. Habilidades de Clase en Combate

### 5.1 Estructura General

Cada clase tiene acceso a **6 habilidades activas** de su árbol. De estas, el jugador puede equipar **4 en la barra rápida** (accesibles con L2/LT + botones de acción, o teclas 1-4 en PC).

- **Cambio entre combates:** Las habilidades equipadas pueden cambiarse libremente en fogatas o ciudades.
- **En combate:** No se pueden cambiar habilidades equipadas. La selección es compromiso táctico.

### 5.2 Costos de Habilidades

| Tipo de Habilidad | Recurso Principal | Costo Típico | Otros Costos |
|---|---|---|---|
| **Físicas** (golpes, cortes, flechas) | SP | 10-50 SP | — |
| **Mágicas** (hechizos, conjuros) | MP | 10-80 MP | Algunas requieren componentes (piedras de maná). |
| **Híbridas** (habilidades de clases mixtas) | SP + MP | Mitad de cada recurso | — |
| **Definitivas** (Ultimates) | 100% del recurso | SP o MP total | Enfriamiento de 2-5 minutos. No se puede reducir con equipo. |
| **Pasivas** | — | — | Ocupan espacio de habilidad pero no requieren activación. |

### 5.3 Tiempo de Lanzamiento

| Categoría | Tiempo | Ejemplos | Interrumpible |
|---|---|---|---|
| **Instantáneo** | 0s | Golpe básico, contraataque post-parry, auto-buff instantáneo | No |
| **Rápido** | 0.5-1s | Corte veloz, flecha básica, chispa mágica, patada | Sí (por daño) |
| **Medio** | 1-2s | Hechizo de proyectil, carga de arma, salto de combate | Sí (por daño) |
| **Lento** | 2-4s | Invocación, ritual, canalización mayor, ultimate | Sí (por daño, excepto superarmadura) |
| **Cargable** | Variable (ver sección 2.4) | Carga de golpe fuerte, hechizo cargado | Sí (por daño, se pierde el progreso de carga) |

**Superarmadura durante lanzamiento:** Las habilidades lentas y algunas habilidades medias otorgan superarmadura durante su animación de lanzamiento. El personaje no puede ser interrumpido por daño, pero SÍ puede ser interrumpido por aturdimiento, derribo o miedo.

### 5.4 Sinergias en Combate

Las sinergias son combinaciones de habilidades entre el protagonista y los compañeros que producen efectos adicionales.

#### Reglas de Sinergia
1. **Activación:** El jugador usa una habilidad marcada con [Sinergia] mientras un compañero está usando su habilidad complementaria en el mismo objetivo o área.
2. **Ventana de sinergia:** 3 segundos. Si el compañero usó su habilidad hace menos de 3s, la sinergia se activa automáticamente.
3. **Notificación:** Un indicador visual (chispa dorada conectando al personaje y compañero) y un sonido especial anuncian la sinergia activada.
4. **Enfriamiento de sinergia:** Cada par de sinergia tiene un enfriamiento de 10-15s entre usos exitosos.

#### Tabla de Sinergias (Ejemplos)

| Habilidad del Jugador | Compañero | Habilidad del Compañero | Efecto de Sinergia | Enfriamiento |
|---|---|---|---|---|
| Golpe Sagrado (Guerrero) | Paladín (compañero clase historia) | Condenar | +50% daño sagrado, cura 10% HP del grupo | 15s |
| Bola de Fuego (Mago) | Druida | Zarzas | Área en llamas persistente 5s (DoT fuego en zona) | 12s |
| Flecha de Escarcha (Arquero) | Mira | Flecha Penetrante | Proyectil que congela en trayectoria + perfora escudos | 10s |
| Carga de Escudo (Centinela) | Vassen | Provocación | Aturde a todos los enemigos en 5m durante 2s | 15s |
| Puñalada Sombra (Asesino) | Garrik | Golpe por la Espalda | Crítico garantizado 4.0×, aplica Veneno | 12s |
| Tormenta de Cuchillas (Guerrero) | Sera | Carga de Velocidad | Remolino de cuchillas que persigue al enemigo 8m | 18s |
| Barrera de Luz (Paladín) | Sacerdote (compañero clase historia) | Curación Mayor | Barrera + curación gradual 5% HP/s por 8s | 20s |
| Ráfaga de Viento (Mago) | Arquero | Lluvia de Flechas | Proyectiles dispersos que cubren área grande | 15s |

#### Cómo Descubrir Sinergias
- El juego no revela las sinergias explícitamente en el tutorial. Se descubren mediante:
  - **Experimentación:** Probar combinaciones de habilidades durante el combate.
  - **Diálogo:** Conversaciones con compañeros en fogatas pueden dar pistas.
  - **Bestiario:** Enemigos derrotados dejan notas que mencionan combinaciones.
  - **Logros:** Algunas sinergias específicas están vinculadas a logros.
- Una vez descubierta, la sinergia queda registrada en el Menú de Combate (sección Sinergias Conocidas).

### 5.5 Habilidades Definitivas (Ultimates)

Cada clase tiene una habilidad definitiva única, que gasta el 100% del recurso principal (SP o MP).

| Clase | Ultimate | Efecto | Recurso | Enfriamiento |
|---|---|---|---|---|
| Guerrero | **Furia del Sol** | Golpe en área 8m, 8.0× daño, derriba todo. | 100% SP | 4 min |
| Paladín | **Santuario** | Zona 10m donde aliados reciben 50% menos daño y regeneran 5% HP/s por 10s. | 100% MP | 5 min |
| Asesino | **Sombra Eterna** | Teletransporte a la espalda del enemigo, 6 golpes rápidos (4.0× total), invisible 3s después. | 100% SP | 3 min |
| Mago | **Meteoro** | Proyectil celestial que impacta área 12m, 10.0× daño, quema el suelo 8s. | 100% MP | 5 min |
| Arquero | **Lluvia Estelar** | 30 flechas en área 15m durante 5s, cada flecha 1.5×. | 100% SP | 4 min |
| Centinela | **Muro Inquebrantable** | Escudo gigante frontal 6m ancho, bloquea todo daño por 5s, aliados detrás reciben 30% menos daño. | 100% SP | 4 min |
| Druida | **Tormenta Natural** | Plantas, rayos y viento en área 12m, daño 2.0×/s por 6s, ralentiza 50% a enemigos. | 100% MP | 4 min |
| Hechicero de Batalla | **Infusión Arcana** | Arma imbuida de todos los elementos, cada golpe aplica un estado diferente por 8s. | 50% SP + 50% MP | 3 min |

**Reglas de Ultimate:**
- No se puede interrumpir al jugador durante la animación de ultimate (superarmadura total).
- Si el jugador recibe daño letal durante la animación de ultimate, el ultimate se completa y luego el personaje muere (prioriza el ultimate).
- Compañeros no usan sus ultimates automáticamente. Se deben ordenar mediante la rueda de comandos "Habilidad especial".
- Los ultimates no escalan con equipo de la misma forma que otras habilidades — su daño base es fijo por nivel de habilidad.

---

## 6. Compañeros en Combate (AI)

### 6.1 Filosofía de Compañeros

Los compañeros en SOLO no son simples esbirros que siguen al jugador. Cada uno tiene:
- **Personalidad de combate:** Preferencias tácticas innatas que determinan su comportamiento base.
- **Órdenes tácticas:** Comandos específicos que el jugador puede dar para override el comportamiento.
- **Nivel de confianza (N):** El rango de niveles en los que el compañero está disponible y qué tan efectivo es.
- **Eventos de historia:** Momentos donde el compañero actúa independientemente.

### 6.2 Mira (Rango N0-N7) — Arquera

**Rol:** Ranged DPS / Apoyo

| Aspecto | Descripción |
|---|---|
| **Comportamiento base** | Se mantiene a 8-15m del enemigo principal. Prioriza la supervivencia sobre el daño. |
| **Prioridad de objetivo** | Enemigos que están atacando al protagonista (fuego de cobertura). Secundario: enemigos a distancia. |
| **Habilidad automática** | Flecha Penetrante contra enemigos con escudo (reduce su defensa de bloqueo 40% por 3s). |
| **En batalla de jefe** | Dispara flechas de apoyo cada 5s automáticamente (daño 1.2×, no gasta recursos). |
| **Defensa personal** | Esquiva hacia atrás si un enemigo se acerca a menos de 4m. No puede bloquear (no tiene escudo). |

**Órdenes Específicas para Mira:**
| Orden | Efecto | Enfriamiento |
|---|---|---|
| "Apuntar a [objetivo]" | Mira cambia su prioridad al objetivo indicado. Mantiene el foco hasta nueva orden o que el objetivo muera. | 3s (solo para cambiar) |
| "Cubrir [compañero]" | Mira dispara a cualquier enemigo que ataque al compañero indicado. Reduce su propio daño en 30% pero protege al objetivo. | — |
| "Disparo de precisión" | Mira canaliza 2s y dispara un proyectil de 4.0× daño. Gasta 2 flechas de su inventario (si no tiene, no puede usarlo). | 15s |
| "Flecha de escarcha" (desbloqueable N5+) | Mira imbuye su siguiente flecha con hielo. Ralentiza 40% por 4s. | 12s |

**Nota de diseño:** Mira está diseñada para ser "fácil de usar" — sigue al jugador, dispara a lo que él ataca, y se mantiene a salvo. El jugador novato puede ignorarla y seguirá siendo útil. El jugador avanzado la optimiza con órdenes específicas.

### 6.3 Vassen (Rango N0-N7) — Tanque / Centinela

**Rol:** Tanque / Defensa / Protección

| Aspecto | Descripción |
|---|---|
| **Comportamiento base** | Se posiciona entre el enemigo más peligroso y el grupo. Mantiene distancia de 3-5m del enemigo. |
| **Provocación automática** | Vassen provoca automáticamente a 2-3 enemigos cercanos cada 8s. Los enemigos provocados lo atacan a él por 4s. |
| **Bloqueo de protección** | Si un ataque se dirige al protagonista y Vassen está a menos de 3m, se interpone y bloquea el ataque (recibe el daño completo, pero reduce el daño al jugador en 80%). |
| **Defensa personal** | Vassen tiene un escudo grande y bloquea pasivamente el 50% del daño frontal. No esquiva. |
| **Habilidad automática** | Grito de Batalla cada 12s: aumenta su defensa 20% por 5s y atrae a todos los enemigos en 8m. |

**Órdenes Específicas para Vassen:**
| Orden | Efecto | Enfriamiento |
|---|---|---|
| "Proteger a [compañero]" | Vassen se mueve para cubrir al compañero indicado. Prioriza su defensa sobre cualquier otra acción. | — |
| "Cargar" | Vassen corre hacia el objetivo señalado, derribando enemigos en el camino (igual que ataque cargado del jugador). Gasta su SP. | 10s |
| "Formación defensiva" | Vassen levanta su escudo y se planta. Reduce daño recibido 80% pero no se mueve ni ataca. Dura 5s o hasta que se ordene "suelta". | 20s |
| "Escudo protector" (desbloqueable N5+) | Vassen extiende su escudo para cubrir a un aliado cercano. Ambos reciben 50% menos daño frontal por 4s. | 25s |

**Nota de diseño:** Vassen es casi indispensable para el jugador novato y útil pero no obligatorio para el avanzado. Su protección automática crea una "red de seguridad" que permite al jugador aprender patrones de ataque sin morir constantemente.

### 6.4 Garrik (Rango N3-N10) — Pícaro / Asesino

**Rol:** Flanqueador / Burst Damage

| Aspecto | Descripción |
|---|---|
| **Comportamiento base** | Se mantiene en los bordes del combate, buscando oportunidades para flanquear. No ataca de frente. |
| **Prioridad de objetivo** | Enemigos de rango menor o igual que él. Evita enemigos de rango superior. |
| **Ataque por detrás** | Si Garrik ataca por detrás (ángulo 120° trasero), hace daño crítico garantizado (3.0×). |
| **Defensa personal** | Garrik nunca bloquea. Esquiva constantemente (cada 2-3s). Su alta velocidad de movimiento (120% base) le permite reposicionarse rápido. |
| **Habilidad automática** | Golpe en la Sombra: cada 10s, si Garrik está detrás de un enemigo, hace un ataque adicional de 2.0×. |

**Órdenes Específicas para Garrik:**
| Orden | Efecto | Enfriamiento |
|---|---|---|
| "Atacar punto débil" | Garrik señala un punto débil en el enemigo objetivo. El próximo ataque de cualquier aliado a ese punto hace daño crítico. | 20s |
| "Flanquear" | Garrik corre en un arco amplio para posicionarse detrás del enemigo objetivo. Tarda 2-4s en reposicionarse, durante los cuales no ataca. | — |
| "Retirada táctica" | Garrik lanza una bomba de humo (cega enemigos en 4m, 3s) y se retira 10m. | 25s |
| "Sangrado profundo" (desbloqueable N6+) | Garrik aplica Hemorragia (DoT 4% HP/s por 8s) en su próximo ataque. | 15s |

**Nota de diseño:** Garrik requiere que el jugador entienda posicionamiento. No es efectivo si el jugador lo ignora — necesita órdenes para flanquear y maximizar su daño. Recompensa el pensamiento táctico.

### 6.5 Sera (Rango N8-N10) — DPS Rápida

**Rol:** Ejecutora / Movilidad / Asalto

| Aspecto | Descripción |
|---|---|
| **Comportamiento base** | Se mueve constantemente alrededor del campo de batalla, atacando y reposicionándose sin pausa. |
| **Movimiento constante** | Si Sera está en movimiento, recibe 50% menos daño (representa su agilidad). Si se detiene, pierde el bonus. |
| **Prioridad de objetivo** | Enemigo con menor HP en el campo de batalla. Sera "ejecuta" — remata enemigos debilitados. |
| **Habilidad automática** | Golpe Raudo: cada 6s, Sera hace un dash a través de un enemigo, dañándolo y reposicionándose al otro lado. |
| **Defensa personal** | Sera tiene 0 capacidad de bloqueo. Su defensa es su velocidad: 125% velocidad de movimiento base, esquiva cada 0.5s sin costo de SP (propio de su clase). |

**Órdenes Específicas para Sera:**
| Orden | Efecto | Enfriamiento |
|---|---|---|
| "Ejecutar" | Sera se lanza al enemigo con menos HP en el campo. Hace 3.0× de daño si el enemigo está por debajo de 20% HP. | 10s |
| "Cubrir mi espalda" | Sera se posiciona detrás del jugador y ataca a cualquier enemigo que se acerque por la retaguardia. | — |
| "Carga de velocidad" | Sera activa un buff de 120% velocidad de movimiento y 30% velocidad de ataque por 6s. Ataca a todos los enemigos en su camino (daño 1.5× cada golpe). | 20s |
| "Cuchillas giratorias" (desbloqueable N10) | Sera hace un remolino de 360° que daña 2.5× a todos los enemigos en 4m y los derriba. | 25s |

**Nota de diseño:** Sera es un personaje de alto nivel introducido tarde. Su estilo es el más agresivo de todos los compañeros. Requiere que el jugador entienda el flujo de batalla para usarla efectivamente — si el jugador es demasiado pasivo, Sera se adelantará sola y morirá.

### 6.6 Rueda de Comandos (Órdenes Rápidas)

Accesible manteniendo el botón de comando (L2/R2 simultáneo, o tecla Q). Al mantener, el tiempo se ralentiza un 50% (no se detiene por completo) para permitir seleccionar sin presión.

#### Comandos Globales (afectan a todos los compañeros)

| Comando | Efecto | Notas |
|---|---|---|
| **Atacar** | Todos los compañeros atacan al objetivo señalado. Prioridad máxima. | Anula cualquier orden previa de defensa. |
| **Defender** | Todos los compañeros se agrupan en torno al jugador (3m) y cambian a postura defensiva. Los compañeros con escudo bloquean, los demás se mantienen cerca pero atacan si hay enemigos en rango. | Dura hasta que se dé otra orden. |
| **Disparar** | Todos los compañeros con ataque a distancia (Mira, magos) disparan al objetivo simultáneamente. Los compañeros cuerpo a cuerpo hacen una pausa de 2s (cubren). | Gasta 3 segundos de "daño perdido" de los cuerpo a cuerpo. |
| **Retirada** | Todos los compañeros se retiran 10 metros del enemigo más cercano y se reagrupan alrededor del jugador. | Ideal para reubicación táctica, evitar AoE, o dar espacio al jugador para usar ultimate. |
| **Habilidad especial** | Cada compañero usa su habilidad ultimate (si tiene recurso para ello). Los compañeros normalmente no usan ultimates sin orden. | — |
| **Dispersión** | Todos los compañeros se separan: cada uno se mueve a un punto cardinal alrededor del enemigo (norte, sur, este, oeste). | Útil contra ataques de área grandes. |
| **Reunión** | Todos los compañeros se agrupan en la posición actual del jugador. | — |

#### Comandos por Compañero
| Compañero | Comando Rápido 1 | Comando Rápido 2 | Comando Rápido 3 |
|---|---|---|---|
| Mira | Cubrir Jugador | Disparo Precisión | Apuntar Objetivo |
| Vassen | Formación Defensiva | Cargar | Proteger Compañero |
| Garrik | Flanquear | Atacar Punto Débil | Retirada Táctica |
| Sera | Ejecutar | Carga de Velocidad | Cubrir Espalda |

**Nota de interfaz:** Los comandos por compañero se asignan a los botones direccionales (arriba/abajo/izquierda/derecha) según el compañero seleccionado con L1/LB. En PC, se pueden asignar a teclas 5-8.

### 6.7 Compañero Fuera de Combate

- Si un compañero muere, no está disponible por el resto de la misión (o 30 minutos si es combate no-misión).
- Si un compañero cae al vacío o se separa demasiado (>50m), reaparece en la última posición segura después de 10s.
- Los compañeros no reciben daño por caídas (excepto muerte por vacío).
- El jugador puede hablar con los compañeros fuera de combate para avanzar diálogos, recibir misiones secundarias o mejorar su equipo.

---

## 7. Sistema de Jefe

### 7.1 Filosofía de Jefes

Los jefes en SOLO no son "muros de stats" — son acertijos de combate. Cada jefe tiene:
- **Patrones de ataque** que el jugador debe aprender y responder.
- **Fases** que cambian las mecánicas del combate.
- **Debilidades explotables** que recompensan la observación.
- **Estrategias alternativas** según la composición del grupo.

**Principios de diseño:**
- Ningún jefe es invencible con la clase incorrecta — siempre hay una ruta viable.
- Los patrones son consistentes pero no 100% predecibles (variación forzada cada 3 repeticiones).
- Las fases no son solo "más HP y más daño" — agregan mecánicas nuevas que cambian cómo se aborda el combate.
- Los jefes tienen "conversaciones de combate" — líneas de diálogo durante la pelea que dan pistas mecánicas y lore.

### 7.2 Fases de Jefe

Cada jefe tiene 2 o 3 fases. La transición ocurre cuando el HP del jefe cruza un umbral (ej. 70%, 40%, 10%).

#### Transición de Fase

| Momento | Efecto | Duración |
|---|---|---|
| **Inicio de transición** | El jefe se vuelve invulnerable (brillo dorado). No se puede dañar. Empuja a todos los personajes 5m hacia atrás. | 0.5s |
| **Animación de fase** | El jefe realiza una animación de transformación: cambia de forma, activa armas nuevas, el escenario se modifica. | 2-3s |
| **Reanudación** | El jefe recupera 20% de su HP máximo (como "segundo aliento"). Una nueva barra de HP aparece. | Instantáneo |
| **Post-transición** | El jugador tiene 3s de gracia donde el jefe no ataca ("ventana de respiro"). Úsalo para posicionarte, curarte o dar órdenes a compañeros. | 3s |

#### Aviso de Fase
- **Texto en pantalla:** Aparece el nombre de la nueva fase (ej. "FASE 2: IRA DEL SOL") con efecto de letras en llamas/hielo según el jefe.
- **Cambio visual:** El jefe cambia de apariencia: color de aura, armas, tamaño, textura de piel.
- **Cambio de música:** La banda sonora cambia a un tema más intenso para cada fase.
- **Diálogo:** El jefe dice una línea única de transición.

#### Fase de Enrage

Si el jugador pasa demasiado tiempo en una fase sin avanzar (ej. >3 minutos en fase 1), se activa el **enrage**:

| Efecto | Valor |
|---|---|
| Daño del jefe | +40% |
| Velocidad de ataque del jefe | +30% |
| Velocidad de movimiento del jefe | +20% |
| Nuevos ataques | El jefe desbloquea 1-2 ataques adicionales de su repertorio de fase posterior |
| Indicador visual | Aura roja pulsante, venas brillantes, ojos rojos |
| Duración | Hasta que el jefe cambie de fase o muera |
| Diálogo | "Ya basta de juegos..." |

El enrage está diseñado para castigar al jugador excesivamente pasivo o que "farme" daño sin avanzar. No es un "temporizador de muerte" — es un incentivador de agresión.

### 7.3 Indicadores de Ataque

Cada ataque de jefe y enemigo elite tiene un indicador visual que comunica su naturaleza y cómo responder.

| Color | Significado | Acción Recomendada | Excepciones |
|---|---|---|---|
| **Amarillo** | Ataque bloqueable. Se puede parry. | Bloquear o parry. | No parry si el ataque es de área (el indicador es amarillo con borde ancho). |
| **Rojo** | Ataque imbloqueable. No se puede parry ni bloquear. | Esquivar. Interrumpir (si tienes un stagger alto). | Algunas clases tienen habilidades que pueden bloquear ataques rojos (ej. Centinela "Muro Inquebrantable"). |
| **Púrpura** | Ataque mágico. | Esquivar (no se bloquea). Barrera mágica si la clase lo tiene. | El Parry Mágico (Paladín, Hechicero de Batalla) puede parry ataques púrpura. |
| **Blanco** | Ataque telegrafiado. Tiene un tiempo de reacción de 1-2s. | Leer la dirección, preparar respuesta. | Es el indicador más común y "justo". |
| **Rojo pulsante** | Ataque instantáneo (sin brillo). Tiempo de reacción <0.5s. | Esquiva predictiva o bloqueo preventivo. | No dependas de reflejos — aprende el patrón. |
| **Azul** | Ataque de compañero / amistoso. | Ignorar. | — |
| **Negro + Púrpura** | Ataque imparables (jefes específicos). No se puede parry, bloquear ni esquivar. | Interrumpir con ataque pesado cargado o ultimate. | Muy raro. Solo 1-2 ataques por jefe. |

#### Zonas de Peligro (Ground Indicators)

| Indicador en Suelo | Significado | Tiempo hasta impacto |
|---|---|---|
| Círculo rojo sólido | Ataque de área. Estar dentro = daño completo. | 1.5-2s |
| Círculo rojo con borde blanco | Ataque de área con retraso variable. El borde blanco parpadea más rápido cerca del impacto. | 1-3s (varía) |
| Líneas rojas en el suelo | Ataque de carga lineal. Moverse perpendicular a las líneas. | 1s |
| Zona morada pulsante | Ataque mágico de área. Esquivar + alejarse. | 2s |
| Círculo azul claro | Zona de curación (amistosa, creada por aliado). | Inmediata |
| Círculo dorado | Zona de buff (amistosa). | Inmediata |
| Sombra en el suelo | Ataque aéreo inminente (el jefe saltó). Moverse fuera de la sombra. | 0.5-1s |

### 7.4 Mecánicas de Jefe

Este sistema describe mecánicas reutilizables que pueden combinarse en diferentes jefes. Cada jefe usa un subconjunto de 3-5 mecánicas.

#### 7.4.1 Mecánicas Universales

**Zonas de peligro (Puddles)**
- El jefe deja zonas de daño en el suelo que persisten por 5-10s.
- Pisar la zona aplica daño y posiblemente un estado negativo.
- Ej: Jefe de fuego deja charcos de lava (Quemado), jefe de hielo deja placas de hielo (Congelado si te resbalas).
- Estrategia: El jugador debe mover al jefe lejos de las zonas o usar las zonas contra él (ej. atraer al jefe de agua a una zona eléctrica).

**Cadenas / Vínculos**
- El jefe ata a un compañero con una cadena de energía/oscura.
- El compañero no puede moverse más allá de 5m de su posición inicial y recibe daño 2% HP/s.
- Romper la cadena: Atacarla (la cadena tiene su propia barra de HP, aproximadamente 20% del HP de un enemigo normal del mismo nivel).
- Si la cadena no se rompe en 10s, el compañero muere instantáneamente (sin oportunidad de revive) o recibe daño masivo.
- Variante: Cadena que conecta dos compañeros — si uno muere, ambos mueren.

**Invulnerabilidad periódica**
- El jefe se vuelve invulnerable por un período (5-10s) hasta que se activa un mecanismo en el escenario.
- El mecanismo puede ser: una palanca, un cristal que hay que destruir, un altar que hay que purificar, un enemigo específico que hay que matar.
- Mientras es invulnerable, el jefe sigue atacando normalmente.
- Indicador: El jefe se vuelve gris/humo y el mecanismo brilla con el mismo color.

**Memoria de Combate**
- El jefe "aprende" de los patrones del jugador.
- Después de 3 repeticiones del mismo ataque o maniobra, el jefe la contrarresta:
  - Si el jugador esquiva siempre a la izquierda, el jefe empieza a atacar a la izquierda.
  - Si el jugador siempre usa parry, el jefe usa ataques imparables más seguido.
  - Si el jugador siempre ataca por detrás, el jefe hace ataques de 360°.
- **Solución:** El jugador debe variar su estrategia conscientemente. Esto fuerza creatividad táctica en lugar de memorización de patrones.
- La memoria se resetea en cada transición de fase.

#### 7.4.2 Mecánicas por Tipo de Jefe

**Jefe de Fuego**
| Mecánica | Efecto |
|---|---|
| **Explosión de Lava** | El jefe golpea el suelo, creando ondas de lava que se expanden en 3 anillos concéntricos. Saltar sobre los anillos o esquivar entre ellos. |
| **Invocación de Brasas** | Cada 30s, el jefe invoca 3-5 brasas que persiguen a los compañeros. Si una brasa alcanza a un compañero, explota (daño AoE 4m) y revive si no se mata a tiempo. |
| **Armadura Ígnea** | Fase 2: El jefe se cubre de lava. Los ataques cuerpo a cuerpo normales queman al atacante. Solo atacar durante ventanas específicas (tras sus ataques pesados). |

**Jefe de Hielo**
| Mecánica | Efecto |
|---|---|
| **Tormenta de Hielo** | El jefe canaliza 3s, luego invoca una tormenta que cubre todo el campo. Reduce temperatura = los personajes pierden SP 2× más rápido. Refugiarse tras pilares de hielo que el mismo jefe crea. |
| **Fijar Objetivo** | El jefe marca a un personaje que será perseguido por estalactitas que caen. El personaje marcado debe moverse constantemente. Las estalactitas dejan pilares de hielo que pueden usarse como cobertura. |
| **Corazón Congelado** | Fase 3: El jefe se encierra en un capullo de hielo invulnerable. El capullo solo se daña con ataques de fuego. Si no se rompe en 15s, el capullo explota (muerte instantánea para todo el grupo). |

**Jefe de Oscuridad**
| Mecánica | Efecto |
|---|---|
| **Ceguera** | El jefe reduce la visibilidad del jugador a 3m (niebla oscura). Solo se ven los indicadores de ataque (colores brillan en la oscuridad). |
| **Sombras** | El jefe crea 3 copias de sí mismo. Una es real, las otras son ilusiones que atacan pero hacen 0 daño (pero el jugador no sabe cuál es cuál hasta que golpea). |
| **Drenaje de Luz** | Fase 2: El jefe absorbe toda la luz del campo. No hay indicadores de ataque visibles. El jugador debe aprender los patrones por sonido (audio cues). |

### 7.5 Recompensas de Jefe

| Tipo de Recompensa | Detalle |
|---|---|
| **Experiencia** | ×5 a ×10 la XP de un enemigo normal del mismo nivel. |
| **Oro** | ×10 a ×20 el oro de un enemigo normal. |
| **Equipo** | 1-3 piezas de equipo de rango igual o superior al nivel del jefe. Garantizado: 1 pieza del set temático del jefe. |
| **Materiales** | Esencia del jefe (usada para mejorar equipo), fragmentos de arma, piedras de maná. |
| **Bestiario** | Entrada desbloqueada con debilidades, patrones, lore. |
| **Logro** | Según el jefe y condiciones especiales (sin compañeros caídos, sin recibir daño de una mecánica específica, etc.). |

---

## 8. Estado y Buffs durante Combate

### 8.1 Filosofía de Estados

Los estados alterados (tanto positivos como negativos) son una capa táctica adicional en el combate. No son "molestias" — son oportunidades y peligros que el jugador debe manejar activamente.

- **Sin acumulación infinita:** La mayoría de estados tienen un máximo de cargas (stack) que evita spam.
- **Tiempo y cura:** La mayoría de estados negativos tienen cura (pociones, habilidades, esperar).
- **Interacción:** Los estados interactúan entre sí (fuego + veneno = combustión).
- **Visibilidad:** Cada estado tiene un icono claro y una barra de tiempo visible.

### 8.2 Estados Negativos

| Estado | Efecto Base | Duración | Máx. Cargas | Cómo se Aplica | Cómo se Cura |
|---|---|---|---|---|---|
| **Aturdido** (Stun) | No puede actuar. No puede moverse, atacar, bloquear, esquivar ni usar habilidades. | 2-3s | 1 | Golpe fuerte con timing específico, parry exitoso del jugador contra enemigo, habilidades de clase, jefes en fases específicas. | No se cura (esperar). Inmunidad de 10s tras terminar el aturdimiento. |
| **Envenenado** (Poison) | Daño cada 2s: 3% del HP máximo del objetivo por tick. | 10s | 5 cargas (15% HP/s máximo) | Armas envenenadas, habilidades de clase (Asesino, Druida), ataques de enemigos venenosos. | Poción antídoto, habilidad de curación, esperar. |
| **Congelado** (Freeze) | Movimiento -50%, velocidad de ataque -30%. Si se acumulan 3 cargas: inmovilización total 2s. | 5s | 3 cargas | Ataques de hielo, habilidades elementales, entorno helado. Si está mojado + hielo = inmovilización instantánea. | Poción de fuego interno, habilidad de calor (ej. Llama Purificadora), fuente de calor cercana. |
| **Quemado** (Burn) | Daño cada 1s: 2% del HP máximo por tick. | 5s | 3 cargas (6% HP/s máximo) | Ataques de fuego, habilidades de clase (Mago, Druida), entorno de lava. | Poción de agua, rodar por el suelo (esquivar 2 veces seguidas), habilidad de agua/hielo. |
| **Electrocutado** (Shock) | SP se vacía 50% más rápido (acciones gastan 50% más SP). | 8s | 1 | Ataques eléctricos, agua + electricidad, habilidades de clase (Hechicero de Batalla). | Poción de tierra, esperar. Si está mojado: el efecto dura 12s en lugar de 8s. |
| **Miedo** (Fear) | Huye del enemigo 3s (el personaje corre en dirección opuesta, no controlable). | 3s | 1 | Habilidades de oscuridad, jefes de tipo oscuro, enemigos con aura de miedo. Rompe cualquier canalización. | No se cura (esperar). Inmunidad de 15s tras terminar. |
| **Drenado** (Drain) | Recibe 50% menos curación de todas las fuentes. | 10s | 1 (no acumula) | Ataques oscuros, habilidades de vampiros/necromantes, algunas mecánicas de jefe. | Poción sagrada, habilidad de luz, esperar. |
| **Petrificado** (Petrify) | No puede moverse. Daño recibido reducido 50% (está "duro"). No puede atacar ni defenderse. | 5s | 1 | Habilidades muy específicas (jefes de roca, gorgonas). Es raro pero peligroso. | No se cura. Compañeros pueden golpear al jugador petrificado para romper la estatua (recibe daño pero se libera). |
| **Cegado** (Blind) | -70% precisión. Los ataques fallan aleatoriamente (70% de probabilidad de fallar). Las habilidades de área aún funcionan pero el jugador no ve los indicadores de ataque enemigo. | 5s | 1 | Proyectiles cegadores, humo, jefes de oscuridad/sombra. | Poción de luz, habilidad de luz/sagrada. |
| **Hemorragia** (Bleed) | Daño cada 3s: 4% del HP máximo por tick. Se acumula con movimiento (correr hace sangrar más). | 8s | 3 cargas (12% HP/3s máximo) | Armas cortantes con bonus de sangrado, habilidades de Garrik. Correr durante hemorragia: duplica el daño del siguiente tick. | Poción de vendaje (detiene la hemorragia instantáneamente), habilidad de curación, quedarse quieto (reduce daño en 50%). |
| **Silenciado** (Silence) | No puede usar habilidades mágicas. El MP no se regenera. No afecta habilidades físicas. | 6s | 1 | Jefes específicos, enemigos mágicos, trampas. | Poción de maná, habilidad de disipación. |
| **Ralentizado** (Slow) | Movimiento -30%, velocidad de ataque -30%. | 6s | 1 (no acumula con Congelado, se reemplaza) | Golpes contundentes, algunas áreas de efecto. | Movimiento constante reduce la duración en 1s por cada 2s de movimiento. |
| **Confusión** (Confuse) | Los controles se invierten (adelante = atrás, izquierda = derecha). Atacar puede dañar a aliados. | 4s | 1 | Jefes psíquicos/mágicos, pociones mal hechas en eventos de mundo. | Esperar. Compañero puede golpear al jugador para "despertarlo" (recibe 10% HP de daño pero se cura la confusión). |

### 8.3 Estados Positivos (Buffs)

| Buff | Efecto | Duración | Máx. Cargas | Cómo se Obtiene |
|---|---|---|---|---|
| **Escudo** (Shield) | Absorbe daño hasta cierto límite (ej. 50% del HP del aplicador). No acumula — el escudo más fuerte reemplaza al más débil. | Hasta que se rompa (por daño) o expire (30s máximo). | 1 | Habilidades de clase (Paladín, Centinela), pociones de escudo, equipo con bonus de escudo al recibir daño. |
| **Apresurado** (Haste) | Velocidad de ataque +20%. Velocidad de movimiento +15%. | 8s | 1 | Habilidades de clase (Asesino, Sera), pociones de velocidad, ciertos sets de armadura (ej. Set del Viento). |
| **Fortaleza** (Fortify) | Defensa física +30%. Defensa mágica +30%. | 8s | 1 | Habilidades de clase (Centinela, Paladín), pociones de piedra, buff de fogata (tras descansar con ciertos materiales). |
| **Poder** (Power) | Daño físico y mágico +25%. | 8s | 1 | Habilidades de clase (Guerrero, Mago), pociones de furia, condiciones específicas (ej. después de matar a un enemigo con postura rota). |
| **Regeneración** (Regen) | HP restaurado 3%/s. | 10s | 1 | Habilidades de clase (Paladín, Sacerdote), pociones de vida, descansar en fogata con hierbas especiales. |
| **Superarmadura** (Hyper Armor) | No eres interrumpido por ataques enemigos durante acciones (ataques, habilidades). El daño se recibe completo. | 5s | 1 | Habilidades de clase lentas (ver sección 5.3), última habilidad de ciertas cadenas de combo, ultimate de Centinela. |
| **Invisibilidad** (Stealth) | Los enemigos no te detectan. El próximo ataque es crítico garantizado 3.0×. Romper sigilo al atacar. | 8s (o hasta atacar) | 1 | Habilidades de clase (Asesino), pociones de sombra, ciertos entornos (sombras profundas). |
| **Absorción Mágica** (Spell Ward) | El próximo hechizo recibido se convierte en MP en lugar de daño (el 100% del daño que habría hecho se suma al MP). | 30s máximo o después de absorber 1 hechizo | 1 | Habilidades de clase (Hechicero de Batalla), equipo mágico raro. |
| **Frenesí** (Berserk) | +50% daño, +30% velocidad de ataque, -30% defensa, no se puede bloquear ni esquivar. Ataques automáticos (el jugador ataca sin input cada 0.8s). | 6s | 1 | Habilidades de clase de riesgo (Guerrero rama berserker), consumibles raros. Muy peligroso pero devastador. |

### 8.4 Interacciones entre Estados

| Combinación | Resultado |
|---|---|
| Quemado + Envenenado | **Combustión:** Cada tick de ambos hace +1.5× daño. Ambos estados se consumen instantáneamente al activarse. |
| Mojado + Hielo | **Congelado instantáneo:** Inmovilización total 3s. Ignora la acumulación normal de cargas de hielo. |
| Mojado + Eléctrico | **Sobrecarga:** Daño eléctrico ×2.0. Stun 1s adicional. |
| Congelado + Eléctrico | **Explosión de cristal:** Daño AoE 4m (3.0×). Consume ambos estados. |
| Aturdido + Postura Rota | **Golpe de Gracia:** El siguiente ataque que conecte hace crítico garantizado y derriba. |
| Escudo + Fortaleza | **Muro de Acero:** El escudo absorbe 30% más daño. Duración de Fortaleza extendida 2s. |
| Apresurado + Poder | **Tempestad:** Cada ataque durante la superposición hace un 5% de daño adicional como ráfaga. |
| Superarmadura + Frenesí | **Máquina de Guerra:** Inmune a interrupciones mientras está en Frenesí. Extremadamente poderoso pero sigue -30% defensa y no puede esquivar/bloquear. |
| Congelado + Quemado | **Vapor:** Ambos estados se cancelan. El área alrededor del jugador se llena de vapor (cubre visual 30% por 3s). |
| Miedo + Drenado | **Pánico Oscuro:** El personaje huye y recibe 50% menos curación mientras huye. Duración extendida a 6s. |

### 8.5 Inmunidades y Resistencias

- Después de que un estado termina, el personaje es inmune al mismo estado por 5-10s (según el estado).
- Aturdimiento: inmunidad 10s.
- Miedo: inmunidad 15s.
- Congelado: inmunidad 8s.
- Los jefes tienen inmunidades reducidas (3-5s) — se les puede re-aplicar estados más rápido.
- Equipo y habilidades pueden otorgar inmunidad pasiva a estados específicos (ej. Anillo de Fuego: inmune a Quemado).

---

## 9. Sistema de Muerte y Respawn

### 9.1 Filosofía de Muerte

SOLO no castiga la muerte con pérdida de progresión (XP). El castigo es **económico y logístico**, no de poder. Esto permite:
- Experimentar con estrategias sin miedo a "perder la partida".
- Aprender patrones de jefe por ensayo y error sin frustración.
- Mantener la tensión de "no quiero morir" por las consecuencias económicas.

### 9.2 Muerte del Jugador

#### Proceso de Muerte
1. **HP llega a 0** → El personaje cae al suelo. No puede actuar.
2. **Período de Agonía (10s):**
   - El jugador está en el suelo, no puede moverse ni atacar.
   - La cámara se aleja ligeramente y la pantalla se torna gris/sepia.
   - El jugador puede ver a los compañeros y enemigos alrededor.
   - **Puede ser revivido por un compañero** (ver sección 9.3).
   - Puede pulsar un botón para "rendirse" y aceptar la muerte inmediatamente.
3. **Muerte Definitiva (fin de la agonía):**
   - Pantalla se oscurece completamente.
   - Texto: "Has caído. El Sol aún brillará."
   - Opciones: "Reintentar desde último punto de control", "Regresar a ciudad", "Cargar partida anterior".

#### Pérdidas por Muerte
| Item | Pérdida | Notas |
|---|---|---|
| **Oro** | 20% del oro actual | Se pierde. No se puede recuperar. Si tienes 0 oro, no pierdes nada. |
| **Progresión** | Último punto de control perdido | Retrocedes a la última fogata o ciudad donde descansaste. Los enemigos normales respawnean (excepto elites y jefes de historia). |
| **XP** | 0% — no se pierde XP | El castigo es económico, no de progresión. |
| **Objetos** | No se pierden objetos | Excepto si mueres en zonas específicas (ej. mazmorras de muerte permanente). |
| **Equipo** | No se pierde equipo | — |
| **Misiones** | El progreso de misiones se mantiene | Excepto misiones con límite de tiempo (si el objetivo era rescatar a alguien y moriste, la misión falla). |

#### Muerte por Caída (Vacío)
- Caer al vacío = muerte instantánea (sin período de agonía).
- Compañeros también mueren instantáneamente.
- Pérdida de oro aumentada al 30%.
- Si es en mazmorra, retrocedes al inicio de la sala anterior (penalización mayor).

### 9.3 Muerte de Compañeros

#### Proceso
1. **HP del compañero llega a 0** → El compañero cae al suelo.
2. **Período de Agonía (30s):**
   - El compañero está tendido en el suelo, visiblemente herido.
   - Un icono sobre su cabeza muestra el tiempo restante.
   - **No puede ser atacado por enemigos** (son ignorados).
3. **Revivir:**
   - Acercarse al compañero caído y presionar interactuar (mantener 5s).
   - Durante la canalización, el jugador no puede moverse ni defenderse (vulnerable).
   - Si el jugador recibe daño, la canalización se interrumpe (debe reiniciar).
   - Después de 5s, el compañero se levanta con 30% de HP.
   - El compañero revive con su SP/MP al 50%.
4. **Muerte Definitiva del Compañero:**
   - Si no se revive en 30s, el compañero "desaparece" (se desvanece en partículas).
   - El compañero reaparece en el próximo descanso (fogata o ciudad).
   - No está disponible por el resto de la misión/área actual.

#### Muerte Permanente (Historia)
**Solo tres compañeros pueden morir permanentemente por eventos de historia:**
- **Caín:** En un punto específico de la trama principal (N5-N6). Su muerte es inevitable (narrativa).
- **Mira:** En una encrucijada de la historia (N7). Dependiendo de elecciones del jugador, puede sobrevivir.
- **Vassen:** En el clímax del arco (N8). Puede ser salvado si se completan ciertas misiones secundarias.

Estos son los ÚNICOS compañeros con muerte permanente. El resto siempre reaparece tras descansar.

#### Comportamiento del Compañero Caído
- Mientras el compañero está en agonía:
  - Otros compañeros pueden revivirlo automáticamente si no están ocupados.
  - La IA de los compañeros prioriza revivir sobre atacar si están cerca.
  - No todos los compañeros pueden revivir — solo Vassen, Mira (si tiene flechas de apoyo desbloqueadas) y el jugador.

### 9.4 Puntos de Control (Fogatas y Posadas)

#### Fogatas
- **Función:** Punto de control + descanso + guardado.
- **Ubicación:** Distribuidas por cada área/mazmorra/piso. Cada 10-15 minutos de juego en promedio.
- **Interacción:** Acercarse y presionar interactuar.

**Al descansar en fogata:**
| Efecto | Detalle |
|---|---|
| HP/MP/SP | Restauración completa al 100%. |
| Guardado | Partida guardada. Al morir, se respawnea en la última fogata donde se descansó. |
| Compañeros | Todos los compañeros reviven (si no están en muerte permanente). |
| Enemigos | Los enemigos normales respawnean (no elites, no jefes ya derrotados). |
| Diálogo | Oportunidad de hablar con compañeros (diálogos de fogata). |
| Mejoras | Se pueden mejorar armas, cambiar habilidades, gestionar inventario. |
| Elixires | Preparar pociones si se tienen materiales y recetas. |

**Tipos de Fogata:**
| Tipo | Apariencia | Efecto Adicional |
|---|---|---|
| Fogata Común | Fuego naranja estándar | Descanso básico. |
| Fogata Mayor | Fuego azul brillante | Descanso + buff de área (todo el grupo recibe "Fortaleza" temporal 15% por 10min de juego). |
| Fogata Oculta | Fuego verde, rodeada de enredaderas | Descanso + acceso a área secreta cercana (cofre, lore, jefe opcional). |
| Fogata de Jefe | Fuego rojo pulsante, siempre frente a la arena del jefe | Descanso completo + venta de objetos por un vendedor fantasmal que aparece solo allí. |
| Fogata de Ciudad | No aplica (es una cama de posada) | Descanso + todos los servicios (mercader, herrero, taberna con misiones). |

#### Posadas en Ciudades
- Descansar en una cama de posada es equivalente a fogata pero con servicios adicionales.
- Servicios: Mercader, Herrero, Taberna (misiones secundarias, información), Baño (buff de descanso completo: +10% HP máximo por 1 hora de juego).
- Costo de posada: 50-200 oro según la ciudad.

### 9.5 Respawn de Enemigos

| Tipo de Enemigo | ¿Respawnea al descansar? | ¿Respawnea al morir? | ¿Respawnea por tiempo? |
|---|---|---|---|
| Enemigos normales | Sí | Sí | No (solo al descansar o morir) |
| Enemigos élite | Sí (después de 3 descansos) | No | No |
| Jefes de área | No (derrota permanente) | No | No |
| Jefes de mazmorra | No (derrota permanente) | No | No |
| Jefes de historia | No (derrota permanente) | No | No |
| Enemigos de evento | Según el evento (varía) | Según el evento (varía) | Según el evento (varía) |

---

## 10. Apéndice: Tablas de Referencia Rápida

### 10.1 Resumen de Acciones y Costos SP

| Acción | Costo SP | Tiempo | Cancelable | Notas |
|---|---|---|---|---|
| Golpe Débil | 5 SP | 0.3-0.5s | Sí (último golpe no) | Primer 3-5 golpes de combo. |
| Golpe Fuerte | 20 SP | 0.8-1.2s | No | Rompe postura. |
| Carga Parcial | 25 SP total | 0.5-1s | No (una vez soltado) | 1.5× daño. |
| Carga Media | 40 SP total | 1-2s | No | 2.5× daño. |
| Carga Completa | 60 SP total | 2-3s | No | 4.0× daño, derribo. |
| Bloquear (por segundo) | 5 SP/s | — | Sí | Reduce daño 60%. |
| Parry | 0 SP (reembolso) | 0.2s (ventana) | No | Aturde 2s, contraataque garantizado. |
| Esquivar | 15 SP | 0.5s | No | I-frames 0.2s. |
| Ataque en Salto | 10 SP | 0.6-0.8s | No | 1.3× daño, derribo. |
| Ataque Descendente | 15 SP | 0.8-1.0s | No | 1.5× daño, stun 1s. |
| Ataque Cargado Corriendo | 30 SP | 1.0-1.5s (carga) | No | 1.5× daño, derribo frontal. |
| Habilidad Rápida (clase) | 10-25 SP | 0.5-1s | Sí (hasta impacto) | Varía por clase. |
| Habilidad Media (clase) | 25-40 SP | 1-2s | No (superarmadura) | Varía por clase. |
| Habilidad Lenta (clase) | 40-50 SP | 2-4s | No (superarmadura) | Varía por clase. |
| Ultimate | 100% SP | 3-5s | No (superarmadura total) | Enfriamiento de minutos. |

### 10.2 Resumen de Multiplicadores de Daño

| Tipo de Golpe | Multiplicador de Daño | Multiplicador de Postura | Costo SP |
|---|---|---|---|
| Golpe Débil (combo inicio) | 0.8×-1.0× | 1.0× | 5 SP |
| Golpe Débil (combo medio) | 1.0×-1.2× | 1.0× | 5 SP |
| Golpe Débil (combo final) | 2.0× | 1.5× | 10-15 SP |
| Golpe Fuerte | 1.8×-2.5× | 1.5× | 20 SP |
| Carga Parcial | 1.5× | 2.0× | 25 SP |
| Carga Media | 2.5× | 3.0× | 40 SP |
| Carga Completa | 4.0× | 5.0× | 60 SP |
| Ataque en Salto | 1.3× | 1.2× | 10 SP |
| Ataque Descendente | 1.5× | 2.0× | 15 SP |
| Ataque Cargado Corriendo | 1.5× | 2.5× | 30 SP |
| Contraataque (post-parry) | 2.0×-3.0× (crítico) | 2.0× | 0 SP (incluido) |
| Golpe de Gracia (postura rota) | 3.0× (crítico) | — | 10 SP |
| Crítico base | 1.5× | 1.0× | Según el ataque |

### 10.3 Progresión de Daño por Nivel (Referencia)

| Nivel | HP Base | Daño Físico Aprox (espada) | Daño Mágico Aprox (bastón) | SP Total |
|---|---|---|---|---|
| 1 | 120 | 15-25 | 12-20 | 100 |
| 5 | 200 | 30-50 | 25-40 | 100 |
| 10 | 300 | 60-100 | 50-80 | 100 |
| 20 | 500 | 120-200 | 100-160 | 105 (si 1 punto en AGU) |
| 30 | 700 | 200-350 | 180-280 | 110 (si 2 puntos en AGU) |
| 50 | 1100 | 400-700 | 350-550 | 120 (si 4 puntos en AGU) |
| 100 | 2100 | 1000-1800 | 800-1400 | 140 (si 8 puntos en AGU) |

*Nota: Valores aproximados con equipo de nivel adecuado y distribución equilibrada de atributos. El daño real varía significativamente según equipo, habilidades y sinergias.*

### 10.4 Tabla de Defensa vs Reducción de Daño

| Defensa del Objetivo | Reducción de Daño Físico |
|---|---|
| 50 | ~14.3% |
| 100 | 25.0% |
| 150 | 33.3% |
| 200 | 40.0% |
| 300 | 50.0% |
| 450 | 60.0% |
| 600 | 66.7% |
| 900 | 75.0% |
| 1200 | 80.0% |

*Fórmula: Reducción = Defensa / (Defensa + 300)*

### 10.5 Resumen de Estados (Infografía de Referencia)

**Estados Negativos (9 principales):**
| Icono | Nombre | Símbolo | Prioridad de Cura |
|---|---|---|---|
| ⚡ | Aturdido | Cabeza con estrellas | Alta (no puedes actuar) |
| ☠️ | Envenenado | Calavera verde | Media (daño progresivo) |
| ❄️ | Congelado | Copo de nieve | Baja (solo ralentiza) |
| 🔥 | Quemado | Llama | Media-Alta (daño rápido) |
| ⚡ | Electrocutado | Rayo | Media (SP se vacía rápido) |
| 👻 | Miedo | Fantasma | Alta (pierdes control) |
| 💀 | Drenado | Calavera negra | Media (menos curación) |
| 🗿 | Petrificado | Estatua | Alta (no puedes hacer nada) |
| 🌑 | Cegado | Ojo tachado | Media (ataques fallan) |

**Estados Positivos (7 principales):**
| Icono | Nombre | Símbolo | Prioridad de Uso |
|---|---|---|---|
| 🛡️ | Escudo | Escudo azul | Siempre útil |
| ⚡ | Apresurado | Reloj con alas | Ideal para DPS |
| 🏰 | Fortaleza | Escudo dorado | Contra físico |
| 💪 | Poder | Brazo flexionado | Antes de burst |
| ❤️ | Regeneración | Corazón pulsante | En medio del combate |
| 🛡️✨ | Superarmadura | Escudo con brillo | Durante habilidades lentas |
| 👤 | Invisibilidad | Figura difusa | Para reposicionar |

### 10.6 Glosario de Términos de Combate

| Término | Definición |
|---|---|
| **HP** | Health Points / Salud. Cuando llega a 0, el personaje cae. |
| **MP** | Mana Points / Maná. Recurso para habilidades mágicas. Se regenera constante. |
| **SP** | Stamina Points / Aguante. Recurso universal para acciones físicas (atacar, bloquear, esquivar). Se regenera rápido. |
| **Postura** | Barra de estabilidad. Vacía = vulnerable (+50% daño, no puede bloquear). |
| **Postura Rota** | Estado en que la postura llegó a 0. El enemigo es vulnerable. |
| **Parry** | Defensa de timing perfecto. Bloquear en el momento exacto del impacto. Aturde al enemigo. |
| **I-Frames** | Invincibility Frames. Frames de animación donde el personaje no puede recibir daño. |
| **DoT** | Damage over Time. Daño progresivo (quemado, envenenado, etc.). |
| **AoE** | Area of Effect. Ataque que afecta un área en lugar de un solo objetivo. |
| **Stun** | Aturdimiento. El personaje no puede actuar. |
| **Knockback** | Empuje. El personaje es lanzado hacia atrás. |
| **Knockdown** | Derribo. El personaje cae al suelo. |
| **Superarmadura** | El personaje no es interrumpido por ataques durante una acción. |
| **Combo** | Secuencia de ataques encadenados. Los combos ligeros tienen 3-5 golpes. |
| **Cancelación** | Interrumpir una animación de ataque con otra acción (esquiva, bloqueo). |
| **Commit** | El último golpe de un combo no se puede cancelar. El jugador se "compromete". |
| **Enrage** | Estado del jefe cuando pasa demasiado tiempo en la misma fase. Ataca más fuerte y rápido. |
| **Sinergia** | Combinación de habilidades entre el jugador y compañeros que produce efectos adicionales. |
| **Agonía** | Período después de caer a 0 HP donde se puede ser revivido. |
| **Fogata** | Punto de control y descanso. Restaura recursos, guarda partida. |
| **CD / Enfriamiento** | Cooldown. Tiempo que debe pasar antes de poder usar una habilidad de nuevo. |
| **Burst** | Ráfaga de daño alta en poco tiempo. |
| **Sustain** | Capacidad de mantener daño constante durante períodos largos. |
| **Peel** | Acción de alejar a un enemigo de un aliado vulnerable. |
| **CC** | Crowd Control. Habilidades que limitan el movimiento o acciones del enemigo (stun, miedo, congelado). |
| **DPS** | Damage Per Second. Medida de daño sostenido. |
| **Overkill** | Daño excesivo más allá del HP restante del enemigo. En SOLO no tiene bonus, pero tampoco penalización. |

---

> **Fin del Documento — Sistema de Combate de SOLO v1.0**
> *Este documento es propiedad del proyecto SOLO (Sunlight of the Last Online). Diseñado para Unreal Engine 5.7 por el equipo de diseño de SOLO.*
