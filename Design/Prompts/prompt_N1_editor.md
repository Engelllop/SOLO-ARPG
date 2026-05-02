# Prompt 43 — Montar N1: Velmar (La Ciudad Colgante) en el Editor

El C++ base ya está (LaVozBoss, Constants), N0 se está montando, y N2 ya tiene su prompt. Ahora toca N1 — la primera ciudad de la Torre. Un respiro después de la entrada, pero con su propio jefe y consecuencias.

---

## Nivel: `N1_Velmar.umap`

### Ubicación y Contexto

Velmar es una ciudad colgante construida en las paredes de un abismo. El grupo llega después de matar al Guardián y cargar el cuerpo de Caín. No es un nivel de combate lineal — es un hub con NPCs, secretos, compras y preparación antes del jefe.

### World Settings

- **Sky:** Cielo abierto, atardecer permanente (BaseColor #FF7744 horizonte, #DDBBAA cenit). Sin estrellas.
- **DirectionalLight:** rotación 35° altitud, color #FF9944, intensidad 0.6. Sombra suave.
- **AtmosphericFog:** Density 0.3, color #EECCAA.
- **PostProcess:** ColorGrading #FFBB99, Bloom 0.15, Saturation -10%, Temperature +5.
- **ExponentialHeightFog:** Density 0.01 en altura de ciudad, 0.08 en el borde del abismo. Color #CCBBAA.
- **Ambient Sound:** AudioVolume cubriendo toda la ciudad con viento suave + campanadas lejanas.
- **GameMode Override:** BP_SOLOGameMode (como N0/N2).
- **DefaultPawn:** BP_SOLOPlayerCharacter.
- **PlayerController:** BP_SOLOPlayerController.

### Arquitectura del Nivel

Velmar tiene **3 niveles verticales** conectados por puentes de cuerda y escaleras. El nivel es un cañón vertical — las paredes de roca son el límite natural.

#### Layout General

```
[Nivel Superior — El Arco]  ← Entrada desde N0
    │ Puente de cuerda 40m
    │
[Nivel Medio — El Mercado]
    │ Escaleras de cuerda 30m
    │
[Nivel Inferior — El Borde]
    │ ↓ Abismo (no renderizar fondo, solo niebla)
```

**Dimensiones totales:** 200m de ancho × 150m de fondo × 300m de altura (de Arco a Borde).

### Geometría Base

#### Paredes del Cañón
- Rock meshes: SM_CliffWall_Gray (color #887766, Roughness 0.8). Repetir en ambas paredes del cañón (separación 150m-200m).
- Altura total: 300m desde el borde visible hasta el fondo.

#### Plataformas/Niveles
- **Nivel Superior (cota +250m):** 80m × 40m de superficie plana. SM_Floor_StoneTile (losas 2m×2m, color #AA9988).
- **Nivel Medio (cota +180m):** 60m × 30m. SM_Floor_WoodPlank (tablones 4m×1m, color #887766).
- **Nivel Inferior (cota +100m):** 40m × 20m. SM_Floor_RockRough (piedra natural, color #665544).

#### Puentes
- **Puente Principal (Arco → Mercado):** SM_Bridge_Rope (40m × 2.5m). Tablones individuales con ligera rotación aleatoria (±2°). Algunos tablones "flojos" (TriggerVolume que hace tambalear al jugador).

### Edificios y Puntos de Interés

#### 1. La Posada del Viento Hundido (Nivel Superior)
- **Mesh:** SM_Building_Inn (8m × 10m × 6m, madera oscura #665544, techo de paja).
- **Interior:** TriggerVolume → translocar a Interior_VientoHundido (subnivel cerrado).
  - Habitaciones: 3 SM_Bed_Simple, 1 SM_Table_Round con vela.
  - Posadero: BP_Tova (ASOLOQuestGiverNPC), NPCID = "NPC.N1.Tova".
  - Diálogos: "el plato del día" (estofado curativo), ofrecer habitación (save point), información sobre la ciudad.
- **Exterior:** Fogata con BP_Campfire, SavePointID "Campfire.N1.Posada".

#### 2. La Forja de Natan (Nivel Superior)
- **Mesh:** SM_Blacksmith (6m × 6m, abierto, techo de madera).
- **NPC:** BP_Natan (ASOLOQuestGiverNPC + SOLOShopKeeper), NPCID = "NPC.N1.Natan".
  - Animación: loop de martilleo sobre SM_Anvil.
  - Inventario: espadas simples, escudos madera reforzada, flechas comunes.
  - Diálogos: lore de la Voz, información de pilares de cristal, historia personal.
  - Condición: información sobre la Voz solo si el jugador ha comprado algo antes.

#### 3. Puesto de Shae (Nivel Medio)
- **Mesh:** SM_Stall_Cloth (3m × 3m, toldo azul descolorido #4488AA).
- **NPC:** BP_Shae (ASOLOQuestGiverNPC + SOLOShopKeeper), NPCID = "NPC.N1.Shae".
  - Inventario: hierbas curativas, vendas, antídotos.
  - Diálogos:
    - **Normal:** curación, lore de plantas que no crecen donde habla la Voz.
    - **Si investigó niña desaparecida:** lore adicional sobre pesadillas compartidas.

#### 4. Taberna "El Columpio" (Nivel Medio)
- **Mesh:** SM_Tavern (8m × 8m, madera + piedra, letrero colgante).
- **Interior:** TriggerVolume → translocar a Interior_Columpio.
  - **NPC Lian (60s, ojo de vidrio):** BP_Lian_Velmar (ASOLOQuestGiverNPC), NPCID = "NPC.N1.Lian".
    - Aparece solo si el jugador está solo (check de compañeros).
    - Diálogo: "Hace veinte años...", historia del coro de voces.
    - Recompensa: pista tácita sobre cómo derrotar a la Voz.
  - **Ambientación:** 3-4 NPCs sentados en mesas (BP_AdventurerPatron), bebiendo en silencio.

#### 5. El Molino Viejo (Nivel Superior, al borde)
- **Mesh:** SM_Windmill (6m × 6m × 10m, aspas rotas). No gira.
- **Interior:** Puerta SM_Door_WoodOld, interactuable. Interior pequeño (5m × 5m).
  - Cajones sellados (3x): SM_Crate_RuneSealed. Al interactuar: reacción de runa (luz + zumbido).
  - Shae aparece corriendo si el jugador forcejea: "¡No toques eso!" (DialogueTrigger).
  - Lore: "Cosas que bajaron con nosotros. Cosas que no deberían haber subido."

#### 6. El Cementerio (al lado del molino)
- **Mesh:** SM_GraveMound (montículo de tierra, 2m × 1m). Sin lápida.
  - **Al llegar de N0:** el montículo está fresco (SM_GraveMound_Fresh). La tierra se ve removida.
  - Interacción: "Aquí yace Caín." Opción de arrodillarse o poner una piedra.
  - **Flag persistente:** Si el jugador puso una piedra, guardar en GameInstance.

#### 7. Escalera Sellada (Nivel Inferior)
- **Mesh:** SM_Staircase_Spiral (tallada en roca, 10m visibles).
- **Barras de metal:** SM_Bars_Iron (3 barras horizontales, 20cm separación).
- **Cartel oxidado:** SM_Sign_Rusty, texto: "MÁS ABAJO NO".
- **Runas de cal:** SM_Rune_Chalk (mallas planas en las barras, material blanco #EEEEEE).
- **Audio:** susurro ambiental (AudioVolume pequeño, trigger al acercarse, volumen 0.1-0.3).

### Mecánica — Puentes y Altura

Simular "altura" sin abrumar:
- **Caída:** si el jugador cae de un nivel a otro (ej: del Arco al Mercado), aplicar 30 de daño de caída y translocar al nivel inferior con animación de aterrizaje.
- **Caída al abismo:** si el jugador cae del Borde al fondo → muerte (aplicar 1000 daño). No hay fondo visible — solo niebla.
- **Puente Principal:** Timeline de oscilación al caminar (leve, 5° en Z, loop). Si corre, la oscilación aumenta (10°).

### NPCs Compañeros (en la ciudad)

- **BP_Vassen_Velmar:** ASOLOCompanionCharacter. Sigue al jugador por la ciudad. Diálogos contextuales según ubicación.
  - En posada: "Necesito una ducha. Y una botella."
  - En tejado (noche): monólogo sobre "no hay salida atrás. Solo abajo."
  - En cementerio: se arrodilla en silencio.
- **BP_Mira_Velmar:** ASOLOCompanionCharacter. A veces se separa — va al molino viejo sola.
  - Si el jugador la sigue: encuentra entrenando detrás de la forja. Sus flechas fallan más de lo normal.

### Eventos con Temporizador

Velmar tiene una línea de tiempo interna de **3 días** (no en tiempo real, sino por triggers de progreso):

#### Día 1 (al llegar)
Trigger: cruzar el puente principal.
- Natan aparece en la entrada (spawn BP_Natan en el arco).
- Diálogo automático: "Bajaron, ¿eh?"
- Grupo va al cementerio (waypoint automático opcional).
- Noche: Vassen borracho en la posada (trigger al entrar a la posada después de las 20:00).

#### Día 2
Trigger: dormir en la posada (BP_Campfire → pasar tiempo).
- Yuri (niño, BP_Yuri, NPCID="NPC.N1.Yuri") aparece en el mercado.
  - Diálogo: "Vi una sombra que habla en el nivel inferior."
  - Desaparece después del diálogo (DestroyActor).
- Shae busca ayuda: su hija desapareció. Quest activada.

#### Día 3
Trigger: dormir segunda noche.
- Si Quest "Niña Desaparecida" no completada:
  - Cuerpo de la niña aparece flotando en el Borde (BP_NinaMuerta, mesh flotante + partículas de agua).
  - Shae llora al lado (diálogo de duelo).
- Si completada:
  - Shae agradece. Diálogo expandido sobre la Voz.

### El Primer Encuentro con la Voz

**Scripted Event — "La Voz en el Mercado"**
1. El jugador está en el Mercado (Nivel Medio).
2. Trigger: zona central del mercado. Solo ocurre una vez.
3. Diálogo automático (sin UI, solo texto en pantalla + voz):
   - "Enterraste a tu amigo." (pausa 2s)
   - "Hiciste bien. No lo dejaste solo."
   - "Baja a verme. Trae a los otros."
4. Después: opción de mencionarlo a Vassen/Mira (Diálogo branching).
5. Flag: "MetVoice" = true en GameInstance.

### El Descenso al Abismo (Transición a Combate)

**Secuencia:**
1. El jugador llega al Nivel Inferior.
2. Guardia (BP_GuardiaVelmar) abre las barras: "Por su cuenta y riesgo."
3. Escalera de caracol baja 300 escalones (representados como trigger de viaje rápido + cinemática).
4. Secuencia de escalada: 3 zonas de rappel. En cada zona:
   - Zona 1 (50m): Cueva con campamento viejo (BP_CampViejo, mesh SM_Tent_Ruined + SM_Bone_Human).
   - Zona 2 (100m): Cueva con nombres grabados (SM_Wall_Names, 9 nombres tachados, 1 sin tachar: "Lian").
   - Zona 3 (150m): Pared lisa. Lámpara parpadea (Timeline de intensidad de luz, parpadeo aleatorio).
5. Fondo: Sala de Cristal (translocar a subnivel Interiores/N1_SalaVoz).

### El Combate — La Voz de Velmar

Crear **BP_VozDeVelmar** (padre: ASOLOEnemyCharacter + SOLOBossController + SOLOBossPhaseComponent).

#### Escenario: Sala de Cristal
- SM_Floor_Crystal (50m diámetro, material traslúcido #AADDFF, Roughness 0.1, Emissive 0.3).
- Bajo el cristal: agua negra (SM_Water_Plane negro con material de ondas lentas).
- **6 Pilares de Cristal:** SM_CrystalPillar (2m alto × 0.5m diámetro). Formación hexagonal, radio 15m. Material emisivo azul #4488FF.
  - HP: 3 hits cada uno.
  - Al romperse: partículas de cristal (NS_CrystalShard).
- **Árbol de Cristal Central:** SM_CrystalTree (6m alto). Material emisivo azul más intenso #66AAFF.
  - Se rompe al final del combate (trigger de transición a Fase 3).

#### Fase 1 — Susurros
- **Mecánica:** No hay enemigo visible.
- **Mecánica "Eco":** Cuando el jugador se mueve 3m lineal, spawn BP_EchoShadow en su posición anterior. El eco ataca después de 2s (15 daño).
  - Timer de 0.5s entre spawns de eco.
- **Mecánica "Susurro":** Cada 30s, un personaje aleatorio (incluyendo al jugador) recibe tag "Charmed".
  - Efecto: caminar hacia el centro a 100cm/s durante 3s.
  - Romper: golpear al personaje (1 daño) o usar vendas en los oídos (item).
- **Objetivo:** Romper 4 pilares (de 6).
  - Cada pilar requiere 3 hits de cualquier arma.

#### Fase 2 — La Grieta
- Trigger: 4 pilares rotos.
- **Mecánica "Brazos de Agua":** Spawn 3 BP_WaterArm en posiciones aleatorias cerca del jugador.
  - Atape tobillo: ralentiza 50%, 5 HP/segundo.
  - Romper: 1 hit (muy frágil).
- **Mecánica "Tono Doloroso":** Los 2 pilares restantes emiten daño continuo en radio 5m (3 HP/s).
  - Prioridad: romper estos pilares rápido.
- El agua sube 10cm (animación del mesh SM_Water_Plane).

#### Fase 3 — La Caída
- Trigger: último pilar roto.
- Árbol de cristal se rompe: onda expansiva (RadialForce, empuja 500 unidades).
- Emerge **BP_VozManifestacion** del agua:
  - Mesh: humanoide 2m, SM_Humanoid_WaterNegro (material negro #111122, Emissive #4455FF en ojos).
  - No tiene HP regenerativo. Lo que recibe, lo absorbe.
  - **Punto débil:** Reflejo en el agua (BP_VozReflejo). El reflejo tiene HP real (3 hits).
    - Cuando el reflejo recibe daño → la manifestación se solidifica 5s → recibe daño real en ese período.
  - **Ataques:** Brazada de agua (proyectil 15 daño, ralentiza 30%), Grito (AOE 8m, stun 1s).
- Fin: reflejo destruido → manifestación se deshace en agua → luz se apaga → diálogo final de la Voz.

### Post-Combate

1. Luz azul se apaga gradualmente (Timeline en Level Blueprint, 3s).
2. Audio: silencio absoluto 3s, luego respiración del grupo.
3. Diálogo automático Voz: "Pasaron. El piso de abajo los espera."
4. Hueco en el centro del suelo: SM_Hatch_CrystalOpen (animación slide).
5. Escalones de cristal hacia abajo (SM_Stair_Crystal, 50 escalones).
6. Al final: puerta de piedra blanca (SM_Door_WhiteStone). Al interactuar: transición a N2.

### Items y Recompensas

| Item | Tipo | Rareza | Efecto |
|------|------|--------|--------|
| Fragmento de la Voz | Accesorio | Poco Común | Resistencia daño mental +10% |
| Colgante de Cristal | Item quest | — | Para forjar en N3 |
| Vendas de Shae | Consumible | Común | Cura 25 HP + rompe susurro |
| Estofado de Tova | Consumible | Común | Cura 40 HP, +10% stamina regen 60s |

### Quests Data Assets (USOLOQuestDataAsset)

1. **Quest.N1.Descender** — Principal. Objetivos: Llegar al fondo (1), Derrotar a la Voz (1), Cruzar a N2 (1). Reward: 200 XP, 100 oro.

2. **Quest.N1.NinaDesaparecida** — Opcional. Objetivo: Investigar desaparición de la niña (encontrar bufanda en el Borde). Reward: 75 XP, información expandida de Shae sobre la Voz. Penalización si falla: niña muere, Shae hostil.

---

### Resumen de archivos a crear:
```
Content/SOLO/Levels/N1_Velmar.umap
Content/SOLO/Levels/Interiores/N1_VientoHundido.umap
Content/SOLO/Levels/Interiores/N1_Columpio.umap
Content/SOLO/Levels/Interiores/N1_SalaVoz.umap
Content/SOLO/Blueprints/N1/BP_Tova.uasset
Content/SOLO/Blueprints/N1/BP_Natan.uasset
Content/SOLO/Blueprints/N1/BP_Shae.uasset
Content/SOLO/Blueprints/N1/BP_Lian_Velmar.uasset
Content/SOLO/Blueprints/N1/BP_Yuri.uasset
Content/SOLO/Blueprints/N1/BP_Vassen_Velmar.uasset
Content/SOLO/Blueprints/N1/BP_Mira_Velmar.uasset
Content/SOLO/Blueprints/N1/BP_VozDeVelmar.uasset
Content/SOLO/Blueprints/N1/BP_VozManifestacion.uasset
Content/SOLO/Blueprints/N1/BP_VozReflejo.uasset
Content/SOLO/Blueprints/N1/BP_EchoShadow.uasset
Content/SOLO/Blueprints/N1/BP_WaterArm.uasset
Content/SOLO/Blueprints/N1/BP_GuardiaVelmar.uasset
Content/SOLO/Blueprints/N1/BP_NinaMuerta.uasset
Content/SOLO/Materials/MI_Crystal_Blue.uasset
Content/SOLO/Materials/MI_Crystal_Tree.uasset
Content/SOLO/Materials/MI_Water_Black.uasset
Content/SOLO/Materials/MI_WaterManifest.uasset
Content/SOLO/Particles/NS_CrystalShard.uasset
Content/SOLO/DataAssets/DT_N1_Quests.uasset
Content/SOLO/DataAssets/DT_N1_Dialogues.uasset
Content/SOLO/DataAssets/DT_N1_Loot.uasset
```

Prioridad: geometría de la ciudad primero, después NPCs y diálogos, después secuencia de descenso, después combate de la Voz.

---

## NOTAS DE IMPLEMENTACIÓN (C++ ya generado)

### A. Inconsistencias corregidas respecto al código existente

1. **SOLON1_Constants.h actualizado**: Los IDs anteriores (NPC_Anselmo, NPC_Lira, QUEST_LuzSantuario, ITEM_CartaLira, etc.) fueron reemplazados por los del prompt (NPC_Tova, NPC_Natan, NPC_Shae, QUEST_Descender, QUEST_NinaDesaparecida, ITEM_FragmentoVoz, etc.).

2. **Boss completamente rediseñado**: El ASOLON1_LaVozBoss anterior tenía mecánicas distintas (Phase0: AlmaErrante waves + invisible, Phase1: materialize + light rays + tracking orbs, Phase2: condense + shockwaves). Fue reescrito para usar el sistema de 3 fases basado en pilares del prompt.

3. **Fases NO basadas en HP**: El sistema base de ASOLOEnemyCharacter usa `PhaseThresholds` (umbrales de HP%). En LaVozBoss, `PhaseThresholds` se deja vacío — las transiciones son gateadas por destrucción de pilares (4/6 → Phase 2, 6/6 → Phase 3).

4. **Story Flags**: USOLOGameInstance NO tenía sistema de flags persistentes. Se añadió `TMap<FName, bool> StoryFlags` con `SetStoryFlag()`, `HasStoryFlag()`, y `ClearAllStoryFlags()`. Las flags de N1 están en SOLON1_Constants.h.

### B. NPCs que son QuestGiver + ShopKeeper

ASOLOQuestGiverNPC y ASOLOShopNPC son actores separados (ambos extienden AActor). No se puede heredar de ambos. **Solución recomendada para BP_Natan y BP_Shae:**

1. Crear el Blueprint como hijo de `ASOLOQuestGiverNPC`.
2. Añadir un `USOLOMarketplaceComponent` al Blueprint (Add Component).
3. En `Interact_Implementation` del BP: verificar si hay quests pendientes → abrir diálogo de quest; si no → abrir tienda (broadcast `OnShopOpened`).

### C. Companion Character

La clase base es `ASOLOCompanionCharacter` ubicada en `Source/SOLOCore/Public/AI/SOLOCompanionCharacter.h` (NO en `Character/`). BP_Vassen_Velmar y BP_Mira_Velmar deben heredar de esta clase.

### D. Archivos C++ nuevos creados

```
Source/SOLOCore/Public/Data/N1/SOLON1_Constants.h                — ACTUALIZADO
Source/SOLOCore/Public/GameFramework/SOLOGameInstance.h           — ACTUALIZADO (+ StoryFlags)
Source/SOLOCore/Private/GameFramework/SOLOGameInstance.cpp        — ACTUALIZADO (+ StoryFlags)

Source/SOLOCore/Public/World/N1/ASOLON1_EchoShadow.h             — NUEVO
Source/SOLOCore/Private/World/N1/ASOLON1_EchoShadow.cpp          — NUEVO

Source/SOLOCore/Public/World/N1/ASOLON1_WaterArm.h               — NUEVO
Source/SOLOCore/Private/World/N1/ASOLON1_WaterArm.cpp            — NUEVO

Source/SOLOCore/Public/World/N1/ASOLON1_CrystalPillar.h          — NUEVO
Source/SOLOCore/Private/World/N1/ASOLON1_CrystalPillar.cpp       — NUEVO

Source/SOLOCore/Public/World/N1/ASOLON1_VelmarDayComponent.h     — NUEVO
Source/SOLOCore/Private/World/N1/ASOLON1_VelmarDayComponent.cpp  — NUEVO

Source/SOLOCore/Public/World/N1/ASOLON1_VoiceEventActor.h        — NUEVO
Source/SOLOCore/Private/World/N1/ASOLON1_VoiceEventActor.cpp     — NUEVO

Source/SOLOCore/Public/Character/N1/ASOLON1_VozManifestacion.h   — NUEVO
Source/SOLOCore/Private/Character/N1/ASOLON1_VozManifestacion.cpp — NUEVO

Source/SOLOCore/Public/Character/N1/ASOLON1_VozReflejo.h         — NUEVO
Source/SOLOCore/Private/Character/N1/ASOLON1_VozReflejo.cpp      — NUEVO

Source/SOLOCore/Public/Character/N1/ASOLON1_LaVozBoss.h          — REESCRITO
Source/SOLOCore/Private/Character/N1/ASOLON1_LaVozBoss.cpp       — REESCRITO
```

### E. Qué falta hacer en Blueprint / Editor (NO tiene C++)

1. **GE_CharmWhisper** (GameplayEffect): Duration 3s, aplica tag `Status.Charmed`, fuerza movimiento hacia el centro a 100cm/s. Se rompe con 1 daño o usando Vendas de Shae.
2. **GE_EchoDamage**: Instant, 15 daño base.
3. **GE_WaterArmSlow**: Duration Infinite (removido al romper), -50% MoveSpeed.
4. **GE_WaterArmDot**: Periodic 1s, 5 daño por tick.
5. **GE_PillarAOE**: Instant, 3 daño base (aplicado cada 1s por C++ tick).
6. **GE_VozInvulnerable**: Infinite Duration, tag `Status.Invulnerable`.
7. **GE_LaVozInvisible**: Infinite Duration, tag `Status.Invisible` + `Status.Untargetable`.
8. **NS_CrystalShard**: Niagara System — partículas de fragmentos de cristal al romper pilares.
9. **BP_NinaMuerta**: Actor simple con mesh flotante + partículas de agua (no necesita C++).
10. **BP_GuardiaVelmar**: ASOLOQuestGiverNPC con diálogo que abre las barras.
11. **BP_CampViejo, BP_CampNombres**: Actores decorativos para las zonas del descenso.
12. **Animaciones y VFX**: Todos los BlueprintImplementableEvents (BP_OnEchoSpawned, BP_OnGrab, BP_OnHit, BP_OnVulnerableStart, etc.) deben implementarse en los Blueprints hijos.

### F. Notas de balance y gameplay

- **EchoShadow spawn**: Solo spawn si el jugador se mueve (velocity > 50 u/s). Cooldown 0.5s. Esto penaliza el movimiento constante y premia la observación.
- **Whisper/Charm**: Cada 30s, target aleatorio. Romper con 1 daño es intencional — los compañeros AI deberían poder romperlo si detectan el tag.
- **Crystal Pillars**: 3 hits cada uno. Las armas del jugador deben funcionar (cualquier TakeDamage > 0 cuenta como 1 hit). No se distingue entre tipos de arma.
- **VozManifestacion**: Es invulnerable excepto durante la ventana de 5s tras golpear al Reflejo. El Reflejo requiere 3 hits para destruirse — cada hit abre una ventana. Al tercer hit, el Reflejo se destruye y la Manifestación muere automáticamente.
- **Water Arms en Phase 2**: Solo 3, muy frágiles (1 hit = break). Son un slow/DoT que obliga al jugador a reaccionar antes de focusear los pilares restantes.
