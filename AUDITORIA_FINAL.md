# Auditoría Final del Proyecto SOLO-ARPG
## Fecha: 2026-05-18
## Objetivo: Identificar TODO lo que falta antes de considerar el Acto I completo

---

## ✅ COMPLETADO (En esta sesión)

### Narrativa
- [x] Reescritura N2-N10 con temas adultos
- [x] Misiones secundarias con diálogos (14)
- [x] Romance Sera expandido (iniciativa femenina, 5 misiones)
- [x] Banter Sera-Garrik hostil
- [x] Escenas post-muerte Mira/Vassen (N8-N10)
- [x] Variantes de romance en transiciones (N8→N9, N9→N10)
- [x] Consecuencias inmediatas de elecciones (N2→N6 afectan N7→N10)
- [x] NPCs por piso (N1-N9 completos)
- [x] Backstory Garrik consolidado (amigo de infancia + amnesia)
- [x] Sera confirmada solo en N7-N8

### Sistemas
- [x] Sistema de facciones consolidado
- [x] GDD actualizado (v1.0)
- [x] Índice Maestro actualizado
- [x] Archivos obsoletos eliminados (6 archivos)

---

## ⚠️ FALTA / DESACTUALIZADO

### 1. Documento N0 (Emberveil dentro de la Torre)
**Estado:** No existe documento específico para N0 dentro de la Torre.
**Tenemos:**
- `inicio_rpg.md` — Tutorial/Prólogo en Luzmarca y Emberveil (overworld)
- `overworld_emberveil.md` — Overworld completo
- `guardián_primera_garganta.md` — Jefe de la Garganta
- `la_primera_garganta.md` — Transición

**Falta:** Un documento que describa N0 como piso de la Torre (Emberveil DENTRO de la Torre, no el overworld).

**Nota:** N0 en la estructura N0-N10 es diferente al overworld. Es Emberveil como se ve DENTRO de la Torre (reconstrucción, eco, etc.).

---

### 2. Jefes del Acto I (N0-N10)
**Estado:** Los jefes están descritos en los documentos de piso, pero hay inconsistencias.

**Jefes actuales (documentos de piso):**
| Piso | Jefe | Documento |
|------|------|-----------|
| Garganta | Guardián de la Garganta | `guardián_primera_garganta.md` |
| N1 | La Voz | `N1_velmar.md` |
| N2 | El Pastor de Cenizas | `N2_el_pastor_de_cenizas.md` |
| N3 | Leviatán del Espejo | `N3_aelthar.md` |
| N4 | El Heraldo Sin Escudo | `N4_el_corredor_de_los_nombres.md` |
| N5 | El Carnicero | `N5_thornwall.md` |
| N6 | Coro de Jade | `N6_camaras_de_jade.md` |
| N7 | La Costurera | `N7_sahal.md` |
| N8 | Arpía de Bronce | `N8_jardines_flotantes.md` |
| N9 | Verdugo de Plata | `N9_argentia.md` |
| N10 | El Centinela de Granito | `N10_el_centinela_de_granito.md` |

**Inconsistencia:** `Design/Lore/bosses_rpg/README.md` tiene jefes DIFERENTES:
- N2: Pastor de Cenizas (coincide)
- N3: Espejo de Aelthar (vs Leviatán del Espejo)
- N4: Tallador de Huesos (vs Heraldo Sin Escudo)
- N5: Cáliz de Sahal (vs Carnicero)
- N6: Costurera (vs Coro de Jade)
- N7: Peso de Dum (vs Costurera)
- N8: Umbral (vs Arpía de Bronce)
- N9: Testigo (vs Verdugo de Plata)

**Decisión necesaria:** ¿Cuál es la estructura canónica?

---

### 3. Documento de N0 (Emberveil dentro de la Torre)
**Estado:** NO EXISTE.

**Necesitamos:**
- Descripción de N0 como piso de la Torre
- ¿Cómo se ve Emberveil reconstruido dentro de la Torre?
- ¿Qué NPCs hay?
- ¿Cuál es el jefe/tutorial?
- Transición de N0 a Garganta

---

### 4. Hadda el Bardo Ciego
**Estado:** Existe `Design/Lore/hadda_el_bardo_ciego.md` pero no está integrado.

**Pregunta:** ¿Hadda aparece en el Preludio (overworld) o en N0-N10?

---

### 5. Sistema de Guardado
**Estado:** `Design/Sistemas/sistema_guardado.md` existe pero no está verificado.

**¿Está actualizado con la estructura N0-N10?**

---

### 6. Clases y Habilidades
**Estado:** `Design/Lore/classes/` existe pero no está integrado con la narrativa.

**¿Las clases afectan diálogos o solo combate?**

---

### 7. Post-Créditos y Finales Detallados
**Estado:** Tenemos `sistema_finales.md` pero no está verificado.

**¿Está actualizado con los 5 finales + variante romance + Final Secreto?**

---

## 📋 RECOMENDACIÓN FINAL

Si querés considerar el Acto I "completo" para implementación, necesitamos:

### CRÍTICO (bloquea implementación)
1. **Documento N0** — Emberveil dentro de la Torre (tutorial)
2. **Consolidar jefes** — Decidir si la estructura actual (Leviatán, Heraldo, Carnicero, etc.) es la canónica
3. **Verificar sistema_finales.md** — ¿Está actualizado?

### MEDIO (mejora calidad)
4. **Integrar Hadda** — ¿Preludio o N0?
5. **Verificar sistema_guardado.md** — ¿Funciona con N0-N10?
6. **Verificar clases** — ¿Afectan narrativa?

### BAJO (polish)
7. **Revisar bosses_rpg/README.md** — Actualizar o eliminar
8. **Revisar ActI/ capítulos** — ¿Siguen siendo relevantes como Preludio?

---

¿Querés que haga los 3 puntos críticos ahora?
