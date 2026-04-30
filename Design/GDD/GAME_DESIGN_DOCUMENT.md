# Solo â€” Game Design Document
**VersiÃ³n:** 0.1 â€” Work In Progress  
**GÃ©nero:** Action RPG / MMORPG-like (single/multiplayer) â€” **3D**  
**Motor:** Unreal Engine 5.7  
**Lenguaje:** C++ + Blueprints (UE5)  

---

## 1. VISIÃ“N DEL JUEGO

Solo es un RPG de acciÃ³n medieval en **3D** construido en Unreal Engine 5.7. El jugador crea y personaliza su personaje, explora un mundo abierto con distintos biomas fotorrealistas (Nanite + Lumen), conquista mazmorras, asciende una torre de 100 pisos y enfrenta jefes Ãºnicos con mecÃ¡nicas especiales. Sistema de economÃ­a viva, crafting profundo y progresiÃ³n de personaje multi-capa.

---

## 2. PERSONAJE Y PERSONALIZACIÃ“N

### 2.1 Razas disponibles
| Raza        | Bono principal              | PenalizaciÃ³n          |
|-------------|-----------------------------|-----------------------|
| Humano      | Stat balanceado, +10% XP   | Ninguna               |
| Elfo        | +AGI, +MAGIA, alcance      | -HP, -DEFENSA FÃSICA  |
| Enano       | +FUERZA, +DEFENSA, crafting | -VELOCIDAD, -MAGIA   |
| Medio-Orco  | +FUERZA, +HP, intimidaciÃ³n | -INTELIGENCIA, -rep   |
| Semihumano  | +VELOCIDAD, +SIGILO        | -HP, -DEFENSA         |
| Demonio     | +MAGIA OSCURA, +CRÃTICO    | -Acceso a ciudades    |
| FeÃ©rico     | +MAGIA ELEMENTAL, vuelo lim.| -HP, -FUERZA FÃSICA  |

### 2.2 Clases (desbloqueables segÃºn raza)
- **Guerrero** â†’ Templario / Berserker / Paladin (nivel 20/40/60)
- **Mago** â†’ Elementalista / Nigromante / Archimago
- **Arquero** â†’ Cazador / Asesino / Espectro
- **Curandero** â†’ Sacerdote / Druida / OrÃ¡culo
- **Herrero** â†’ Artesano / Alquimista / Forjador de Runas

### 2.3 Atributos base
```
FUERZA      â€” daÃ±o fÃ­sico, peso de equipo
DESTREZA    â€” velocidad de ataque, crÃ­tico, sigilo
INTELIGENCIAâ€” daÃ±o mÃ¡gico, slots de hechizos
VITALIDAD   â€” HP mÃ¡ximo, resistencia de estados
AGILIDAD    â€” velocidad de movimiento, evasiÃ³n
ESPÃRITU    â€” manÃ¡, cooldown de skills, bonos de gremio
SUERTE      â€” drop rate, crÃ­tico extra, eventos aleatorios
```

### 2.4 PersonalizaciÃ³n visual
- GÃ©nero, altura, complexiÃ³n corporal (slider)
- Tono de piel, color de ojos, color/largo de cabello
- Tatuajes, cicatrices, marcas raciales
- Nombre del personaje + tÃ­tulo (ganado in-game)
- Avatar visible en el HUD y en el perfil de gremio

---

## 3. SISTEMA DE PROGRESIÃ“N

### 3.1 Niveles
- Nivel mÃ¡ximo base: **100**
- Nivel post-juego (Transcendencia): **T1-T5** (cada uno +100 niveles efectivos)
- XP viene de: matar enemigos, quests, explorar zonas, crafting, comercio

### 3.2 Ãrbol de habilidades
- Ãrbol activo: habilidades de combate (ataques, combos, ultimates)
- Ãrbol pasivo: bonos de stats, auras, resistencias
- Ãrbol de oficio: crafting, alquimia, pesca, minerÃ­a, cocina

### 3.3 MaestrÃ­a de armas
- Cada tipo de arma tiene su propio medidor de maestrÃ­a (0-100)
- A mayor maestrÃ­a: nuevos combos, animaciones especiales, bonos pasivos
- MaestrÃ­a legendaria (100): desbloquea habilidad Ãºnica del arma

---

## 4. SISTEMA DE INVENTARIO

### 4.1 Estructura
- **Mochila** â€” 40 slots base, ampliable con upgrades
- **Equipamiento** â€” 12 slots (cabeza, pecho, piernas, pies, manos, anillo x2, collar, capa, arma principal, arma secundaria/escudo, reliquia)
- **AlmacÃ©n** â€” accesible en ciudades, 200 slots por personaje
- **BaÃºl de gremio** â€” compartido entre miembros

### 4.2 Rareza de Ã­tems
```
â¬œ ComÃºn       â€” gris   â€” base, sin atributos extras
ðŸŸ¢ Poco ComÃºn  â€” verde  â€” 1-2 atributos menores
ðŸ”µ Raro        â€” azul   â€” 2-3 atributos, 1 mayor
ðŸŸ£ Ã‰pico       â€” morado â€” 3-4 atributos, efectos especiales
ðŸŸ  Legendario  â€” naranjaâ€” stats Ãºnicos, lore propio, aspecto especial
ðŸ”´ MÃ­tico      â€” rojo   â€” 1 por servidor, eventos especiales Ãºnicos
```

### 4.3 Sets de equipamiento
- Equipar 2/4/6 piezas de un set activa bonos de set
- Sets principales: Armadura del Abismo, Set del Bosque Eterno, Forja del DragÃ³n Antiguo, Manto del Errante

---

## 5. SISTEMA DE COMBATE

### 5.1 MecÃ¡nicas base
- Ataque normal, ataque cargado, esquiva (i-frames), bloqueo
- Combos: secuencias de inputs para habilidades especiales (como SAO Sword Skills)
- Postura: agresiva (+daÃ±o, -defensa) / defensiva (+defensa, -daÃ±o) / balanceada
- Sistema de parry: ventana pequeÃ±a para contraataque con daÃ±o extra

### 5.2 Sword Skills (habilidades de combate activas)
- Cada habilidad tiene: nombre, animaciÃ³n, costo de manÃ¡/estamina, cooldown, daÃ±o, efecto especial
- Ejemplos:
  - **Lanzadera Horizontal** â€” tajo rÃ¡pido de lado a lado
  - **Tormenta de Espadas** â€” mÃºltiples golpes en Ã¡rea
  - **Golpe del Firmamento** â€” salto + caÃ­da con AoE
  - **Escudo de Luz** â€” refleja el primer ataque
  - **Sombra del Cazador** â€” teletransporte detrÃ¡s del objetivo

### 5.3 Efectos de estado
```
Quemado, Envenenado, Paralizado, Congelado
Maldito, Aturdido, Confundido, Sangrando
Bendecido, Escudado, Enraizado, Invisble
```

---

## 6. MUNDO Y MAPA

### 6.1 Biomas del Overworld
| Zona              | Nivel sugerido | CaracterÃ­stica                |
|-------------------|---------------|-------------------------------|
| Valle Inicial     | 1-10          | Tutorial, enemies dÃ©biles     |
| Bosque Ã‰lfico     | 10-25         | Enemigos furtivos, trampas    |
| Desierto Ardiente | 20-35         | Debuffs de calor, tormentas   |
| Tundra Glacial    | 35-55         | Blizzards, enemigos de hielo  |
| Pantano Oscuro    | 50-70         | Veneno ambiental, niebla      |
| MontaÃ±a del DragÃ³n| 65-85         | Vuelo, corrientes de aire     |
| Tierra Corrupta   | 85-100+       | Debuffs constantes, jefes     |
| Isla de la Nada   | Solo Transcend.| Zona post-game, infinita      |

### 6.2 Ciudades principales (5 ciudades)
Cada ciudad tiene:
- Mercado / Casas de subastas
- Taberna (misiones secundarias, info de gremios)
- Forja (crafting)
- Templo (quests de clase, buffs)
- Gremio de aventureros (misiones diarias/semanales)
- Banco / AlmacÃ©n personal

---

## 7. MAZMORRAS (DUNGEONS)

### 7.1 Tipos de mazmorras
- **Mazmorra normal** â€” 3-5 salas, jefe al final, instanciada por grupo
- **Mazmorra Ã©lite** â€” 6-10 salas, 1-2 minijefes + jefe, drops mejores
- **Mazmorra legendaria** â€” Semanal, drops Ãºnicos, mecÃ¡nicas de puzzle/combate mixtas
- **Raid** â€” 10-20 jugadores, 3 fases de jefe, loot de gremio

### 7.2 MecÃ¡nicas de mazmorras
- Trampas ambientales (flechas, espinas, pozos)
- Puertas que requieren llaves/puzzles
- Eventos de oleada de enemigos antes del jefe
- Cofres con loot aleatorio por ruta
- Timer de oro (bonus loot si completas rÃ¡pido)

---

## 8. LA TORRE PRINCIPAL (100 PISOS)

Inspirada directamente en Aincrad de SAO.

### 8.1 Estructura
- **100 pisos** â€” cada 10 pisos hay un Jefe de Piso
- **Jefes de Piso (10):** Ãºnicos, mecÃ¡nicas complejas, drops especiales
- **Pisos especiales:** cada 25 pisos hay un evento de historia obligatorio
- La torre se desbloquea despuÃ©s de completar el arco inicial del juego

### 8.2 MecÃ¡nicas de la torre
- No se puede usar teletransporte dentro de la torre (salvo cristales especiales)
- Cada piso tiene su propio bioma/tema visual
- Guardianes secundarios antes del jefe de piso
- Los primeros en matar cada jefe de piso obtienen un Ã­tem mÃ­tico Ãºnico
- Sistema de "Primer Ataque" que registra al jugador en la historia del servidor

### 8.3 Jefes de Piso (ejemplos)
| Piso | Nombre              | Tema           | MecÃ¡nica especial                    |
|------|---------------------|----------------|--------------------------------------|
| 10   | El Centinela        | Piedra         | Escudos rotativos, fase berserk       |
| 20   | Hidra del Abismo    | Agua           | Regenera cabezas, veneno de Ã¡rea     |
| 30   | El Rey Lich         | No-muerto      | Invoca minions, fase de alma         |
| 40   | DragÃ³n de Cristal   | Hielo/Luz      | Refleja hechizos, armadura de fases  |
| 50   | Coloso de Hierro    | MecÃ¡nico       | Armas cambiantes, gimmick de debilidad|
| 60   | La Sombra Eterna    | Oscuridad      | Copia habilidades del jugador        |
| 70   | SeÃ±or de las Bestias| Naturaleza     | Invoca oleadas de animales           |
| 80   | El Heraldo CaÃ­do    | Divino/Corrupto| Fase de luz y sombra, mecÃ¡nica coop  |
| 90   | Tiempo Fracturado   | Temporal       | Reinicia el tiempo, counter mechanic |
| 100  | **El Arquitecto**   | CÃ³smico        | **Jefe Final â€” 5 fases, todas las mecÃ¡nicas anteriores** |

---

## 9. JEFES ESPECIALES (Fuera de la Torre)

- **Jefes de Mundo:** Aparecen en el overworld cada X horas, toda la zona puede atacarlos. Drops Ãºnicos.
- **Jefes Ocultos:** Encuentros secretos tras resolver puzzles o condiciones especiales (hora del dÃ­a, objetos especÃ­ficos en el inventario, etc.)
- **Jefes Legendarios:** Solo invocables con un Ã­tem de invocaciÃ³n raro. Drops mÃ­ticos.
- **El DragÃ³n Antiguo** â€” Jefe de mundo mÃ¡ximo nivel, requiere raid de 50 jugadores.

---

## 10. SISTEMA DE ITEMS Y DROPS

### 10.1 Drops
- Cada enemigo tiene una tabla de drops pÃºblica/investigable
- Drop rate aumenta con el atributo SUERTE
- **Drop especial de jefe:** 1 Ã­tem garantizado de rareza Ã©pica o superior
- **Drop de primer kill:** Ãtem Ãºnico (solo existe 1 en el servidor)

### 10.2 Crafting
- Materiales obtenibles via: drops, minerÃ­a, herboristerÃ­a, pesca, desmantelar Ã­tems
- Recetas: algunas conocidas por defecto, otras encontrables en el mundo (libros, maestros)
- Calidad de craft: depende del stat de ArtesanÃ­a y nivel de herramienta
- Crafting falla-able: chance de romper materiales, mitigable con perks

### 10.3 Sistema de Runas
- Las runas se insertan en slots de Ã­tems (armas/armaduras con slots)
- Bonos: +elementos, +stats, efectos especiales, set de runas (combinar 3 activa efecto)
- Runas obtenibles: drops, crafting de alquimia, compra en mercado

---

## 11. ECONOMÃA

### 11.1 Monedas
```
Cobre (C)  â€” moneda base
Plata (P)  â€” 100C = 1P
Oro  (O)   â€” 100P = 1O
Cristal (CR)â€” moneda premium, drop raro en jefes o logros especiales
```

### 11.2 Mercado / Casa de Subastas
- Los jugadores pueden listar Ã­tems con precio fijo o subasta con tiempo
- Historial de precios visible
- ComisiÃ³n del mercado: 5% al vendedor
- Mercado de materiales separado (stack market)
- LÃ­mite de listings por jugador segÃºn nivel

### 11.3 Transacciones directas
- Trade entre jugadores: ventana de intercambio con confirmaciÃ³n doble
- No hay drop al morir (opcionalmente toggleable en modos PvP)
- Sistema anti-scam: cooldown en Ã­tems reciÃ©n adquiridos vÃ­a trade

---

## 12. SISTEMA DE QUESTS

### 12.1 Tipos
- **Historia principal** â€” progresa el arco narrativo, desbloquea zonas y la Torre
- **Secundarias** â€” opcionales, lore extra, recompensas Ãºnicas
- **Diarias/Semanales** â€” tareas repetibles del gremio de aventureros
- **De gremio** â€” quests colaborativas, loot de gremio
- **Ocultas** â€” sin marcador en el mapa, se descubren explorando o hablando con NPCs
- **De evento** â€” temporales (fiestas, aniversarios, eventos de servidor)

### 12.2 Sistema de reputaciÃ³n
- Cada ciudad/facciÃ³n tiene su medidor de reputaciÃ³n (0-10000)
- A mayor reputaciÃ³n: descuentos, quests exclusivas, Ã­tems Ãºnicos de facciÃ³n, tÃ­tulos

---

## 13. SISTEMA DE GREMIOS

- Crear gremio: costo en oro + requisito de nivel
- **Rango dentro del gremio:** LÃ­der â†’ Oficiales â†’ Miembros â†’ Reclutas
- **Casa del gremio:** edificio comprable, ampliable, con baÃºl, tablero de misiones, salÃ³n de trofeos
- **Guerra de gremios:** PvP instanciado, captura de banderas o eliminaciÃ³n
- **Liga de gremios:** ranking semanal, loot de temporada para los top guilds

---

## 14. SISTEMAS ADICIONALES RECOMENDADOS

### 14.1 Sistema de Logros
- Logros por exploraciÃ³n, combate, crafting, social, etc.
- Recompensas: tÃ­tulos, cosmÃ©ticos, bonos permanentes de stats pequeÃ±os

### 14.2 Sistema de Mascotas/Familiares
- Criaturas que acompaÃ±an al jugador, dan bonos pasivos
- Evolucionan con el tiempo, pueden equipar mini-Ã­tems
- Algunas son drops raros de jefes especÃ­ficos

### 14.3 Sistema de Monturas
- Caballos, grifos, dragones pequeÃ±os segÃºn nivel/raza
- Velocidad de mapa aumentada, algunas permiten combate montado
- PersonalizaciÃ³n visual de montura

### 14.4 Sistema de Housing
- Casas comprables en ciudades o en el overworld
- DecoraciÃ³n con muebles (crafteable/comprable)
- BaÃºl extra, jardÃ­n para cultivo, taller personal

### 14.5 Sistema de Combate PvP
- Duelos: 1v1 en zonas seguras (consentido)
- Zonas PvP: Ã¡reas del overworld donde el PvP es libre
- Arenas: instanciadas, rankeadas, con temporadas y rewards

### 14.6 Sistema de Pesca / Mini-juegos
- Minijuego de pesca, recolecciÃ³n, cocina
- Recetas de comida que dan buffs temporales
- Concursos de pesca, cocina, artesanÃ­a con premios

---

## 15. STACK TECNOLÃ“GICO â€” UNREAL ENGINE 5.7 (3D)

**Motor definitivo:** Unreal Engine 5.7  
**Lenguaje principal:** C++ para sistemas core + Blueprints para lÃ³gica de gameplay y diseÃ±o de niveles  

### 15.1 Features de UE5 que aprovecharemos

| Feature UE5        | Uso en Solo                                              |
|--------------------|---------------------------------------------------------------|
| **Nanite**         | GeometrÃ­a ultra-detallada en castillos, cuevas, la Torre      |
| **Lumen**          | IluminaciÃ³n dinÃ¡mica global â€” antorchas, hechizos, dawn/dusk  |
| **World Partition**| Overworld abierto sin pantallas de carga                      |
| **Chaos Physics**  | Enemigos con ragdoll, destrucciÃ³n de entorno en combate       |
| **MetaHuman**      | Base para la personalizaciÃ³n de personajes 3D                 |
| **Niagara VFX**    | Efectos de habilidades (fuego, hielo, magia, impactos)        |
| **Mass Entity**    | IA de hordas de enemigos optimizada                           |
| **Control Rig**    | Animaciones procedurales de combate e interacciÃ³n             |
| **Verse (Script)** | LÃ³gica de juego adicional de alto nivel                       |
| **EOS (Online)**   | Multijugador, sesiones, inventario en nube                    |

### 15.2 Estructura de mÃ³dulos C++ recomendada

```
Solo.uproject
Source/
  Solo/           â€” mÃ³dulo principal
  SOLOCore/       â€” sistemas base (inventario, stats, save)
  SOLOCombat/     â€” combate, skills, estados
  SOLOWorld/      â€” generaciÃ³n de mundo, mazmorras, torre
  SOLOUI/         â€” widgets, HUD, menÃºs (UMG)
  SOLOOnline/     â€” multijugador, economÃ­a, gremios (EOS)
```

### 15.3 Plugins recomendados (Marketplace / Open Source)
- **Advanced Locomotion System (ALS)** â€” animaciones de personaje 3D de calidad
- **Procedural Dungeon Plugin** â€” generaciÃ³n procedural de mazmorras
- **Inventory Framework Plugin (IFP)** â€” base de inventario 3D robusta
- **Voxel Plugin** â€” terreno destructible/modificable
- **EasyMultiplayer Invader** â€” facilita la integraciÃ³n de multijugador

---

## 16. ROADMAP SUGERIDO

```
FASE 1 â€” Fundamentos (Meses 1-3)
  âœ¦ Movimiento del personaje, cÃ¡mara
  âœ¦ Sistema de combate bÃ¡sico
  âœ¦ Inventario base (equipo + mochila)
  âœ¦ 1 zona de prueba con enemies

FASE 2 â€” Sistemas Core (Meses 4-6)
  âœ¦ PersonalizaciÃ³n de personaje
  âœ¦ Sistema de stats y leveling
  âœ¦ NPCs, diÃ¡logos, quests bÃ¡sicas
  âœ¦ 1 mazmorra completa con jefe

FASE 3 â€” EconomÃ­a y Social (Meses 7-9)
  âœ¦ Sistema de crafting
  âœ¦ Mercado entre jugadores
  âœ¦ Sistema de gremios base
  âœ¦ 2-3 zonas del overworld

FASE 4 â€” La Torre (Meses 10-14)
  âœ¦ 20 pisos iniciales de la torre
  âœ¦ 2 jefes de piso completos
  âœ¦ Historia principal Acto 1

FASE 5 â€” Contenido y Polish (Mes 15+)
  âœ¦ Resto de la torre
  âœ¦ Todas las clases y razas
  âœ¦ PvP, eventos, housing
  âœ¦ Balance, bugs, optimizaciÃ³n
```

---

*Este documento es vivo â€” se actualiza conforme el proyecto evoluciona.*