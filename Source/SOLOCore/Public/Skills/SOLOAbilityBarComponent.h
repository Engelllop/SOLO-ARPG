#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skills/SOLOSwordSkillTypes.h"
#include "SOLOAbilityBarComponent.generated.h"

class USOLOSwordSkillDataAsset;
class UAbilitySystemComponent;
class USOLOSkillMasteryComponent;

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * Broadcast cada vez que cambia el contenido de un slot del AbilityBar.
 * El widget WG_AbilityBar se suscribe aquÃ­ para refrescar visualmente el slot.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityBarChanged,
	int32, SlotIndex,
	FAbilitySlot, NewSlot);

/** Broadcast cuando se desbloquea un nuevo slot del AbilityBar */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUnlocked,
	int32, SlotIndex);

// â”€â”€â”€ COMPONENT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * USOLOAbilityBarComponent
 *
 * Gestiona los 6 slots del AbilityBar del jugador:
 *   - AsignaciÃ³n de skills a slots (y otorgamiento de la ability al ASC).
 *   - EliminaciÃ³n de skills (y revocaciÃ³n de la ability del ASC).
 *   - Swap entre dos slots (para drag & drop en la UI).
 *   - ActivaciÃ³n directa de la ability en un slot a travÃ©s del ASC.
 *   - Desbloqueo progresivo de slots (comienza con 2, se desbloquean por nivel).
 *
 * IntegraciÃ³n con PlayerController:
 *   El PlayerController llama a ActivateSkillInSlot(0-3) cuando el jugador
 *   presiona Skill1-Skill4 respectivamente.
 *
 * Vive en el PlayerState junto al ASC para garantizar persistencia entre respawns.
 */
UCLASS(ClassGroup = "SOLO", meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOAbilityBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLOAbilityBarComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// â”€â”€â”€ Delegates pÃºblicos â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Notifica a la UI cuando el contenido de un slot cambia */
	UPROPERTY(BlueprintAssignable, Category = "AbilityBar|Delegates")
	FOnAbilityBarChanged OnAbilityBarChanged;

	/** Notifica a la UI cuando se desbloquea un slot */
	UPROPERTY(BlueprintAssignable, Category = "AbilityBar|Delegates")
	FOnSlotUnlocked OnSlotUnlocked;

	// â”€â”€â”€ ConfiguraciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** NÃºmero total de slots del AbilityBar (siempre 6) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityBar|Config", Meta = (ClampMin = 1, ClampMax = 6))
	int32 NumSlots = 6;

	/** NÃºmero de slots desbloqueados al inicio de la partida */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityBar|Config", Meta = (ClampMin = 1))
	int32 InitialUnlockedSlots = 2;

	// â”€â”€â”€ API pÃºblica â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Asigna SkillData al slot SlotIndex.
	 * Si el slot ya tenÃ­a una skill, la reemplaza (revocando la ability anterior).
	 * Otorga la nueva ability al ASC y almacena el handle resultante.
	 *
	 * @return false si: el Ã­ndice es invÃ¡lido, el slot estÃ¡ bloqueado,
	 *         SkillData es nulo, la skill no estÃ¡ desbloqueada en el MasteryComponent
	 *         o la ability no pudo otorgarse al ASC.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool AssignSkillToSlot(int32 SlotIndex, USOLOSwordSkillDataAsset* SkillData);

	/**
	 * Elimina la skill del slot SlotIndex y revoca la ability del ASC.
	 *
	 * @return false si el slot estÃ¡ vacÃ­o o el Ã­ndice es invÃ¡lido.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool RemoveSkillFromSlot(int32 SlotIndex);

	/**
	 * Intercambia el contenido de dos slots.
	 * Actualiza los handles del ASC si es necesario.
	 * Ãštil para drag & drop en WG_AbilityBar.
	 *
	 * @return false si alguno de los Ã­ndices es invÃ¡lido.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	bool SwapSlots(int32 SlotA, int32 SlotB);

	/** Devuelve una copia del FAbilitySlot para el Ã­ndice indicado */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	FAbilitySlot GetSlot(int32 SlotIndex) const;

	/**
	 * Desbloquea el siguiente slot disponible (incrementa UnlockedSlots en 1).
	 * No hace nada si ya estÃ¡n todos los slots desbloqueados.
	 * Hace broadcast de OnSlotUnlocked con el Ã­ndice del slot reciÃ©n desbloqueado.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void UnlockSlot(int32 SlotIndex);

	/** Devuelve true si el slot con ese Ã­ndice estÃ¡ desbloqueado */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	bool IsSlotUnlocked(int32 SlotIndex) const;

	/** Devuelve cuÃ¡ntos slots estÃ¡n desbloqueados actualmente */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	int32 GetUnlockedSlots() const { return UnlockedSlots; }

	/**
	 * Activa la ability asignada al slot SlotIndex a travÃ©s del ASC.
	 * No hace nada si el slot estÃ¡ vacÃ­o o el handle es invÃ¡lido.
	 * Llamado por el PlayerController en respuesta a las acciones Skill1-Skill4.
	 */
	UFUNCTION(BlueprintCallable, Category = "AbilityBar")
	void ActivateSkillInSlot(int32 SlotIndex);

	/**
	 * Devuelve todos los slots (incluyendo vacÃ­os) para que la UI pueda
	 * iterar y construir su representaciÃ³n visual completa.
	 */
	UFUNCTION(BlueprintPure, Category = "AbilityBar")
	const TArray<FAbilitySlot>& GetAllSlots() const { return AssignedSlots; }

private:
	// â”€â”€â”€ Datos replicados â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Estado de los 6 slots; inicializado en BeginPlay con entradas vacÃ­as */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "AbilityBar|Data")
	TArray<FAbilitySlot> AssignedSlots;

	/** CuÃ¡ntos slots estÃ¡n actualmente desbloqueados (el resto aparecen grises en la UI) */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "AbilityBar|Data")
	int32 UnlockedSlots = 2;

	// â”€â”€â”€ Referencias cacheadas â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** ASC del propietario; resuelto en BeginPlay desde el PlayerState */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> CachedASC;

	/** MasteryComponent del propietario; necesario para validar si una skill estÃ¡ desbloqueada */
	UPROPERTY()
	TObjectPtr<USOLOSkillMasteryComponent> CachedMasteryComponent;

	// â”€â”€â”€ Utilidades internas â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Resuelve y cachea el ASC y el MasteryComponent desde el PlayerState propietario */
	void CacheDependencies();

	/** Devuelve true si SlotIndex estÃ¡ dentro del rango [0, NumSlots) */
	bool IsValidSlotIndex(int32 SlotIndex) const;

	/**
	 * Otorga la ability de SkillData al ASC y devuelve el handle resultante.
	 * Devuelve un handle invÃ¡lido si la ability no pudo otorgarse.
	 */
	FGameplayAbilitySpecHandle GrantAbilityToASC(USOLOSwordSkillDataAsset* SkillData);

	/** Revoca la ability identificada por Handle del ASC */
	void RevokeAbilityFromASC(FGameplayAbilitySpecHandle Handle);

	/** Hace broadcast de OnAbilityBarChanged y marca el componente como dirty para replicaciÃ³n */
	void NotifySlotChanged(int32 SlotIndex);
};
