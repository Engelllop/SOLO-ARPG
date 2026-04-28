# Lore de los Jefes de Piso

Los 10 jefes que cierran cada decena de pisos son los **hitos narrativos** de la Torre. No son obstáculos: son argumentos. Cada uno encarna una pregunta que el Arquitecto le hace al jugador sobre sí mismo y sobre el mundo.

---

## Archivos completados

| Piso | Nombre | Subtítulo | Estado |
|---|---|---|---|
| 10 | El Centinela de Granito | La Prueba de Materia | ✓ |
| 20 | La Hidra del Abismo | La Prueba de Fluidez | ✓ |
| 30 | Malakar, el Rey Lich | La Prueba de Trascendencia | ✓ |
| 40 | Aurum, el Dragón de Cristal | La Prueba de Claridad | ✓ |
| 50 | Unidad Cero, el Coloso de Hierro | La Prueba de Orden | ✓ |
| 60 | La Sombra Eterna | La Prueba de Identidad | ✓ |
| 70 | Valerius, el Señor de las Bestias | La Prueba de Instinto | ✓ |
| 80 | Azariel, el Heraldo Caído | La Prueba de Fe | ✓ |
| 90 | Chronos, el Tiempo Fracturado | La Prueba de Realidad | ✓ |
| 100 | El Arquitecto | El Origen | ✓ |

## Archivos completados

```
piso_10_centinela.md
piso_20_hidra.md
piso_30_malakar.md
piso_40_aurum.md
piso_50_unidad_cero.md
piso_60_sombra_eterna.md
piso_70_valerius.md
piso_80_azariel.md
piso_90_chronos.md
piso_100_el_arquitecto.md
```

> El piso 100 amplía [`../01_el_arquitecto.md`](../01_el_arquitecto.md) con los tres finales, el combate y la cinemática del sello. Ambos archivos son complementarios.

---

## El arco temático de los jefes

**Pisos 10–50: Pruebas del mundo externo**
P10 materia → P20 adaptación → P30 fracaso ajeno → P40 verdad → P50 orden vs caos

**Pisos 60–90: Pruebas del mundo interno**
P60 identidad → P70 instinto → P80 fe → P90 realidad fracturada

**Piso 100: El origen**
¿Qué haces con la respuesta cuando la tienes?

---

## Plantilla sugerida (copiar a cada ficha)

```markdown
# Piso XX — [Nombre del Jefe]

## Identidad in-universe
[Quién/qué es. Cómo lo perciben los habitantes. Mitos asociados.]

## Identidad canon (la verdad oculta)
[Qué módulo del Patrón representa. Por qué existe. Qué prueba el Arquitecto con él.]

## Aspecto físico
[Descripción visual. Lo que los conceptos artísticos deben capturar.]

## Mecánicas de combate (resumen narrativo)
[Cómo se siente pelearlo. La mecánica especial citada en el GDD, expandida narrativamente. Sin números — eso va al GDD/balance.]

## Diálogos (si los hay)
[Pre-combate, durante, post-combate. Líneas clave.]

## Drops únicos (lore de ítems)
[Drop garantizado del jefe + descripción in-universe del ítem.]

## Manifestación del Primer Ataque
[Qué aparece en la estatua, qué dice el pergamino, qué runa graba en el arma.]

## Conexiones con otros jefes / con el lore mayor
[Cómo este jefe se relaciona con la cosmología, con otros jefes, con personajes recurrentes.]

## Hook de quest secundaria asociada
[Si hay una quest del overworld que se relaciona con este jefe, esbozarla aquí.]
```

---

## Notas de coherencia (leer antes de escribir cada ficha)

- **Cada jefe es un módulo del Patrón.** Plantéalo como una hipótesis del Arquitecto que debe probarse. ¿Qué pregunta hace este combate? Ver [`../03_la_torre.md`](../03_la_torre.md) para inspiración (ya hay una pregunta sembrada por bioma).
- **Ningún jefe es "malvado".** Cumplen función. Algunos lo saben. Otros no. La Sombra Eterna es la más tragique porque sabe perfectamente lo que es.
- **La progresión narrativa va de lo físico a lo existencial.** P10 = pelear contra una roca. P100 = pelear contra una idea. Todos los pisos intermedios son escalones suaves entre esos dos extremos. Si una ficha rompe esa curva, falta consistencia.
- **Cada jefe debe tener al menos un detalle vulnerable o doloroso.** El jugador no debe odiar a ninguno. El examen final del Acto V (decidir el destino del Tejido) requiere que el jugador haya empatizado con la maquinaria.
- **Los Heraldos Caídos** mencionados en [`../01_el_arquitecto.md`](../01_el_arquitecto.md) están relacionados con el jefe del piso 80 (El Heraldo Caído). Cuando se escriba esa ficha, conectar.
- **El Rey Lich (P30) y la Sombra Eterna (P60) son ambos antiguos Persistentes que fallaron.** Esto es coherencia importante. Sus fichas deben respetar este detalle común — son los dos espejos del protagonista que no lo lograron.
- **El piso 50 (Coloso de Hierro)** es un buen lugar para introducir la idea de que el Arquitecto experimenta también con la **civilización industrial**, no solo con la naturaleza/magia. Preparar al jugador para que el metalenguaje informático del Acto V no sea totalmente sorpresivo.

---

## Cómo encajar con la mecánica del juego

Los siguientes elementos del GDD ya tienen relación directa con cada ficha:

- **Drop especial de jefe** (1 ítem garantizado de rareza épica o superior) → describir el ítem in-universe.
- **Drop de primer kill** (ítem mítico único en el servidor) → este es el ítem que aparecerá en las leyendas. Su descripción debe ser memorable y pesada. Los míticos son canon eterno.
- **Mecánicas de fase del jefe** ya están definidas en [`../../GDD/GAME_DESIGN_DOCUMENT.md`](../../GDD/GAME_DESIGN_DOCUMENT.md#83-jefes-de-piso-ejemplos) en formato muy resumido. Las fichas aquí amplían esas notas, pero no las contradicen.

---

## Coordinación con `Source/`

Cada jefe del piso eventualmente tendrá un asset de gameplay:

- Un `EnemyCharacter` derivado con su lógica de fases en C++.
- Un `LootTable` (ya existe `LT_Boss`, posiblemente cada jefe tendrá la suya).
- Un `BehaviorTree` específico.

**No hace falta que el lore se ate al código todavía.** Pero al escribir la ficha, ten en mente que las "mecánicas de combate" descritas narrativamente serán traducidas a `BTService` / `BTTask` por el equipo de gameplay. Sé concreto: *"el jefe alterna entre dos estados, identificables visualmente por X"* es traducible. *"el jefe es impredecible y misterioso"* no lo es.
