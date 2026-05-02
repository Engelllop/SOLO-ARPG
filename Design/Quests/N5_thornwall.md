# N5 — Thornwall: La Ciudad Fortificada
## Tema: Violación y Esclavitud Sexual

### Contexto
Ciudad humana fortificada. "La última civilización antes del abismo", dicen. En realidad es un burdel de 3 pisos. Las mujeres aquí no existen — son **producto**. Los hombres tampoco son libres — son esclavos o clientes.

### La Verdad
Thornwall sobrevive vendiendo placer. Los que bajan de pisos superiores (mujeres, niños, hombres jóvenes) son capturados en la puerta, drogados y vendidos. Hay tres categorías:
- **Las Pájaras** (mujeres de lujo) — encerradas en la torre este
- **Los Bestias** (hombres esclavos) — en las minas, mueren en 6 meses
- **Los Inocentes** (niños) — en los sótanos, precio especial

### Señales para el jugador

1. **Carteles en las calles:** "Mujer joven, 15 años. Pelo rubio. 500 de oro. Negociable."
2. **El Martillo y el Yunque:** La forja de Torben no es una forja. Es una **sala de subastas**.
   - NPC Torben no es herrero. Es el **traficante principal**.
   - Su "brazo mecánico" es una prótesis que le dieron a cambio de 100 esclavos.
3. **El mercado de pieles:** Las "pieles" son humanas. Hay capas hechas de piel de mujer (SM_Cape_HumanSkin, textura con pezones visibles).
4. **La taberna:** Se escuchan gritos desde el sótano. El posadero dice: "Es el gato. Gatos peleándose."
5. **Marta (tabernera):** Si le preguntas, se ríe. "¿Crees que esto es malo? Allá arriba se mueren de hambre. Aquí por lo menos comen."

### Encuentro Clave — "La Pájara en la Jaula"
1. Al entrar a la torre este, hay una jaula de oro (SM_Cage_Golden, 3m×3m). Las barras están manchadas de semen seco.
2. Dentro: BP_SlaveGirl (NPC, 14-16 años, desnuda, solo cubierta por harapos SM_Cloth_Rags)
   - No habla. Solo tiembla. Tiene moretones en los muslos y marcas de mordidas en los hombros.
   - Tiene un collar de hierro (SM_Collar_Iron). En el collar: "Virgen. 250 de oro."
3. **Opción A:** Pagas 250 de oro. Ella es liberada. Corre hacia la salida. No la vuelves a ver.
   - +20 XP. Si vuelves a Thornwall más tarde: NPC en la ciudad dice que se ahorcó en un árbol.
4. **Opción B:** Matas a Torben primero. Ella sale corriendo pero los guardias la alcanzan. El collar explota (veneno). Muere frente a ti.
5. **Opción C:** No haces nada. Más tarde, la ves siendo subastada en la plaza. Hace contacto visual contigo mientras la rematan. No hay diálogo. No hay opción. Solo su mirada.

**Nota de diseño:** No hay opción donde la chica sobreviva feliz. El mundo de la Torre no funciona así.

### El Carnicero — Reescrito

**Escenario:** Celda de máxima seguridad, 25m×20m. No es una celda. Es la habitación de **El Carnicero** — el verdugo que disfruta su trabajo.

**Fase 1 — El Verdugo**
- BP_Carnicero (ASOLOEnemyCharacter):
  - Mesh: SM_Humanoid_Executioner (2.5m, capucha negra, mandil de carnicero manchado de sangre, hacha de dos manos SM_Axe_Executioner)
  - HP: 250
  - **Ataques:**
    - **Golpe de Hacha:** 25 daño, sangra 5 HP/s por 3s. Slow pero devastador.
    - **Pisotón:** Pisa al jugador, 20 daño + inmoviliza 1s. "Quieta, perra."
    - **Colección:** Invoca un esclavo que corre hacia el jugador y explota (15 daño). El esclavo tiene cara de terror.
  - **Diálogo durante combate:**
    - "Otra Pájara que cree que puede escapar."
    - "Tu carne va a adornar mi pared."
    - [Si el protagonista es mujer] "Te voy a disfrutar despacio."
    - [Si el protagonista es hombre] "Los hombres gritan más bonito."

**Fase 2 — La Bestia**
- Trigger: 150 HP perdidos
- El Carnicero se arranca la capucha. Tiene **dos caras**. Una humana, una de cerdo.
- **Ataques aumentados:** Doble golpe de hacha, más rápido.
- **Mecánica "El Hombre y la Bestia":** Cada 10s, cambia entre forma humana (defensiva) y bestia (ofensiva).
  - Humana: bloquea ataques, dice "¿Por qué no te rindes?"
  - Bestia: ataque salvaje AOE, gruñidos guturales
- **Objetivo:** 15 hits adicionales

**Fase 3 — La Muerte del Carnicero**
- Trigger: muerte
- **Diálogo final:** "Tú... tú no eres como los otros... ¿Quién eres?"
- **CG:** Antes de morir, suelta el hacha. Por un segundo, parece humano. "Mi madre... me vendió aquí cuando tenía 8."
- No hay redención. Muere. Pero el jugador sabe que fue una víctima también. Eso es peor.

### Post-Combate
1. La ciudad entra en caos. Los ciudadanos huyen. Los esclavos son liberados.
2. El jugador puede abrir todas las celdas en 3 minutos (timer, tower defense de esclavos escapando)
3. Los esclavos liberados te agradecen. Una mujer te da un anillo de plata (SM_Ring_Silver). "Era de mi hija. Se la llevaron."
4. **Transición:** Escalera de piedra → N6
5. Recompensa: 250 XP, 130 oro, **Llave de Jade** (item clave)

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Capa de Piel Humana | +10% sigilo. Descripción: "Huele a mujer." |
| Anillo de la Hija | +5% resistencia. "Todavía caliente." |
| Hacha del Carnicero | 22 daño. +15% sangrado. "Todavía gotea." |
| Recibo de Venta (Torben) | Lore. "Vendida: Mujer, 14, pelo castaño. Precio: 200." |
