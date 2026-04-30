# Prompt 42 — Montar N2: El Pastor de Cenizas en el Editor

El C++ base ya está completo, los niveles N0 (Emberveil) y N1 (Velmar) se están montando en el editor. Ahora toca N2 — las Llanuras Quemadas.

---

## Nivel: `N2_PastorDeCenizas.umap`

### World Settings

- No Landscape — usar BSP geometry modular + meshes de roca calcinada
- Sky: SustainLevelSky o SkySphere con material de ceniza (BaseColor #666666, sin estrellas, sin sol) + fog de altura (Height 80m, Density 0.03)
- DirectionalLight: rotación 0° (sin luz direccional fuerte). Luz ambiental #667788, intensidad 0.3
- PostProcess: ColorGrading #BB9988 (tinte naranja/gris), Bloom 0.2, AmbientCubemap de "atardecer carbonizado"
- ExponentialHeightFog: en todo el mapa, Density 0.05, color ceniza #888877
- GameMode Override: BP_SOLOGameMode (como N0/N1)
- DefaultPawn: BP_SOLOPlayerCharacter
- PlayerController: BP_SOLOPlayerController

### Mecánica — Medidor de Calor

El N2 tiene un **medidor de calor** de 5 segmentos (implementado via GameplayEffect):

Crear en Data Assets o Blueprint:
- `GE_HeatExposure`: Infinite GE, aplica tag `Status.Heat.Segment1` cada 12s si el jugador está al aire libre. Stacking: 5 stacks.
- `GE_Segment1`: -10% stamina regen. `GE_Segment2`: visibilidad reducida (PostProcess de calor). `GE_Segment3`: +25% fire damage received. `GE_Segment4`: jadeo audible (AI detection range +50%). `GE_Segment5`: -5 HP/s + alucinaciones visuales (partículas distorsión).

**Refugios térmicos** (7 total): Volúmenes que aplican `GE_HeatShelter` (stack inverso, reduce medidor 2/stacks cada entrada) mientras el jugador está dentro. Marcar con un ParticleSystem de vapor frío azul.

**Actors helper:**
- `BP_HeatZone`: Box collider, aplica GE_HeatExposure más rápido si el jugador overlap. Poner sobre suelo agrietado, charcos de brea, etc.
- `BP_HeatShelter`: Box collider, aplica GE_HeatShelter si el jugador overlap. Poner en cuevas, ruinas de piedra blanca. Emitir partículas de vapor.

### Zonas del bioma

#### 1. Llanuras Quemadas (800m × 600m, área principal)

Ground: SM_Floor_Cinder (losas de ceniza, 4m x 4m, color negro-gris, Roughness 0.9). Repetir con noise variation.

**Geometry:**
- Árboles petrificados: SM_TreeCharred (variante alta 15m, baja 8m). 50-60 instancias distribuidas aleatoriamente con Foliage tool.
- Ruinas de piedra: SM_RuinPillar (2m × 2m × 4m, caído o vertical). 10-15 en el mapa.
- Refugios térmicos: 7 cuevas pequeñas (SM_CaveEntrance 3m × 3m, altura 2.5m)

**Foliage:**
- Ceniza suelta: partículas de ceniza volando (NiagaraSystem, density 50, drift wind)
- Pasto muerto: SM_DeadGrass (density 0.03), color #554433

**Audio:**
- AudioVolume cubriendo toda la llanura: wind_ambient.wav, low_bass_hum.wav
- Silencio absoluto entre ráfagas de viento

**Enemy spawners (ASOLOEnemySpawner):**
- 10 puntos de Perros de Ceniza (manadas 3-5). EnemyClass: BP_PerroCeniza
- 5 puntos de Espectros de Calor. EnemyClass: BP_EspectroCalor
- 1 spawner de Gusanos de Brasa (en la Garganta Humeante, único)

#### 2. El Bosque Petrificado (200m × 200m, noreste)

- Árboles de carbón más denso: foliage density 0.08, SM_TreeCharred
- Sombra proyectada: reduce heat buildup en 50% (poner BP_HeatShelter de menor efecto)
- Suelo: SM_Floor_Petrified (losas grises, Roughness 0.6)

**Evento — Hombre Atrapado:**
StaticMesh de un hombre (SM_HumanMale_Trapped) incrustado en SM_TreePetrified. Su mitad inferior es piedra.

**Opciones de interacción (Level Blueprint + Dialogue):**
1. **Rescatar:** Interactuar 30s → 3 oleadas de Perros de Ceniza → hombre cae → pista del núcleo del Pastor
2. **Ignorar:** Nada
3. **Matar:** Golpe rápido → item Fragmento de Piedra Humana → Mira horrorizada

#### 3. Campamento de Lian (30m × 30m, centro-oeste)

- Tienda de lona: SM_Tent_Ragged (3m × 4m)
- Fogata central encendida: BP_Campfire con SavePointID "Campfire.N2.Lian"
- Puesto de venta: SM_CrateStack (3 frascos con líquido, animación de partículas)
- NPC Lian: BP_Lian (ASOLOQuestGiverNPC), NPCID = "NPC.N2.Lian"

**Diálogos:**
Node Start: "Velmar te dejó salir vivo. Eso ya es más de lo que logró la mayoría."
Node HeatAdvice: "La Torre no quiere que mueras de calor. Quiere que el calor te desespere." → Explica sistema de 5 segmentos.
Node PerroAdvice: "Se regeneran porque la ceniza es su madre. Si les quitas la ceniza — agua, hielo — dejan de levantarse."
Node PastorAdvice: "No es un monstruo. Es un ritual. El Pastor es quien quedó a cargo de quemar lo que la Torre ya no necesita."

#### 4. La Garganta Humeante (grieta de 40m × 200m, atraviesa el mapa E-W)

- Paredes de roca: SM_CliffWall_Basalt (color negro, Roughness 0.9)
- Fondo: resplandor rojizo, punto light rojo #FF4400, intensidad 200
- Puentes: 3 SM_Bridge_Stone (8m × 3m). 2 estables, 1 inestable (oscila con Timeline)
- Caer: si el jugador toca el TriggerVolume en el fondo → muerte por calor (aplicar GE de 1000 daño de fuego)

**Gusano de Brasa (minijefe):**
BP_Worm_Brasa: ASOLOEnemyCharacter. Tamaño 8m × 1m. Mesh: cilindro segmentado con material de brasa (Emissive #FF6600). Animación: abrir boca (atacar) / cerrar boca (defensa).
- Ataque: escupir brasa (proyectil, 15 daño, aplica tag Heat.Segment1)
- Vulnerable solo cuando abre la boca (1.5s ventana)
- Recompensa: Colmillo de Brasa (item material)

#### 5. El Nido del Pastor (100m × 100m, extremo norte)

- Depresión circular: 100m diámetro, -2m de altura. Paredes de piedra negra lisa (SM_Wall_Obsidian, Roughness 0.95)
- Suelo: SM_Floor_Obsidian, Emissive débil #332211
- Ceniza humeante: partículas de vapor caliente en bordes
- Cofre de hierro antes de la entrada (SM_ChestIron, desbloqueado al derrotar al Pastor)

### Jefe — El Pastor de Cenizas

Crear **BP_PastorDeCenizas** (padre: ASOLOEnemyCharacter + SOLOBossController + SOLOBossPhaseComponent).

**Modelo:**
- Mesh: humanoide 2.5m, túnica de ceniza (material translúcido #888888, Roughness 0.3, Emissive #FF4400 débil)
- Báculo: SM_Staff_Burned, 2m largo
- Sin rostro visible — solo capucha con dos puntos de luz naranja

**Phase Thresholds:**
- Fase 1: HP 100%-67%. Ataques: Estallido de Ceniza (AOE 5m, 15 daño), Brazos de Humo (summon 2 brazos de humo que persiguen), Marcha Lenta (se mueve hacia jugador a 100cm/s)
- Fase 2: HP 66%-34%. Ataques: Columnas de Fuego (AOE aleatorio, 1s warning), Ráfaga de Viento (knockback 10m), Cuchillas de Ceniza (projectiles en abanico, 3), Perros de Ceniza Fantasma (summon 2 que atacan y desaparecen en 8s)
- Fase 3: HP 33%-0%. El Pastor se desintegra. Núcleo Expuesto: esfera 1.5m diámetro, material brillante blanco (#FFFFFF Emissive máx). Mecánica: solo 30s para matarlo. Punto débil: polo sur del núcleo (3x daño si el jugador rescató al Hombre Atrapado y recibió la pista).

**Diálogo jefe:** El Pastor NUNCA habla. Solo sonidos: crujido de ceniza, aliento pesado.

**Post-pelea:**
1. El cielo se aclara gradualmente (interpolar de #888877 a #AAAAAA en 3s usando Timeline en Level Blueprint)
2. El calor se disipa (remover GE_HeatExposure de todos los actores)
3. El Nido se enfría
4. Lian aparece en la entrada del Nido: "Lo hiciste. La llanura se enfría. Ahora puedes bajar."
5. Aparece cofre de hierro: Túnica de Ceniza Inerte, Núcleo de Fuego Blanco, Fragmento del Báculo, Llave del Túnel
6. Puerta de túnel a N3 se abre (SM_Door_Crystal, animación slide up, al este del Nido)

### Fogatas

1. **Campfire.N2.Lian** — en el campamento de Lian, activa
2. **Campfire.N2.RefugioCentral** — en un refugio térmico en el centro de la llanura, activa al llegar
3. **Campfire.N2.Nido** — antes del Nido del Pastor, se activa al acercarse

### Quests Data Assets (USOLOQuestDataAsset)

1. **Quest.N2.DerrotarPastor** — Principal. Objectives: Kill Pastor (1), Reach Túnel N3 (1). Reward: 150 XP, 50 oro. Prerrequisito: llegar a N2.

2. **Quest.N2.LianSuministros** — Opcional. Objetivo: matar 5 Perros de Ceniza + recolectar 3 Fragmentos de Brasa. Reward: 50 XP, Cántaro de Agua Fría.

3. **Quest.N2.HombreAtrapado** — Opcional. Objetivo: rescatar o matar al Hombre Atrapado en Bosque Petrificado. Rewards condicionales.

### Items

- Túnica de Ceniza Inerte (armadura, Rareza Poco Común, +20% resistencia a fuego)
- Núcleo de Fuego Blanco (item de quest, para forjar en N3)
- Fragmento del Báculo (item decorativo / lore)
- Cántaro de Agua Fría (consumible, reduce 3 segmentos de calor)
- Colmillo de Brasa (material de mejora, usado en N5)
- Fragmento de Piedra Humana (item raro, si mató al hombre)
- Llave del Túnel (abre puerta a N3)

---

**Resumen de archivos:**
```
Content/SOLO/Levels/N2_PastorDeCenizas.umap
Content/SOLO/Blueprints/BP_HeatZone.uasset
Content/SOLO/Blueprints/BP_HeatShelter.uasset
Content/SOLO/Blueprints/BP_PerroCeniza.uasset
Content/SOLO/Blueprints/BP_EspectroCalor.uasset
Content/SOLO/Blueprints/BP_Worm_Brasa.uasset
Content/SOLO/Blueprints/BP_PastorDeCenizas.uasset
Content/SOLO/Blueprints/BP_Lian.uasset
Content/SOLO/GameplayEffects/GE_HeatExposure.uasset
Content/SOLO/GameplayEffects/GE_Segment1-5.uasset
Content/SOLO/GameplayEffects/GE_HeatShelter.uasset
Content/SOLO/DataAssets/DT_N2_Quests.uasset
Content/SOLO/DataAssets/DT_N2_Loot.uasset
Content/SOLO/DataAssets/DT_N2_Dialogues.uasset
Content/SOLO/Materials/MI_N2_Cinder.uasset
Content/SOLO/Materials/MI_N2_Obsidian.uasset
Content/SOLO/Particles/NS_CinderDrift.uasset
Content/SOLO/Particles/NS_HeatDistortion.uasset
```

Prioridad: bioma open world primero, después el sistema de calor, después enemigos, después jefe.
