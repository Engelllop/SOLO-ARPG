# Sistema de Crafting Completo — SOLO (Sunlight of the Last Online)

> Versión: 1.0  
> Creado: 2026-04-29  
> Propósito: Definir el sistema completo de creación de objetos para SOLO, incluyendo estaciones, materiales, recetas, maestría y economía.

---

## 1. Estaciones de Crafting

El crafting en SOLO se realiza en **estaciones especializadas** distribuidas por las ciudades de los pisos principales. Cada ciudad tiene un **nivel de estación** que determina la rareza máxima de objetos que se pueden crear allí.

### 1.1 Tipos de Estación

| Estación | Función Principal | Clase Beneficiada |
|---|---|---|
| **Fragua del Herrero** | Forjar armas, armaduras, escudos, herramientas | Herrero, todas las clases (equipo) |
| **Mesa de Alquimia** | Pociones, elíxires, venenos, antídotos | Alquimista, todas las clases (consumibles) |
| **Taller de Encantamiento** | Encantar armas/armaduras, insertar gemas | Alquimista, Mago |
| **Telar de Tejidos** | Armadura ligera, túnicas, capas | Druida, Mago, Arquero |
| **Torno de Flechas** | Munición, arcos, ballestas | Cazador, Arquero |
| **Yunque de Reparación** | Reparar equipo dañado | Herrero (todas las clases) |
| **Banco de Gemas** | Cortar y pulir gemas para inserción | Alquimista, Herrero |
| **Altar de Esencias** | Refinar esencias, crear materiales raros | Alquimista, Mago, Nigromante |

### 1.2 Niveles de Estación por Ciudad

| Ciudad | Piso | Nivel Estación | Rareza Máxima | Rango Máximo | Notas |
|---|---|---|---|---|---|
| **Emberveil** | N0 | ★☆☆☆☆ | Común | E | Solo recetas de iniciación |
| **Velmar** | N1 | ★★☆☆☆ | Poco Común | D | Herrero básico disponible |
| **Aelthar** | N3 | ★★★☆☆ | Raro | C | Taller de encantamiento básico |
| **Thornwall** | N5 | ★★★★☆ | Épico | B | Forja avanzada, gemas |
| **Sahal** | N7 | ★★★★☆ | Épico | B | Altar de esencias superior |
| **Argentia** | N9 | ★★★★★ | Legendario | A | Mejor herrero del mundo |

**Nota:** Argentia (N9) es la única ciudad donde se pueden fabricar objetos de **rango A** (Legendarios y Míticos de alto nivel). Para objetos Míticos de rango S o superior, se requieren estaciones especiales fuera de las ciudades o misiones especiales.

---

## 2. Materiales

### 2.1 Minerales

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Fragmento de Hierro | Común | N1 Velmar (minas), N2 (restos quemados) | Picar vetas de hierro dañadas |
| Hierro | Común | N1, N2, N3 | Fundir 3 Fragmentos de Hierro |
| Acero | Poco Común | N3 Aelthar, N4 bosque petrificado | Fundir Hierro + Carbón Mineral |
| Plata | Poco Común | N3 (cuevas plateadas), N5 | Vetas de plata |
| Oro | Raro | N5 Thornwall, N6 cámaras jade | Vetas de oro, cofres |
| Jade | Raro | N6 cámaras jade, N7 Sahal | Vetas de jade, drop de gólems |
| Cristal de Alma | Épico | N7 Sahal, N8 jardines flotantes | Criaturas mágicas, geodas |
| Piedra de Toque | Legendario | N9 Argentia, N10 Centinela | Eventos mundiales, jefes de piso |

### 2.2 Madera

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Madera Podrida | Común | N1 Velmar (bosque muerto), N2 | Árboles muertos |
| Roble Viejo | Común | N1, N3 Aelthar | Talar robles |
| Madera Petrificada | Poco Común | N4 bosque petrificado | Árboles petrificados |
| Raíz de Jade | Raro | N6 cámaras jade | Raíces brillantes en cavernas |
| Ceniza de Árbol | Épico | N2 (bioma quemado), N5 | Árboles quemados por fuego ancestral |

### 2.3 Pieles y Cuero

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Piel de Lobo | Común | N1 Velmar | Lobos grises, lobos de hielo |
| Cuero Curtido | Común | Cualquier piso | Procesar 3 pieles con sal |
| Piel de Gárgola | Poco Común | N3 Aelthar, N5 | Gárgolas de piedra |
| Escama de Serpiente | Raro | N4 bosque petrificado, N7 Sahal | Serpientes gigantes |
| Piel de Arpía | Épico | N6 cámaras jade, N8 | Arpías jade |

### 2.4 Esencias

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Esencia de Fuego | Común | N2 bioma quemado | Elementales de fuego |
| Esencia de Agua | Común | N1 (ríos), N3 | Elementales de agua |
| Esencia de Jade | Raro | N6 cámaras jade | Constructos de jade |
| Esencia del Velo | Épico | N8 jardines flotantes | Criaturas etéreas |
| Polvo de Ceniza | Poco Común | N2, N5 | Criaturas quemadas, cenizas de batalla |

### 2.5 Partes de Monstruos

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Colmillo de Jabalí | Común | N1 Velmar | Jabalíes de Velmar |
| Garra de Oso | Común | N1, N3 | Osos pardos |
| Ojo de Murciélago | Común | N1 cuevas, N3 sótanos | Murciélagos de cueva |
| Aguijón de Escorpión | Poco Común | N2 bioma quemado, N7 | Escorpiones de fuego |
| Ala de Gárgola | Poco Común | N3, N5 | Gárgolas |
| Corazón de Constructo | Raro | N6 cámaras jade, N9 | Gólems, autómatas |

### 2.6 Materiales Raros / Únicos

| Material | Rareza | Pisos/Zonas | Cómo se Obtiene |
|---|---|---|---|
| Lágrima de Elfa | Épico | N3 Aelthar (misiones élficas) | Misiones de reputación Honrado |
| Piedra de Toque | Legendario | N9, N10 | Ver minerales |
| Fragmento del Tejido | Mítico | N10 Centinela | Jefe final, eventos de mundo |
| Ceniza de Estrella | Legendario | N8 jardines flotantes (noche) | Evento de lluvia de estrellas |
| Esencia del Borrador | Mítico | N10 | El Guardián del Tejido |

---

## 3. Recetas por Categoría

### 3.1 Armas

Cada tipo de arma tiene **6 niveles** progresivos. Los nombres de nivel son:

1. **Bronce** (Común, Rango E)
2. **Hierro** (Común, Rango D)
3. **Acero** (Poco Común, Rango C)
4. **Plateada/Élite** (Raro, Rango B)
5. **Oro/Jade** (Épico, Rango A)
6. **Legendaria** (Legendario, Rango S)

#### Espadas

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Espada de Bronce | Común | E |
| 2 | Espada de Hierro | Común | D |
| 3 | Espada de Acero Templado | Poco Común | C |
| 4 | Espada Plateada del Alba | Raro | B |
| 5 | Espada de Jade del Sol | Épico | A |
| 6 | Hoja del Tejido Solar | Legendario | S |

#### Arcos

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Arco de Madera Podrida | Común | E |
| 2 | Arco de Roble | Común | D |
| 3 | Arco Compuesto de Acero | Poco Común | C |
| 4 | Arco Plateado de Aelthar | Raro | B |
| 5 | Arcoluna de Jade | Épico | A |
| 6 | Gran Arco del Centinela | Legendario | S |

#### Dagas

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Daga de Hueso | Común | E |
| 2 | Daga de Hierro | Común | D |
| 3 | Daga de Acero Afilado | Poco Común | C |
| 4 | Daga de Sombra Plateada | Raro | B |
| 5 | Daga de Veneno de Jade | Épico | A |
| 6 | Aguijón del Borrador | Legendario | S |

#### Hachas (y Hachas Dobles)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Hacha de Leñador | Común | E |
| 2 | Hacha de Batalla de Hierro | Común | D |
| 3 | Hacha Doble de Acero | Poco Común | C |
| 4 | Hacha Plateada del Norte | Raro | B |
| 5 | Partecráneos de Jade | Épico | A |
| 6 | Hacha de la Ceniza Eterna | Legendario | S |

#### Bastones y Báculos

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Bastón de Madera Podrida | Común | E |
| 2 | Bastón de Roble | Común | D |
| 3 | Báculo de Ceniza | Poco Común | C |
| 4 | Bastón Plateado Lunar | Raro | B |
| 5 | Báculo de Jade de Sahal | Épico | A |
| 6 | Cetro del Tejido Estelar | Legendario | S |

#### Martillos y Mazas

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Martillo de Herrero | Común | E |
| 2 | Maza de Hierro | Común | D |
| 3 | Martillo de Guerra de Acero | Poco Común | C |
| 4 | Maza Plateada del Juicio | Raro | B |
| 5 | Martillo de Jade de Argentia | Épico | A |
| 6 | Martillo del Creador | Legendario | S |

#### Katanas (Exclusiva de Sera)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Hoja de Iniciación | Común | E |
| 2 | Katana de Hierro | Común | D |
| 3 | Katana del Viento de Acero | Poco Común | C |
| 4 | Hoja Plateada del Amancer | Raro | B |
| 5 | Katana de Jade Carmesí | Épico | A |
| 6 | Filo del Sol Poniente | Legendario | S |
| 7 | **Hoja Única: Sombra del Último Sol** | Mítico | SS |

#### Hojas Gemelas (Exclusiva de Garrik)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Colmillos de Lobo | Común | E |
| 2 | Hojas de Hierro Gemelas | Común | D |
| 3 | Hojas Gemelas de Acero | Poco Común | C |
| 4 | Hojas Plateadas del Crepúsculo | Raro | B |
| 5 | Hojas de Jade de la Tormenta | Épico | A |
| 6 | Hojas del Velo Partido | Legendario | S |
| 7 | **Hoja Única: Baile de Cuchillas Eternas** | Mítico | SS |

---

### 3.2 Armaduras

#### Armadura Ligera (Cuero/Mallas)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Túnica de Piel de Lobo | Común | E |
| 2 | Armadura de Cuero Curtido | Común | D |
| 3 | Malla de Gárgola | Poco Común | C |
| 4 | Armadura de Escamas Plateadas | Raro | B |
| 5 | Coraza de Jade Etéreo | Épico | A |
| 6 | Vestimenta del Velo Solar | Legendario | S |

#### Armadura Pesada (Placas)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Placas de Bronce | Común | E |
| 2 | Armadura de Placas de Hierro | Común | D |
| 3 | Armadura de Acero Completa | Poco Común | C |
| 4 | Placas Plateadas del Guardián | Raro | B |
| 5 | Armadura de Jade del Héroe | Épico | A |
| 6 | Coraza del Sol Eterno | Legendario | S |

#### Túnicas / Batas (Mago)

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Túnica de Aprendiz | Común | E |
| 2 | Túnica de Roble Tejido | Común | D |
| 3 | Bata de Ceniza y Seda | Poco Común | C |
| 4 | Túnica Plateada del Vidente | Raro | B |
| 5 | Vestiduras de Jade de Sahal | Épico | A |
| 6 | Túnica del Tejedor Estelar | Legendario | S |

#### Escudos

| Nivel | Nombre | Rareza | Rango |
|---|---|---|---|
| 1 | Escudo de Madera Podrida | Común | E |
| 2 | Escudo de Hierro | Común | D |
| 3 | Escudo de Acero Torres | Poco Común | C |
| 4 | Escudo Plateado del Alba | Raro | B |
| 5 | Escudo de Jade del Guardián | Épico | A |
| 6 | Muro del Sol Inquebrantable | Legendario | S |

### Accesorios (6 recetas)

---

**Receta #17: Anillo de Plata del Cazador**
- **Rareza:** Raro
- **Rango Requerido:** B
- **Materiales:** Plata x2, Colmillo de Jabalí x2, Esencia de Fuego x1
- **Estación:** Fragua del Herrero (nivel 4+)
- **Ciudad Mínima:** Thornwall (N5)
- **Tiempo Estimado:** 3 minutos
- **Efecto/Estadísticas:** +6% daño, +3% crítico

---

**Receta #18: Anillo del Sol Furioso**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Oro x3, Esencia del Velo x2, Fragmento del Tejido x1
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 15 minutos
- **Efecto/Estadísticas:** +15% daño, +8% crítico, +5% velocidad de ataque. Al matar, 15% de ganar furia (+20% daño 5s).

---

**Receta #19: Amuleto Plateado de Aelthar**
- **Rareza:** Raro
- **Rango Requerido:** B
- **Materiales:** Plata x2, Lágrima de Elfa x1, Esencia de Agua x2, Piel de Gárgola x1
- **Estación:** Taller de Encantamiento (nivel 4+)
- **Ciudad Mínima:** Aelthar (N3, reputación Respetado+)
- **Tiempo Estimado:** 5 minutos
- **Efecto/Estadísticas:** +8% regeneración de maná, +5% curación recibida

---

**Receta #20: Amuleto del Último Sol**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Piedra de Toque x1, Ceniza de Estrella x1, Oro x3, Esencia del Velo x2
- **Estación:** Taller de Encantamiento (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 20 minutos
- **Efecto/Estadísticas:** +15% todas las resistencias, +8% defensa. Pasiva: "Resplandor del Sol" — al recibir daño letal, sobrevive con 1 HP (cooldown 5 min).

---

**Receta #21: Botas del Velo Solar**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x1, Esencia del Velo x3, Piel de Arpía x2, Raíz de Jade x1
- **Estación:** Telar de Tejidos (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 12 minutos
- **Efecto/Estadísticas:** +15% velocidad, +10% salto, +5% evasión. Pasiva: "Paso del Velo" — esquivar un ataque otorga invisibilidad 2s.

---

**Receta #22: Guantes del Tejedor**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Jade x2, Piel de Arpía x2, Oro x2, Esencia del Velo x1
- **Estación:** Telar de Tejidos (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 12 minutos
- **Efecto/Estadísticas:** +10% crítico, +8% precisión, +3% velocidad de ataque. Pasiva: "Toque Preciso" — los golpes críticos ignoran 10% de defensa.

---

### Consumibles (6 recetas)

---

**Receta #23: Poción de Vida Mayor**
- **Rareza:** Poco Común
- **Rango Requerido:** C
- **Materiales:** Esencia de Agua x2, Raíz de Jade (triturada) x1, Cristal Vacío x1
- **Estación:** Mesa de Alquimia (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 30 segundos (x5 pociones)
- **Efecto/Estadísticas:** Cura 500 HP instantáneo

---

**Receta #24: Poción de Vida Élfica**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Esencia del Velo x2, Lágrima de Elfa x1, Cristal de Alma (triturado) x1, Cristal Vacío x1
- **Estación:** Mesa de Alquimia (nivel 5+)
- **Ciudad Mínima:** Argentia (N9) o Sahal (N7)
- **Tiempo Estimado:** 1 minuto (x3 pociones)
- **Efecto/Estadísticas:** Cura 2000 HP + regeneración 200 HP/s durante 5s

---

**Receta #25: Elíxir de Velocidad**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Esencia del Velo x2, Piel de Arpía (pluma) x1, Polvo de Ceniza x1, Cristal Vacío x1
- **Estación:** Mesa de Alquimia (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 2 minutos (x2 elíxires)
- **Efecto/Estadísticas:** +20% velocidad de movimiento, +15% velocidad de ataque durante 60 segundos

---

**Receta #26: Bálsamo de Jade**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Esencia de Jade x3, Polvo de Ceniza x2, Jade (triturado) x1, Aceite esencial x1
- **Estación:** Mesa de Alquimia (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 3 minutos (x2 bálsamos)
- **Efecto/Estadísticas:** +15% daño mágico, +5% regeneración de maná durante 30 minutos

---

**Receta #27: Antídoto Élfico**
- **Rareza:** Raro
- **Rango Requerido:** B
- **Materiales:** Lágrima de Elfa x1, Esencia de Agua x2, Roble Viejo (corteza) x1, Cristal Vacío x1
- **Estación:** Mesa de Alquimia (nivel 4+)
- **Ciudad Mínima:** Aelthar (N3, reputación Honrado+)
- **Tiempo Estimado:** 1 minuto (x3 antídotos)
- **Efecto/Estadísticas:** Cura veneno, enfermedad y maldición. Otorga 10% resistencia a veneno por 10 min.

---

**Receta #28: Elíxir del Guardián**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Piedra de Toque x1, Ceniza de Estrella x1, Esencia del Velo x2, Corazón de Constructo x1
- **Estación:** Mesa de Alquimia (nivel 5+) + Altar de Esencias
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 10 minutos (1 elíxir)
- **Efecto/Estadísticas:** Inmunidad a todo daño durante 10 segundos. Cooldown de uso: 5 minutos.

---

### Mejoras (6 recetas)

---

**Receta #29: Filo Ardiente (Encantamiento de Arma)**
- **Rareza:** Poco Común
- **Rango Requerido:** D
- **Materiales:** Esencia de Fuego x3, Carbón Mineral x1
- **Estación:** Taller de Encantamiento (nivel 2+)
- **Ciudad Mínima:** Velmar (N1)
- **Tiempo Estimado:** 1 minuto
- **Efecto/Estadísticas:** El arma encantada obtiene +5% daño de fuego. Duración: permanente (1 ranura de encantamiento).

---

**Receta #30: Coraza de Jade (Encantamiento de Armadura)**
- **Rareza:** Raro
- **Rango Requerido:** C
- **Materiales:** Esencia de Jade x2, Jade x1, Polvo de Ceniza x1
- **Estación:** Taller de Encantamiento (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 3 minutos
- **Efecto/Estadísticas:** La armadura obtiene +8% defensa. Duración: permanente (1 ranura de encantamiento).

---

**Receta #31: Inserción de Rubí**
- **Rareza:** Raro
- **Rango Requerido:** B
- **Materiales:** Rubí (cortado) x1, Esencia de Fuego x2, Oro x1
- **Estación:** Banco de Gemas (nivel 3+)
- **Ciudad Mínima:** Thornwall (N5)
- **Tiempo Estimado:** 2 minutos
- **Efecto/Estadísticas:** En arma: +5% daño de fuego. En armadura: +5% resistencia a fuego. Ocupa 1 ranura de gema.

---

**Receta #32: Inserción de Diamante**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Diamante (cortado) x1, Cristal de Alma x2, Piedra de Toque x1
- **Estación:** Banco de Gemas (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 5 minutos
- **Efecto/Estadísticas:** En arma: +8% daño crítico. En armadura: +8% defensa +3% todas las resistencias. Ocupa 1 ranura de gema.

---

**Receta #33: Brillo Solar (Encantamiento de Arma Legendario)**
- **Rareza:** Legendario
- **Rango Requerido:** A
- **Materiales:** Fragmento del Tejido x1, Cristal de Alma x2, Ceniza de Estrella x1, Esencia del Velo x2
- **Estación:** Taller de Encantamiento (nivel 5+) + Altar de Esencias
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 15 minutos
- **Efecto/Estadísticas:** El arma obtiene +20% daño radiante. Los ataques tienen 10% de chance de cegar al enemigo 3s. Duración: permanente.

---

**Receta #34: Reparación de Arma Épica**
- **Rareza:** Épico (servicio)
- **Rango Requerido:** A
- **Materiales:** Del arma a reparar + Plata x2 + Esencia de Jade x1
- **Estación:** Yunque de Reparación (nivel 4+)
- **Ciudad Mínima:** Thornwall (N5)
- **Tiempo Estimado:** 3 minutos
- **Efecto/Estadísticas:** Repara el arma al 100% de su durabilidad. Nota: el material base requerido varía según el tipo de arma.

---

### Recetas Únicas (2 recetas)

---

**Receta #35: Hoja Única — Sombra del Último Sol (Katana de Sera)**
- **Rareza:** Mítico
- **Rango Requerido:** SS
- **Materiales:** Fragmento del Tejido x2, Piedra de Toque x3, Ceniza de Estrella x2, Lágrima de Elfa x2, Cristal de Alma x5, Esencia del Borrador x1
- **Estación:** Fragua del Herrero (nivel 5+, maestro armero) + Altar de Esencias
- **Ciudad Mínima:** Argentia (N9) — requiere misión de herrero legendario
- **Tiempo Estimado:** 1 hora (en tiempo real)
- **Efecto/Estadísticas:** Daño 100-130, +12 Destreza, +20% crítico, +15% velocidad ataque. Pasiva "Último Rayo": al bajar de 30% HP, los próximos 3 golpes son críticos garantizados. Pasiva "Sombra Solar": el 20% del daño infligido se ignora como daño verdadero. Exclusiva de Sera. Solo se puede craftear UNA VEZ por personaje.

---

**Receta #36: Hoja Única — Baile de Cuchillas Eternas (Hojas Gemelas de Garrik)**
- **Rareza:** Mítico
- **Rango Requerido:** SS
- **Materiales:** Fragmento del Tejido x2, Piedra de Toque x2, Ceniza de Estrella x2, Piel de Arpía x2, Cristal de Alma x4, Esencia del Borrador x1
- **Estación:** Fragua del Herrero (nivel 5+, maestro armero) + Altar de Esencias
- **Ciudad Mínima:** Argentia (N9) — requiere misión de herrero legendario
- **Tiempo Estimado:** 1 hora
- **Efecto/Estadísticas:** Daño 75-95 (cada hoja), +10 Destreza, +18% crítico, +12% velocidad ataque. Pasiva "Danza Eterna": cada 3 golpes consecutivos al mismo objetivo, el cuarto golpe es automáticamente crítico y aplica sangrado. Pasiva "Hojas del Velo": 15% de evasión cuerpo a cuerpo. Exclusiva de Garrik. Solo se puede craftear UNA VEZ por personaje.

---

## 5. Sistema de Maestría

El sistema de maestría permite a las clases **Herrero** y **Alquimista** especializarse y desbloquear recetas superiores. Cada especialización tiene **10 niveles** de maestría.

### 5.1 Maestría del Herrero

| Nivel | Especialización | Beneficio Desbloqueado | Puntos Requeridos |
|---|---|---|---|
| 1 | General | Recetas Comunes (rango E) | 0 (base) |
| 2 | Armas | Recetas Poco Comunes (rango D) — Armas | 50 |
| 3 | Armas | Recetas Poco Comunes (rango C) — Armas | 150 |
| 4 | Armaduras | Recetas Poco Comunes (rango C) — Armaduras | 150 |
| 5 | Armas | Recetas Raras (rango B) — Armas | 500 |
| 6 | Armaduras | Recetas Raras (rango B) — Armaduras | 500 |
| 7 | Accesorios | Recetas Raras (rango B) — Accesorios | 500 |
| 8 | Armas/Armaduras | Recetas Épicas (rango A) — Especialidad elegida | 1500 |
| 9 | Armas/Armaduras/Accesorios | Recetas Legendarias (rango S) — Especialidad elegida | 3000 |
| 10 | Maestro Herrero | Recetas Míticas (rango SS) — Una receta única | 5000 + Misión |

**Puntos de Maestría:** Se obtienen al craftear objetos exitosamente (+1 a +20 según rareza). También por misiones de herrero y descubrir recetas.

**Especialización Máxima:** Un herrero puede alcanzar nivel 10 en UNA especialidad (Armas, Armaduras o Accesorios) o nivel 8 en dos especialidades. No es posible ser maestro en las tres.

### 5.2 Maestría del Alquimista

| Nivel | Especialización | Beneficio Desbloqueado | Puntos Requeridos |
|---|---|---|---|
| 1 | General | Recetas Comunes (rango E) | 0 (base) |
| 2 | Pociones | Pociones nivel 2 (Común, rango D) | 50 |
| 3 | Pociones | Pociones nivel 3 (Poco Común, rango C) | 150 |
| 4 | Encantamientos | Encantamientos básicos (rango C-D) | 200 |
| 5 | Pociones | Pociones nivel 4 (Raro, rango B) | 500 |
| 6 | Encantamientos | Encantamientos medios (rango B) | 600 |
| 7 | Pociones | Pociones nivel 5 (Épico, rango A) | 1500 |
| 8 | Encantamientos | Encantamientos avanzados (rango A) | 1500 |
| 9 | Elíxires | Recetas de Elíxires (rango S) | 3000 |
| 10 | Maestro Alquimista | Recetas Míticas + Pociones Únicas | 5000 + Misión |

### 5.3 Maestría Transversal (Todas las Clases)

Cualquier clase puede aprender **recetas básicas** en las estaciones de crafting sin necesidad de ser Herrero o Alquimista:

- **Nivel 1 (Gratis):** Pociones básicas, vendas, reparación de equipo común
- **Nivel 2 (50 puntos):** Pociones menores, antídotos básicos
- **Nivel 3 (200 puntos):** Encantamientos básicos (requiere ser Alquimista o Mago)

Para recetas de rango C o superior, se requiere pertenecer a la clase Herrero o Alquimista.

---

## 6. Economía del Crafting

### 6.1 Costo de Materiales en Tiendas

Los jugadores pueden comprar materiales básicos en los mercados de cada ciudad. Los precios varían según la ciudad y la rareza:

| Material | Precio Base (Velmar) | Precio (Argentia) | Disponibilidad |
|---|---|---|---|
| Fragmento de Hierro | 10 oro | — | Todas las ciudades |
| Hierro (ya fundido) | 50 oro | 80 oro | Ciudades N3+ |
| Acero | 150 oro | 200 oro | Ciudades N3+ |
| Plata | 300 oro | 400 oro | Ciudades N5+ |
| Oro | 800 oro | 1000 oro | Ciudades N7+ |
| Jade | — | 2500 oro | Sahal, Argentia |
| Cristal de Alma | — | 5000 oro | Argentia (cantidad limitada) |
| Piedra de Toque | — | 15000 oro | Argentia (1 por día) |
| Madera Podrida | 5 oro | — | Velmar |
| Roble Viejo | 15 oro | 25 oro | Velmar, Aelthar |
| Esencia de Fuego | 40 oro | 80 oro | Ciudades N2+ |
| Esencia de Agua | 40 oro | 80 oro | Ciudades N1+ |
| Piel de Lobo | 20 oro | — | Velmar |
| Cuero Curtido | 60 oro | 100 oro | Todas |

### 6.2 Ahorro al Recolectar vs Comprar

Comparativa de costo para crear una **Espada de Acero Templado** (Poco Común):

| Material | Cant. | Precio Tienda | Costo Recolectar (estimado) |
|---|---|---|---|
| Acero | 3 | 450 oro | ~90 oro (3 mineral de hierro + carbón) |
| Roble Viejo | 2 | 30 oro | ~5 oro (talando 2 árboles) |
| Carbón Mineral | 2 | 20 oro | ~5 oro (minería) |
| **Total** | | **500 oro** | **~100 oro** |

**Ahorro:** ~80% al recolectar tú mismo.

Para una **Espada Plateada del Alba** (Raro):

| Material | Cant. | Precio Tienda | Costo Recolectar |
|---|---|---|---|
| Plata | 4 | 1200 oro | ~200 oro |
| Acero | 2 | 300 oro | ~60 oro |
| Esencia de Agua | 2 | 80 oro | ~30 oro (cazar elementales) |
| Piel de Gárgola | 1 | 200 oro | ~40 oro (cazar gárgola) |
| **Total** | | **1780 oro** | **~330 oro** |

**Ahorro:** ~82%.

### 6.3 Valor de Reventa de Items Crafteados

| Item | Costo Recolectar | Costo Comprando | Precio de Venta NPC | Precio Mercado Jugador (estimado) |
|---|---|---|---|---|
| Espada de Bronce | ~35 oro | 50 oro | 25 oro | 40-60 oro |
| Espada de Acero Templado | ~100 oro | 500 oro | 300 oro | 400-600 oro |
| Espada Plateada del Alba | ~330 oro | 1780 oro | 900 oro | 1400-1800 oro |
| Poción de Vida Menor (x5) | ~20 oro | 75 oro | 40 oro | 60-80 oro |
| Poción de Vida Mayor (x5) | ~80 oro | 300 oro | 180 oro | 250-350 oro |
| Elíxir de Velocidad | ~150 oro | 600 oro | 350 oro | 500-750 oro |
| Bálsamo de Jade | ~200 oro | 1000 oro | 500 oro | 800-1200 oro |

**Margen recomendado de NPC:** Las tiendas compran items crafteados al 50-60% del costo de materiales comprados en tienda. Esto fomenta la recolección o el comercio entre jugadores.

### 6.4 Items Únicos (Bound on Craft — Solo 1 Vez)

| Item | Clase | Rango | Costo Aprox. Materiales |
|---|---|---|---|
| Sombra del Último Sol | Sera (Katana) | SS | ~75,000 oro en materiales raros |
| Baile de Cuchillas Eternas | Garrik (Hojas Gemelas) | SS | ~70,000 oro |
| Corazón del Tejido (Amuleto) | Todas | Mítico | ~60,000 oro + misión |
| Armadura del Sol Poniente | Guerrero/Paladín | SS | ~80,000 oro + jefe N10 |
| Cetro del Tejido Estelar | Mago/Nigromante | SS | ~70,000 oro + jefe N10 |

Estos items son **Bound on Craft (BoC)** — una vez creados, se vinculan al personaje. Solo se puede obtener **UNO por personaje** (no se puede vender ni transferir). Esto asegura que cada item único sea verdaderamente especial.

### 6.5 Consideraciones Económicas Adicionales

- **Impuesto de Estación:** Usar estaciones en ciudades ajenas a tu facción/reputación cuesta 10-30% más en materiales.
- **Reputación y Descuentos:** Con reputación **Honrado** en una ciudad, obtienes 15% de descuento en materiales. Con **Héroe**, 25%.
- **Fallos de Crafting:** En recetas de rango S y superiores, existe un 5-10% de probabilidad de fallo (pérdida de materiales). Esto se reduce con maestría.
- **Calidad Variable:** Items crafteados por maestros tienen chance de obtener **calidad superior** (+5-15% stats) con nivel de maestría alto.
- **Subasta entre Jugadores:** Los items crafteados (no únicos) se pueden vender entre jugadores. Los precios de mercado suelen ser 2-3x el valor de venta a NPC.

---

## Apéndice: Flujo de Progresión Recomendada

| Rango del Jugador | Ciudad Base | Actividad de Crafting |
|---|---|---|
| E (Nuevo) | Emberveil (N0) | Aprender recetas básicas, recolectar madera podrida y fragmentos |
| D | Velmar (N1) | Craftear equipo de hierro, pociones menores. Primeras especializaciones |
| C | Aelthar (N3) | Equipo de acero, encantamientos básicos. Elegir especialidad de maestro |
| B | Thornwall (N5) | Equipo plateado, gemas básicas. Maestría nivel 5+ |
| A | Sahal (N7) o Argentia (N9) | Equipo de jade, pociones élficas. Maestría nivel 8+ |
| S | Argentia (N9) | Equipo legendario, elíxires. Maestría nivel 9 |
| SS+ | Argentia / N10 | Items únicos y míticos. Maestría máxima |

---

> *

#### Anillos

| Tipo | Nivel | Nombre | Rareza |
|---|---|---|---|
| Daño | 1 | Anillo de Colmillo | Común |
| Daño | 2 | Anillo de Fuerza de Hierro | Común |
| Daño | 3 | Anillo de Acero Afilado | Poco Común |
| Daño | 4 | Anillo Plateado del Cazador | Raro |
| Daño | 5 | Anillo de Jade del Berserker | Épico |
| Daño | 6 | Anillo del Sol Furioso | Legendario |
| Defensa | 1 | Anillo de Cuero Resistente | Común |
| Defensa | 2 | Anillo de Hierro Sólido | Común |
| Defensa | 3 | Anillo de Acero Vigía | Poco Común |
| Defensa | 4 | Anillo Plateado Guardián | Raro |
| Defensa | 5 | Anillo de Jade Protector | Épico |
| Defensa | 6 | Anillo del Muro Solar | Legendario |
| Resistencia | 1 | Anillo de Piel de Lobo | Común |
| Resistencia | 2 | Anillo de Resistencia de Hierro | Común |
| Resistencia | 3 | Anillo de Acero Ignífugo | Poco Común |
| Resistencia | 4 | Anillo Plateado Antídoto | Raro |
| Resistencia | 5 | Anillo de Jade de la Tierra | Épico |
| Resistencia | 6 | Anillo de la Vitalidad Solar | Legendario |

#### Amuletos

| Nivel | Nombre | Rareza | Habilidad Pasiva |
|---|---|---|---|
| 1 | Colgante de Colmillo | Común | +2% daño bestias |
| 2 | Amuleto de Hierro Viejo | Común | +3% defensa |
| 3 | Colgante de Escama | Poco Común | +5% resistencia a veneno |
| 4 | Amuleto Plateado de Aelthar | Raro | +8% regeneración de maná |
| 5 | Colgante de Jade Lunar | Épico | +10% daño crítico |
| 6 | Amuleto del Último Sol | Legendario | +15% a todas las resistencias |
| 7 | **Único: Corazón del Tejido** | Mítico | Revive 1 vez por combate |

#### Botas

| Nivel | Nombre | Rareza | Efecto |
|---|---|---|---|
| 1 | Botas de Piel de Lobo | Común | +2% velocidad |
| 2 | Botas de Cuero | Común | +3% velocidad |
| 3 | Botas de Gárgola | Poco Común | +5% velocidad, +3% salto |
| 4 | Botas Plateadas del Viento | Raro | +8% velocidad, +5% salto |
| 5 | Botas de Jade del Cazador | Épico | +12% velocidad, +8% salto |
| 6 | Botas del Velo Solar | Legendario | +15% velocidad, +10% salto, +5% evasión |

#### Guantes

| Nivel | Nombre | Rareza | Efecto |
|---|---|---|---|
| 1 | Guantes de Piel de Lobo | Común | +1% crítico |
| 2 | Guantes de Cuero | Común | +2% crítico |
| 3 | Guantes de Gárgola | Poco Común | +3% crítico, +2% precisión |
| 4 | Guantes Plateados del Tirador | Raro | +5% crítico, +3% precisión |
| 5 | Guantes de Jade del Asesino | Épico | +7% crítico, +5% precisión |
| 6 | Guantes del Tejedor | Legendario | +10% crítico, +8% precisión, +3% velocidad ataque |

---

### 3.4 Consumibles

#### Pociones de Vida

| Nivel | Nombre | Rareza | Curación |
|---|---|---|---|
| 1 | Poción de Vida Menor | Común | 100 HP |
| 2 | Poción de Vida | Común | 250 HP |
| 3 | Poción de Vida Mayor | Poco Común | 500 HP |
| 4 | Poción de Vida Suprema | Raro | 1000 HP |
| 5 | Poción de Vida Élfica | Épico | 2000 HP + regeneración 5s |

#### Pociones de Maná

| Nivel | Nombre | Rareza | Restauración |
|---|---|---|---|
| 1 | Poción de Maná Menor | Común | 50 MP |
| 2 | Poción de Maná | Común | 150 MP |
| 3 | Poción de Maná Mayor | Poco Común | 350 MP |
| 4 | Poción de Maná Suprema | Raro | 750 MP |
| 5 | Poción de Maná Lunar | Épico | 1500 MP + regeneración 5s |

#### Otros Consumibles

| Nombre | Rareza | Efecto |
|---|---|---|
| Antídoto Básico | Común | Cura veneno común |
| Antídoto Avanzado | Poco Común | Cura veneno y enfermedad |
| Antídoto Élfico | Raro | Cura veneno, enfermedad y maldición |
| Venda de Lino | Común | Cura 50 HP fuera de combate |
| Venda de Seda | Poco Común | Cura 150 HP fuera de combate |
| Venda Encantada | Raro | Cura 300 HP (también en combate) |
| Bálsamo de Fuego | Poco Común | +10% daño de fuego 30 min |
| Bálsamo de Agua | Poco Común | +10% resistencia agua 30 min |
| Bálsamo de Jade | Épico | +15% daño mágico 30 min |
| Elíxir de Velocidad | Épico | +20% velocidad 60s |
| Elíxir de Fuerza | Épico | +25% daño físico 60s |
| Elíxir del Guardián | Legendario | Inmunidad 10s |

---

### 3.5 Mejoras

#### Encantamientos de Armas

| Nombre | Rareza | Efecto | Rango Mínimo | Material |
|---|---|---|---|---|
| Filo Ardiente | Poco Común | +5% daño de fuego | D | Esencia de Fuego x3 |
| Filo Helado | Poco Común | +5% daño de hielo | D | Esencia de Agua x3 |
| Filo de Jade | Raro | +8% daño mágico | C | Esencia de Jade x2 |
| Velo Cortante | Épico | +12% daño perforante | B | Esencia del Velo x2 |
| Brillo Solar | Legendario | +20% daño radiante | A | Fragmento del Tejido x1 |
| Toque del Borrador | Mítico | +30% daño verdadero | SS | Esencia del Borrador x1 |

#### Encantamientos de Armadura

| Nombre | Rareza | Efecto | Rango Mínimo | Material |
|---|---|---|---|---|
| Resistencia al Fuego | Poco Común | +5% res. fuego | D | Esencia de Fuego x2 |
| Resistencia al Agua | Poco Común | +5% res. agua | D | Esencia de Agua x2 |
| Coraza de Jade | Raro | +8% defensa | C | Esencia de Jade x2 |
| Velo Protector | Épico | +10% todas las res. | B | Esencia del Velo x2 |
| Armadura Solar | Legendario | +15% defensa +10% res. | A | Fragmento del Tejido x1 |

#### Inserción de Gemas

Las gemas se insertan en ranuras de equipo (armas y armaduras pueden tener 1-3 ranuras según rareza).

| Gema | Rareza | Efecto en Arma | Efecto en Armadura |
|---|---|---|---|
| Jade Bruto | Poco Común | +3% daño mágico | +3% defensa |
| Rubí | Raro | +5% daño de fuego | +5% res. fuego |
| Zafiro | Raro | +5% daño de hielo | +5% res. hielo |
| Esmeralda | Épico | +5% daño veneno | +5% res. veneno +2% HP |
| Diamante | Legendario | +8% daño crítico | +8% defensa +3% res. todas |
| Gema del Sol | Mítico | +12% daño radiante | +10% todas res. +5% HP |

#### Reparación de Equipo

El equipo se desgasta con el uso. La reparación requiere:

| Tipo de Equipo | Material Base | Costo Adicional |
|---|---|---|
| Arma Común | 1x Fragmento de Hierro / Madera Podrida | 10 oro |
| Arma Poco Común | 1x Hierro / Roble Viejo + Esencia básica | 50 oro |
| Arma Rara | 2x Acero / Madera Petrificada + Esencia | 200 oro |
| Arma Épica | 2x Plata/Oro + Esencia de Jade | 500 oro |
| Arma Legendaria | 1x Cristal de Alma + Piedra de Toque | 2000 oro |
| Arma Mítica | 1x Fragmento del Tejido | 5000 oro + misión |

---

## 4. Recetas Detalladas (30+ Recetas Completas)

### Armas (10 recetas)

---

**Receta #1: Espada de Bronce**
- **Rareza:** Común
- **Rango Requerido:** E
- **Materiales:** Fragmento de Hierro x3, Madera Podrida x1
- **Estación:** Fragua del Herrero (nivel 1+)
- **Ciudad Mínima:** Emberveil (N0)
- **Tiempo Estimado:** 30 segundos
- **Efecto/Estadísticas:** Daño 8-12, +1 Fuerza

---

**Receta #2: Espada de Acero Templado**
- **Rareza:** Poco Común
- **Rango Requerido:** C
- **Materiales:** Acero x3, Roble Viejo x2, Carbón Mineral x2
- **Estación:** Fragua del Herrero (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 2 minutos
- **Efecto/Estadísticas:** Daño 25-35, +4 Fuerza, +2% crítico

---

**Receta #3: Espada Plateada del Alba**
- **Rareza:** Raro
- **Rango Requerido:** B
- **Materiales:** Plata x4, Acero x2, Esencia de Agua x2, Piel de Gárgola x1
- **Estación:** Fragua del Herrero (nivel 4+)
- **Ciudad Mínima:** Thornwall (N5)
- **Tiempo Estimado:** 5 minutos
- **Efecto/Estadísticas:** Daño 45-60, +6 Fuerza, +5% daño contra no-muertos

---

**Receta #4: Arco de Roble**
- **Rareza:** Común
- **Rango Requerido:** D
- **Materiales:** Roble Viejo x3, Piel de Lobo x2
- **Estación:** Torno de Flechas (nivel 1+)
- **Ciudad Mínima:** Velmar (N1)
- **Tiempo Estimado:** 1 minuto
- **Efecto/Estadísticas:** Daño 10-15, +2 Destreza

---

**Receta #5: Arcoluna de Jade**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Jade x3, Raíz de Jade x2, Esencia de Jade x3, Piel de Arpía x1
- **Estación:** Torno de Flechas (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 10 minutos
- **Efecto/Estadísticas:** Daño 55-70, +8 Destreza, +10% daño mágico, +5% precisión

---

**Receta #6: Daga de Acero Afilado**
- **Rareza:** Poco Común
- **Rango Requerido:** C
- **Materiales:** Acero x2, Piel de Gárgola x1, Esencia de Fuego x1
- **Estación:** Fragua del Herrero (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 2 minutos
- **Efecto/Estadísticas:** Daño 18-28, +3 Destreza, +4% crítico

---

**Receta #7: Hacha de la Ceniza Eterna**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x3, Ceniza de Árbol x2, Ceniza de Estrella x1, Piel de Gárgola x2, Oro x4
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 20 minutos
- **Efecto/Estadísticas:** Daño 80-100, +10 Fuerza, +15% daño de fuego, +8% crítico, el 10% de golpear en área

---

**Receta #8: Báculo de Jade de Sahal**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Jade x3, Raíz de Jade x2, Cristal de Alma x1, Esencia del Velo x2
- **Estación:** Fragua del Herrero (nivel 4+) o Altar de Esencias (nivel 4+)
- **Ciudad Mínima:** Sahal (N7)
- **Tiempo Estimado:** 8 minutos
- **Efecto/Estadísticas:** Daño mágico 50-65, +8 Inteligencia, +10% regeneración de maná, +8% daño de hechizos

---

**Receta #9: Katana de Jade Carmesí (Sera)**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Jade x3, Plata x2, Esencia de Fuego x2, Lágrima de Elfa x1, Piel de Arpía x1
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 15 minutos
- **Efecto/Estadísticas:** Daño 60-75, +7 Destreza, +12% crítico, +8% velocidad de ataque. Exclusiva para Sera.

---

**Receta #10: Hojas del Velo Partido (Garrik)**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Jade x2, Esencia del Velo x3, Piedra de Toque x1, Piel de Arpía x2
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 25 minutos
- **Efecto/Estadísticas:** Daño 55-70 (cada hoja), +9 Destreza, +15% crítico, +10% velocidad ataque, +5% evasión. Exclusiva para Garrik.

---

### Armaduras (6 recetas)

---

**Receta #11: Armadura de Cuero Curtido**
- **Rareza:** Común
- **Rango Requerido:** D
- **Materiales:** Cuero Curtido x4, Piel de Lobo x2, Hilo de Seda x1
- **Estación:** Telar de Tejidos (nivel 1+)
- **Ciudad Mínima:** Velmar (N1)
- **Tiempo Estimado:** 2 minutos
- **Efecto/Estadísticas:** Defensa 12, +2 Destreza

---

**Receta #12: Armadura de Acero Completa**
- **Rareza:** Poco Común
- **Rango Requerido:** C
- **Materiales:** Acero x5, Piel de Gárgola x2, Carbón Mineral x3
- **Estación:** Fragua del Herrero (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 5 minutos
- **Efecto/Estadísticas:** Defensa 30, +4 Fuerza, +3% resistencia física

---

**Receta #13: Armadura de Jade del Héroe**
- **Rareza:** Épico
- **Rango Requerido:** A
- **Materiales:** Jade x4, Raíz de Jade x2, Escama de Serpiente x2, Esencia de Jade x3, Oro x2
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 15 minutos
- **Efecto/Estadísticas:** Defensa 55, +6 Fuerza, +8% todas las resistencias, +5% HP máximo

---

**Receta #14: Túnica del Tejedor Estelar**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x2, Esencia del Velo x3, Piel de Arpía x2, Lágrima de Elfa x1, Ceniza de Estrella x1
- **Estación:** Telar de Tejidos (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 20 minutos
- **Efecto/Estadísticas:** Defensa 40, +10 Inteligencia, +15% daño de hechizos, +20% regeneración de maná

---

**Receta #15: Escudo de Acero Torres**
- **Rareza:** Poco Común
- **Rango Requerido:** C
- **Materiales:** Acero x3, Roble Viejo x2, Piel de Gárgola x1
- **Estación:** Fragua del Herrero (nivel 3+)
- **Ciudad Mínima:** Aelthar (N3)
- **Tiempo Estimado:** 3 minutos
- **Efecto/Estadísticas:** Defensa 20, +5% bloqueo, +2 Vitalidad

---

**Receta #16: Muro del Sol Inquebrantable**
- **Rareza:** Legendario
- **Rango Requerido:** S
- **Materiales:** Cristal de Alma x3, Jade x2, Piedra de Toque x1, Escama de Serpiente x2, Oro x3
- **Estación:** Fragua del Herrero (nivel 5+)
- **Ciudad Mínima:** Argentia (N9)
- **Tiempo Estimado:** 20 minutos
- **Efecto/Estadísticas:** Defensa 50, +12% bloqueo, +8% todas las resistencias, +10 Vitalidad. Al bloquear, 20% de devolver daño.

---