# N7 — Sahal: La Ciudad de Adobe
## Tema: Genocidio y Descuartizamiento

### Contexto
Sahal es la primera "ciudad libre" que el jugador encuentra. Gente común. Mercados. Niños jugando en las calles. Vida normal. **Demasiado normal.** Porque Sahal fue masacrada hace 200 años y nadie se lo dijo.

### La Verdad
La ciudad no está viva. **Está muerta.** Todos los que ves son fantasmas — atrapados en un loop de su último día de vida. El día que **La Costurera** llegó. Ella no es una persona. Es una entidad que **teje almas en mantas de sufrimiento**.

La ciudad entera fue descuartizada. No por soldados — por una sola entidad que hilaba sus cuerpos en un telar gigante. Cada persona de Sahal fue descosida viva, sus extremidades separadas y vueltas a coser en configuraciones grotescas. Luego, sus almas fueron atrapadas en el telar para siempre.

### Señales para el jugador

1. **Los ciudadanos no sangran.** Si los golpeas (ataque accidental), sale polvo, no sangre.
2. **Los niños juegan a "la costurera"** — una niña persigue a otras cosiendo sus sombras con una ramita
3. **No entra ni sale nadie de la ciudad.** Si ves desde la entrada, la gente "desaparece" al llegar a los bordes
4. **El mercado vende "mantas"** — SM_Blanket_Weaved. Si las examinas de cerca, ves que están hechas de cabello humano.
5. **Los perros** — hay perros callejeros (BP_Dog_Sahal). Si los sigues, te llevan a montones de huesos. Huesos humanos. Todos del mismo tamaño. Cortados limpios.

### Mecánica Especial — "El Despertar"
- Al entrar, la ciudad parece normal. Los NPCs son amables. Te ofrecen comida, refugio.
- **Pero hay un timer invisible de 5 minutos.** Después de 5 minutos, el velo se rompe:
  - Los NPCs dejan de moverse
  - Sus caras se distorsionan (se ven como costuras)
  - Empiezan a gritar — todos al mismo tiempo
  - **CG:** El telar aparece en el centro de la plaza. La ciudad se vuelve una morgue al aire libre.
  - Ya no puedes volver atrás. La puerta de entrada se cierra.

### Zonas del Nivel (antes del Despertar)

1. **Puerta del Sol** — Guardias sonríen. "Bienvenido a Sahal. Quédate todo lo que quieras." (su sonrisa es demasiado amplia — se ve la costura en la comisura de los labios)

2. **Plaza del Silencio** — Una vieja vendiendo frutas. Las frutas son ojos humanos (SM_Fruit_Eye). Si interactúas: "Son frutas, viajero. Prueba una." (si la pruebas: 5 daño, náusea)

3. **El Zoco** — Ropa y joyería. Los collares son dedos ensartados (SM_Necklace_Fingers). Los brazaletes son costillas (SM_Bracelet_Rib).

4. **La Torre de la Costurera** (noreste) — Una torre alta. Del techo cuelgan hilos que llegan a todas las casas. **Cada casa es una marioneta.** Dentro de cada casa: una familia descuartizada, los pedazos unidos por hilos.

### El Combate — La Costurera

**Fase 1 — La Ciudad Despierta**
- Todos los NPCs se vuelven enemigos (BP_CostureraMarioneta ×∞):
  - Los niños corren hacia ti con agujas (SM_Needle_Bone, 8 daño)
  - Los adultos te persiguen con instrumentos de cocina
  - Los viejos te maldicen (no dañan, pero reducen velocidad)
- **No puedes matarlos a todos.** El nivel no tiene límite de spawn. **Tienes que llegar a la torre.**

**Fase 2 — El Telar**
- Entras a la torre. El telar (SM_Loom_Giant, 8m×6m) ocupa toda la pared del fondo.
- En el telar: una manta gigante (SM_Blanket_Voider). La manta tiene la forma de la ciudad completa. Cada persona de Sahal está tejida en la manta.
- **La Costurera** está sentada frente al telar.
  - Mesh: SM_Humanoid_Seamstress (2m, sin rostro, dedos como agujas, cuerpo hecho de retazos de piel humana)
  - **Diálogo:**
    - "Has llegado. Te estaba esperando."
    - "Mira tu ciudad. Cada puntada es un grito."
    - "Mira a tus amigos. ¿Crees que no los he visto?"
    - "Mira y Vassen... son hermosos. Perfectos para la manta."
  - HP: 150

**Fase 2.1 — Mira y Vassen controlados**
- De repente, Mira y Vassen se congelan
- Hilos de plata salen de sus articulaciones
- **MiraMarioneta:** Dispara flechas, 15 daño, llora entre disparos. "No quiero... no quiero hacer esto..."
- **VassenMarioneta:** Escudo + espada, 20 daño, 100 HP. "Corre... corre mientras puedas..."
- **Mecánica:** Cada 10s, los hilos se tensan → Mira y Vassen son forzados a atacar. **Si los atacas, les haces daño real.** Si no los atacas, te hacen daño a ti.
  - No hay opción limpia.

**Fase 2.2 — Muerte de Mira y Vassen**
- Después de 40s (o 30 hits total recibidos por el grupo), Mira cae de rodillas
- Vassen se libera del control 2s. Corre hacia la Costurera. La apuñala.
- La Costurera lo agarra del cuello. Lo levanta. **CG:** Lo descuartiza en cámara lenta — primero el brazo derecho, luego el izquierdo, luego las piernas. Los pedazos caen al suelo. Vassen sigue vivo 2s después del último corte. Mira su torso. Mira al protagonista. Sonríe. Muere.
- Mira grita. Su control se rompe. Corre hacia el telar. Se corta la garganta con un hilo de plata para no ser cosida viva.
- **CG:** Mira cae sobre el telar. Su sangre fluye hacia la manta. La manta absorbe todo. La manta se vuelve roja por un momento.

**Fase 3 — La Costurera directa**
- La Costurera se levanta. Ahora tiene la cara de Vassen cosida en su hombro derecho y el brazo de Mira cosido en su lugar del brazo faltante.
- **Ataques:**
  - **Aguja de Vassen:** Golpe lento pero devastador, 30 daño. El brazo de Mira sostiene la aguja.
  - **Danza de Retazos:** Patada, 15 daño, 3 golpes rápidos. La cara de Vassen grita en cada golpe.
  - **Hilos de Control:** 3 hilos que atan al jugador (si no esquiva). Inmoviliza 3s. Durante esos 3s: la Costurera se acerca lentamente, aguja en alto. "Tus amigos te esperan."
- **Objetivo:** 30 hits totales
  - Cada 5 hits: la Costurera pierde un retazo de piel → se ve más esquelética
  - A 10 HP: cae de rodillas. "No... la manta... no está terminada..."

### Post-Combate
1. La Costurera muere. La manta en el telar se deshace. **Miles de almas son liberadas** — aparecen como puntos de luz que vuelan hacia el techo.
2. El cuerpo de Vassen está en pedazos. El de Mira está sobre el telar.
3. **Garrik se arrodilla.** Llora. "No... no puede ser..."
4. **Diálogo (Garrik):** "Los voy a enterrar. Tú sigue. Ve. Termina lo que empezamos."
5. **Enterramiento (cinemática):** Garrik entierra los pedazos de Vassen en una fosa. Envuelve el cuerpo de Mira en la manta rota y la entierra al lado.
6. **Sera aparece** — al borde de la torre. No mira. Solo está ahí. Esperando.
7. **Transición:** Fade a negro. El grupo camina hacia N8.

### Items y Recompensas

| Item | Efecto |
|------|--------|
| Última Flecha de Mira | +15% daño proyectil. "Todavía tiene su sangre." |
| Escudo Roto de Vassen | +20% resistencia. "Pedazo del brazo de Vassen todavía cosido." |
| Aguja de la Costurera | Arma. 25 daño, perfora armadura. "Todavía caliente." |
| Manta de Sahal | Lore. Se puede leer en el inventario. Nombres de todas las víctimas. 8,473 nombres. |

### Archivos C++ (N7)
```
Public/Character/N7/ASOLON7_Costurera.h + .cpp
Public/Character/N7/ASOLON7_MiraMarioneta.h + .cpp
Public/Character/N7/ASOLON7_VassenMarioneta.h + .cpp
Public/Data/N7/SOLON7_Constants.h
Public/NPCs/N7/BP_Sera.h + .cpp
```
