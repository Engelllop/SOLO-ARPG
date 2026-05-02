# N4 — El Bosque Petrificado
## Tema: Hambruna y Canibalismo Forzado

### Contexto
Lo que parece un bosque de árboles petrificados no lo es. **Son personas.** Miles de personas que murieron de hambre, petrificadas en posiciones de desesperación. Algunas familias enteras abrazadas. Madres con bebés en brazos. Gente arrodillada pidiendo comida al cielo vacío.

### La Historia
Cuando la Torre empezó a crecer, los pueblos de alrededor quedaron atrapados. Sin tierra fértil, sin animales, sin ríos. La gente empezó a comer lo que encontraba: corteza, tierra, después entre ellos. Los que sobrevivieron se comieron a los muertos. Después a los débiles. Después a sus hijos.

Cuando ya no quedó nada, el bosque los petrificó. No por magia — por **inanición total**. Sus cuerpos simplemente se quedaron sin recursos para mantenerse líquidos.

### Mecánica Especial — Medidor de Hambre
- **UI:** Barra de 5 segmentos decreciente en la esquina superior izquierda
- **Empieza en 5.** Se reduce 1 segmento por cada 60s en el bosque.
- **Efectos:**
  - 4: Visión borrosa (PostProcess blur 0.2)
  - 3: Velocidad reducida 15%
  - 2: Daño reducido 25%
  - 1: Alucinaciones de comida (objetos brillantes que no existen)
  - 0: **Muerte.** Caes al suelo. Despiertas en la entrada del bosque con 1 HP y todos los items consumibles perdidos.
- **Recuperación:** Comer. Hay pocas fuentes de comida:
  - Hongos (50% probabilidad de veneno)
  - Carne de enemigos (asqueroso, restaura 2 segmentos)
  - **Carne humana** (de los cuerpos petrificados — ver abajo)

### Elección Moral — "La Última Cena"
En una cueva del bosque (escondida, noreste), hay un campamento. Una fogata apagada. Una familia petrificada alrededor.
- **Mesa central:** Una pierna humana cocinada (SM_Meat_HumanLeg). La familia se la estaba comiendo. El niño en brazos de la madre — la madre le estaba dando el trozo más grande.
- **Opción A — Comer:** Restaura todos los segmentos de hambre. +20 HP temporal. **Efecto permanente:** Los cuerpos alrededor "reaccionan" — las cabezas giran lentamente hacia ti. Nunca dejas de ver sus ojos. (-5% a resistencias mentales permanente)
- **Opción B — No comer:** No ganas nada. Pero un espíritu del bosque (BP_Spirit_Famine) aparece: "Gracias por no profanar." Te da **Semilla de Vida** (item clave para jardín en N9).
- **Opción C — Entierras los cuerpos:** Toma 30s de animación. Los espíritus de la familia aparecen: "Que encuentres lo que nosotros no pudimos." Te dan +10 HP max permanente.

### Zonas del Nivel
1. **El Claro de los Ahogados** (oeste) — cuerpos colgando de árboles. Se ahorcaron cuando se quedaron sin niños que comer.
2. **El Lecho del Río Seco** — las huellas luminosas son de pies descalzos, algunos de niños, que huyeron. No hay huellas de vuelta.
3. **La Arboleda de los Susurros** — los "susurros" no son palabras. Es el viento pasando por costillas humanas expuestas. Cada árbol es una persona petrificada.
4. **La Madre Petrificada** (centro, en el camino al jefe) — estatua de mujer arrodillada, brazos extendidos. Su vientre está abierto. Un feto momificado cuelga del cordón umbilical petrificado. Interactuar: élfico grabado: "Mi hijo. Mi última comida. Perdóname."

### El Guardián Petrificado — Reescrito

**Fase 1 — El Coloso del Hambre**
- BP_ColosoHambre (ASOLOEnemyCharacter):
  - Mesh: SM_Colossus_Famine (gigante de 6m, costillas marcadas, barriga hundida, boca abierta en grito perpetuo)
  - HP: 300
  - **Ataques:**
    - **Golpe de Inanición:** AOE 4m. Si golpea, succiona 1 segmento de hambre del jugador (el jugador pierde hambre, el coloso se cura 20 HP)
    - **Vómito de Huesos:** Proyectil AOE 3m, 18 daño, ralentiza. Huesos humanos.
    - **Grito de Madre:** Stun AOE 5m, 1s. Sonido de madre llorando por su hijo.
- **Objetivo:** 15 hits. Cada hit rompe parte de la armadura de huesos.

**Fase 2 — El Núcleo de Desesperación**
- El coloso cae. Emerge un corazón humano (SM_Heart_Human, late, 1m diámetro, venas conectan al suelo)
- **Mecánica "Latido":** Cada 10s, el corazón late fuerte → onda de choque AOE 10m, 15 daño + empuja
- **Mecánica "Sangre de Inocentes":** El corazón tiene 4 "hijos" (SM_Fetus_Heart) alrededor. Golpear un hijo: el corazón recibe daño pero el hijo grita.
  - Hijos rotos: 3 → el corazón se rompe más rápido (5 hits vs 10 hits)
- **Fin:** Corazón destruido → todos los hijos caen al suelo → silencio

### Post-Combate
1. El corazón se desintegra en polvo rojo
2. Por primera vez, se escucha **viento moviendo hojas** — el bosque ha estado en silencio absoluto todo este tiempo
3. Una niña fantasma (BP_GirlGhost) aparece al borde del claro. Señala la salida. Desaparece.
4. Puerta de árbol petrificado → N5
5. Recompensa: 200 XP, 100 oro, **Semilla de Vida** (item opcional)

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Semilla de Vida | Para jardín en N9 (si no comiste) |
| Carne Humana Cocida | Restaura 5 hambre. +10 HP. Aparece en inventario con icono de asco. |
| Costilla de Madre | Material raro. +15% daño perforante. Descripción: "Todavía caliente." |
| Diario de un Padre | "Comí a mi hija ayer. Todavía siento su sabor. Quiero morir." |
