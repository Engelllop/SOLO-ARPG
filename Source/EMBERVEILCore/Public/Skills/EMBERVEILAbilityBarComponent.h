#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skills/EMBERVEILSwordSkillTypes.h"
#include "EMBERVEILAbilityBarComponent.generated.h"

class UEMBERVEILSwordSkillDataAsset;
class UAbilitySystemComponent;
class UEMBERVEILSkillMasteryComponent;

// ─── DELEGATES ────────────────────────────────────────────────────────────────

/**
 * Broadcast cada vez que cambia el contenido de un slot del AbilityBar.
 * El widget WG_AbilityBar se suscribe aquí para refrescar visualmente el slot.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityBarChanged,
	int32, SlotIndex,
	FAbilitySlot, NewSlot);

/** Broadcast cuando se desbloquea un nuevo slot del AbilityBar */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUnlocked,
	int32, SlotIndex);

// ─── COMPONENT ────────────────────────────────────────────────────────────────

/**
 * UEMBERVEILAbilityBarComponent
 *
 * Gestiona los 6 slots del AbilityBar del jugador:
 *   - Asignación de skills a slots (y otorgamiento de la ability al ASC).
 *   - Eliminación de skills (y revocación de la ability del ASC).
 *   - Swap entre dos slots (para drag & drop en la UI).
 *   - Activación directa de la ability en un slot a través del ASC.
 *   - Desbloqueo progresivo de slots (comienza con 2, se desbloquean por nivel).
 *
 * Integración con PlayerController:
 *   El PlayerController llama a ActivateSkillInSlot(0-3) cuando el jugador
 *   presiona Skill1-Skill4 respectivamente.
 *
 * Vive en el PlayerState junto al ASC para garantizar persistencia entre respawns.
 */
UCLASS(ClassGroup = "EMBERVEIL", meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILAbilityBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEMBERVEILAbilityBarComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ─── Delegates públicos ───────────────────────────────────────────────────

	/** Notifica a la UI cuando el contenido de un slot cambia */
	UPROPERTY(BlueprintAssignable, Category = "AbilityBar|Delegates")
	FOnAbilityBarChanged OnAbilityBarChanged;

	/** Notifica a la UI cuando se desbloquea un slot */
	UPROPERTY(BlueprintAssignable, Category = "AbilityBar|Delegates")
	FOnSlotUnlocked OnSlotUnlocked;

	// ─── Configuración ────────────────────────────────────────────────────────

	/** Número total de slots del AbilityBar (siempre 6) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityBar|Config", Meta = (ClampMin = 1, ClampMax = 6))
	int32 NumSlots = 6;

	/** Número de slots desbloqueados al inicio de la partida */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityBar|Config", Meta = (ClampMin = 1))
	int32 InitialUnlockedSlots = 2;

	// ─── API pública ──────────────────────────────────────────────────────────

	/**
	 * Asigna SkillData al slot SlotIndex.
	 * Si el slot ya tenía una skill, la reemplaza (revocando la ability anterior).
	 * Otorga la nueva ability al ASC y almacena el handle resultante.
	 *
	 * @return false si: el índice es inválido, el slot está bloqueado,
	 *         SkillData es nulo, la skill no está desbloqueada en el MasteryComponent
	 *         o la ability no pudo otorgarse al ASC.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool AssignSkillToSlot(int32 SlotIndex, UEMBERVEILSwordSkillDataAsset* SkillData);

	/**
	 * Elimina la skill del slot SlotIndex y revoca la ability del ASC.
	 *
	 * @return false si el slot está vacío o el índice es inválido.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool RemoveSkillFromSlot(int32 SlotIndex);

	/**
	 * Intercambia el contenido de dos slots.
	 * Actualiza los handles del ASC si es necesario.
	 * Útil para drag & drop en WG_AbilityBar.
	 *
	 * @return false si alguno de los índices es inválido.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool SwapSlots(int32 SlotA, int32 SlotB);

	/** Devuelve una copia del FAbilitySlot para el índice indicado */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	FAbilitySlot GetSlot(int32 SlotIndex) const;

	/**
	 * Desbloquea el siguiente slot disponible (incrementa UnlockedSlots en 1).
	 * No hace nada si ya están todos los slots desbloqueados.
	 * Hace broadcast de OnSlotUnlocked con el índice del slot recién desbloqueado.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void UnlockSlot(int32 SlotIndex);

	/** Devuelve true si el slot con ese índice está desbloqueado */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	bool IsSlotUnlocked(int32 SlotIndex) const;

	/** Devuelve cuántos slots están desbloqueados actualmente */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	int32 GetUnlockedSlots() const { return UnlockedSlots; }

	/**
	 * Activa la ability asignada al slot SlotIndex a través del ASC.
	 * No hace nada si el slot está vacío o el handle es inválido.
	 * Llamado por el PlayerController en respuesta a las acciones Skill1-Skill4.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void ActivateSkillInSlot(int32 SlotIndex);

	/**
	 * Devuelve todos los slots (incluyendo vacíos) para que la UI pueda
	 * iterar y construir su representación visual completa.
	 */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	const TArray<FAbilitySlot>& GetAllSlots() const { return AssignedSlots; }

private:
	// ─── Datos replicados ─────────────────────────────────────────────────────

	/** Estado de los 6 slots; inicializado en BeginPlay con entradas vacías */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "AbilityBar|Data")
	TArray<FAbilitySlot> AssignedSlots;

	/** Cuántos slots están actualmente desbloqueados (el resto aparecen grises en la UI) */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "AbilityBar|Data")
	int32 UnlockedSlots = 2;

	// ─── Referencias cacheadas ────────────────────────────────────────────────

	/** ASC del propietario; resuelto en BeginPlay desde el PlayerState */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	/** MasteryComponent del propietario; necesario para validar si una skill está desbloqueada */
	UPROPERTY()
	TObjectPtr<UEMBERVEILSkillMasteryComponent> CachedMasteryComponent;

	// ─── Utilidades internas ──────────────────────────────────────────────────

	/** Resuelve y cachea el ASC y el MasteryComponent desde el PlayerState propietario */
	void CacheDependencies();

	/** Devuelve true si SlotIndex está dentro del rango [0, NumSlots) */
	bool IsValidSlotIndex(int32 SlotIndex) const;

	/**
	 * Otorga la ability de SkillData al ASC y devuelve el handle resultante.
	 * Devuelve un handle inválido si la ability no pudo otorgarse.
	 */
	FGameplayAbilitySpecHandle GrantAbilityToASC(UEMBERVEILSwordSkillDataAsset* SkillData);

	/** Revoca la ability identificada por Handle del ASC */
	void RevokeAbilityFromASC(FGameplayAbilitySpecHandle Handle);

	/** Hace broadcast de OnAbilityBarChanged y marca el componente como dirty para replicación */
	void NotifySlotChanged(int32 SlotIndex);
};
