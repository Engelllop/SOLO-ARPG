# N3 — Aelthar: La Ciudad Espejo
## Tema: Abuso Sexual Infantil (velado → revelado)

### Contexto
Ciudad elfa construida sobre un lago de espejos. Todo es bello, pulcro, ordenado. Los elfos sonríen. Te ofrecen comida, refugio, compañía. **Demasiado amables.** Hay niños elfos en las calles — pero nunca ves a los mismos dos veces seguidas.

### La Verdad
Bajo la ciudad, hay sótanos. En los sótanos, celdas. En las celdas, niños. Los elfos de Aelthar **compran y venden niños** que bajan de pisos superiores o que nacen aquí. El templo sumergido no es un templo — es el **archivo de transacciones**.

### Señales para el jugador
1. **Niños que no hablan** — NPC_ChildElf, siempre miran al suelo, brazos con moretones (textura SM_Body_Bruises)
2. **Puertas cerradas en los sótanos** (SM_Door_IronLocked) — se escuchan llantos detrás
3. **Diálogo de Garrik** si investiga: "Estos niños... no sonríen como los adultos."
4. **Tumba de niños** en el jardín trasero del templo — 12 lápidas pequeñas (SM_Tombstone_Child)
5. **Carta en el archivo** (leíble): "Vendido al comprador #47. Edad: 6. Precio: 300 de oro."

### Encuentro clave — "El Archivo Abierto"
- Al investigar suficiente, un elfo anciano (Mareth, el archivero) confiesa si se le presiona:
  - "Son huérfanos. De los que no sobreviven. Los vendemos a los pisos bajos."
  - "¿Crees que esto es malo? La Torre es peor. La Torre **crea** huérfanos."
- Opción: **Matar a Mareth** → ciudad hostil, guardias atacan. **Perdonarlo** → flag de culpa.

### El Leviatán del Espejo — Fases Reescritas

**Fase 1 — La Máscara**
- El Leviatán aparece como una **figura angelical** (SM_Angel_White, 4m, alas blancas, rostro hermoso)
- El agua del lago refleja una versión **distorsionada** (garras, cuernos)
- **Mecánica:** Mientras mires al Leviatán directamente, ves al ángel. Si miras el reflejo, ves al demonio.
- La verdad está en el reflejo. Siempre.

**Fase 2 — El Espejo Roto**
- El ángel se rompe. Debajo: un cuerpo humanoide con **caras de niños** incrustadas en el torso (SM_Body_Faces)
- Cada cara llora. Cada cara abre los ojos cuando el Leviatán ataca.
- **Mecánica "Eco Infantil":** Cada 15s, una cara llora fuerte → stun AOE 3s + aparece un niño fantasma que corre (no se puede interactuar)
- **Diálogo (Garrik):** "Dios... esos niños... están atrapados ahí."

**Fase 3 — El Corazón del Espejo**
- El Leviatán se abre. En su interior: un feto de jade (SM_Fetus_Jade, emisivo, latido)
- **Mecánica:** Hay que **no golpear el feto**. Golpear el espejo alrededor.
  - Si golpeas el feto: el latido se acelera + aparecen más fantasmas
- **Objetivo:** 10 golpes al borde del espejo → el espejo se rompe → el Leviatán se desvanece
- **Post-combate:** El agua del lago se vuelve roja por 3s. Luego clara.

### Post-Combate
1. Los elfos de la ciudad huyen o se suicidan (NPCs caen de rodillas, se desvanecen)
2. **CG:** El archivo arde. Las celdas se abren. Los niños salen corriendo hacia la luz.
3. **Transición a N4:** Un niño se acerca al protagonista. No habla. Señala una puerta en el fondo del lago. Sonríe. Desaparece.
4. **Recompensa:** 200 XP, 100 oro, **Marca de Culpa** (item — reduce todas las resistencias en 10% pero da +20% daño a jefes)

### Items especiales

| Item | Efecto |
|------|--------|
| Marca de Culpa | -10% res, +20% daño jefes. No se puede dropear. |
| Carta del Archivo | Lore. Puedes leerla en el inventario. |
| Diario de un Niño | "Mamá dijo que volvería. No volvió." |
