#pragma once

#include "CoreMinimal.h"
#include "Character/EMBERVEILCharacterBase.h"
#include "EMBERVEILEnemyCharacter.generated.h"

class UWidgetComponent;
class UEMBERVEILLootComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseChangedSignature, int32, NewPhase);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEMBERVEILEnemyDeathSignature, AActor*, DeadEnemy);

// ─── Enums & Structs ──────────────────────────────────────────────────────────

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Common    UMETA(DisplayName = "Común"),
	Elite     UMETA(DisplayName = "Élite"),
	Boss      UMETA(DisplayName = "Jefe"),
	WorldBoss UMETA(DisplayName = "Jefe de Mundo")
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * Personaje enemigo base de EMBERVEIL.
 *
 * El ASC vive aquí directamente (no en PlayerState) porque los enemigos no
 * persisten entre niveles. Incluye healthbar flotante, sistema de percepción
 * básico, fases de jefe y recompensas de experiencia al morir.
 */
UCLASS()
class EMBERVEILCORE_API AEMBERVEILEnemyCharacter : public AEMBERVEILCharacterBase
{
	GENERATED_BODY()

public:
	AEMBERVEILEnemyCharacter();

	// ─── ICombatInterface ─────────────────────────────────────────────────────
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual void  Die_Implementation(AActor* InstigatorActor) override;

	// ─── Healthbar ────────────────────────────────────────────────────────────

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHealthBar();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideHealthBar();

	// ─── Percepción ───────────────────────────────────────────────────────────

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	bool IsAggro() const { return bIsAggro; }

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	float GetAggroLostRadius() const { return AggroLostRadius; }

	/** Sincroniza target y aggro desde el AIController (solo servidor / lógica de IA). */
	void SetCombatFocus(AActor* NewTarget);

	// ─── Fases de jefe ────────────────────────────────────────────────────────

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	bool IsBossEnemy() const { return bIsBoss; }

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	int32 GetBossCurrentPhase() const { return CurrentPhase; }

	/**
	 * Comprueba si el HP actual cruzó algún umbral de fase.
	 * Llamado desde PostGameplayEffectExecute del AttributeSet cuando baja Health.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Phases")
	void CheckPhaseTransition();

	/**
	 * Evalúa umbrales de fase según el porcentaje de HP actual (0–1).
	 * Los umbrales deben estar ordenados de mayor a menor (p. ej. 0.75, 0.5, 0.25).
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Phases")
	void CheckBossPhase(float HPPercent);

	/** Multicast: fase de jefe avanzada (1-based tras cada umbral cruzado). */
	UPROPERTY(BlueprintAssignable, Category = "Boss|Phases")
	FOnBossPhaseChangedSignature OnBossPhaseChanged;

	/** Multicast: el enemigo acaba de morir (primera muerte). */
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
	FOnEMBERVEILEnemyDeathSignature OnEnemyDied;

	/**
	 * BlueprintNativeEvent — permite que los Blueprints manejen la transición
	 * visual/de comportamiento de cada fase sin recompilar C++.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boss|Phases")
	void OnPhaseChanged(int32 NewPhase);
	virtual void OnPhaseChanged_Implementation(int32 NewPhase);

	// ─── Experiencia ──────────────────────────────────────────────────────────

	/**
	 * Aplica Experience al ASC del jugador (PlayerState) vía GameplayEffect con SetByCaller Data.XPReward.
	 * Solo se ejecuta en el servidor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rewards")
	void GrantExperienceToKiller(AActor* Killer);

	/** Guarda el instigador del último golpe letal (servidor) para acreditar XP si Die llega sin parámetro. */
	void RememberKillInstigator(AActor* InstigatorAvatar);

	UFUNCTION(BlueprintPure, Category = "Enemy|Identity")
	FText GetBossDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	int32 GetBossHealthBarPhaseCount() const;

protected:
	virtual void BeginPlay() override;

	// ─── Configuración de identidad ───────────────────────────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Identity")
	EEnemyType EnemyType = EEnemyType::Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Identity")
	int32 EnemyLevel = 1;

	/** Nombre mostrado en la barra de jefe del HUD (si está vacío, se usa el nombre del actor). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Identity")
	FText BossDisplayName;

	// ─── Healthbar ────────────────────────────────────────────────────────────

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TObjectPtr<UEMBERVEILLootComponent> LootComponent;

	// ─── Percepción ───────────────────────────────────────────────────────────

	/** Radio dentro del cual detecta al jugador y entra en estado de combate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Aggro")
	float AggroRadius = 1200.f;

	/** Radio a partir del cual pierde al target y vuelve al estado de patrulla. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Aggro")
	float AggroLostRadius = 2000.f;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Aggro")
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Aggro")
	bool bIsAggro = false;

	// ─── Fases de jefe ────────────────────────────────────────────────────────

	/**
	 * Indica que este enemy usa el sistema de fases.
	 * Cuando es false, CheckPhaseTransition es un no-op para evitar overhead.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases")
	bool bIsBoss = false;

	/**
	 * Porcentajes de HP (0–1) en orden descendente en los que cambia de fase.
	 * Ej: {0.60f, 0.25f} → cambia a fase 1 al 60% y a fase 2 al 25%.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss", ClampMin = "0.0", ClampMax = "1.0"))
	TArray<float> PhaseThresholds;

	UPROPERTY(BlueprintReadOnly, Category = "Boss|Phases")
	int32 CurrentPhase = 0;

	/** GE opcional (+AttackSpeed u otros) aplicado en cada transición de fase. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TSubclassOf<UGameplayEffect> BossPhaseAttackSpeedEffect;

	/** Clase de enemigo común para refuerzos (2 por transición). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TSubclassOf<AEMBERVEILEnemyCharacter> ReinforcementEnemyClass;

	/** Radio alrededor del boss para SpawnActor de refuerzos. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss", ClampMin = "50.0"))
	float ReinforcementSpawnRadius = 600.f;

	/** Burst Niagara al cambiar de fase. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TObjectPtr<UNiagaraSystem> BossPhaseTransitionVFX;

	// ─── Recompensas ──────────────────────────────────────────────────────────

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards")
	float XPReward = 100.f;

	/**
	 * Multiplicador global de XP — se usa para eventos de doble XP, dificultad, etc.
	 * Modificado en runtime desde GameMode o desde un sistema de eventos globales.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
	float ExperienceMultiplier = 1.f;

	/**
	 * Si está asignado, sustituye al GE por defecto UEMBERVEIL_GE_GrantExperience.
	 * Debe ser Instant, aditivo sobre Experience con SetByCaller Data.XPReward.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards|GAS")
	TSubclassOf<UGameplayEffect> ExperienceGrantEffect;

private:
	/** Healthbar alpha actual — interpolado durante Show/Hide para el fade. */
	float HealthBarAlpha = 0.f;

	FTimerHandle HealthBarFadeTimer;

	TWeakObjectPtr<AActor> PendingKillInstigator;

	FTimerHandle BossHealthBarHideTimer;

	void NotifyLocalPlayersShowBossHealthBar();
	void ScheduleHideBossHealthBarDelayed();
	void ExecuteHideBossHealthBarOnLocalHud();

	void ApplyBossPhaseAttackSpeedEffect();
	void SpawnBossReinforcements();
	void PlayBossPhaseTransitionVFX();
	void SyncBossPhaseToBlackboard();
};
