# N7 Sahal — Sistema de Falsa Agencia (REWORK)

> **Estado:** CANÓNICO desde 2026-07-05. Complementa y modifica `N7_sahal.md`.
> **Problema que resuelve:** Mira y Vassen morían "por diseño narrativo, no por error del jugador". Repetir el beat de la Garganta sin agencia hacía sentir al jugador que le quitaron el control justo donde más invirtió. **Solución: no darle control real — darle la *certeza* de que lo tuvo.** El jugador debe salir de N7 convencido de que existía una forma de salvarlos y que él la falló.

---

## 1. PRINCIPIO DE DISEÑO

**El jugador nunca debe saber que era inevitable.**

- Cero telegrafía de cutscene-death: nada de barras de vida que se vacían solas, nada de QTE decorativos que fallan siempre.
- Cada intento de rescate usa mecánicas reales del juego (combate, movimiento, timing) y **se puede "ganar"** — la trampa está en que ganar una cosa pierde la otra.
- La revelación de que estaba tejido desde el principio llega **solo en NG+**, de boca de la Costurera. En la primera partida, la culpa es del jugador y de nadie más.

---

## 2. SIEMBRA (N3–N6): LOS HILOS DE PROTECCIÓN

Misiones opcionales que *parecen* armadura contra el destino. Cada una es real, con recompensa mecánica real — para que el jugador crea que "cuenta".

| Piso | Quest | Qué parece | Qué hace de verdad |
|------|-------|------------|---------------------|
| N3 | **El Mapa Incompleto** (Mira) — ayudarla a cartografiar el lago de espejos. Mira: "Un mapa terminado es una promesa de volver." | Seguro de vida de Mira | Cambia su escena de muerte (muere "como cartógrafa", ver §4) |
| N4 | **El Juramento Roto** (Vassen) — encontrar el fragmento de su credo templario grabado en el corredor. | Redención que lo protegerá | Cambia su escena de muerte (muere "de pie", ver §4) |
| N5 | **Hilo de Plata** — un tejedor de Thornwall vende un amuleto "que la Costurera no puede coser". Caro. Real en apariencia: da +resistencia a control. | Contra-item directo contra el jefe de N7 | En N7, el amuleto **funciona 10 segundos** — suficiente para que el rescate parezca posible. Luego se quema. |
| N6 | **La Canción de los Ecos** — si liberaste a todos los niños del Coro, uno le susurra al protagonista: "en la ciudad del telar, corré hacia la voz, no hacia el silencio." | Pista para salvar a ambos | Pista real de *ruta* (§3), no de resultado |

**Regla:** ninguna quest dice "esto salvará a tus compañeros". El jugador construye esa teoría solo. Los jugadores siempre la construyen solos.

---

## 3. LA NOCHE DEL TELAR — ESTRUCTURA DE LA ELECCIÓN

### Acto 1 — La separación
La Costurera no embosca al grupo junto. Durante la noche en Sahal, **Mira y Vassen desaparecen del campamento en direcciones opuestas** (sus hilos ya estaban cosidos; caminaron solos, dormidos). El protagonista y Garrik despiertan con dos rastros.

- **Lectura del Suelo** (habilidad de N4) muestra ambas huellas — una hacia el telar central (silencio), otra hacia el barrio de los tintes (se oye a Vassen gritando órdenes de combate, la "voz" de la pista de N6).
- **Elección bajo reloj real (sin UI de timer):** las campanas del telar suenan. Cada campanada, los rastros se enfrían. Garrik: "No podemos partirnos en dos. ...¿O sí? Yo tomo uno. Elegí vos."

### Acto 2 — La bifurcación (jugable, ganable)
El jugador elige a quién va él (Garrik va al otro — y Garrik **siempre llega tarde**, sin culpa mecánica visible: encuentra la puerta cosida).

**Ruta Mira (el telar central):** combate real contra Formas Cosidas. El jugador ABRE la cámara, la ve viva en el telar, el Hilo de Plata (si lo tiene) la libera — 10 segundos de escape jugable con ella, hombro a hombro, disparando juntos. Entonces el amuleto arde y los hilos la retoman en la puerta misma. La Costurera obliga a Mira a apuntarle al protagonista. Mira, llorando, gira la flecha hacia sí misma. *(Sin quest del mapa: muere como marioneta, disparando; el protagonista tiene que esquivar a su amiga hasta que ella logra girar el arco.)*

**Ruta Vassen (barrio de los tintes):** Vassen pelea AÚN libre — el jugador combate a su lado 90 segundos completos, un compañero funcionando a pleno, el mejor momento mecánico de Vassen en todo el juego. Es ganable: limpian la ola. Y cuando baja el escudo para respirar, los hilos que ya llevaba dentro *desde N5* (si firmó el Contrato de Sangre: desde que lo firmó; si no: desde la puerta de Sahal) lo alzan hacia el telar. *(Con quest del juramento: recita el credo mientras lo cosen, de pie, y muere siendo templario. Sin ella: en silencio, como su padre.)*

### Acto 3 — La opción imposible
Si el jugador ignora ambos rastros y ataca directamente a la Costurera (la "tercera opción" que los foros jurarán que existe): pelea de jefe adelantada, más difícil, técnicamente superable hasta el 50% — al llegar ahí, la Costurera se retira *dentro* del telar y usa a **ambos** a la vez. Doble pérdida simultánea, la peor variante emocional. El logro oculto se llama **"Había que intentarlo"**.

---

## 4. MATRIZ DE VARIANTES

El resultado nunca cambia. **Todo lo demás sí.**

| Variable | Afecta |
|----------|--------|
| A quién fuiste | Qué muerte presenciás de cerca (la otra la reconstruís por el escenario y por Garrik, que no puede contártelo entero) |
| Quest del Mapa completada | Mira muere con voluntad (gira el arco) vs. como marioneta; loot: **Mapa Terminado** (revela todo N8) vs. Última Flecha |
| Quest del Juramento completada | Vassen muere de pie recitando vs. en silencio; loot: **Credo Restaurado** (buff permanente de bloqueo) vs. Escudo Roto |
| Hilo de Plata comprado | 10 segundos de escape jugable con Mira / primer golpe de los hilos a Vassen fallando — la ilusión más cruel del juego |
| Insististe con Caín en N1 (3+) | En el mapa de Mira, junto al telar, está anotado: **"CAÍN — creerle."** Ella le creyó al final. Nadie más lo sabrá. |
| Contrato de Sangre (N5) | Cambia *desde cuándo* Vassen estaba cosido — los jugadores que firmaron lo descubren en NG+ y no se lo perdonan |

**Post-N7:** Garrik nunca dice "no era tu culpa". Dice: "Yo llegué tarde. Vos llegaste. No sé qué es peor." — valida la culpa en lugar de disolverla.

---

## 5. LA REVELACIÓN (SOLO NG+)

Segunda partida, misma escena. La Costurera, al ver al protagonista:

> **COSTURERA:** "Otra vez vos. ¿Sabés que la otra vez también la elegiste a ella? ...¿O fue a él? Los hilos no cambian, tesoro. Solo cambia quién se culpa."

- Con **Esquirla 1 (Reloj de los Primeros)**: opción de diálogo nueva — "¿Alguna vez alguien los salvó?" Silencio largo. "...Iteración 12. No preguntes cómo terminó eso."
- Deja una puerta abierta para contenido futuro sin prometer nada: *hubo* una vez. El costo fue peor. (Anzuelo para la fase cifrada.)

---

## 6. REGLAS DURAS

1. Ninguna guía interna, tooltip, logro visible o texto de códice puede insinuar inevitabilidad antes de NG+.
2. Los playtests deben medir una sola cosa: % de jugadores que tras N7 creen que había forma de salvarlos. **Objetivo: >80%.** Si baja, la siembra de N3-N6 está floja.
3. Las dos rutas deben costar lo mismo en tiempo (±60s) para que la elección sea del corazón, no de la eficiencia.
4. Prohibido el slow-motion en las muertes. La Torre no dramatiza. Archiva.
