# Sistema de Guardado y Fogatas

> **SOLO (Sunlight of the Last Online)** — RPG medieval oscuro en Unreal Engine 5.7
> **Versión del documento:** 1.0
> **Última actualización:** 2026-04-29

---

## Tabla de Contenidos

1. [Fogatas — Puntos de Control](#1-fogatas--puntos-de-control)
2. [Sistema de Guardado](#2-sistema-de-guardado)
3. [Muerte y Respawn](#3-muerte-y-respawn)
4. [Implementación Técnica](#4-implementación-técnica)
5. [Prompt para Claude Code](#5-prompt-para-claude-code)

---

## 1. Fogatas — Puntos de Control

### Diseño Visual

| Elemento | Descripción |
|----------|-------------|
| **Base** | Círculo de piedras irregulares (variedad de 3–5 meshes) con leña carbonizada |
| **Fuego apagado** | Cenizas frías, madera negra, sin partículas, luz tenue azul-grisácea |
| **Fuego encendido** | Partículas Niagara naranja/rojo, humo gris ascendente, chispas |
| **Iluminación** | Point light naranja cálido, intensidad 8, radio 600uu, parpadeo suave, luz volumétrica |
| **Visibilidad lejana** | Visible desde 10,000uu. Humo billboarded con LOD. Halo naranja en horizonte |
| **Sonido** | Fuego crepitante 3D (loop) + viento suave, attenuación 2000uu |
| **Música** | Crossfade 3s a tema calmado (cuerdas, 40 BPM) al sentarse. Transición inversa al levantarse |
| **Prompt** | Widget "Presiona [F] para descansar" con fading. Aparece solo fuera de combate y sin enemigos en radio 3000uu |

**Implementación:** UNiagaraComponent (no Cascade), LODs de partículas (500/200/50), USoundConcurrency.

### Mecánica de Uso

1. **Activación inicial:** Presionar F → fogata se enciende (transición 2s) → checkpoint activado → auto-save.
2. **Descanso:** Presionar F en fogata activa → animación de sentarse → curación HP/MP/SP en 3s → eliminar estados negativos → Menú de Descanso (equipo, habilidades solo consulta, diario, mapa, visiones del Tejido si aplica).
3. **Post-descanso:** Enemigos del piso se reagrupan. Cofres y loot NO se regeneran. Auto-save.

### Límites

| Restricción | Condición | Comportamiento |
|-------------|-----------|----------------|
| Combate | Jugador tiene aggro | No aparece prompt. Menú se cierra si se abre estando en combate |
| Enemigos cercanos | Enemigos vivos en radio 3000uu | Prompt muestra "No puedes descansar con enemigos cerca" |
| Descansando | En animación | Input bloqueado hasta levantarse |
| N10 | Estás en El Vacío | No hay fogatas |
| Ataque a fogata | N6/N7: enemigos específicos pueden apagar la fogata | Fogata se apaga. Reavivar requiere canalización 5s. Si estaba descansando, interrupción violenta |

### Fogatas por Piso

| Piso | Nombre | Cant. | Ubicaciones |
|------|--------|-------|-------------|
| N0 | El Bosque Hundido | 5 | Entrada, Claro del Ciervo, Ruinas del Puente, Cabaña del Leñador, Garganta del Errante |
| N1 | Velmar | 3 | Entrada, Mercado de Sombras, Santuario de Velmar |
| N2 | Las Criptas | 3 | Campamento de Lian, Refugio Central, Antes del Nido |
| N3 | Los Barrios | 2 | Entrada, Zona Residencial |
| N4 | Alcantarillado | 2 | Inicio, Antes del Guardián |
| N5 | El Patíbulo | 2 | Plaza de los Susurros, Cuartel de Thornwall |
| N6 | El Coro Susurrante | 2 | Entrada del Coro, Antes del Coro de Jade |
| N7 | El Gran Bazar | 2 | Bazar, Entrada al Templo Olvidado |
| N8 | Jardines Colgantes | 3 | Llegada, Jardines Bajos, Jardines Altos |
| N9 | Ciudadela de Argentia | 2 | Plaza, Bastión de Argentia |
| N10 | El Vacío | 0 | Sin fogatas — no hay vuelta atrás |
| **Total** | | **26** | |

### Fogatas Especiales — Visiones del Tejido

| Piso | Fogata | Visión |
|------|--------|--------|
| N0 | Garganta del Errante | Caín niño riendo bajo un sol real |
| N1 | Santuario de Velmar | Caín siendo enterrado |
| N2 | Campamento de Lian | El sol real (recuerdo ajeno) |
| N3 | Plataforma de Hadda | Todas las estrofas de la canción |
| N5 | Cuartel de Thornwall | Primera vez que entraste a la Torre (recuerdo de Emberveil) |
| N6 | Antes del Coro de Jade | Mira y Vassen vivas (futuro o recuerdo) |
| N8 | Jardines Altos | Origen de Sera en el Tejido |
| N9 | Bastión de Argentia | El Arquitecto (cara borrosa) |

Cada visión es una ULevelSequence opcional. Al completarla se desbloquea una entrada de lore.

### Diagrama — Interacción con Fogata

```
Acercarse a fogata
  │
  ├─ ¿Activada? ── NO ── ¿En combate/enemigos cerca?
  │                          ├─ Sí → Prompt: "No puedes descansar con enemigos cerca"
  │                          └─ No → Prompt: "Presiona F para descansar"
  │                                     │
  │                                     └─ [F] presionado → ACTIVAR FOGATA
  │                                                        │
  │                                                        └→ DESCANSAR
  │
  └─ Sí → Prompt: "Presiona F para descansar"
           │
           └─ [F] presionado → DESCANSAR
                                 │
                                 ├─ Curar HP/MP/SP al 100%
                                 ├─ Limpiar estados negativos
                                 ├─ Abrir Menú de Descanso
                                 │    ├─ Equipo (consulta)
                                 │    ├─ Habilidades (consulta)
                                 │    ├─ Diario
                                 │    ├─ Mapa
                                 │    ├─ Visión del Tejido (si aplica)
                                 │    └─ Levantarse
                                 │
                                 └─ [Levantarse]
                                      ├─ Reagrupar enemigos del piso
                                      ├─ Auto-save
                                      └─ Cerrar menú
```


## 2. Sistema de Guardado

### Estructura de Save Data

Las siguientes estructuras C++ definen el contrato de datos del sistema de guardado:

```cpp
// SOLOSaveTypes.h

USTRUCT(BlueprintType)
struct FCompanionSaveData
{
    GENERATED_BODY()
    UPROPERTY() FString CompanionID;         // "MIRA", "VASSEN", "GARRIK", "SERA"
    UPROPERTY() bool bIsAlive = true;
    UPROPERTY() int32 CurrentHP;
    UPROPERTY() int32 MaxHP;
    UPROPERTY() int32 CurrentMP;
    UPROPERTY() int32 MaxMP;
    UPROPERTY() TArray<int32> EquippedItems;
    UPROPERTY() TArray<int32> UnlockedAbilities;
    UPROPERTY() FVector CompanionPosition;
    UPROPERTY() float AffectionLevel;         // 0.0 - 1.0
    UPROPERTY() TArray<FString> UnlockedDialogues;
};

USTRUCT(BlueprintType)
struct FQuestSaveData
{
    GENERATED_BODY()
    UPROPERTY() FString QuestID;              // "QUEST_MAIN_01"
    UPROPERTY() FString QuestName;
    UPROPERTY() int32 CurrentStage;
    UPROPERTY() TArray<bool> ObjectiveStates;
    UPROPERTY() bool bIsMainQuest = false;
    UPROPERTY() bool bIsCompleted = false;
};

USTRUCT(BlueprintType)
struct FQuestObjectiveProgress
{
    GENERATED_BODY()
    UPROPERTY() FString QuestID;
    UPROPERTY() FString ObjectiveID;
    UPROPERTY() int32 CurrentCount;
    UPROPERTY() int32 RequiredCount;
};

USTRUCT(BlueprintType)
struct FItemSaveData
{
    GENERATED_BODY()
    UPROPERTY() int32 ItemID;
    UPROPERTY() FString ItemInstanceID;        // GUID para ítems únicos
    UPROPERTY() int32 Quantity;
    UPROPERTY() bool bIsEquipped = false;
    UPROPERTY() int32 SlotIndex;               // -1 si no equipado
    UPROPERTY() float Durability;              // 1.0 nuevo, 0.0 roto
    UPROPERTY() TArray<FGameplayTag> Modifiers;
};

USTRUCT(BlueprintType)
struct FSOLOSaveData
{
    GENERATED_BODY()

    // Meta
    UPROPERTY() FString SaveVersion;           // "1.0"
    UPROPERTY() FDateTime SaveTimestamp;
    UPROPERTY() FString SaveLocationLabel;

    // Jugador
    UPROPERTY() FString PlayerName;
    UPROPERTY() int32 Level = 1;
    UPROPERTY() int32 XP = 0;
    UPROPERTY() int32 ClassID = 0;
    UPROPERTY() TArray<int32> UnlockedSkills;
    UPROPERTY() TArray<int32> EquippedItems;

    // Atributos
    UPROPERTY() int32 HP = 100, MaxHP = 100;
    UPROPERTY() int32 MP = 50, MaxMP = 50;
    UPROPERTY() int32 SP = 80, MaxSP = 80;
    UPROPERTY() TArray<FGameplayTag> ActiveBuffs;
    UPROPERTY() TArray<FGameplayTag> PermanentFlags;

    // Progresion
    UPROPERTY() int32 CurrentFloor = 0;
    UPROPERTY() FString CurrentLevel;
    UPROPERTY() FVector PlayerPosition;
    UPROPERTY() FRotator PlayerRotation;
    UPROPERTY() int32 Gold = 0;

    // Companeros
    UPROPERTY() TArray<FCompanionSaveData> Companions;

    // Quests
    UPROPERTY() TArray<FQuestSaveData> ActiveQuests;
    UPROPERTY() TArray<FString> CompletedQuests;
    UPROPERTY() TArray<FQuestObjectiveProgress> QuestProgress;

    // Flags narrativos
    UPROPERTY() bool bCainDead = false;
    UPROPERTY() bool bMiraDead = false;
    UPROPERTY() bool bVassenDead = false;
    UPROPERTY() bool bGarrikJoined = false;
    UPROPERTY() bool bSeraJoined = false;
    UPROPERTY() bool bFragmentoArquitecto = false;
    UPROPERTY() bool bFinalBUnlocked = false;

    // Reputacion
    UPROPERTY() TMap<FString, int32> Reputation;

    // Inventario
    UPROPERTY() TArray<FItemSaveData> Inventory;

    // Fogatas
    UPROPERTY() TArray<FString> ActivatedBonfires;
    UPROPERTY() FString LastBonfireID;

    // Diario / Lore
    UPROPERTY() TArray<FString> UnlockedLoreEntries;
    UPROPERTY() TArray<FString> ReadBooks;

    // Tiempo
    UPROPERTY() int32 TotalPlayTime = 0;       // segundos
    UPROPERTY() int32 InGameDay = 1;
    UPROPERTY() int32 InGameHour = 6;

    // Eventos de mundo
    UPROPERTY() TMap<FString, float> EventCooldowns;

    // Muerte
    UPROPERTY() int32 DeathsSinceLastBonfire = 0;
};

UCLASS()
class SOLO_API USOLOSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY() FSOLOSaveData SaveData;
};
```

### Comportamiento del Save

| Trigger | Momento | Acción |
|---------|---------|--------|
| Descanso en fogata | Al completar curación, antes del menú | Save checkpoint completo |
| Cambio de piso | Trigger de transición | Save silencioso sin UI |
| Quest principal completada | bIsCompleted → true | Save inmediato |
| Muerte | Al activar respawn | Save con penalidad de oro |
| Evento narrativo crítico | Post-cinemática | Save de protección |

**Manual save:** Menú pausa → Guardar. No en combate, cinemática, ni N10. Confirmación requerida para sobrescribir.

**Slot único:** `SOLO_Save_0.sav` + backup `SOLO_Save_Backup.sav` (corrupción → restauración automática).

**Tamaño estimado:** 15KB (nueva) → 80KB (20h) → 100KB (40h+).

**Carga:** Menú → "Continuar" → validar integridad → migrar si versión desactualizada → aplicar estado al mundo.

### Save en N10 — Zona Sin Retorno

1. Auto-save al entrar (último save hasta post-combate).
2. No saves intermedios durante N10.
3. Post-combate → auto-save con flag `bGameCompleted = true`.
4. Modo exploración post-final: enemigos débiles, bosses muertos, diálogos de despedida.

### Diagrama — Ciclo de Save

```
Inicio juego ("Continuar")
  │
  ├─ Cargar SOLO_Save_0.sav
  │    ├─ Corrupto? → Restaurar backup
  │    └─ OK → Validar versión
  │
  ├─ Deserializar FSOLOSaveData
  ├─ Migrar si versión desactualizada
  │
  └─ Aplicar estado al mundo
       ├─ Posicionar jugador
       ├─ Cargar inventario
       ├─ Cargar quests
       ├─ Activar fogatas
       ├─ Reagrupar enemigos
       └─ Aplicar flags narrativos
              │
              ▼
         [Game Loop]
              │
         ┌────┴────┐
         ▼         ▼
   [Trigger]  [Manual Save]
         │         │
         └────┬────┘
              │
         Backup automático
              │
         Escribir SOLO_Save_0.sav
```


## 3. Muerte y Respawn

### Mecánica de Muerte

Diseño punitivo pero justo: no se pierde XP ni nivel, solo oro y posición.

### Estado de Agonía

Cuando HP llega a 0:

1. **Entrada en agonía:**
   - Slow-motion 0.3x (1s)
   - Filtro rojo/negro que se intensifica
   - Latido cardíaco descendente
   - El jugador puede moverse a 20% velocidad durante **10 segundos**
   - No puede atacar ni usar ítems (excepto pociones de Revive)

2. **Compañeros en agonía:**
   - Entran en estado de agonía cuando su HP llega a 0
   - Duran **30 segundos** en lugar de 10
   - Se arrastran hacia el jugador si están cerca (< 500uu)
   - Se quedan donde cayeron si están lejos

3. **Opciones durante agonía:**
   - **Poción Revive** (ítem raro): revive instantáneamente al objetivo con 30% HP
   - **Sera (compañero):** habilidad única "Tejer Vida" que revive a un aliado caído (cooldown 120s)
   - **Si nadie revive:** temporizador expira → muerte definitiva

### Respawn

Al morir:

1. Pantalla se oscurece (fade negro, 2s). Aparece texto: **"Has muerto"** (fuente serif, tamaño grande, aparición lenta).
2. Texto secundario: **"Descansar en la última fogata"** con efecto de fade-in.
3. Opciones: [Descansar en la fogata] (confirmación automática tras 3s si no se presiona nada).
4. Al confirmar: fade-out, aparición en la última fogata activada (`LastBonfireID`).

**Penalidades al respawn:**
- Pérdida del 20% del oro (redondeado hacia abajo). Si tiene 0 oro, no hay penalidad.
- **NO se pierde XP, NO se pierde nivel, NO se pierden ítems.**
- Todos los enemigos del piso se reagrupan en posiciones aleatorias predefinidas.
- Los compañeros respawn en la misma fogata con 50% HP.
- El jugador respawn con 30% HP/MP/SP.

### Excepciones y Casos Especiales

| Caso | Comportamiento |
|------|----------------|
| Muerte en N10 | Respawn en N9 (Argentia). Última fogata de N9 es el checkpoint |
| Muerte durante pelea vs La Costurera (controla Mira/Vassen) | Respawn justo antes del trigger de la pelea. El evento continúa |
| Muerte narrativa de Caín | Permanente. No revive. Cambia flags narrativos y diálogos futuros |
| Muerte narrativa de Mira | Permanente. Afecta finales y quests secundarias |
| Muerte narrativa de Vassen | Permanente. Su habilidad única se pierde para siempre |
| Muerte de Garrik | Si muere en combate, revive con el jugador en la fogata (no es narrativa) |
| Muerte de Sera | Si muere en combate, revive. Si muere en evento narrativo de Jardines Altos, es permanente (revelación del Tejido) |

### Límite de Muerte y Sistema de Ánimo

- **No hay límite de muertes.** Puedes morir 100 veces.
- Cada muerte consume 20% del oro. A 0 oro, sin penalidad adicional.
- Contador interno: `DeathsSinceLastBonfire` en FSOLOSaveData.
- Después de **10 muertes consecutivas sin descansar en una fogata:**
  - Aparece un NPC en la siguiente fogata que visites
  - NPC: **Lian** (si está viva en la historia) o **El Mercader de Recuerdos**
  - Diálogo único de ánimo: "El camino es duro, pero no estás solo. El Tejido aún te necesita."
  - El contador se reinicia al tener este diálogo
  - Este evento ocurre solo **una vez por partida**

### Diagrama — Ciclo de Muerte y Respawn

```
HP = 0
  │
  ▼
[ESTADO DE AGONÍA]
  │
  ├─ Jugador: 10s para ser revivido
  │   Compañeros: 30s para ser revividos
  │
  ├─ ¿Alguien revive?
  │    ├─ Sí: Revive con 30% HP → Continúa el juego
  │    │    (poción Revive o habilidad de Sera)
  │    │
  │    └─ No: → MUERTE DEFINITIVA
  │              │
  │              ▼
  │         ┌───────────────────┐
  │         │  "Has muerto"     │
  │         │  Fade negro 2s    │
  │         └────────┬──────────┘
  │                  │
  │                  ▼
  │         ┌───────────────────┐
  │         │  Penalidad:       │
  │         │  -20% oro         │
  │         │  Si oro = 0: sin  │
  │         │  penalidad extra  │
  │         └────────┬──────────┘
  │                  │
  │                  ▼
  │         ┌───────────────────┐
  │         │  Respawn en       │
  │         │  última fogata    │
  │         │  (LastBonfireID)  │
  │         │                   │
  │         │  Jugador: 30% HP  │
  │         │  Compañeros: 50%  │
  │         └────────┬──────────┘
  │                  │
  │                  ▼
  │         ┌───────────────────┐
  │         │  Reagrupar        │
  │         │  enemigos del     │
  │         │  piso             │
  │         │  Auto-save        │
  │         └────────┬──────────┘
  │                  │
  │                  ▼
  │         ┌───────────────────┐
  │         │  DeathsSinceLast  │
  │         │  Bonfire++         │
  │         │                   │
  │         │  ¿>= 10?          │
  │         │   ├─ Sí: ¿NPC de  │
  │         │   │   ánimo ya    │
  │         │   │   apareció?   │
  │         │   │   ├─ No →     │
  │         │   │   │ Spawn NPC │
  │         │   │   └─ Sí → OK │
  │         │   └─ No: OK       │
  │         └───────────────────┘
  │
  └─ [Continúa el juego]
```


---

## 4. Implementación Técnica

### Clases y Componentes

| Clase | Tipo | Propósito |
|-------|------|-----------|
| `USOLOSaveGame` | USaveGame | Contenedor serializable de FSOLOSaveData |
| `USOLOSaveSubsystem` | UGameInstanceSubsystem | Gestión central de saves (auto, manual, carga, backup) |
| `ASOLOBonfireInteractive` | AActor | Fogata interactiva: mesh, partículas, luz, detección de enemigos |
| `USOLOBonfireComponent` | UActorComponent | Lógica reutilizable de fogata (estado, visión, interacción) |
| `USOLODeathHandler` | UObject | Manejo de estado de agonía, respawn, penalidades |
| `USOLOAgonyComponent` | UActorComponent | Temporizador de agonía para jugador y compañeros |
| `USOLOFloorManager` | UObject (subsistema) | Gestión de reagrupamiento de enemigos por piso |

### USOLOSaveSubsystem — API

```cpp
UCLASS()
class SOLO_API USOLOSaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // --- Save ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    void SaveGame(bool bIsManual = false);

    UFUNCTION(BlueprintCallable, Category = "Save")
    void AutoSave();  // Trigger automático

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool ManualSave();  // Retorna false si no puede guardar (combate, N10, etc.)

    // --- Load ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    bool LoadGame();  // Carga SOLO_Save_0

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool HasSaveData();  // Existe slot de save?

    // --- Delete ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    void DeleteSave();  // Requiere confirmación del jugador

    // --- Access ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    FSOLOSaveData GetSaveData() const;

    UFUNCTION(BlueprintCallable, Category = "Save")
    void UpdateSaveData(const FSOLOSaveData& NewData);

    // --- Flags ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    void SetNarrativeFlag(const FString& FlagName, bool bValue);

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool GetNarrativeFlag(const FString& FlagName) const;

    // --- Checkpoint ---
    UFUNCTION(BlueprintCallable, Category = "Save")
    void SetLastBonfire(const FString& BonfireID);

    UFUNCTION(BlueprintCallable, Category = "Save")
    FString GetLastBonfireID() const;

private:
    void CreateBackup();
    void RestoreBackup();
    bool ValidateSaveData(const FSOLOSaveData& Data);
    void MigrateSaveData(FSOLOSaveData& Data);
};
```

### ASOLOBonfireInteractive — Especificación

```cpp
UCLASS()
class SOLO_API ASOLOBonfireInteractive : public AActor
{
    GENERATED_BODY()

public:
    // Configuración visual
    UPROPERTY(EditAnywhere, Category = "Bonfire")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Bonfire")
    UNiagaraComponent* FireEffect;       // Partículas de fuego

    UPROPERTY(EditAnywhere, Category = "Bonfire")
    UPointLightComponent* FireLight;     // Luz dinámica

    UPROPERTY(EditAnywhere, Category = "Bonfire")
    UAudioComponent* FireSound;

    // Datos
    UPROPERTY(EditAnywhere, Category = "Bonfire")
    FString BonfireID;                    // "N0_GARGANTA"

    UPROPERTY(EditAnywhere, Category = "Bonfire")
    bool bHasVision = false;             // Tiene visión del Tejido?

    UPROPERTY(EditAnywhere, Category = "Bonfire")
    FName VisionSequencePath;            // Ruta del LevelSequence

    // Estado
    UPROPERTY(BlueprintReadOnly, Category = "Bonfire")
    bool bActivated = false;

    UPROPERTY(BlueprintReadOnly, Category = "Bonfire")
    bool bUnderAttack = false;           // Fogata siendo atacada

    // Métodos
    UFUNCTION(BlueprintCallable, Category = "Bonfire")
    void ActivateBonfire();              // Encender fogata

    UFUNCTION(BlueprintCallable, Category = "Bonfire")
    void Rest();                         // Iniciar descanso

    UFUNCTION(BlueprintCallable, Category = "Bonfire")
    void Extinguish();                   // Apagar (ataque enemigo)

    UFUNCTION(BlueprintCallable, Category = "Bonfire")
    void Relight();                      // Reavivar (canalización jugador)

    UFUNCTION(BlueprintCallable, Category = "Bonfire")
    bool IsSafeToRest();                 // Chequea enemigos cercanos

    // Overlap
    UFUNCTION()
    void OnPlayerEnter(UPrimitiveComponent* Overlapped, AActor* Other,
                       UPrimitiveComponent* OtherComp, int32 BodyIndex,
                       bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlayerExit(UPrimitiveComponent* Overlapped, AActor* Other,
                      UPrimitiveComponent* OtherComp, int32 BodyIndex);

    // Input
    void OnInteract();                   // Bind a tecla F
};
```

### USOLODeathHandler — Especificación

```cpp
UCLASS()
class SOLO_API USOLODeathHandler : public UObject
{
    GENERATED_BODY()

public:
    // Configuración
    UPROPERTY(EditAnywhere, Category = "Death")
    float PlayerAgonyDuration = 10.0f;    // Segundos

    UPROPERTY(EditAnywhere, Category = "Death")
    float CompanionAgonyDuration = 30.0f;

    UPROPERTY(EditAnywhere, Category = "Death")
    float GoldPenaltyPercent = 0.20f;     // 20%

    UPROPERTY(EditAnywhere, Category = "Death")
    float RespawnHealthPercent = 0.30f;   // 30% HP

    UPROPERTY(EditAnywhere, Category = "Death")
    float CompanionRespawnHP = 0.50f;     // 50% HP compañeros

    // Métodos
    UFUNCTION(BlueprintCallable, Category = "Death")
    void EnterAgony(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Death")
    void ReviveTarget(AActor* Target, float HealthPercent, AActor* Reviver);

    UFUNCTION(BlueprintCallable, Category = "Death")
    void OnDeath();

    UFUNCTION(BlueprintCallable, Category = "Death")
    void RespawnAtLastBonfire();

    UFUNCTION(BlueprintCallable, Category = "Death")
    void ApplyGoldPenalty();

    UFUNCTION(BlueprintCallable, Category = "Death")
    void RegroupEnemies();

    // UI
    UFUNCTION(BlueprintImplementableEvent, Category = "Death")
    void ShowDeathScreen();

    UFUNCTION(BlueprintImplementableEvent, Category = "Death")
    void ShowAgonyEffects(float Intensity);

    // Delegados
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDied);
    UPROPERTY(BlueprintAssignable)
    FOnPlayerDied OnPlayerDied;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompanionDied, FString, CompanionID);
    UPROPERTY(BlueprintAssignable)
    FOnCompanionDied OnCompanionDied;

private:
    FTimerHandle AgonyTimer;
    bool bIsInAgony = false;
    int32 DeathsSinceLastBonfire = 0;
};
```

### Persistencia de Datos

```cpp
// Implementación central del save
void USOLOSaveSubsystem::SaveGame(bool bIsManual)
{
    // 1. Crear backup del save actual
    CreateBackup();

    // 2. Crear instancia de USOLOSaveGame
    USOLOSaveGame* SaveInstance = Cast<USOLOSaveGame>(
        UGameplayStatics::CreateSaveGameObject(USOLOSaveGame::StaticClass())
    );

    // 3. Poblar datos desde el estado actual del mundo
    PopulateSaveData(SaveInstance->SaveData);

    // 4. Escribir a slot
    SaveInstance->SaveData.SaveTimestamp = FDateTime::Now();
    SaveInstance->SaveData.SaveVersion = TEXT("1.0");

    const bool bSuccess = UGameplayStatics::SaveGameToSlot(
        SaveInstance,
        TEXT("SOLO_Save_0"),
        0
    );

    // 5. Feedback visual
    if (bSuccess)
    {
        if (bIsManual)
            ShowManualSaveFeedback();
        else
            ShowAutoSaveFeedback();
    }
    else
    {
        // Fallback: restaurar backup, log error
        RestoreBackup();
        UE_LOG(LogSOLO, Error, TEXT("Save failed! Backup restored."));
    }
}

bool USOLOSaveSubsystem::LoadGame()
{
    // 1. Verificar existencia
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("SOLO_Save_0"), 0))
        return false;

    // 2. Cargar desde slot
    USOLOSaveGame* LoadedGame = Cast<USOLOSaveGame>(
        UGameplayStatics::LoadGameFromSlot(TEXT("SOLO_Save_0"), 0)
    );

    if (!LoadedGame)
    {
        // Corrupción detectada — intentar backup
        LoadedGame = Cast<USOLOSaveGame>(
            UGameplayStatics::LoadGameFromSlot(TEXT("SOLO_Save_Backup"), 0)
        );

        if (!LoadedGame)
            return false; // Ambos corruptos

        NotifyPlayer(TEXT("Save data was corrupted. Restored from backup."));
    }

    // 3. Validar y migrar
    if (!ValidateSaveData(LoadedGame->SaveData))
        return false;

    if (LoadedGame->SaveData.SaveVersion != TEXT("1.0"))
        MigrateSaveData(LoadedGame->SaveData);

    // 4. Aplicar al mundo
    ApplySaveDataToWorld(LoadedGame->SaveData);

    return true;
}
```

### Consideraciones de Rendimiento

| Aspecto | Estrategia |
|---------|------------|
| **Frecuencia de escritura** | Auto-saves espaciados mínimo 5s entre sí. Save manual respeta este cooldown |
| **Serialización** | UE5.7 serializa USaveGame en segundo plano. No bloquea el game thread |
| **Partículas de fogatas** | LOD system: 500 partículas cerca, 200 media, 50 lejos. Desactivar si >10 fogatas en pantalla (nunca ocurre) |
| **Overlap de detección** | SphereOverlapActors cada 0.5s, no cada tick. Usar timer |
| **Backup en disco** | Solo se crea backup al escribir save, no al cargar. Máximo 2 archivos (~200KB total) |
| **Recuperación de corrupción** | Validación CRC del save file al cargar. Si falla, restaurar backup automáticamente |
| **Memoria en runtime** | FSOLOSaveData se mantiene en RAM (GameInstanceSubsystem). ~5–10KB en memoria de runtime |


---

## 5. Prompt para Claude Code

Copiá y pegá este prompt en Claude Code para generar la implementación automáticamente:

```
Tarea: Implementar el sistema de guardado, fogatas y muerte para SOLO (Unreal Engine 5.7).

Basado en las especificaciones detalladas en sistema_guardado.md, crear los siguientes assets:

1. SOLOSaveGame (USaveGame):
   - Contiene FSOLOSaveData con todas las sub-estructuras listadas en el documento
   - Incluir: FCompanionSaveData, FQuestSaveData, FQuestObjectiveProgress, FItemSaveData
   - Agregar UPROPERTY() a todos los campos para serialización

2. SOLOSaveSubsystem (UGameInstanceSubsystem):
   - Auto-save: al descansar en fogatas, cambiar de piso, completar quests principales, al morir
   - Manual save: desde el menú, no disponible en combate o N10
   - Cargar el save más reciente al hacer clic en "Continuar"
   - Borrar save con confirmación requerida
   - Backup automático antes de cada escritura (SOLO_Save_Backup)
   - Validación CRC del archivo al cargar, restauración automática de backup si hay corrupción
   - Migración de versiones de save data

3. SOLOBonfireInteractive (AActor):
   - StaticMesh para fogata apagada (cenizas) y encendida (con fuego)
   - Sistema Niagara de partículas de fuego con LODs
   - Point light dinámica con parpadeo
   - Prompt de interacción "Presiona F para descansar"
   - Al interactuar: curar HP/MP/SP al 100% en 3s, eliminar estados negativos
   - Animación de sentarse junto al fuego
   - Menú de descanso: equipo (consulta), habilidades (consulta), diario, mapa, visiones
   - Detección de enemigos cercanos (SphereOverlap, radio 3000uu)
   - Sistema de ataque a fogata (N6 y N7): enemigos pueden apagar la fogata
   - Reavivar fogata con canalización de 5s
   - Compatible con fogatas especiales que tienen visiones del Tejido (ULevelSequence)

4. SOLODeathHandler (UObject):
   - Estado de agonía: 10s timer para jugador, 30s para compañeros
   - Slow-motion y filtro rojo en agonía
   - Revivir con poción Revive o habilidad de Sera
   - Respawn en la última fogata activada con penalidad: 20% oro
   - Reagrupar enemigos del nivel (reubicación aleatoria predefinida)
   - Pantalla de muerte con fade y texto "Has muerto"
   - Sistema de ánimo: NPC especial después de 10 muertes sin descansar

Requisitos adicionales:
- Slot único: "SOLO_Save_0" con backup "SOLO_Save_Backup"
- Usar UGameplayStatics::SaveGameToSlot / LoadGameFromSlot
- Tamaño estimado: 15KB (nuevo) a 100KB (completo)
- No se permite guardar en N10 hasta post-combate
- Post-final: auto-save con flag de juego completado y exploración post-créditos
- Las fogatas especiales (8 en total) tienen visiones del Tejido opcionales
- No se puede cambiar de clase ni reasignar habilidades en fogatas
- Persistencia entre sesiones: al iniciar "Continuar partida" carga el último save
```

---

> **Fin del documento — Sistema de Guardado y Fogatas v1.0**
