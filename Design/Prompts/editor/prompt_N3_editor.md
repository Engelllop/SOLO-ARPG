# N3 — Aelthar (Ciudad Espejo) — Editor Setup
> Basado en: Design/Quests/N3_aalthar.md
> Código C++: Source/SOLOCore/Public/Character/N3/

---

## Resumen Temático
**TEMA: Abuso sexual infantil (velado → revelado).** Ciudad élfica de espejos que es una fachada para una red de tráfico de menores. El jugador llega creyendo que es una misión diplomática/honorífica (espejismo), descubre la verdad cuando el reflejo de un niño pide ayuda.

---

## C++ Classes Generadas

### ASOLON3_LeviathanBoss (Jefe Final)
- `Public/Character/N3/ASOLON3_LeviathanBoss.h`
- `Private/Character/N3/ASOLON3_LeviathanBoss.cpp`
- **3 Fases:**
  - **Fase 1:** Invulnerable. Spawnea reflejos (ASOLON3_LeviathanReflejo) cada 15s. Daño es redirigido al jugador. El jugador debe sobrevivir mientras busca el punto débil.
  - **Fase 2:** Destruir 8 MirrorShards. Cuando ≤3 quedan → pasa a Fase 3.
  - **Fase 3:** Spawnea MirrorCore. Hay que pegarle 10 veces al core para vencerlo.

### ASOLON3_LeviathanReflejo (Minion)
- `Public/Character/N3/ASOLON3_LeviathanReflejo.h`
- `Private/Character/N3/ASOLON3_LeviathanReflejo.cpp`
- Réplica del jefe que ataca al jugador. Se destruye sola o al avanzar de fase.

### ASOLON3_MirrorShard (World Actor)
- `Public/World/N3/ASOLON3_MirrorShard.h`
- `Private/World/N3/ASOLON3_MirrorShard.cpp`
- Astilla de espejo destructible. 3 hits para romper. Llama a `OwnerLeviathan->OnShardDestroyed(this)` al destruirse.

### ASOLON3_MirrorCore (World Actor)
- `Public/World/N3/ASOLON3_MirrorCore.h`
- `Private/World/N3/ASOLON3_MirrorCore.cpp`
- Núcleo vulnerable en Fase 3. Llama a `OwnerLeviathan->RegisterCoreHit()` cuando recibe daño.

### BP_Garrik_Aelthar (NPC / Ally)
- `Public/Character/N3/BP_Garrik_Aelthar.h`
- `Private/Character/N3/BP_Garrik_Aelthar.cpp`
- Guerrero élfico (compañero temporal en Aelthar). Tiene TwinBladeAttack con daño crítico, sistema simple de StoryFlags (Set/Has).

### SOLON3_Constants.h
- `Public/Data/N3/SOLON3_Constants.h`
- IDs de quest, NPCs, items y diálogos para N3.

---

## Blueprint Tasks (Debes Crear)

### 1. Nivel: L_N3_Aelthar
- **Tamaño:** ~200x200 unidades
- **Atmósfera:** Ciudad de espejos brillante, arquitectura élfica blanca/dorada — todo se ve hermoso. Luz cálida, música de arpa.
- **Layout:**
  - **Entrada:** Puente de espejo sobre lago cristalino. Bienvenida de Garrik.
  - **Plaza Central:** Fuente de espejo líquido. Aquí ocurre el primer diálogo con el Senescal.
  - **Mercado:** Puestos de espejos, telas. Aquí Garrik nota algo raro (diálogo opcional).
  - **Templo-Espejo:** Fachada religiosa. La entrada al Leviatán está aquí.
  - **Sótano (subnivel):** Celdas con niños. Ambiente oscuro, húmedo. El reflejo del niño que pide ayuda está en la celda central.
  - **Cámara del Leviatán:** Sala circular enorme con espejos en las paredes y el techo. Piso reflectante.

### 2. Blueprint del Leviatán
- Crear BP_LeviathanBoss (hereda de ASOLON3_LeviathanBoss)
- Asignar `ReflejoClass` → BP_LeviathanReflejo
- Asignar `MirrorShardClass` → BP_MirrorShard
- Asignar `MirrorCoreClass` → BP_MirrorCore
- **Implementar eventos BlueprintImplementableEvent:**
  - `AttackResonanceWave` → Onda expansiva de espejo. Daña en área. VFX: anillo reflectante.
  - `AttackMirrorBeam` → Rayo láser de espejo. Daño continuo mientras apunta al jugador.
  - `AttackWaterSurge` → Oleada de agua espejada que empuja al jugador.
  - `OnMirrorDamageRedirect` → Al recibir daño en Fase 1, notificar con partículas y sonido.

### 3. Blueprint del Reflejo (BP_LeviathanReflejo)
- Hereda de ASOLON3_LeviathanReflejo
- Mesh: copia del Leviathan pero semitransparente
- Ataca con versiones más débiles de los ataques del boss
- Muere en 3 golpes

### 4. Blueprint de MirrorShard (BP_MirrorShard)
- StaticMesh: astilla de espejo puntiaguda
- Flota rotando lentamente
- 3 hits para romper (cambia material con cada hit)
- Al romperse: VFX de cristales explotando

### 5. Blueprint de MirrorCore (BP_MirrorCore)
- StaticMesh: esfera de cristal brillante
- Pulsa con luz cada heartbeat
- Cambia color cuando recibe daño

### 6. Blueprint de Garrik (BP_Garrik_Aelthar)
- Hereda de BP_Garrik_Aelthar (C++)
- AnimBlueprint para espadas gemelas
- Diálogos por Timeline:
  - `OnWelcomeDialog` → Al llegar al puente
  - `OnSuspectDialog` → En el mercado (opcional)
  - `OnTruthDialog` → Al descubrir las celdas
  - `OnFightDialog` → Antes de la pelea

### 7. NPCs y Ambientación
- **Senescal:** Elfo anciano, sonrisa amable, ropas lujosas. Da la bienvenida, ofrece tour.
- **Niños en celdas:** Modelos infantiles (escala 0.6x), sucios. No atacan. No hablan. Solo miran.
- **El Reflejo:** Punto de luz/NPC fantasmal en la celda — dice "no confíes en los espejos" cuando el jugador se acerca.
- **Ciudadanos (decoración):** Elfos que caminan. Si el jugador les habla, dan respuestas amables genéricas.

---

## Secuencia de Eventos

1. **Llegada:** Garrik recibe al jugador en el puente. "Aelthar, la ciudad espejo. Dicen que es el lugar más hermoso del reino."
2. **Exploración:** El jugador puede hablar con NPCs, comprar en mercado. Garrik se separa temporalmente.
3. **El Reflejo:** En una fuente/charco, el jugador ve el reflejo de un niño que no está allí. "Ayúdame. No son espejos. Son ventanas."
4. **Investigación:** Garrik vuelve, escéptico. Siguen pistas → encuentran entrada al sótano.
5. **Sótano:** Celdas con niños. Garrik: "Esto... esto no puede ser." Diálogo de revelación.
6. **Templo:** Entrada sellada. Garrik dice que él cubrirá la entrada (se queda atrás).
7. **Leviatán:** Fight 3 fases. Al derrotarlo:
   - Espejos del techo se rompen
   - Luz solar entra
   - Los niños... no salen corriendo. Se quedan. No saben qué es "afuera".
   - Garrik entra, ve la escena, calla.
   - **No hay final feliz.** Solo una ciudad que ya no es hermosa.

---

## Post-N3
- Las puertas de la Torre aparecen en la plaza central (una vez derrotado Leviatán).
- Garrik se queda en Aelthar para cuidar a los niños (no sigue al jugador).
- El jugador obtiene: **Fragmento de Espejo** (item clave para Ending B).

---

## Implementación en el Editor

1. Crear carpeta `Map/N3/` en Content Browser
2. Crear `L_N3_Aelthar` → Level
3. Agregar Volumes:
   - PostProcessVolume: Bloom suave, ColorGrading cálido
   - LightmassImportanceVolume
4. Colocar PlayerStart en la entrada del puente
5. Construir geometría (BSP o StaticMeshes importados)
6. Colocar BP_NPC_Senescal en la fuente
7. Colocar BP_Garrik_Aelthar (automatico al iniciar nivel)
8. Colocar BP_LeviathanBoss en la Cámara del Leviatán
9. Colocar puntos de spawn de celdas con niños
10. Configurar EventDispatcher para la secuencia de revelación
