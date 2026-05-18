# SOLO — Game Design Document
**Versión:** 1.0 — Acto I Completado  
**Género:** Action RPG (single-player + cooperative) — **3D**  
**Motor:** Unreal Engine 5.7  
**Lenguaje:** C++ + Blueprints (UE5)  
**Tono:** Adulto. Dolor real. Suspenso. Drama. No fantasía épica genérica.

---

## 1. VISIÓN DEL JUEGO

SOLO es un ARPG en 3D construido en Unreal Engine 5.7. El jugador controla a un protagonista que asciende una Torre de 100 pisos, cada uno un mundo completo con ciudades, fauna, misterios y horror existencial.

**Estructura narrativa:**
- **Preludio/Overworld:** Mundo exterior gigante (Emberveil, Velmar, Sahal, etc.) para exploración, comercio, y jugadores que prefieren no pelear siempre
- **Acto I (N0-N10):** Contenido principal en solitario. La Torre devora. El protagonista pierde. Y sigue.
- **Acto II (N11-N99):** Modo cooperativo (en desarrollo)
- **Acto III (N100):** Confrontación final con el Arquitecto

**Pilares de diseño:**
1. **Dolor real:** Las muertes duelen. Las elecciones dejan marcas. No hay "game over" — solo "seguir".
2. **Suspenso existencial:** La Torre no es un dungeon. Es una pregunta que no querés hacerte.
3. **Romance adulto:** No inocente. No juvenil. Conexión entre personas rotas.
4. **Consecuencias:** NG+ no es "más difícil". Es "diferente". Lo que hiciste antes, cambia el mundo.

---

## 2. ESTRUCTURA NARRATIVA

### 2.1 Preludio — Overworld

**Ubicación:** Emberveil y alrededores
**Duración:** 2-3 horas
**Contenido:**
- Tutorial (combate, movimiento, diálogos)
- Ciudades gigantes con NPCs, comercio, misiones secundarias
- Fauna, misterios, exploración
- **Para jugadores que no quieren pelear siempre**

**Quests principales (ActI/):**
- Q01-Q05: Emberveil y Velmar
- Q06-Q10: Camino a Sahal
- Q11-Q15: Sahal y Tirenne
- Q16-Q20: Pantano y Garrik
- Q21-Q22: La Aguja y entrada a la Torre

**Compañeros en Preludio:**
- Caín (amigo de infancia, muere en Garganta)
- Mira (arquera, dibujante de mapas)
- Vassen (templario caído)
- Garrik (amigo de infancia, DESAPARECE antes de la Torre)

**Nota:** Garrik NO aparece en el Preludio como compañero jugable. Desaparece una noche antes de que el grupo entre a la Torre. Se reencuentra en N2.

---

### 2.2 Acto I — La Torre (N0-N10)

**Estructura:** 11 pisos (N0 = Emberveil dentro de la Torre, Garganta = transición, N1-N10 = pisos)

| Piso | Nombre | Tema | Compañeros | Jefe |
|------|--------|------|------------|------|
| N0 | Emberveil | Tutorial | Caín | — |
| Garganta | La Primera Caída | Transición | Caín (muere) | Guardián de la Garganta |
| N1 | Velmar | Ciudad mercante | Mira, Vassen | La Voz |
| N2 | Pastor de Cenizas | Ceniza, olvido | Mira, Vassen, Garrik | El Pastor |
| N3 | Aelthar | Lago de espejos | Mira, Vassen, Garrik | Leviatán del Espejo |
| N4 | Corredor de los Nombres | Legado, olvido | Mira, Vassen, Garrik | El Heraldo Sin Escudo |
| N5 | Thornwall | Ciudad de contratos | Mira, Vassen, Garrik | El Carnicero |
| N6 | Cámaras de Jade | Experimentos, vida | Mira, Vassen, Garrik | Coro de Jade |
| N7 | Sahal | Ciudad de adobe | Mira, Vassen, Garrik | La Costurera |
| N8 | Jardines Flotantes | Cristal, paz | Garrik, Sera (se une) | Arpía de Bronce |
| N9 | Argentia | Plata, ancianos | Garrik, Sera | Verdugo de Plata |
| N10 | El Centinela | Piedra, final | Garrik, Sera | El Centinela de Granito |

**Muertes en Acto I:**
- Caín: Garganta (N0→N1)
- Mira: N7 (Sahal, La Costurera)
- Vassen: N7 (Sahal, La Costurera)

**Romance:**
- Sera se une en N8. Romance opcional, adulto, con iniciativa femenina.

**Finales del Acto I (N10):**
- A: Escape (destruir la Torre)
- B: Nuevo Arquitecto (reemplazar al Arquitecto)
- C: Heredero (aceptar el ciclo)
- D: Destructor (destruir TODO)
- E: Sacrificio (morir para salvar a otros)
- Variante Romance: Si romance con Sera está activo, hay variantes adicionales
- Final Secreto: NG+7, requiere todas las Esquirlas

---

### 2.3 Temas por Piso (Adultos, No Genéricos)

| Piso | Tema | Descripción |
|------|------|-------------|
| N0 | Pérdida de inocencia | Caín muere. El protagonista aprende que la Torre no perdona. |
| N1 | Duelo negado | Velmar finge que todo está bien. La gente se niega a llorar. |
| N2 | Identidad borrada | El Pastor olvidó quién era. Garrik no reconoce al protagonista. |
| N3 | Verdad vs comodidad | Los elfos saben la verdad sobre la Torre. Pero es más fácil no saber. |
| N4 | Legado y olvido | ¿Vale la pena ser recordado? ¿O es mejor ser olvidado? |
| N5 | Precio de la supervivencia | Contratos de esclavitud. ¿Qué harías para seguir vivo? |
| N6 | Vida que no pidió existir | Experimentos fallidos. ¿Tienen derecho a vivir? |
| N7 | Pérdida irreversible | Mira y Vassen mueren. No hay resurrección. No hay consuelo. |
| N8 | Conexión post-trauma | Sera aparece. ¿Cómo te conectás después de perder a alguien? |
| N9 | Renuncia y arrepentimiento | Ancianos que se rindieron. ¿Se arrepienten? |
| N10 | Confrontación con el creador | El Arquitecto no quiere ganar. Quiere perder. |

---

## 3. PERSONAJES

### 3.1 Protagonista

- **Edad:** 18-20 años
- **Origen:** Luzmarca
- **Personalidad:** Definida por elecciones del jugador
- **Característica:** No es el "elegido". Es alguien que sigue adelante a pesar de todo.

### 3.2 Compañeros

**Caín (N0-Garganta):**
- Amigo de infancia. Optimista. Muere rápido.
- Función: Enseñar al jugador que la Torre no perdona.

**Mira (N1-N7):**
- Arquera, dibujante de mapas. Quiere que todo dure.
- Muere en N7. Su muerte es lenta, personal, cruel.

**Vassen (N1-N7):**
- Templario caído. Bebe para olvidar. Su padre se ahogó borracho.
- Muere en N7. Se convierte en escudo. Literalmente.

**Garrik (N2-N10):**
- Amigo de infancia... pero no lo recuerda. Amnesia por reencarnación.
- Es Lian (5ta reencarnación). Recupera fragmentos de memoria progresivamente.
- Relación con Sera: Se llevan mal. Pero se respetan.

**Sera (N8-N10):**
- Mujer de 28-32 años. Katana. No humana (fragmento del Tejido).
- Iniciativa femenina en el romance. Directa. Vulnerable.
- Miedo: desaparecer. Ser olvidada.

---

## 4. SISTEMAS

### 4.1 Sistema de Rangos

| Rango | Requisito | Descripción |
|-------|-----------|-------------|
| D | Inicial | Principiante. Sin habilidades especiales. |
| C | 3 pisos completados | Novato. Acceso a ciudades. |
| C+ | 5 pisos + 1 jefe | Intermedio. Mejores misiones. |
| B | 7 pisos + 2 jefes | Avanzado. Forja avanzada. |
| B+ | 8 pisos + 3 jefes | Experto. Misiones épicas. |
| A | 9 pisos + 4 jefes | Maestro. Oswin accesible. |
| A+ | 10 pisos + 5 jefes | Leyenda. Armas legendarias. |
| S | 10 pisos + todos los jefes + Esquirlas | Mítico. Final Secreto desbloqueado. |

### 4.2 Sistema de Facciones

7 facciones con reputación cruzada. Ver `Design/Sistemas/sistema_facciones_CONSOLIDADO.md`.

### 4.3 Sistema de Esquirlas (New Game Plus)

10 Esquirlas que modifican el mundo en NG+:

| # | Nombre | Efecto NG+ |
|---|--------|------------|
| 1 | Reloj de los Primeros | NPCs recuerdan conversaciones pasadas |
| 2 | Espejo Roto | Visión alternativa de escenas clave |
| 3 | Llave de Plata | Acceso a zonas bloqueadas |
| 4 | Ceniza Viva | Enemigos más fuertes, mejores drops |
| 5 | Trono Vacío | Jefes tienen fases adicionales |
| 6 | Flor de Cristal | Jardines de N8 florecen |
| 7 | Eco Eterno | El Eco de N6 te sigue |
| 8 | Sombra Gemela | Doble de ti mismo como aliado |
| 9 | Último Aliento | Modo hardcore (muerte = restart) |
| 10 | Semilla del Arquitecto | Final Secreto desbloqueado |

### 4.4 Sistema de Consecuencias

**Mismo playthrough:**
- Elecciones en N2-N6 afectan N7-N10
- Ejemplo: Dejar crecer la flor en N6 → zona secreta en N8
- Ejemplo: Contrato de Sangre (romper) → asesinos en N6-N10

**NG+:**
- NPCs recuerdan lo que hiciste
- Nuevas opciones de diálogo
- Jefes con fases adicionales
- Final Secreto (NG+7)

---

## 5. COMBATE

### 5.1 Estilo

- Action RPG en tercera persona
- Combos, esquiva (i-frames), bloqueo, parry
- Posturas: agresiva / defensiva / balanceada
- Sword Skills: habilidades activas con cooldown

### 5.2 Compañeros en combate

- IA controlada (no jugable directamente)
- Cada compañero tiene estilo único:
  - Mira: Arquera, soporte, daño a distancia
  - Vassen: Tanque, escudo, protección
  - Garrik: Pícaro, sigilo, combos rápidos
  - Sera: Katana, velocidad, esquivas perfectas

### 5.3 Muerte de compañeros

- **Permanente.** No hay resurrección.
- Mira y Vassen mueren en N7 (historia)
- Garrik puede morir en N5 (elección del jugador) o N10 (fallo mecánico)
- Sera solo puede morir en N10 (fallo mecánico específico)

---

## 6. MUNDO

### 6.1 Overworld (Preludio)

- **Emberveil:** Ciudad inicial. Tutorial. Gremio de aventureros.
- **Velmar:** Ciudad mercante. Mercado negro. Primeras elecciones morales.
- **Sahal:** Ciudad de adobe. Tejedores. Misterios.
- **Pantano:** Zona hostil. Garrik desaparece aquí.
- **Refugio:** Último lugar seguro antes de la Torre.

### 6.2 La Torre (N0-N100)

- Cada piso es un mundo completo
- No hay teletransporte dentro de la Torre
- Cada piso tiene: ciudad, dungeon, jefe, NPCs, misiones secundarias
- Biomas únicos: ceniza, espejos, contratos, cristal, adobe, etc.

---

## 7. JEFES

### 7.1 Jefes del Acto I (N0-N10)

| Piso | Jefe | Mecánica | Tono |
|------|------|----------|------|
| Garganta | Guardián de la Garganta | Tutorial | Caín muere |
| N1 | La Voz | Ilusión, sonido | Duelo negado |
| N2 | El Pastor de Cenizas | Círculos, ovejas invisibles | Identidad borrada |
| N3 | Leviatán del Espejo | Reflejos, agua | Verdad vs comodidad |
| N4 | El Heraldo Sin Escudo | Espera, honor | Legado y olvido |
| N5 | El Carnicero | Contratos, esclavitud | Precio de supervivencia |
| N6 | Coro de Jade | Experimentos, ecos | Vida no pedida |
| N7 | La Costurera | Hilos, coser | Pérdida irreversible |
| N8 | Arpía de Bronce | Vuelo, viento | Conexión post-trauma |
| N9 | Verdugo de Plata | Juicio, arrepentimiento | Renuncia |
| N10 | El Centinela de Granito | 5 fases, todas las mecánicas | Confrontación |

### 7.2 El Arquitecto (N100)

- Jefe final del juego
- 5 fases
- No quiere ganar. Quiere perder.
- Diálogo: "Llegaste. Sabía que llegarías. Siempre llegás."

---

## 8. MISIONES SECUNDARIAS

### 8.1 Cantidad por piso

| Piso | Misiones secundarias |
|------|---------------------|
| N1 | 4 (incluyendo "La Niña Desaparecida") |
| N2 | 3 (incluyendo "Las Ovejas del Pastor") |
| N3 | 4 (incluyendo "El Último Registro") |
| N4 | 3 (incluyendo "El Nombre Perdido") |
| N5 | 5 (incluyendo "El Contrato de Sangre") |
| N6 | 4 (incluyendo "El Eco y la Verdad") |
| N7 | 3 (incluyendo "La Costurera y el Cuaderno") |
| N8 | 4 (incluyendo romance con Sera) |
| N9 | 3 (incluyendo "La Noche de las Estrellas") |

### 8.2 Tipos

- **Investigación:** "La Niña Desaparecida"
- **Elección moral:** "El Contrato de Sangre" (4 opciones)
- **Character moment:** "El Último Trago" (Vassen)
- **Romance:** "La Noche de las Estrellas" (Sera)
- **Lore:** "Pescando Recuerdos" (pierdes diálogos guardados)

---

## 9. DIÁLOGOS Y BANTER

### 9.1 Sistema de diálogos

- Opciones múltiples con consecuencias
- No hay "opción correcta". Solo distintos tipos de dolor.
- NG+: NPCs recuerdan elecciones pasadas

### 9.2 Banter entre compañeros

- **Mira-Vassen:** Amistad, protección mutua
- **Garrik-Mira:** Humor, nostalgia
- **Sera-Garrik:** Hostilidad con respeto mutuo (ver `Design/Dialogues/banter_sera_garrik.md`)

---

## 10. ARTE Y SONIDO

### 10.1 Estilo visual

- **Overworld:** Fotorrealista (Nanite + Lumen). Ciudades vivas, fauna, clima.
- **Torre:** Estilizado. Cada piso tiene paleta única. Ceniza gris, espejos azules, cristal verde, adobe naranja.
- **Efectos:** La muerte no es explosión. Es vacío. Silencio. Ausencia.

### 10.2 Música

- **Overworld:** Orquestal, melancólica, con esperanza
- **Torre:** Minimalista. A veces solo silencio. A veces un solo instrumento.
- **Combate:** Intenso, pero no épico. Más como "suspense" que "gloria".
- **Momentos clave:** Sin música. Solo ambiente.

### 10.3 Sonido

- La katana de Sera es silenciosa. No hay "ring" metálico.
- La Voz en N1 no tiene voz. Tiene... presencia.
- El Pastor camina en círculos. El sonido de sus pasos es el único sonido en kilómetros.

---

## 11. TECNOLOGÍA

### 11.1 Stack

| Feature | Uso |
|---------|-----|
| Nanite | Geometría detallada en ciudades, Torre |
| Lumen | Iluminación dinámica (antorchas, cristales) |
| World Partition | Overworld abierto sin pantallas de carga |
| Chaos Physics | Ragdoll, destrucción |
| MetaHuman | Base para personajes |
| Niagara VFX | Habilidades, efectos ambientales |
| Control Rig | Animaciones procedurales |

### 11.2 Módulos C++

```
SOLO.uproject
Source/
  SOLOCore/       — stats, inventario, save
  SOLOCombat/     — combate, skills, IA compañeros
  SOLOWorld/      — generación de pisos, Torre
  SOLOUI/         — widgets, HUD, diálogos
  SOLOOnline/     — cooperativo (futuro)
```

---

## 12. ROADMAP

### Fase 1 — Fundamentos (Meses 1-3)
- Movimiento, cámara, combate básico
- Sistema de diálogos con consecuencias
- N0 (Emberveil) + Garganta

### Fase 2 — Acto I Core (Meses 4-8)
- N1-N5 completos (ciudades, jefes, misiones)
- Sistema de facciones
- Compañeros: Mira, Vassen, Garrik

### Fase 3 — Acto I Final (Meses 9-12)
- N6-N10 completos
- Sistema de Esquirlas (NG+)
- Romance con Sera
- 5 finales + Final Secreto

### Fase 4 — Polish (Meses 13-15)
- Overworld completo (Preludio)
- Misiones secundarias restantes
- Balance, bugs, optimización

### Fase 5 — Cooperativo (Meses 16+)
- N11-N99 (modo cooperativo)
- Sistema de gremios
- Eventos temporales

---

## 13. DOCUMENTACIÓN RELACIONADA

| Documento | Contenido |
|-----------|-----------|
| `Design/Quests/HISTORIA_COMPLETA_N0_N10.md` | Historia principal Acto I |
| `Design/Quests/N[1-10]_[nombre].md` | Pisos individuales con diálogos |
| `Design/Quests/misiones_secundarias_dialogos.md` | Misiones secundarias expandidas |
| `Design/Quests/sera_romance_expandido.md` | Romance con Sera |
| `Design/Dialogues/banter_sera_garrik.md` | Banter Sera-Garrik |
| `Design/Lore/companero_garrik.md` | Backstory Garrik (amnesia) |
| `Design/Lore/companero_sera.md` | Backstory Sera |
| `Design/Sistemas/sistema_facciones_CONSOLIDADO.md` | Facciones y reputación |
| `Design/Sistemas/sistema_esquirlas.md` | New Game Plus |
| `Design/Sistemas/sistema_finales.md` | Finales y variantes |

---

*Documento actualizado: 2026-05-18*
*Estructura consolidada: Preludio + N0-N10*
