#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOCharacterBase.h"
#include "SOLOEnemyCharacter.generated.h"

class UWidgetComponent;
class USOLOLootComponent;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseChangedSignature, int32, NewPhase);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSOLOEnemyDeathSignature, AActor*, DeadEnemy);

// â”€â”€â”€ Enums & Structs â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Common    UMETA(DisplayName = "ComÃºn"),
	Elite     UMETA(DisplayName = "Ã‰lite"),
	Boss      UMETA(DisplayName = "Jefe"),
	WorldBoss UMETA(DisplayName = "Jefe de Mundo")
};

// â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * Personaje enemigo base de SOLO.
 *
 * El ASC vive aquÃ­ directamente (no en PlayerState) porque los enemigos no
 * persisten entre niveles. Incluye healthbar flotante, sistema de percepciÃ³n
 * bÃ¡sico, fases de jefe y recompensas de experiencia al morir.
 */
UCLASS()
class SOLOCORE_API ASOLOEnemyCharacter : public ASOLOCharacterBase
{
	GENERATED_BODY()

public:
	ASOLOEnemyCharacter();

	// â”€â”€â”€ ICombatInterface â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual void  Die_Implementation(AActor* InstigatorActor) override;

	// â”€â”€â”€ Healthbar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowHealthBar();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideHealthBar();

	// â”€â”€â”€ PercepciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	bool IsAggro() const { return bIsAggro; }

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	AActor* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION(BlueprintPure, Category = "AI|Aggro")
	float GetAggroLostRadius() const { return AggroLostRadius; }

	/** Sincroniza target y aggro desde el AIController (solo servidor / lÃ³gica de IA). */
	void SetCombatFocus(AActor* NewTarget);

	// â”€â”€â”€ Fases de jefe â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	bool IsBossEnemy() const { return bIsBoss; }

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	int32 GetBossCurrentPhase() const { return CurrentPhase; }

	/**
	 * Comprueba si el HP actual cruzÃ³ algÃºn umbral de fase.
	 * Llamado desde PostGameplayEffectExecute del AttributeSet cuando baja Health.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Phases")
	void CheckPhaseTransition();

	/**
	 * EvalÃºa umbrales de fase segÃºn el porcentaje de HP actual (0â€“1).
	 * Los umbrales deben estar ordenados de mayor a menor (p. ej. 0.75, 0.5, 0.25).
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss|Phases")
	void CheckBossPhase(float HPPercent);

	/** Multicast: fase de jefe avanzada (1-based tras cada umbral cruzado). */
	UPROPERTY(BlueprintAssignable, Category = "Boss|Phases")
	FOnBossPhaseChangedSignature OnBossPhaseChanged;

	/** Multicast: el enemigo acaba de morir (primera muerte). */
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
	FOnSOLOEnemyDeathSignature OnEnemyDied;

	/**
	 * BlueprintNativeEvent â€” permite que los Blueprints manejen la transiciÃ³n
	 * visual/de comportamiento de cada fase sin recompilar C++.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Boss|Phases")
	void OnPhaseChanged(int32 NewPhase);
	virtual void OnPhaseChanged_Implementation(int32 NewPhase);

	// â”€â”€â”€ Experiencia â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Aplica Experience al ASC del jugador (PlayerState) vÃ­a GameplayEffect con SetByCaller Data.XPReward.
	 * Solo se ejecuta en el servidor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rewards")
	void GrantExperienceToKiller(AActor* Killer);

	/** Guarda el instigador del Ãºltimo golpe letal (servidor) para acreditar XP si Die llega sin parÃ¡metro. */
	void RememberKillInstigator(AActor* InstigatorAvatar);

	UFUNCTION(BlueprintPure, Category = "Enemy|Identity")
	FText GetBossDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "Boss|Phases")
	int32 GetBossHealthBarPhaseCount() const;

protected:
	virtual void BeginPlay() override;

	// â”€â”€â”€ ConfiguraciÃ³n de identidad â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Identity")
	EEnemyType EnemyType = EEnemyType::Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Identity")
	int32 EnemyLevel = 1;

	/** Nombre mostrado en la barra de jefe del HUD (si estÃ¡ vacÃ­o, se usa el nombre del actor). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Identity")
	FText BossDisplayName;

	// â”€â”€â”€ Healthbar â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TObjectPtr<USOLOLootComponent> LootComponent;

	// â”€â”€â”€ PercepciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

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

	// â”€â”€â”€ Fases de jefe â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Indica que este enemy usa el sistema de fases.
	 * Cuando es false, CheckPhaseTransition es un no-op para evitar overhead.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases")
	bool bIsBoss = false;

	/**
	 * Porcentajes de HP (0â€“1) en orden descendente en los que cambia de fase.
	 * Ej: {0.60f, 0.25f} â†’ cambia a fase 1 al 60% y a fase 2 al 25%.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss", ClampMin = "0.0", ClampMax = "1.0"))
	TArray<float> PhaseThresholds;

	UPROPERTY(BlueprintReadOnly, Category = "Boss|Phases")
	int32 CurrentPhase = 0;

	/** GE opcional (+AttackSpeed u otros) aplicado en cada transiciÃ³n de fase. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TSubclassOf<UGameplayEffect> BossPhaseAttackSpeedEffect;

	/** Clase de enemigo comÃºn para refuerzos (2 por transiciÃ³n). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TSubclassOf<ASOLOEnemyCharacter> ReinforcementEnemyClass;

	/** Radio alrededor del boss para SpawnActor de refuerzos. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss", ClampMin = "50.0"))
	float ReinforcementSpawnRadius = 600.f;

	/** Burst Niagara al cambiar de fase. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Phases",
		meta = (EditCondition = "bIsBoss"))
	TObjectPtr<UNiagaraSystem> BossPhaseTransitionVFX;

	// â”€â”€â”€ Recompensas â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards")
	float XPReward = 100.f;

	/**
	 * Multiplicador global de XP â€” se usa para eventos de doble XP, dificultad, etc.
	 * Modificado en runtime desde GameMode o desde un sistema de eventos globales.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rewards")
	float ExperienceMultiplier = 1.f;

	/**
	 * Si estÃ¡ asignado, sustituye al GE por defecto USOLO_GE_GrantExperience.
	 * Debe ser Instant, aditivo sobre Experience con SetByCaller Data.XPReward.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rewards|GAS")
	TSubclassOf<UGameplayEffect> ExperienceGrantEffect;

private:
	/** Healthbar alpha actual â€” interpolado durante Show/Hide para el fade. */
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
