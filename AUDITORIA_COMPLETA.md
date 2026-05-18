# Auditoría Completa del Proyecto SOLO-ARPG
## Fecha: 2026-05-18
## Estado: Acto I (N0-N10) completado, pero con inconsistencias críticas

---

## 🔴 PROBLEMAS CRÍTICOS

### 1. DUALIDAD NARRATIVA: Dos Actos I diferentes

**Estructura A: `Design/Quests/ActI/` (Capítulos 1-5)**
- Mundo exterior: Emberveil, Velmar, Sahal, Pantano, Refugio
- 22 quests principales (Q01-Q22)
- Sera aparece en Q19 ("La Figura de Capa Negra") ANTES de entrar a la Torre
- Garrik aparece en Q14-Q17 (Cruzando el Pantano)
- La Torre se entra al FINAL del Acto I (Q21)

**Estructura B: `Design/Quests/N0-N10` (Pisos de la Torre)**
- Dentro de la Torre: N0 (Emberveil overworld), Garganta, N1-N10
- Sera aparece en N7 (Sahal, dentro de la Torre) y se une en N8
- Garrik aparece en N2 (Pastor de Cenizas)
- Mira y Vassen mueren en N7

**INCONSISTENCIA:** Son dos historias diferentes. Sera no puede aparecer ANTES y DESPUÉS de entrar a la Torre. Garrik no puede estar en el Pantano y en N2.

**DECISIÓN NECESARIA:** ¿Cuál es la estructura canónica?
- Opción 1: El Acto I (ActI/) es el tutorial/mundo exterior. La Torre empieza después.
- Opción 2: N0-N10 ES el Acto I completo. El mundo exterior es solo N0.

**RECOMENDACIÓN:** La estructura N0-N10 es más completa y actualizada. ActI/ debería integrarse como preludio o tutorial expandido, NO como historia paralela.

---

### 2. GARRIK: Dos versiones incompatibles

**Versión A (ActI/):**
- Amigo de infancia del protagonista
- Entró a la Torre un año antes
- Encontrado en el Pantano (Cap.4), herido
- Elección Q17: llevarlo o dejarlo

**Versión B (N0-N10):**
- Aparece en N2 (Pastor de Cenizas)
- No sabe quién es (amnesia/reencarnación)
- Es Lian (5ta reencarnación, Esquirla 6ta Edad)
- No es amigo de infancia... o no lo recuerda

**INCONSISTENCIA:** ¿Es amigo de infancia o no? ¿Tiene amnesia o no?

**RECOMENDACIÓN:** Mantener la versión B (N2). Es más interesante narrativamente. La versión A (ActI/) puede adaptarse como:
- Garrik SÍ es amigo de infancia
- Pero olvidó quién era (reencarnación)
- En N2, no reconoce al protagonista inicialmente
- Más adelante recupera fragmentos

---

### 3. SERA: Dos versiones incompatibles

**Versión A (ActI/):**
- Aparece en Q19 ("La Figura de Capa Negra")
- Está en la Aguja (mundo exterior)
- Se une ANTES de entrar a la Torre

**Versión B (N0-N10):**
- Aparece en N7 (Sahal, dentro de la Torre)
- Observa desde lejos
- Se une en N8 (Jardines Flotantes)

**INCONSISTENCIA:** Sera no puede estar en dos lugares.

**RECOMENDACIÓN:** Mantener la versión B (N7-N8). Es más impactante emocionalmente (después de la muerte de Mira/Vassen). La versión A debería eliminarse o adaptarse como:
- La "figura de capa negra" del ActI/ podría ser Sera... pero como visión/eco
- O podría ser otro personaje (Elia, la que vio más allá)

---

### 4. MIRA Y VASSEN: Muerte inconsistente

**Versión A (ActI/):**
- Mira se une en Q03 (automático)
- Vassen se une en Q06 (automático)
- Ambos llegan al Refugio (final del Acto I)
- No mueren en el Acto I

**Versión B (N0-N10):**
- Mira y Vassen se unen en N1 (Velmar)
- Mueren en N7 (Sahal, dentro de la Torre)

**INCONSISTENCIA:** ¿Mueren o no? ¿Cuándo?

**RECOMENDACIÓN:** Mantener la muerte en N7. Es el momento emocional más fuerte del Acto I. La versión A debería adaptarse para que Mira y Vassen:
- Se unan en el mundo exterior (ActI/)
- Entren a la Torre con el protagonista
- Mueran en N7

---

### 5. CAÍN: Solo existe en N0-N10

**Problema:** En ActI/, no hay mención de Caín. Pero en N0-N10, Caín es el amigo de infancia que muere en la Garganta.

**RECOMENDACIÓN:** Integrar a Caín en el ActI/ como compañero inicial que muere al entrar a la Torre (Garganta).

---

## 🟡 PROBLEMAS MEDIOS

### 6. BESTIARIO DUPLICADO

**Archivos:**
- `Design/Lore/bestiario_completo.md`
- `Design/Lore/bestiario_diario.md`
- `Design/Lore/bestiario_N3_N6.md`
- `Design/Bestiario/bestiario_completo.md`

**Problema:** Hay 4 bestiarios. ¿Cuál es el canónico?

**RECOMENDACIÓN:** Consolidar en uno solo. `Design/Lore/bestiario_N3_N6.md` es el más reciente y completo para Acto I.

---

### 7. SISTEMAS DUPLICADOS

**Archivos:**
- `Design/Sistemas/sistema_facciones.md`
- `Design/Sistemas/reputacion_facciones.md`
- `Design/Sistemas/eventos_facciones.md`

**Problema:** Tres archivos sobre facciones. ¿Son complementarios o duplicados?

**RECOMENDACIÓN:** Consolidar en `sistema_facciones.md`.

---

### 8. NPCs N3-N6 vs NPCs por piso

**Archivos:**
- `Design/Quests/NPCs_N3_Aelthar.md`
- `Design/Quests/NPCs_N4_Corredor.md`
- `Design/Quests/NPCs_N5_Thornwall.md`
- `Design/Quests/NPCs_N6_CamarasJade.md`

**Pero faltan:**
- NPCs_N1_Velmar.md
- NPCs_N2_Pastor.md
- NPCs_N7_Sahal.md
- NPCs_N8_Jardines.md
- NPCs_N9_Argentia.md

**RECOMENDACIÓN:** Crear los NPCs faltantes o integrarlos en los documentos de piso.

---

### 9. MISIONES SECUNDARIAS: Tres archivos

**Archivos:**
- `Design/Quests/misiones_secundarias.md` (original)
- `Design/Quests/misiones_secundarias_expandidas.md` (tabla resumen)
- `Design/Quests/misiones_secundarias_dialogos.md` (diálogos completos)

**Problema:** ¿Cuál es el maestro?

**RECOMENDACIÓN:** `misiones_secundarias_dialogos.md` es el más completo. Los otros deberían marcarse como obsoletos o integrarse.

---

### 10. GDD DESACTUALIZADO

**Archivo:** `Design/GDD/GAME_DESIGN_DOCUMENT.md`

**Problema:** Probablemente refleja la versión antigua (ActI/ como mundo exterior).

**RECOMENDACIÓN:** Actualizar con la estructura N0-N10.

---

## 🟢 MEJORAS SUGERIDAS

### 11. FALTAN: NPCs por nivel (N1, N2, N7, N8, N9)

**Prioridad:** Media
**Trabajo:** Crear documentos NPCs_N1, N2, N7, N8, N9 siguiendo el formato de N3-N6.

---

### 12. FALTAN: Banter entre Sera y Garrik

**Prioridad:** Media
**Trabajo:** En `Design/Dialogues/banter_companeros.md` hay banter Mira-Vassen y Garrik-Mira. Pero falta banter Sera-Garrik (N8-N10).

---

### 13. FALTAN: Transiciones con romance activo

**Prioridad:** Baja
**Trabajo:** Las transiciones N0-N10 (`transiciones_N0_N10.md`) no tienen variantes de romance. Podrían añadirse líneas de Sera si el romance está activo.

---

### 14. FALTAN: Escenas post-muerte Mira/Vassen en N8-N10

**Prioridad:** Media
**Trabajo:** Después de N7, el grupo está devastado. Pero en N8-N10, Garrik y el protagonista rara vez mencionan a Mira/Vassen. Debería haber:
- Momentos donde Garrik mencione a Mira
- El protagonista vea algo que le recuerde a Vassen
- Sera preguntando quiénes eran (si no los conoció)

---

### 15. FALTAN: Consecuencias de elecciones en N2-N6 para N7-N10

**Prioridad:** Media
**Trabajo:** Muchas elecciones en N2-N6 tienen consecuencias en NG+, pero pocas en el mismo playthrough. Ejemplos:
- Si dejaste crecer la flor en N6: ¿aparece en N8?
- Si liberaste al Eco en N6: ¿te encuentra en N8?
- Si pagaste la deuda de Maris en N5: ¿aparece en N9?

---

## 📋 PLAN DE ACCIÓN RECOMENDADO

### Fase 1: Consolidar estructura (CRÍTICO)
1. Decidir: ¿ActI/ es preludio o se elimina?
2. Si es preludio: adaptar para que sea consistente con N0-N10
3. Si se elimina: migrar elementos únicos (Hadda, Tirenne, Cinco Veteranos) a N0-N10

### Fase 2: Corregir personajes (CRÍTICO)
4. Consolidar backstory de Garrik (amigo de infancia + amnesia)
5. Consolidar aparición de Sera (solo N7-N8)
6. Integrar muerte de Caín en Garganta (N0→N1)

### Fase 3: Completar contenido (MEDIO)
7. Crear NPCs faltantes (N1, N2, N7, N8, N9)
8. Añadir banter Sera-Garrik
9. Añadir escenas post-muerte Mira/Vassen
10. Añadir consecuencias de elecciones en mismo playthrough

### Fase 4: Limpiar duplicados (BAJO)
11. Consolidar bestiarios
12. Consolidar sistemas de facciones
13. Marcar archivos obsoletos
14. Actualizar GDD

### Fase 5: Polish (BAJO)
15. Variantes de romance en transiciones
16. Revisar consistencia de nombres (Lian=Garrik, etc.)
17. Revisar que todos los documentos referencien la misma estructura

---

*Auditoría creada: 2026-05-18*
*Próximo paso: Decidir qué hacer con ActI/*
