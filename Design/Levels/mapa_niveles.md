# Mapa de Niveles â€” Layout Conceptual de SOLO

> **Juego:** SOLO (Sunlight of the Last Online)
> **GÃ©nero:** RPG medieval 3D
> **Motor:** Unreal Engine 5.7
> **PropÃ³sito:** Este documento describe el layout conceptual de cada piso de la Torre para guiar a los level designers (Claude Code, etc.) y evitar diseÃ±os genÃ©ricos.
> **Nota:** Las medidas son aproximadas. No se requiere precisiÃ³n al metro, solo direcciÃ³n artÃ­stica y de gameplay.

---

## Ãndice

- [N0 â€” Emberveil (Overworld)](#n0--emberveil-overworld)
- [N1 â€” Velmar (Ciudad Colgante)](#n1--velmar-ciudad-colgante)
- [N2 â€” El Pastor de Cenizas (Bioma)](#n2--el-pastor-de-cenizas-bioma)
- [N3 â€” Aelthar (Ciudad Elfa)](#n3--aelthar-ciudad-elfa)
- [N4 â€” El Corredor de los Nombres (Bioma)](#n4--el-corredor-de-los-nombres-bioma)
- [N5 â€” Thornwall (Ciudad Fortaleza)](#n5--thornwall-ciudad-fortaleza)
- [N6 â€” CÃ¡maras de Jade (Bioma)](#n6--cÃ¡maras-de-jade-bioma)
- [N7 â€” Sahal (Ciudad Desierto)](#n7--sahal-ciudad-desierto)
- [N8 â€” Jardines Flotantes (Bioma)](#n8--jardines-flotantes-bioma)
- [N9 â€” Argentia (Ciudad de Plata)](#n9--argentia-ciudad-de-plata)
- [N10 â€” El Centinela de Granito (Jefe Final)](#n10--el-centinela-de-granito-jefe-final)
- [Resumen General](#resumen-general)
- [Notas de DiseÃ±o Adicionales](#notas-de-diseÃ±o-adicionales)

---

## N0 â€” Emberveil (Overworld)

**TamaÃ±o:** ~2 km Ã— 2 km (equivalente a un mapa de mundo abierto pequeÃ±o)
**Tipo:** Overworld / Hub central
**PropÃ³sito:** IntroducciÃ³n al mundo, tutoriales implÃ­citos, ambientaciÃ³n. Primer contacto con el misterio de la Torre.

### Zonas

#### 1. Emberveil (Ciudad Principal)
- **TamaÃ±o:** ~200 m Ã— 200 m
- **DescripciÃ³n:** Ciudad amurallada con calles estrechas de adoquÃ­n irregular. Plaza central con fuente seca y estatua de un caballero desconocido.
- **Edificios clave:**
  - **Gremio de Aventureros:** Edificio de piedra gris de dos pisos. TablÃ³n de misiones adentro. Primer NPC que da la misiÃ³n principal.
  - **La Posada del Errante:** Posada con chimenea siempre encendida. El jugador puede descansar por monedas. HabitaciÃ³n propia disponible.
  - **HerrerÃ­a de Ferrum:** Herrero que mejora armas bÃ¡sicas. Yunque siempre sonando.
  - **Mercado:** Puestos de madera en la plaza. Verduras, carne, telas. NPC comerciantes.
  - **Capilla de la Luz:** PequeÃ±a capilla al norte. Sacerdote que habla de la "MaldiciÃ³n de la Torre".
- **Murallas:** 10 m de alto, piedra gris con musgo. Puerta de madera con refuerzos de hierro al este y oeste.
- **Fogata:** En la plaza central, frente al Gremio.

#### 2. Bosque Vigilia
- **TamaÃ±o:** 800 m Ã— 600 m
- **UbicaciÃ³n:** Al este de Emberveil
- **DescripciÃ³n:** Bosque denso de Ã¡rboles altos (40-60 m). Copa cerrada que deja pasar poca luz. Caminos de tierra que serpentean entre raÃ­ces gigantes.
- **Enemigos:** JabalÃ­es (solos o en parejas), Lobos (manadas de 3-5, aparecen al anochecer)
- **Elementos visuales:** Rayos de luz que atraviesan el dosel arbÃ³reo. Niebla baja al amanecer. Sonido de pÃ¡jaros y ramas que crujen.
- **Puntos de interÃ©s:**
  - Claro con un Ã¡rbol caÃ­do (primer encuentro con jabalÃ­)
  - CabaÃ±a de cazador abandonada (cofre de madera adentro)
  - RÃ­o pequeÃ±o que cruza el camino (agua potable, animaciÃ³n de beber)
- **Fogata:** En el claro central, cerca de un Ã¡rbol gigante con marcas de garras.

#### 3. Lago Negro
- **TamaÃ±o:** 500 m Ã— 400 m
- **UbicaciÃ³n:** Al norte de Emberveil
- **DescripciÃ³n:** Lago de agua oscura y profunda. Orillas de piedra negra. Niebla baja permanente que oculta la superficie.
- **Enemigos:** Serpientes del Lago (emergen del agua, atacan en la orilla)
- **Elementos visuales:** Agua negra como espejo en calma. Niebla que se mueve lentamente. Sin sonido de animales.
- **Mazmorra â€” CÃ¡maras Sumergidas:**
  - **Entrada:** En el fondo del lago. Requiere contener la respiraciÃ³n o equipo especial. Grietas en el lecho rocoso.
  - **TamaÃ±o:** 150 m Ã— 100 m (cuevas subacuÃ¡ticas)
  - **Enemigos:** Peces carnÃ­voros, espÃ­ritus de ahogados
  - **Cofre:** 1 de hierro al final (guanteletes de buceo â€” extienden la respiraciÃ³n bajo el agua)
- **Fogata:** En la orilla norte, junto a una roca plana.

#### 4. Llanuras del Viento
- **TamaÃ±o:** 600 m Ã— 400 m
- **UbicaciÃ³n:** Al oeste de Emberveil
- **DescripciÃ³n:** Pastizales secos y ondulados. Viento constante que agita la hierba y levanta polvo. Ãrboles solitarios y retorcidos.
- **Enemigos:** Enemigos voladores ocasionales (aves rapaces agresivas, pequeÃ±os dragones juveniles)
- **Elementos visuales:** Hierba dorada que se mece. Molino de viento abandonado en la distancia. Cielo despejado.
- **Puntos de interÃ©s:**
  - Molino abandonado (escalera rota, cofre de madera en el piso superior)
  - CÃ­rculo de piedras antiguas (posible evento de invocaciÃ³n)
- **Fogata:** Junto al molino, protegida del viento por una pared derrumbada.

#### 5. Garganta del Errante
- **TamaÃ±o:** 300 m Ã— 200 m
- **UbicaciÃ³n:** Al sur de Emberveil
- **DescripciÃ³n:** CaÃ±Ã³n rocoso de paredes verticales (30-40 m de alto). Suelo de arena y roca suelta. Formaciones rocosas que crean pasadizos estrechos.
- **Enemigos:** El Errante (NPC hostil opcional, evento del Gremio). TambiÃ©n algunos murciÃ©lagos en cuevas laterales.
- **Evento del Gremio:**
  - **ActivaciÃ³n:** Hablar con el recepcionista del Gremio sobre "rumores en la Garganta"
  - **Objetivo:** Investigar la desapariciÃ³n de un mercader en la Garganta
  - **El Errante:** Humanoide andrajoso que ataca con cuchillo oxidado. Al derrotarlo, drops: diario del mercader (da pista sobre la Torre), monedas.
  - **Recompensa del Gremio:** Primera armadura bÃ¡sica mejorada.
- **Elementos visuales:** Rocas rojizas. Cactus y arbustos secos. Sonido de viento silbando entre las rocas.
- **Fogata:** Cerca de la entrada de la Garganta (la Ãºnica zona sin fogata interior).

#### 6. Entrada a la Torre
- **TamaÃ±o:** 30 m Ã— 30 m (edificio) + 100 m Ã— 80 m (subterrÃ¡neo)
- **UbicaciÃ³n:** Centro-norte del mapa, en una colina rocosa
- **DescripciÃ³n:** Edificio de piedra gris sin ventanas. Puerta de hierro negro de 5 m de alto. Sin marcas ni inscripciones. Se abre solo cuando el jugador ha completado el evento del Gremio.
- **El GuardiÃ¡n de la Primera Garganta:**
  - **UbicaciÃ³n:** Sala subterrÃ¡nea bajo la entrada de la Torre
  - **TamaÃ±o:** 80 m Ã— 60 m, sala de combate circular
  - **DescripciÃ³n:** GÃ³lem de piedra de 4 m de alto. Brazos de roca. Ojos que brillan en azul tenue.
  - **MecÃ¡nica:** Ataca con golpes de brazo y pisadas que crean ondas de choque. Punto dÃ©bil en la espalda.
  - **Recompensa:** Llave de la Torre (abre la puerta de hierro negro) + primera arma mÃ¡gica (espada con daÃ±o de luz).
- **Fogata:** No hay fogata en esta zona (es el punto de transiciÃ³n).

### Datos Generales de N0

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 5 (una por zona exterior, ninguna en la entrada de la Torre) |
| **Cofres de madera** | 3 (Bosque Vigilia, Llanuras del Viento, Garganta del Errante) |
| **Cofres de hierro** | 1 (CÃ¡maras Sumergidas) |
| **Puntos de spawn enemigo** | ~40 puntos, con rotaciÃ³n diurna/nocturna |
| **NPCs importantes** | Recepcionista del Gremio, Posadero, Ferrum (herrero), Sacerdote de la Capilla |
| **Misiones** | Tutorial (matar 3 jabalÃ­es), Evento del Gremio (El Errante), CÃ¡maras Sumergidas |
| **MÃºsica ambiente** | Tranquila durante el dÃ­a, tensa al atardecer, silencio con grillos de noche |
| **IluminaciÃ³n** | Ciclo dÃ­a/noche completo. DÃ­as despejados, noches estrelladas. |

---

## N1 â€” Velmar (Ciudad Colgante)

**TamaÃ±o total:** 500 m Ã— 300 m
**Tipo:** Ciudad vertical sobre el abismo
**PropÃ³sito:** Primer piso de la Torre. Shock visual: el jugador sale de un sÃ³tano y aparece en una ciudad que cuelga sobre el vacÃ­o.

### Estructura General

Velmar estÃ¡ construida sobre plataformas de madera y piedra suspendidas por cadenas gigantes ancladas a las paredes del abismo. No hay suelo â€” solo el vacÃ­o blanco/gris infinito debajo. La ciudad tiene 3 niveles de altura:
- **Nivel Superior (N1a):** Arco de Entrada + Santuario
- **Nivel Medio (N1b):** El Mercado
- **Nivel Inferior (N1c):** El Borde

### Ãreas

#### 1. Arco de Entrada (N1a)
- **TamaÃ±o:** 50 m Ã— 30 m
- **DescripciÃ³n:** Escaleras de piedra tallada que suben desde la base de la Torre (Garganta N0). Puerta de madera oscura de 8 m de alto con relieves de Ã¡rboles retorcidos. Al atravesarla: primera vista de la ciudad colgante.
- **Elementos visuales:** Campanas de bronce en el arco, suenan con el viento. El abismo es visible inmediatamente mÃ¡s allÃ¡ de la puerta. Cielo gris/azul artificial. Luz tenue, como atardecer perpetuo.
- **TransiciÃ³n:** Escaleras de madera que bajan al Mercado.

#### 2. El Mercado (N1b)
- **TamaÃ±o:** 200 m Ã— 150 m
- **DescripciÃ³n:** Plaza principal de Velmar. Puestos de madera, tiendas de tela. La taberna "El Abismo" al borde de la plataforma (literalmente cuelga sobre el vacÃ­o).
- **Puntos clave:**
  - **Taberna "El Abismo":** Barra, mesas, chimenea, terraza sobre el vacÃ­o.
  - **Puesto de Anselmo:** Mercader de objetos Ãºnicos (linternas, cuerdas).
  - **Tienda de Telas:** Vendedora de capas y ropa ligera. Pista sobre la niÃ±a desaparecida.
- **NPCs:** Anselmo, DueÃ±o de la Taberna, Vendedora de Telas, Ciudadanos varios.
- **Sonido ambiente:** Voces de mercaderes, crujido de maderas, campanas distantes.
- **Fogata:** En el centro del Mercado, junto a una estatua de piedra.

#### 3. El Borde (N1c)
- **TamaÃ±o:** 100 m Ã— 200 m
- **DescripciÃ³n:** Zona residencial pobre. Casas de madera podrida y telas remendadas que cuelgan del borde. Algunas casas sujetas solo por cuerdas.
- **Importancia narrativa:** La niÃ±a desaparecida vivÃ­a aquÃ­. Casa pequeÃ±a al borde. Adentro: cama deshecha, juguete, carta diciendo que "La Voz la llamÃ³".
- **Peligros:** Plataformas podridas que se rompen si corres. GÃ¡rgolas Durmientes en techos (se activan con ruido). Almas Errante aparecen al anochecer.
- **Elementos visuales:** Casas grises/podridas. Cuerdas y maderas rotas. El abismo a 2 pasos. Sin barandillas.
- **Fogata:** No (zona de peligro).

#### 4. El Santuario (N1d)
- **TamaÃ±o:** 50 m Ã— 50 m
- **DescripciÃ³n:** Edificio de piedra blanca en la parte mÃ¡s alta de Velmar. Sin ventanas. Interior: sala circular con techo abovedado de 15 m.
- **Jefe â€” La Voz:** Entidad incorpÃ³rea de niebla brillante con ojos.
  - **Fases:** 1) Invoca Almas Errante (3 oleadas) â†’ 2) Se materializa, rayos de luz cegadora â†’ 3) Se condensa en nÃºcleo, golpear mientras esquivas aturdimiento.
  - **DiÃ¡logo:** "Â¿Viniste por la niÃ±a? TambiÃ©n la llamÃ© a ella."
- **Recompensa:** Amuleto de Luz, 1 cofre de hierro (armadura ligera).
- **TransiciÃ³n a N2:** Puerta de luz al fondo. Emerges en las Llanuras Quemadas.
- **Fogata:** SÃ­, antes del jefe.

### Datos Generales de N1

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 3 (entrada, Mercado, Santuario antes del jefe) |
| **Cofres de madera** | 2 (Mercado, casa de la niÃ±a en El Borde) |
| **Cofres de hierro** | 1 (Santuario tras vencer a La Voz) |
| **Enemigos** | Almas Errante (nocturnas), GÃ¡rgolas Durmientes (activas por ruido) |
| **Puntos de spawn** | ~15 puntos (10 Almas Errante nocturnas, 5 gÃ¡rgolas estÃ¡ticas) |
| **Misiones** | Principal: Encontrar a la niÃ±a desaparecida. Secundarias: Paquete de Anselmo, Taberna. |
| **NPCs clave** | Madre de la niÃ±a, Anselmo, DueÃ±o de la Taberna |
| **Colores dominantes** | Gris, azul pÃ¡lido, marrÃ³n oscuro |
| **IluminaciÃ³n** | Perpetuo atardecer artificial. Linternas colgantes. Sin sol real. |
| **Sonido ambiente** | Viento, crujido de maderas y cadenas, campanas distantes. Sin pÃ¡jaros. |

---

## N2 â€” El Pastor de Cenizas (Bioma)

**TamaÃ±o total:** 1 km Ã— 1 km
**Tipo:** Llanura abierta desÃ©rtica de ceniza
**PropÃ³sito:** Primer bioma abierto dentro de la Torre. Introduce el medidor de calor como mecÃ¡nica de supervivencia.

### MecÃ¡nica Especial â€” Medidor de Calor
Barra de 5 segmentos. Se llena al aire libre en ~60s (correr acelera). Al llenarse: daÃ±o progresivo. Refugios tÃ©rmicos lo reducen. Objeto "Polvo de Escarcha" reduce 2 segmentos. NPC Lian vende Polvo de Escarcha.

### Ãreas

#### 1. Llanuras Quemadas
- **TamaÃ±o:** 800 m Ã— 600 m
- **DescripciÃ³n:** Ceniza gris hasta el tobillo. Ãrboles petrificados negros cada 30-50 m. Cielo gris/naranja.
- **Refugios tÃ©rmicos:** Cuevas, ruinas de piedra, estructuras quemadas. Cada ~200 m hay al menos uno.
- **Enemigos:** Perros de Ceniza (manadas 3-5), Cazadores Calcinados (solos/parejas).
- **ExploraciÃ³n lateral:** Restos de campamentos con notas de lore.
- **Sonido ambiente:** Silencio opresivo, viento que levanta ceniza que cruje.

#### 2. El Bosque Petrificado
- **TamaÃ±o:** 200 m Ã— 200 m (este del mapa)
- **DescripciÃ³n:** Ãrboles de carbÃ³n. Ceniza hasta la rodilla. Sombra que reduce calor.
- **Evento:** Hombre atrapado bajo Ã¡rbol caÃ­do. OpciÃ³n A: ayudarlo (informaciÃ³n + pociÃ³n). OpciÃ³n B: ignorarlo (muere, encuentras su cadÃ¡ver con nota).
- **Enemigos:** Perros de Ceniza, 1 Cazador Calcinado.

#### 3. Campamento de Lian
- **TamaÃ±o:** 30 m Ã— 30 m (centro-oeste)
- **DescripciÃ³n:** Tienda de lona, fogata encendida, puesto con frascos.
- **NPC Lian:** Mujer de ~40 aÃ±os. Vende Polvo de Escarcha, Cuerda, BrÃºjula rota. Da pista sobre el tÃºnel a N3.
- **Fogata:** SÃ­, permanente.

#### 4. El Nido del Pastor
- **TamaÃ±o:** 100 m Ã— 100 m (arena circular)
- **UbicaciÃ³n:** Extremo norte
- **DescripciÃ³n:** DepresiÃ³n circular de piedra negra lisa. Rodeada de ceniza humeante.
- **Jefe â€” El Pastor:** Humanoide alto con tÃºnicas quemadas y mÃ¡scara de madera carbonizada.
  - **Fase 1:** Garras de ceniza + proyectiles. Invoca Perros (2 oleadas).
  - **Fase 2 (50% HP):** Vuela, el suelo se calienta (medidor mÃ¡s rÃ¡pido). Rayos de ceniza.
  - **Fase 3 (20% HP):** Cae, mÃ¡scara rota, rostro humano quemado. Ataques rÃ¡pidos cuerpo a cuerpo.
  - **Post-pelea:** Se desintegra. Su mÃ¡scara es loot (revela que fue un aventurero antes).
- **Recompensa:** 1 cofre de hierro (armadura de ceniza + resistencia al calor), Ceniza del Pastor (quest item), Llave del TÃºnel.
- **Fogata:** Antes del Nido (Ãºltimo punto de control).
- **TransiciÃ³n a N3:** TÃºnel subterrÃ¡neo de 50 m al norte, cristales bioluminiscentes azules.

### Datos Generales de N2

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 3 (Campamento Lian, Refugio Central, antes del Nido) |
| **Cofres de madera** | 2 (campamento abandonado, Bosque Petrificado) |
| **Cofres de hierro** | 1 (Nido del Pastor) |
| **Enemigos** | Perros de Ceniza (~15 spawns), Cazadores Calcinados (~8 spawns) |
| **Puntos de spawn totales** | ~30 puntos |
| **NPCs clave** | Lian (vendedora), Hombre atrapado (evento) |
| **Colores dominantes** | Gris ceniza, negro, naranja tenue |
| **IluminaciÃ³n** | Sin sol. Luminosidad ambiental gris/naranja. |
| **Sonido ambiente** | Silencio sepulcral, viento con ceniza, crujido bajo los pies |

---

## N3 â€” Aelthar (Ciudad Elfa)

**TamaÃ±o total:** 400 m Ã— 400 m
**Tipo:** Ciudad sobre lago de espejos
**PropÃ³sito:** Contraste visual con N1/N2. Belleza y serenidad que esconde amenaza bajo el agua. Lore de la civilizaciÃ³n anterior.

### Ãreas

#### 1. Plataforma de Entrada
- **TamaÃ±o:** 50 m Ã— 30 m
- **DescripciÃ³n:** MÃ¡rmol blanco con madreperla. Puente de cuerda de 20 m hacia la primera isla. Vista panorÃ¡mica de toda la ciudad reflejada en agua perfectamente quieta.
- **NPC:** Thalia (guardiana) â€” da la bienvenida, advierte sobre "lo que yace bajo el lago".
- **Fogata:** SÃ­.

#### 2. El Lago de Espejos
- **TamaÃ±o:** 300 m Ã— 300 m
- **DescripciÃ³n:** Agua perfectamente quieta que refleja todo. Ciudad construida sobre islas conectadas por puentes arqueados de piedra blanca. DifÃ­cil distinguir real de reflejo.
- **Plataforma de Hadda:** Isla central (20 m Ã— 20 m). Hadda, bardo elfa, toca mÃºsica. El sonido viaja sobre el agua.
- **IluminaciÃ³n:** Bioluminiscencia azul/verde. La luz parece venir desde abajo.
- **Sonido ambiente:** MÃºsica de Hadda (lute/arpa), agua goteando.

#### 3. Distrito Residencial
- **TamaÃ±o:** Varias islas pequeÃ±as (10-20 m cada una)
- **DescripciÃ³n:** Casas de marfil translÃºcido que brillan desde dentro. Jardines colgantes con flores bioluminiscentes.
- **NPCs:** Kaelen (herrero elfo â€” mejora armas con brillo azul), Sylas (elfo joven â€” quiere explorar el JardÃ­n Hundido), Elena (curandera â€” sabe del GuardiÃ¡n: "no es malvado, solo protege algo").
- **Fogata:** SÃ­, en una plaza con fuente.

#### 4. El JardÃ­n Hundido (Zona Opcional / Lore)
- **TamaÃ±o:** Visible ~200 m Ã— 200 m bajo el agua
- **DescripciÃ³n:** Ciudad Ã©lfica sumergida. Edificios de piedra blanca con algas brillantes. Estatuas rotas.
- **Acceso:** Requiere equipo especial (respirador Ã©lfico o pociÃ³n). MisiÃ³n de Sylas.
- **Contenido:** Lore de la civilizaciÃ³n Ã©lfica, su caÃ­da, la creaciÃ³n de la Torre. Cofre con Capa de Agua (respiraciÃ³n bajo agua por tiempo limitado).
- **Peligros:** Anguilas elÃ©ctricas, corrientes que arrastran.

#### 5. Cueva del Jefe
- **TamaÃ±o:** 100 m Ã— 80 m (detrÃ¡s de una cascada al oeste)
- **DescripciÃ³n:** Cueva de caliza con estalactitas. Piscinas cristalinas. Bioluminiscencia verde.
- **Jefe â€” LeviatÃ¡n / GuardiÃ¡n del Lago:** Serpiente marina gigante, 3 cabezas pequeÃ±as + 1 grande. Cuerpo de ~20 m.
  - **Fase 1:** Cabezas laterales atacan (rÃ¡fagas de agua, mordiscos). Cabeza central dispara rayo a presiÃ³n.
  - **Fase 2 (60% HP):** Se sumerge, emerge en diferentes puntos. Agua sube a cintura.
  - **Fase 3 (30% HP):** Cuerpo completo fuera del agua. Golpes cuerpo a cuerpo + ondas de choque.
  - **DiÃ¡logo (telepÃ¡tico):** "Â¿Has venido por lo que guardo?"
- **Recompensa:** 1 cofre de plata (Arco de Agua â€” flechas que ralentizan), Escama del GuardiÃ¡n, Llave de Cristal.
- **TransiciÃ³n a N4:** Puerta de luz en la pared norte.
- **Fogata:** No (callejÃ³n sin salida hasta vencer al jefe).

### Datos Generales de N3

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Plataforma de Entrada, Distrito Residencial) |
| **Cofres de madera** | 2 (Distrito Residencial, JardÃ­n Hundido) |
| **Cofres de hierro** | 1 (JardÃ­n Hundido â€” respirador Ã©lfico) |
| **Cofres de plata** | 1 (Cueva del Jefe) |
| **Enemigos** | Solo anguilas en JardÃ­n Hundido. Sin enemigos en zona segura. Solo GuardiÃ¡n como jefe. |
| **NPCs clave** | Thalia, Kaelen, Sylas, Elena, Hadda |
| **Misiones** | Principal: Derrotar al GuardiÃ¡n del Lago. Secundaria: Ayudar a Sylas, Escuchar canciÃ³n de Hadda. |
| **Colores dominantes** | Blanco mÃ¡rmol, azul turquesa, verde bioluminiscente |
| **IluminaciÃ³n** | Luz difusa desde abajo. Sin sombras duras. Reflejos perfectos. |
| **Sonido ambiente** | MÃºsica de Hadda, agua goteando, eco en cuevas. Sin viento. |

---

## N4 â€” El Corredor de los Nombres (Bioma)

**TamaÃ±o total:** 800 m Ã— 300 m (corredor lineal)
**Tipo:** CaÃ±Ã³n estrecho, pasillo de los caÃ­dos
**PropÃ³sito:** Silencio, reflexiÃ³n, lore de aventureros anteriores. El pasillo mÃ¡s opresivo de la Torre.

### Ãreas

#### 1. Entrada
- **TamaÃ±o:** 50 m Ã— 30 m
- **DescripciÃ³n:** Puerta de piedra de 10 m de alto desde N3. Inscripciones Ã©lficas: "MÃ¡s allÃ¡ de esta puerta, el silencio mora. Quien habla, invoca."
- **Fogata:** SÃ­.

#### 2. El Pasillo de los Nombres
- **TamaÃ±o:** 500 m Ã— 20-50 m de ancho
- **DescripciÃ³n:** Paredes de 50 m de alto cubiertas de nombres de aventureros caÃ­dos. Nombres recientes brillan. Cada ~50 m, un nombre brilla mucho â€” al interactuar, se ve un recuerdo (lore).
- **Enemigos:** Golems de Madera (lentos, mucho HP, cada ~80 m = 3 total). Ecos Petrificados (rÃ¡pidos, poca vida, grupos de 2-4 cada ~100 m).
- **Sonido ambiente:** SILENCIO ABSOLUTO. Sin mÃºsica. Solo pasos y crujidos.
- **IluminaciÃ³n:** Tenue. Nombres brillan con luz frÃ­a. Techo invisible.

#### 3. Bosque Petrificado (Cueva Lateral)
- **TamaÃ±o:** 100 m Ã— 100 m (abertura camuflada a mitad del pasillo)
- **DescripciÃ³n:** Cueva con Ã¡rboles petrificados. Techo bajo (5-8 m).
- **Enemigos:** Golems de Madera (2-3), Ecos (pocos).
- **Cofre:** 1 de hierro (espada de piedra).

#### 4. La Arena del GuardiÃ¡n
- **TamaÃ±o:** 100 m Ã— 80 m
- **DescripciÃ³n:** Sala circular, techo abierto a luz blanca. Paredes lisas de 5 m.
- **Jefe â€” GuardiÃ¡n Petrificado:** GÃ³lem de 6 m, capas de piedra.
  - **Fase 1:** Golpes que rompen suelo. Invoca 2 Golems pequeÃ±os.
  - **Fase 2 (60% HP):** Se rompe, nÃºcleo brillante expuesto. Proyectiles de piedra.
  - **Fase 3 (25% HP):** Solo cuerpo a cuerpo rÃ¡pido. NÃºcleo brilla antes de atacar.
  - **Post-pelea:** Se desmorona. Susurro colectivo: "...gracias..." Puerta a N5 aparece.
- **Recompensa:** 1 cofre de plata (Armadura de Piedra), Llave de la Puerta.
- **Fogata:** Antes de la Arena.

### Datos Generales de N4

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Entrada, antes de la Arena) |
| **Cofres de hierro** | 1 (Bosque Petrificado) |
| **Cofres de plata** | 1 (Arena del GuardiÃ¡n) |
| **Enemigos** | Golems de Madera (~6), Ecos Petrificados (~10-12), GuardiÃ¡n Petrificado (jefe) |
| **Puntos de spawn** | Sin respawn (zona lineal). DistribuciÃ³n fija. |
| **Misiones** | Principal: Atravesar el Corredor, derrotar GuardiÃ¡n. ExploraciÃ³n: Cueva lateral + nombres brillantes. |
| **Colores dominantes** | Gris piedra, blanco hueso, negro, azul tenue |
| **IluminaciÃ³n** | Casi oscuridad total. Solo nombres tallados y nÃºcleo del GuardiÃ¡n. |
| **Sonido ambiente** | Silencio absoluto. Pasos, crujidos, susurros de ecos. |

---

## N5 â€” Thornwall (Ciudad Fortaleza)

**TamaÃ±o total:** 300 m Ã— 300 m
**Tipo:** Ciudad fortaleza militar
**PropÃ³sito:** Centro de operaciones. NPCs con misiones y mejoras. Ãšltima ciudad "normal" antes de que la Torre se vuelva extraÃ±a.

### Ãreas

#### 1. Puerta Principal
- **TamaÃ±o:** Muralla de 15 m Ã— 50 m
- **DescripciÃ³n:** Muralla de piedra gris. Puerta levadiza reforzada. Foso seco.
- **Guardias:** Yoren (capitÃ¡n) y dos guardias. "Bienvenido a Thornwall. No preguntes cÃ³mo llegamos aquÃ­."
- **Elementos visuales:** Banderas azul/dorado desteÃ±idas. Antorchas. Viento constante.

#### 2. Calle Principal
- **TamaÃ±o:** 200 m Ã— 20 m
- **DescripciÃ³n:** AdoquÃ­n. Edificios de piedra de dos pisos. Faroles de aceite cada 15 m.
- **Comercios:** Herreria de Brom (mejoras con metal de la Torre), Alquimia de Lysandra (pociones), Taberna "El Ãšltimo BastiÃ³n" (Viejo Cazador Mathias).
- **Mathias:** "Lleva aÃ±os ahÃ­ abajo. Nadie ha vuelto para contarlo." (informaciÃ³n sobre el Carnicero).

#### 3. Cuartel
- **TamaÃ±o:** 50 m Ã— 50 m
- **DescripciÃ³n:** Sala de mapas, armario de armas, habitaciÃ³n del comandante.
- **Comandante Roderic/Drenna:** Da misiones oficiales. TablÃ³n con misiones: 1) Limpiar ratas, 2) Derrotar al Carnicero (obligatorio), 3) Estandarte perdido, 4) Herrero desaparecido.
- **Fogata:** SÃ­.

#### 4. Alcantarillas (Mazmorra)
- **TamaÃ±o:** 200 m Ã— 100 m (bajo la ciudad)
- **DescripciÃ³n:** TÃºneles abovedados. Agua sucia hasta el tobillo. Ratas gigantes. Olor a podredumbre.
- **Estructura:** Entrada (rejilla con llave), Pasillos principales, Sala de bombas, Nido del Carnicero.
- **Jefe â€” El Carnicero:** Humanoide deforme de 3 m con gancho oxidado.
  - **Fase 1:** Carga en lÃ­nea recta. Invoca ratas (3 oleadas de 5).
  - **Fase 2 (50% HP):** Golpes circulares del gancho. Charcos de sangre que ralentizan.
  - **Fase 3 (20% HP):** Se arrastra, ataca piernas. RÃ¡pido pero menos daÃ±o.
  - **Llave del Carnicero:** Drop obligatorio para abrir puerta a N6.
- **Recompensa:** 1 cofre de plata (Armadura de Cuero Reforzado), 1 cofre de hierro (espada de hoja ancha).

#### 5. Muralla Este
- **TamaÃ±o:** 30 m Ã— 15 m
- **DescripciÃ³n:** Vista al abismo con puentes rotos. Puerta de hierro se abre con llave.
- **TransiciÃ³n a N6:** Puente de piedra parcialmente intacto hacia las CÃ¡maras de Jade.

### Datos Generales de N5

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Plaza Principal, Cuartel) |
| **Cofres de hierro** | 2 (calle principal misiÃ³n secundaria, Alcantarillas) |
| **Cofres de plata** | 1 (Nido del Carnicero) |
| **Enemigos** | Ratas Gigantes (~30, respawnean parcialmente), El Carnicero (jefe). Ciudad segura. |
| **NPCs clave** | Roderic, Mathias, Brom, Lysandra, Yoren, Hargen |
| **Misiones** | Principal: Derrotar al Carnicero para obtener la llave a N6. Secundarias: TablÃ³n del cuartel. |
| **Colores dominantes** | Gris piedra, azul desteÃ±ido, marrÃ³n oscuro, naranja de antorchas |
| **IluminaciÃ³n** | Cielo artificial blanco/gris. Antorchas en calles. Alcantarillas: solo linternas. |
| **Sonido ambiente** | Viento constante en murallas. Martillo de Brom. Murmullos de taberna. Goteo y chillidos en alcantarillas. |

---

## N6 â€” CÃ¡maras de Jade (Bioma)

**TamaÃ±o total:** 500 m Ã— 500 m (cuevas interconectadas)
**Tipo:** Caverna de cristales
**PropÃ³sito:** Primer bioma "extraÃ±o" de la Torre. Muestra su verdadera naturaleza alienÃ­gena.

### MecÃ¡nica Especial â€” El Zumbido
Los cristales de jade emiten un zumbido constante que cambia de tono. En ciertas Ã¡reas, el zumbido causa alucinaciones (pantalla distorsionada, sombras, voces). Estar mucho tiempo en El Zumbido drena la salud.

### Ãreas

#### 1. Entrada
- **TamaÃ±o:** CaÃ­da de 10 m por agujero en el techo
- **DescripciÃ³n:** Puente de piedra desde N5 â†’ saliente rocoso â†’ caÃ­da controlada a piscina de agua verde brillante.
- **Fogata:** SÃ­, junto a la piscina.

#### 2. CÃ¡maras Principales
- **TamaÃ±o:** 300 m Ã— 300 m (interconectado)
- **DescripciÃ³n:** MÃºltiples salas conectadas por tÃºneles. Paredes de piedra negra pulida con cristales de jade (10 cm a 3 m). Techo a 30 m.
- **Salas:**
  - **Sala de la Entrada:** 50 Ã— 50 m. Pilares con cristales.
  - **Sala del Eco:** 100 Ã— 80 m. Cristales vibran con sonido.
  - **Sala de los Reflejos:** 80 Ã— 60 m. Paredes de cristal que desorientan.
  - **Sala de la CaÃ­da:** 40 Ã— 40 m. Agujero a nivel inferior opcional.
  - **El Zumbido (Ã¡rea central):** 100 Ã— 100 m. Zumbido ensordecedor. Alucinaciones tras 30s.
- **Enemigos:**
  - **Constructos de Jade:** Golems de 3 m, lentos, daÃ±o alto. 1 cada ~80 m (5-6 total).
  - **MurciÃ©lagos de Cristal:** Grupos de 8-12, rÃ¡pidos, daÃ±o bajo pero abruman. Chirrido que aturde.

#### 3. Sala del Coro
- **TamaÃ±o:** 100 m Ã— 80 m
- **DescripciÃ³n:** Arena circular al norte de El Zumbido. Cristales en cÃ­rculo perfecto apuntando al centro. Coro de Jade en el centro.
- **Jefe â€” El Coro de Jade:** Entidad de cristales flotantes que giran alrededor de un nÃºcleo.
  - **Fase 1:** Rayos de luz verde (proyectiles). Cristales giran en patrones.
  - **Fase 2 (60% HP):** Invoca 4 Constructos pequeÃ±os. Zumbido intensificado (alucinaciones).
  - **Fase 3 (30% HP):** Se condensa en cristal gigante. Rayo continuo que esquivar.
  - **DiÃ¡logo (telepÃ¡tico):** "Has llegado lejos... pero el camino se vuelve mÃ¡s extraÃ±o."
  - **Post-pelea:** Se desintegra. Zumbido cesa. Puerta a N7 se abre.
- **Recompensa:** 1 cofre de oro (CorazÃ³n de Jade â€” regeneraciÃ³n lenta fuera de combate), 1 cofre de plata (Armadura de Jade â€” resistencia mÃ¡gica), 1 cofre de hierro (daga de jade).
- **Fogata:** Antes de la Sala del Coro.

### Datos Generales de N6

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Entrada, antes del Coro) |
| **Cofres de hierro** | 2 (CÃ¡maras Principales, Sala del Coro) |
| **Cofres de plata** | 1 (Sala del Coro) |
| **Cofres de oro** | 1 (detrÃ¡s del Coro) |
| **Enemigos** | Constructos de Jade (~6), MurciÃ©lagos de Cristal (~20), Coro de Jade (jefe) |
| **Puntos de spawn** | Constructos sin respawn. MurciÃ©lagos respawnean al tocar cristales. |
| **Misiones** | Principal: Atravesar CÃ¡maras, derrotar al Coro. ExploraciÃ³n: Sala de la CaÃ­da. |
| **Colores dominantes** | Verde jade brillante, negro pulido, blanco tenue |
| **IluminaciÃ³n** | Bioluminiscencia verde. Casi sin sombras. Zumbido cegador. |
| **Sonido ambiente** | Zumbido constante que cambia de tono. Cristales que "cantan" en el Coro. |

---

## N7 â€” Sahal (Ciudad Desierto)

**TamaÃ±o total:** 400 m Ã— 300 m
**Tipo:** Ciudad desÃ©rtica con templo subterrÃ¡neo
**PropÃ³sito:** Momento mÃ¡s oscuro de la historia. Muerte de Mira y Vassen. El jugador experimenta pÃ©rdida.

### Ãreas

#### 1. Puerta de Arena
- **TamaÃ±o:** 30 m Ã— 20 m
- **DescripciÃ³n:** Arco de piedra dorada de 8 m. Cortina de arena que cae constantemente.
- **Elementos visuales:** Arena en cortina. Cielo anaranjado/rojizo. Ondas de calor visibles.

#### 2. Calles de Sahal
- **TamaÃ±o:** 200 m Ã— 200 m
- **DescripciÃ³n:** Adobe beige. Casas de 1-2 pisos. Toldos de lona. Arena en cada esquina. Muchas casas cerradas con tablas.
- **SensaciÃ³n:** Ciudad fantasma. Habitantes escondidos o muertos. Aire de derrota.
- **Edificio notable:** Mezquita en ruinas con cÃºpula derrumbada (refugio temporal).
- **Sonido ambiente:** Arena cruje bajo los pies. Perro ladra a lo lejos. Viento que levanta arena. Sin mÃºsica.

#### 3. El Bazar
- **TamaÃ±o:** 100 m Ã— 100 m
- **DescripciÃ³n:** Ãšnico lugar activo. Puestos de madera y toldos.
- **Comerciantes:**
  - **Zara (niÃ±a de los pÃ¡jaros):** ~12 aÃ±os, cuida pÃ¡jaros en jaulas. InformaciÃ³n sobre la Costurera.
  - **Hakeem:** Mercader de especias. Vende incienso protector.
  - **Ishmael:** Herrero de hojas curvas (sables, cimitarras). Mejora filos.
  - **Rashid:** Guardia que patrulla. Informa sobre desapariciones.
- **Fogata:** Centro del bazar.

#### 4. El Templo de la Costurera (Mazmorra)
- **TamaÃ±o:** 200 m Ã— 150 m (bajo la plaza central)
- **Entrada:** Escotilla de madera, escalera de caracol de 20 m.
- **DescripciÃ³n:** Templo de piedra arenisca. Pasillos estrechos. Celdas con cuerpos cosidos a las paredes.
- **Evento narrativo â€” Muerte de Mira y Vassen:**
  - Mira y Vassen (aventureros conocidos antes) entran primero al templo.
  - La Costurera embosca en la primera sala. Mira muere atravesada. Vassen atrapado y cosido a la pared.
  - El jugador solo puede observar. PropÃ³sito: dar peso emocional al combate.
- **Estructura:**
  - **Sala del Sacrificio:** 50 Ã— 40 m. Donde ocurre la emboscada.
  - **Pasillos de las Agujas:** 80 m de largo con trampas de agujas.
  - **Sala de los Hilos:** 60 Ã— 50 m. TelaraÃ±as de acero cortantes.
  - **La Arena Final:** 80 Ã— 80 m.
- **Enemigos:** Hijos de la Costurera (humanoides envueltos en hilo, ~15), TelaraÃ±as de Acero (trampas).

#### 5. La Arena Final
- **TamaÃ±o:** 80 m Ã— 80 m
- **DescripciÃ³n:** Sala circular, techo abovedado de 15 m. Paredes con cuerpos cosidos. Telar gigante de hueso y metal en el centro.
- **Jefe â€” La Costurera:** Anciana encorvada, dedos como agujas, vestido de retazos humanos. EmpuÃ±a dos agujas de tejer de metal.
  - **Fase 1:** Teje hilos de acero (trampas). Agujas (proyectiles). Invoca 2 Hijos.
  - **Fase 2 (50% HP):** Hilos que atrapan (inmovilizan). Cuerpo a cuerpo con agujas.
  - **Fase 3 (20% HP):** Telar roto. Se transforma: cuerpo alargado, brazos-hilos. Ataca desde el techo.
  - **DiÃ¡logo:** "Mira y Vassen... no fueron los primeros. La Torre necesita su carne."
  - **Post-pelea:** Se deshace en hilos. Telar roto. Puerta de luz a N8.
- **Recompensa:** 1 cofre de oro (La Aguja de los Recuerdos â€” daga de daÃ±o psÃ­quico), 1 cofre de plata (TÃºnica de Hilo), 1 cofre de hierro, Cartas de Mira y Vassen.
- **Fogata:** Dentro de la Arena, antes del combate.

### Datos Generales de N7

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Bazar, entrada al Templo) |
| **Cofres de hierro** | 2 (Bazar misiÃ³n, Templo) |
| **Cofres de plata** | 1 (Sala de los Hilos) |
| **Cofres de oro** | 1 (Arena Final) |
| **Enemigos** | Hijos de la Costurera (~15), La Costurera (jefe). Calles de Sahal: casi sin enemigos. |
| **NPCs clave** | Zara, Hakeem, Ishmael, Rashid, El Mercader de Recuerdos |
| **Misiones** | Principal: Derrotar a la Costurera. Evento narrativo: Muerte de Mira y Vassen. |
| **Colores dominantes** | Adobe beige, cielo anaranjado/rojizo, marrÃ³n oscuro |
| **IluminaciÃ³n** | Luz de sol artificial de la Torre (naranja). Antorchas en el templo. |
| **Sonido ambiente** | Arena crujiente, viento, perros lejanos. En templo: silencio hÃºmedo, agujas rasgando. |

---

## N8 â€” Jardines Flotantes (Bioma)

**TamaÃ±o total:** 600 m Ã— 600 m (vertical, mÃºltiples niveles)
**Tipo:** Plataformas flotantes / vertical
**PropÃ³sito:** Primer encuentro con Sera. Plataformas sobre el vacÃ­o. El jugador debe navegar verticalmente mientras lucha contra enemigos aÃ©reos.

### Estructura General

Plataformas flotantes de piedra blanca a diferentes alturas (10-50 m sobre el vacÃ­o). Se accede a travÃ©s de escaleras de cuerda, puentes colgantes y saltos. El vacÃ­o blanco estÃ¡ debajo de todo.

### Ãreas

#### 1. Plataforma de Llegada
- **TamaÃ±o:** 50 m Ã— 50 m
- **DescripciÃ³n:** Donde aparece Sera por primera vez. Observa al jugador desde lejos, desde una plataforma mÃ¡s alta (a 30 m). No interactÃºa â€” solo observa y desaparece.
- **Elementos visuales:** Flores blancas que brillan. Piedra blanca con enredaderas verdes.
- **Fogata:** SÃ­.

#### 2. Los Jardines Bajos
- **Altura:** 10-20 m sobre el vacÃ­o
- **DescripciÃ³n:** Varias plataformas conectadas por puentes de cuerda que crujen. Flores brillantes (blancas, azules, violetas). Estatuas rotas de Ã¡ngeles y bestias.
- **Enemigos:** GÃ¡rgolas de Bronce (patrullan plataformas). Flores CarnÃ­voras (estacionarias, atrapan si te acercas).
- **Elementos visuales:** Enredaderas que cuelgan hacia el vacÃ­o. PÃ©talos brillantes que caen lentamente.
- **Fogata:** SÃ­.

#### 3. Los Jardines Altos
- **Altura:** 30-50 m
- **DescripciÃ³n:** Plataformas mÃ¡s grandes. Viento mÃ¡s fuerte que afecta el salto. Sombras del Tejido se mueven sigilosamente.
- **Enemigos adicionales:** Sombras del Tejido (sigilosas, aparecen detrÃ¡s del jugador, atacan por sorpresa. Poca vida, mucho daÃ±o si no se detectan).
- **Elementos visuales:** Viento que mueve flores y enredaderas. Luz blanca mÃ¡s intensa.
- **Fogata:** SÃ­.

#### 4. El Nido de la ArpÃ­a
- **TamaÃ±o:** 100 m Ã— 100 m (a 40 m de altura, plataforma central)
- **DescripciÃ³n:** La plataforma mÃ¡s grande. CÃ­rculo de piedra blanca con un altar en el centro y plumas de bronce esparcidas.
- **Jefe â€” ArpÃ­a de Bronce:** Criatura humanoide-ave de 4 m de envergadura, plumas de bronce, garras afiladas.
  - **Fase 1:** Vuela en cÃ­rculos, dispara plumas como proyectiles. Aterriza para ataques de garras.
  - **Fase 2 (50% HP):** Invoca 2 GÃ¡rgolas de Bronce. Vuela mÃ¡s rÃ¡pido, crea rÃ¡fagas de viento que empujan.
  - **Fase 3 (20% HP):** Cae al centro, plumas rotas. Ataca frenÃ©ticamente con garras y pico.
  - **Post-pelea:** Cae al suelo. Se desintegra en plumas de bronce. Puerta de luz al norte.
- **Recompensa:** 1 cofre de oro (Alas de Bronce â€” permite planear por 5 segundos), 3 cofres de plata (armadura ligera, arco de plumas, objetos).
- **Fogata:** No hay en la plataforma del jefe (es arena final).
- **TransiciÃ³n a N9:** Puerta de luz detrÃ¡s del altar.

### Datos Generales de N8

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 3 (Llegada, Jardines Bajos, Jardines Altos) |
| **Cofres de plata** | 3 (dos en Jardines Altos, uno en Jardines Bajos) |
| **Cofres de oro** | 1 (Nido de la ArpÃ­a) |
| **Enemigos** | GÃ¡rgolas de Bronce (~8), Flores CarnÃ­voras (~12 estacionarias), Sombras del Tejido (~6 sigilosas), ArpÃ­a de Bronce (jefe) |
| **Puntos de spawn** | GÃ¡rgolas respawnean parcialmente. Sombras no. Flores son estÃ¡ticas. |
| **Misiones** | Principal: Llegar al Nido, derrotar a la ArpÃ­a. Evento: Primer avistamiento de Sera. |
| **Colores dominantes** | Blanco piedra, verde enredadera, bronce (plumas), violeta (flores) |
| **IluminaciÃ³n** | Luz blanca perpetua de las flores. Vacio blanco debajo. Sin sombras duras. |
| **Sonido ambiente** | Viento suave, crujido de puentes de cuerda, aleteo de gÃ¡rgolas. |

---

## N9 â€” Argentia (Ciudad de Plata)

**TamaÃ±o total:** 350 m Ã— 350 m
**Tipo:** Ciudad de plata / Ãºltimo bastiÃ³n
**PropÃ³sito:** Ãšltima ciudad antes del jefe final. PreparaciÃ³n, descanso, lore final. Contraste con el resto de la Torre â€” aquÃ­ todo es limpio y plateado.

### Ãreas

#### 1. Puerta de Plata
- **TamaÃ±o:** Arco de 10 m Ã— 8 m
- **DescripciÃ³n:** Arco de metal pulido que brilla intensamente. Sin puerta real â€” solo un umbral de luz plateada.
- **Elementos visuales:** Metal pulido reflectante. Luz frÃ­a. Sin polvo ni suciedad.

#### 2. Plaza Principal
- **TamaÃ±o:** 150 m Ã— 150 m
- **DescripciÃ³n:** Plaza de mÃ¡rmol blanco con fuentes de agua cristalina. Edificios plateados de lÃ­neas limpias. Faroles de luz frÃ­a.
- **Comercios:**
  - **Herreria de Ferrus:** Herrero que trabaja con plata viva. Mejora armas al mÃ¡ximo nivel.
  - **CartÃ³grafa Iris/Rashida:** Mapa del camino a N10 (puzle para encontrar la entrada correcta).
  - **La Posada del Moribundo:** Posada limpia. El Moribundo (NPC) â€” un aventurero que llegÃ³ hasta aquÃ­ pero no pudo continuar. Da el Ãºltimo lore sobre el Centinela.
- **Fogata:** SÃ­, en la plaza.

#### 3. El BastiÃ³n
- **TamaÃ±o:** 100 m Ã— 100 m
- **DescripciÃ³n:** Cuartel de la Capitana Elia. Ãšltimo bastiÃ³n militar antes de N10. Edificio de plata y mÃ¡rmol.
- **Capitana Elia:** Comandante seria que prepara al jugador para el jefe final. "No sÃ© quÃ© hay mÃ¡s allÃ¡. Nadie ha vuelto para contarlo."
- **MisiÃ³n de Elia:** Recuperar un objeto del Verdugo de Plata (encerrado en la mazmorra). Recompensa: uno de los mejores objetos del juego.
- **Fogata:** SÃ­.

#### 4. La Mazmorra del Verdugo
- **TamaÃ±o:** 100 m Ã— 80 m (bajo el BastiÃ³n)
- **DescripciÃ³n:** Mazmorra de plata y mÃ¡rmol blanco. Celdas vacÃ­as y limpias. El Verdugo estÃ¡ en la celda mÃ¡s profunda.
- **Jefe menor â€” El Verdugo de Plata:** Caballero de armadura plateada, sin rostro visible. EmpuÃ±a un hacha de plata.
  - MecÃ¡nicas simples: carga, golpe en Ã¡rea, combo de 3 golpes.
  - Drop: Hacha de Plata (requisito para la misiÃ³n de Elia).
- **Recompensa de Elia:** Armadura de Plata (mÃ¡xima defensa, resistencia mÃ¡gica) + 1 cofre de oro.

#### 5. La Puerta a N10
- **TamaÃ±o:** 20 m Ã— 10 m
- **DescripciÃ³n:** Escalera infinita hacia arriba desde el centro de la plaza. Solo luz blanca al final. No hay paredes â€” solo escalones que flotan hacia el vacÃ­o blanco.
- **Elementos visuales:** Escalones de plata que flotan. Luz blanca que lo envuelve todo. Silencio absoluto.
- **Nota de diseÃ±o:** El jugador sabe que es el final. No hay vuelta atrÃ¡s una vez que sube.

### Datos Generales de N9

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | 2 (Plaza Principal, BastiÃ³n) |
| **Cofres de plata** | 2 (plaza por misiones, mazmorra del Verdugo) |
| **Cofres de oro** | 2 (misiÃ³n de Elia, Verdugo) |
| **Enemigos** | Solo el Verdugo de Plata (jefe menor). Ciudad completamente segura. |
| **NPCs clave** | Elia (capitana), Ferrus (herrero), Iris/Rashida (cartÃ³grafa), El Moribundo, Caballero de Plata CaÃ­do (espÃ­ritu opcional) |
| **Misiones** | Principal: Prepararse para N10. Secundaria: MisiÃ³n de Elia (Verdugo + recompensa). Lore con El Moribundo. |
| **Colores dominantes** | Plata pulida, mÃ¡rmol blanco, luz frÃ­a |
| **IluminaciÃ³n** | Luz blanca frÃ­a y limpia. Sin sombras. Contrasta con el resto de la Torre. |
| **Sonido ambiente** | Silencio respetuoso. Pasos que ecoan en el mÃ¡rmol. Agua de fuentes. |

---

## N10 â€” El Centinela de Granito (Jefe Final)

**TamaÃ±o:** Plataforma circular de 50 m de diÃ¡metro
**Tipo:** Arena de jefe final
**PropÃ³sito:** La cima de la Torre. El enfrentamiento definitivo. Decisiones que determinan el final.

### Estructura

Una sola plataforma circular de piedra gris flotando en el vacÃ­o blanco infinito. No hay paredes, no hay techo, no hay suelo mÃ¡s allÃ¡ de la plataforma. Solo blanco absoluto y la plataforma gris.

### El Entorno
- **VacÃ­o blanco:** No se ve nada mÃ¡s allÃ¡ de la plataforma. El blanco es total, sin textura, sin horizonte.
- **Silencio:** No hay mÃºsica. No hay viento. Solo el sonido de los pasos del jugador en la piedra.
- **SensaciÃ³n:** Aislamiento total. El jugador y el Centinela son los Ãºnicos seres en la existencia.

### El Centinela
- **DescripciÃ³n:** Estatua de 8 m de alto en el centro de la plataforma. Granito gris. InmÃ³vil, sin ojos visibles. No se mueve hasta que el jugador se acerca a 20 m.

### Fases del Combate

#### Fase 1 â€” El Centinela FÃ­sico
- **ActivaciÃ³n:** Al acercarse a 20 m, el Centinela cobra vida. Se mueve lentamente.
- **Ataques:**
  - **Golpe de puÃ±o:** Brazo de piedra golpea el suelo. Crea ondas de choque que se expanden.
  - **PisotÃ³n:** Pisa fuerte, rompe secciones del suelo (crea agujeros).
  - **Carga:** Se desplaza lentamente hacia el jugador, brazos extendidos.
- **Punto dÃ©bil:** Las grietas en su torso (donde el granito es mÃ¡s delgado).

#### Fase 2 â€” El NÃºcleo (60% HP)
- **TransiciÃ³n:** El Centinela se detiene. Su torso se abre, revelando un nÃºcleo brillante (luz dorada/blanca).
- **Ataques:**
  - **Rayos de luz:** El nÃºcleo dispara rayos de luz dorada en todas direcciones.
  - **InvocaciÃ³n de escombros:** Fragmentos de piedra levitan y se lanzan hacia el jugador.
  - **Golpes mÃ¡s rÃ¡pidos:** Los brazos de piedra se mueven mÃ¡s rÃ¡pido.
- **Punto dÃ©bil:** El nÃºcleo mismo. Atacar el nÃºcleo causa mÃ¡s daÃ±o pero expone al jugador a contraataques.

#### Fase 3 â€” El Arquitecto (20% HP)
- **TransiciÃ³n:** El Centinela se rompe completamente. Del interior emerge una figura humanoide de luz blanca brillante. El Arquitecto.
- **DescripciÃ³n:** Humanoide de 3 m de alto, hecho de luz blanca pura, sin rasgos faciales. Flota sobre el suelo.
- **DiÃ¡logo:**
  - "Has llegado. No muchos lo logran."
  - "Esta Torre... la construÃ­ para contener algo. Pero al final, la Torre se convirtiÃ³ en la prisiÃ³n. Para todos nosotros."
  - "Puedes liberarme. O puedes quedarte. La elecciÃ³n es tuya."
- **Ataques:**
  - **Rayos de luz blanca:** Proyectiles rÃ¡pidos en patrones.
  - **TeletransportaciÃ³n:** Desaparece y reaparece en otra posiciÃ³n.
  - **PrisiÃ³n de luz:** Te atrapa en un cÃ­rculo de luz que se cierra. Debes romperlo antes de que se cierre.
- **Nota:** El Arquitecto no ataca para matar. Sus ataques son precisos, casi como un examen final. El jugador ha demostrado ser digno.

### DespuÃ©s del Combate

El Arquitecto se desvanece. El suelo de la plataforma se regenera (los agujeros se cierran). Aparecen dos opciones:

#### Final A â€” La Puerta de Luz
- **DescripciÃ³n:** Puerta de luz blanca en el borde de la plataforma.
- **Significado:** El jugador decide liberar al Arquitecto y abrir la Torre. La luz se derrama hacia abajo, liberando a los atrapados.
- **Final:** El jugador camina hacia la luz. Se ve ascender. La Torre se desvanece lentamente.
- **Post-crÃ©ditos:** Escena de Emberveil al amanecer. El cielo es real por primera vez. Gente mirando hacia arriba.

#### Final B â€” El Trono de Piedra Blanca
- **DescripciÃ³n:** Trono de piedra blanca en el centro de la plataforma.
- **Significado:** El jugador decide quedarse y ocupar el lugar del Arquitecto. Se sienta en el trono. La Torre permanece.
- **Final:** El jugador se sienta. La cÃ¡mara se aleja. La Torre se ve desde afuera, completa. Una nueva era comienza.
- **Post-crÃ©ditos:** Escena de Sera (de N8) ascendiendo por la Torre, ahora vacÃ­a de enemigos. El ciclo comienza de nuevo.

### Datos Generales de N10

| Elemento | Cantidad/Detalle |
|----------|-----------------|
| **Fogatas** | NINGUNA. No hay puntos de control. Si mueres, reinicias en N9. |
| **Cofres de oro** | 2 (detrÃ¡s de donde estaba el Centinela) |
| **Enemigos** | Solo el Centinela de Granito + el Arquitecto. No hay enemigos menores. |
| **Misiones** | Final del juego. DecisiÃ³n: Puerta de Luz (Final A) o Trono de Piedra (Final B). |
| **Colores dominantes** | Blanco absoluto, gris plataforma, luz dorada/blanca |
| **IluminaciÃ³n** | Luz blanca sin fuente aparente. El vacÃ­o blanco lo envuelve todo. |
| **Sonido ambiente** | Silencio absoluto hasta que el Centinela despierta. Luego: impacto de piedra, luz zumbando. |

---

## Resumen General

### Tabla de Pisos

| Piso | TamaÃ±o | Tipo | Fogatas | Cofres (M/H/P/O) | Enemigos principales |
|:----:|:------:|------|:-------:|:-----------------:|----------------------|
| **N0** | 2 km Ã— 2 km | Overworld | 5 | 3/1/0/0 | JabalÃ­es, Lobos, Serpientes, El Errante, GuardiÃ¡n de la Primera Garganta |
| **N1** | 500 Ã— 300 m | Ciudad vertical | 3 | 2/1/0/0 | Almas Errante, GÃ¡rgolas Durmientes, La Voz |
| **N2** | 1 km Ã— 1 km | Bioma abierto | 3 | 2/1/0/0 | Perros de Ceniza, Cazadores Calcinados, El Pastor |
| **N3** | 400 Ã— 400 m | Ciudad lago | 2 | 2/1/1/0 | Anguilas, GuardiÃ¡n del Lago |
| **N4** | 800 Ã— 300 m | Corredor lineal | 2 | 0/1/1/0 | Golems de Madera, Ecos Petrificados, GuardiÃ¡n Petrificado |
| **N5** | 300 Ã— 300 m | Ciudad fortaleza | 2 | 0/2/1/0 | Ratas Gigantes, El Carnicero |
| **N6** | 500 Ã— 500 m | Caverna cristales | 2 | 0/2/1/1 | Constructos de Jade, MurciÃ©lagos, Coro de Jade |
| **N7** | 400 Ã— 300 m | Ciudad desierto | 2 | 0/2/1/1 | Hijos de la Costurera, La Costurera |
| **N8** | 600 Ã— 600 m(v) | Vertical | 3 | 0/0/3/1 | GÃ¡rgolas, Flores CarnÃ­voras, Sombras, ArpÃ­a de Bronce |
| **N9** | 350 Ã— 350 m | Ciudad plata | 2 | 0/0/2/2 | Verdugo de Plata |
| **N10** | 50 m Ã¸ | Arena final | 0 | 0/0/0/2 | Centinela de Granito + El Arquitecto |

### Totales

| Elemento | Cantidad |
|----------|:--------:|
| **Fogatas totales** | ~26 |
| **Cofres de madera** | ~9 |
| **Cofres de hierro** | ~11 |
| **Cofres de plata** | ~10 |
| **Cofres de oro** | ~7 |
| **Cofres totales** | ~37 |
| **Enemigos Ãºnicos** | ~25 tipos diferentes |
| **Jefes** | 11 (uno por piso, mÃ¡s GuardiÃ¡n en N0) |
| **NPCs con nombre** | ~35 |

### DistribuciÃ³n de Tipos de Piso
- **Ciudades:** 5 (N0 parcial, N1, N3, N5, N7, N9)
- **Biomas abiertos:** 3 (N2, N6, N8)
- **Corredor lineal:** 1 (N4)
- **Arena de jefe final:** 1 (N10)

### ProgresiÃ³n de Dificultad
- **N0-N2:** Tutorial / baja. El jugador aprende mecÃ¡nicas bÃ¡sicas.
- **N3-N5:** Media. Introduce mecÃ¡nicas especiales (medidor de calor, sigilo en N4).
- **N6-N8:** Alta. AlienÃ­gena. Enemigos con patrones complejos. Alucinaciones.
- **N9-N10:** MÃ¡xima. PreparaciÃ³n + jefe final. Sin errores.

---

## Notas de DiseÃ±o Adicionales
  - N9: Misión de Elia, conversación con el Moribundo (lore final del juego)
