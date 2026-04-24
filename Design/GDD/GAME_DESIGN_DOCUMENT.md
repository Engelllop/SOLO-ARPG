# EMBERVEIL — Game Design Document
**Versión:** 0.1 — Work In Progress  
**Género:** Action RPG / MMORPG-like (single/multiplayer) — **3D**  
**Motor:** Unreal Engine 5.7  
**Lenguaje:** C++ + Blueprints (UE5)  

---

## 1. VISIÓN DEL JUEGO

EMBERVEIL es un RPG de acción medieval en **3D** construido en Unreal Engine 5.7. El jugador crea y personaliza su personaje, explora un mundo abierto con distintos biomas fotorrealistas (Nanite + Lumen), conquista mazmorras, asciende una torre de 100 pisos y enfrenta jefes únicos con mecánicas especiales. Sistema de economía viva, crafting profundo y progresión de personaje multi-capa.

---

## 2. PERSONAJE Y PERSONALIZACIÓN

### 2.1 Razas disponibles
| Raza        | Bono principal              | Penalización          |
|-------------|-----------------------------|-----------------------|
| Humano      | Stat balanceado, +10% XP   | Ninguna               |
| Elfo        | +AGI, +MAGIA, alcance      | -HP, -DEFENSA FÍSICA  |
| Enano       | +FUERZA, +DEFENSA, crafting | -VELOCIDAD, -MAGIA   |
| Medio-Orco  | +FUERZA, +HP, intimidación | -INTELIGENCIA, -rep   |
| Semihumano  | +VELOCIDAD, +SIGILO        | -HP, -DEFENSA         |
| Demonio     | +MAGIA OSCURA, +CRÍTICO    | -Acceso a ciudades    |
| Feérico     | +MAGIA ELEMENTAL, vuelo lim.| -HP, -FUERZA FÍSICA  |

### 2.2 Clases (desbloqueables según raza)
- **Guerrero** → Templario / Berserker / Paladin (nivel 20/40/60)
- **Mago** → Elementalista / Nigromante / Archimago
- **Arquero** → Cazador / Asesino / Espectro
- **Curandero** → Sacerdote / Druida / Oráculo
- **Herrero** → Artesano / Alquimista / Forjador de Runas

### 2.3 Atributos base
```
FUERZA      — daño físico, peso de equipo
DESTREZA    — velocidad de ataque, crítico, sigilo
INTELIGENCIA— daño mágico, slots de hechizos
VITALIDAD   — HP máximo, resistencia de estados
AGILIDAD    — velocidad de movimiento, evasión
ESPÍRITU    — maná, cooldown de skills, bonos de gremio
SUERTE      — drop rate, crítico extra, eventos aleatorios
```

### 2.4 Personalización visual
- Género, altura, complexión corporal (slider)
- Tono de piel, color de ojos, color/largo de cabello
- Tatuajes, cicatrices, marcas raciales
- Nombre del personaje + título (ganado in-game)
- Avatar visible en el HUD y en el perfil de gremio

---

## 3. SISTEMA DE PROGRESIÓN

### 3.1 Niveles
- Nivel máximo base: **100**
- Nivel post-juego (Transcendencia): **T1-T5** (cada uno +100 niveles efectivos)
- XP viene de: matar enemigos, quests, explorar zonas, crafting, comercio

### 3.2 Árbol de habilidades
- Árbol activo: habilidades de combate (ataques, combos, ultimates)
- Árbol pasivo: bonos de stats, auras, resistencias
- Árbol de oficio: crafting, alquimia, pesca, minería, cocina

### 3.3 Maestría de armas
- Cada tipo de arma tiene su propio medidor de maestría (0-100)
- A mayor maestría: nuevos combos, animaciones especiales, bonos pasivos
- Maestría legendaria (100): desbloquea habilidad única del arma

---

## 4. SISTEMA DE INVENTARIO

### 4.1 Estructura
- **Mochila** — 40 slots base, ampliable con upgrades
- **Equipamiento** — 12 slots (cabeza, pecho, piernas, pies, manos, anillo x2, collar, capa, arma principal, arma secundaria/escudo, reliquia)
- **Almacén** — accesible en ciudades, 200 slots por personaje
- **Baúl de gremio** — compartido entre miembros

### 4.2 Rareza de ítems
```
⬜ Común       — gris   — base, sin atributos extras
🟢 Poco Común  — verde  — 1-2 atributos menores
🔵 Raro        — azul   — 2-3 atributos, 1 mayor
🟣 Épico       — morado — 3-4 atributos, efectos especiales
🟠 Legendario  — naranja— stats únicos, lore propio, aspecto especial
🔴 Mítico      — rojo   — 1 por servidor, eventos especiales únicos
```

### 4.3 Sets de equipamiento
- Equipar 2/4/6 piezas de un set activa bonos de set
- Sets principales: Armadura del Abismo, Set del Bosque Eterno, Forja del Dragón Antiguo, Manto del Errante

---

## 5. SISTEMA DE COMBATE

### 5.1 Mecánicas base
- Ataque normal, ataque cargado, esquiva (i-frames), bloqueo
- Combos: secuencias de inputs para habilidades especiales (como SAO Sword Skills)
- Postura: agresiva (+daño, -defensa) / defensiva (+defensa, -daño) / balanceada
- Sistema de parry: ventana pequeña para contraataque con daño extra

### 5.2 Sword Skills (habilidades de combate activas)
- Cada habilidad tiene: nombre, animación, costo de maná/estamina, cooldown, daño, efecto especial
- Ejemplos:
  - **Lanzadera Horizontal** — tajo rápido de lado a lado
  - **Tormenta de Espadas** — múltiples golpes en área
  - **Golpe del Firmamento** — salto + caída con AoE
  - **Escudo de Luz** — refleja el primer ataque
  - **Sombra del Cazador** — teletransporte detrás del objetivo

### 5.3 Efectos de estado
```
Quemado, Envenenado, Paralizado, Congelado
Maldito, Aturdido, Confundido, Sangrando
Bendecido, Escudado, Enraizado, Invisble
```

---

## 6. MUNDO Y MAPA

### 6.1 Biomas del Overworld
| Zona              | Nivel sugerido | Característica                |
|-------------------|---------------|-------------------------------|
| Valle Inicial     | 1-10          | Tutorial, enemies débiles     |
| Bosque Élfico     | 10-25         | Enemigos furtivos, trampas    |
| Desierto Ardiente | 20-35         | Debuffs de calor, tormentas   |
| Tundra Glacial    | 35-55         | Blizzards, enemigos de hielo  |
| Pantano Oscuro    | 50-70         | Veneno ambiental, niebla      |
| Montaña del Dragón| 65-85         | Vuelo, corrientes de aire     |
| Tierra Corrupta   | 85-100+       | Debuffs constantes, jefes     |
| Isla de la Nada   | Solo Transcend.| Zona post-game, infinita      |

### 6.2 Ciudades principales (5 ciudades)
Cada ciudad tiene:
- Mercado / Casas de subastas
- Taberna (misiones secundarias, info de gremios)
- Forja (crafting)
- Templo (quests de clase, buffs)
- Gremio de aventureros (misiones diarias/semanales)
- Banco / Almacén personal

---

## 7. MAZMORRAS (DUNGEONS)

### 7.1 Tipos de mazmorras
- **Mazmorra normal** — 3-5 salas, jefe al final, instanciada por grupo
- **Mazmorra élite** — 6-10 salas, 1-2 minijefes + jefe, drops mejores
- **Mazmorra legendaria** — Semanal, drops únicos, mecánicas de puzzle/combate mixtas
- **Raid** — 10-20 jugadores, 3 fases de jefe, loot de gremio

### 7.2 Mecánicas de mazmorras
- Trampas ambientales (flechas, espinas, pozos)
- Puertas que requieren llaves/puzzles
- Eventos de oleada de enemigos antes del jefe
- Cofres con loot aleatorio por ruta
- Timer de oro (bonus loot si completas rápido)

---

## 8. LA TORRE PRINCIPAL (100 PISOS)

Inspirada directamente en Aincrad de SAO.

### 8.1 Estructura
- **100 pisos** — cada 10 pisos hay un Jefe de Piso
- **Jefes de Piso (10):** únicos, mecánicas complejas, drops especiales
- **Pisos especiales:** cada 25 pisos hay un evento de historia obligatorio
- La torre se desbloquea después de completar el arco inicial del juego

### 8.2 Mecánicas de la torre
- No se puede usar teletransporte dentro de la torre (salvo cristales especiales)
- Cada piso tiene su propio bioma/tema visual
- Guardianes secundarios antes del jefe de piso
- Los primeros en matar cada jefe de piso obtienen un ítem mítico único
- Sistema de "Primer Ataque" que registra al jugador en la historia del servidor

### 8.3 Jefes de Piso (ejemplos)
| Piso | Nombre              | Tema           | Mecánica especial                    |
|------|---------------------|----------------|--------------------------------------|
| 10   | El Centinela        | Piedra         | Escudos rotativos, fase berserk       |
| 20   | Hidra del Abismo    | Agua           | Regenera cabezas, veneno de área     |
| 30   | El Rey Lich         | No-muerto      | Invoca minions, fase de alma         |
| 40   | Dragón de Cristal   | Hielo/Luz      | Refleja hechizos, armadura de fases  |
| 50   | Coloso de Hierro    | Mecánico       | Armas cambiantes, gimmick de debilidad|
| 60   | La Sombra Eterna    | Oscuridad      | Copia habilidades del jugador        |
| 70   | Señor de las Bestias| Naturaleza     | Invoca oleadas de animales           |
| 80   | El Heraldo Caído    | Divino/Corrupto| Fase de luz y sombra, mecánica coop  |
| 90   | Tiempo Fracturado   | Temporal       | Reinicia el tiempo, counter mechanic |
| 100  | **El Arquitecto**   | Cósmico        | **Jefe Final — 5 fases, todas las mecánicas anteriores** |

---

## 9. JEFES ESPECIALES (Fuera de la Torre)

- **Jefes de Mundo:** Aparecen en el overworld cada X horas, toda la zona puede atacarlos. Drops únicos.
- **Jefes Ocultos:** Encuentros secretos tras resolver puzzles o condiciones especiales (hora del día, objetos específicos en el inventario, etc.)
- **Jefes Legendarios:** Solo invocables con un ítem de invocación raro. Drops míticos.
- **El Dragón Antiguo** — Jefe de mundo máximo nivel, requiere raid de 50 jugadores.

---

## 10. SISTEMA DE ITEMS Y DROPS

### 10.1 Drops
- Cada enemigo tiene una tabla de drops pública/investigable
- Drop rate aumenta con el atributo SUERTE
- **Drop especial de jefe:** 1 ítem garantizado de rareza épica o superior
- **Drop de primer kill:** Ítem único (solo existe 1 en el servidor)

### 10.2 Crafting
- Materiales obtenibles via: drops, minería, herboristería, pesca, desmantelar ítems
- Recetas: algunas conocidas por defecto, otras encontrables en el mundo (libros, maestros)
- Calidad de craft: depende del stat de Artesanía y nivel de herramienta
- Crafting falla-able: chance de romper materiales, mitigable con perks

### 10.3 Sistema de Runas
- Las runas se insertan en slots de ítems (armas/armaduras con slots)
- Bonos: +elementos, +stats, efectos especiales, set de runas (combinar 3 activa efecto)
- Runas obtenibles: drops, crafting de alquimia, compra en mercado

---

## 11. ECONOMÍA

### 11.1 Monedas
```
Cobre (C)  — moneda base
Plata (P)  — 100C = 1P
Oro  (O)   — 100P = 1O
Cristal (CR)— moneda premium, drop raro en jefes o logros especiales
```

### 11.2 Mercado / Casa de Subastas
- Los jugadores pueden listar ítems con precio fijo o subasta con tiempo
- Historial de precios visible
- Comisión del mercado: 5% al vendedor
- Mercado de materiales separado (stack market)
- Límite de listings por jugador según nivel

### 11.3 Transacciones directas
- Trade entre jugadores: ventana de intercambio con confirmación doble
- No hay drop al morir (opcionalmente toggleable en modos PvP)
- Sistema anti-scam: cooldown en ítems recién adquiridos vía trade

---

## 12. SISTEMA DE QUESTS

### 12.1 Tipos
- **Historia principal** — progresa el arco narrativo, desbloquea zonas y la Torre
- **Secundarias** — opcionales, lore extra, recompensas únicas
- **Diarias/Semanales** — tareas repetibles del gremio de aventureros
- **De gremio** — quests colaborativas, loot de gremio
- **Ocultas** — sin marcador en el mapa, se descubren explorando o hablando con NPCs
- **De evento** — temporales (fiestas, aniversarios, eventos de servidor)

### 12.2 Sistema de reputación
- Cada ciudad/facción tiene su medidor de reputación (0-10000)
- A mayor reputación: descuentos, quests exclusivas, ítems únicos de facción, títulos

---

## 13. SISTEMA DE GREMIOS

- Crear gremio: costo en oro + requisito de nivel
- **Rango dentro del gremio:** Líder → Oficiales → Miembros → Reclutas
- **Casa del gremio:** edificio comprable, ampliable, con baúl, tablero de misiones, salón de trofeos
- **Guerra de gremios:** PvP instanciado, captura de banderas o eliminación
- **Liga de gremios:** ranking semanal, loot de temporada para los top guilds

---

## 14. SISTEMAS ADICIONALES RECOMENDADOS

### 14.1 Sistema de Logros
- Logros por exploración, combate, crafting, social, etc.
- Recompensas: títulos, cosméticos, bonos permanentes de stats pequeños

### 14.2 Sistema de Mascotas/Familiares
- Criaturas que acompañan al jugador, dan bonos pasivos
- Evolucionan con el tiempo, pueden equipar mini-ítems
- Algunas son drops raros de jefes específicos

### 14.3 Sistema de Monturas
- Caballos, grifos, dragones pequeños según nivel/raza
- Velocidad de mapa aumentada, algunas permiten combate montado
- Personalización visual de montura

### 14.4 Sistema de Housing
- Casas comprables en ciudades o en el overworld
- Decoración con muebles (crafteable/comprable)
- Baúl extra, jardín para cultivo, taller personal

### 14.5 Sistema de Combate PvP
- Duelos: 1v1 en zonas seguras (consentido)
- Zonas PvP: áreas del overworld donde el PvP es libre
- Arenas: instanciadas, rankeadas, con temporadas y rewards

### 14.6 Sistema de Pesca / Mini-juegos
- Minijuego de pesca, recolección, cocina
- Recetas de comida que dan buffs temporales
- Concursos de pesca, cocina, artesanía con premios

---

## 15. STACK TECNOLÓGICO — UNREAL ENGINE 5.7 (3D)

**Motor definitivo:** Unreal Engine 5.7  
**Lenguaje principal:** C++ para sistemas core + Blueprints para lógica de gameplay y diseño de niveles  

### 15.1 Features de UE5 que aprovecharemos

| Feature UE5        | Uso en EMBERVEIL                                              |
|--------------------|---------------------------------------------------------------|
| **Nanite**         | Geometría ultra-detallada en castillos, cuevas, la Torre      |
| **Lumen**          | Iluminación dinámica global — antorchas, hechizos, dawn/dusk  |
| **World Partition**| Overworld abierto sin pantallas de carga                      |
| **Chaos Physics**  | Enemigos con ragdoll, destrucción de entorno en combate       |
| **MetaHuman**      | Base para la personalización de personajes 3D                 |
| **Niagara VFX**    | Efectos de habilidades (fuego, hielo, magia, impactos)        |
| **Mass Entity**    | IA de hordas de enemigos optimizada                           |
| **Control Rig**    | Animaciones procedurales de combate e interacción             |
| **Verse (Script)** | Lógica de juego adicional de alto nivel                       |
| **EOS (Online)**   | Multijugador, sesiones, inventario en nube                    |

### 15.2 Estructura de módulos C++ recomendada

```
EMBERVEIL.uproject
Source/
  EMBERVEIL/           — módulo principal
  EMBERVEILCore/       — sistemas base (inventario, stats, save)
  EMBERVEILCombat/     — combate, skills, estados
  EMBERVEILWorld/      — generación de mundo, mazmorras, torre
  EMBERVEILUI/         — widgets, HUD, menús (UMG)
  EMBERVEILOnline/     — multijugador, economía, gremios (EOS)
```

### 15.3 Plugins recomendados (Marketplace / Open Source)
- **Advanced Locomotion System (ALS)** — animaciones de personaje 3D de calidad
- **Procedural Dungeon Plugin** — generación procedural de mazmorras
- **Inventory Framework Plugin (IFP)** — base de inventario 3D robusta
- **Voxel Plugin** — terreno destructible/modificable
- **EasyMultiplayer Invader** — facilita la integración de multijugador

---

## 16. ROADMAP SUGERIDO

```
FASE 1 — Fundamentos (Meses 1-3)
  ✦ Movimiento del personaje, cámara
  ✦ Sistema de combate básico
  ✦ Inventario base (equipo + mochila)
  ✦ 1 zona de prueba con enemies

FASE 2 — Sistemas Core (Meses 4-6)
  ✦ Personalización de personaje
  ✦ Sistema de stats y leveling
  ✦ NPCs, diálogos, quests básicas
  ✦ 1 mazmorra completa con jefe

FASE 3 — Economía y Social (Meses 7-9)
  ✦ Sistema de crafting
  ✦ Mercado entre jugadores
  ✦ Sistema de gremios base
  ✦ 2-3 zonas del overworld

FASE 4 — La Torre (Meses 10-14)
  ✦ 20 pisos iniciales de la torre
  ✦ 2 jefes de piso completos
  ✦ Historia principal Acto 1

FASE 5 — Contenido y Polish (Mes 15+)
  ✦ Resto de la torre
  ✦ Todas las clases y razas
  ✦ PvP, eventos, housing
  ✦ Balance, bugs, optimización
```

---

*Este documento es vivo — se actualiza conforme el proyecto evoluciona.*
