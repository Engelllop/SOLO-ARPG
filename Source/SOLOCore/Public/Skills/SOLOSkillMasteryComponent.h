#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skills/SOLOSwordSkillTypes.h"
#include "SOLOSkillMasteryComponent.generated.h"

class USOLOSwordSkillDataAsset;

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Broadcast cuando un tipo de arma sube de nivel de maestrÃ­a */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMasteryLevelUp,
	EWeaponType, WeaponType,
	int32, NewLevel);

/** Broadcast cuando se desbloquea una nueva skill (por subida de nivel o manualmente) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked,
	FName, SkillID);

// â”€â”€â”€ COMPONENT â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * USOLOSkillMasteryComponent
 *
 * Componente que vive en el PlayerState y trackea:
 *   - El progreso de maestrÃ­a (XP + nivel) para cada tipo de arma.
 *   - El conjunto de skills desbloqueadas (por ID).
 *   - El tipo de arma actualmente equipada.
 *
 * Cuando AddMasteryXP alcanza un umbral de nivel, sube automÃ¡ticamente el
 * nivel y llama a TryAutoUnlockSkills para desbloquear skills elegibles.
 *
 * MaestrÃ­a y skills desbloqueadas se replican como TArray (map/set replicados no soportados por UHT).
 */
UCLASS(ClassGroup = "SOLO", meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOSkillMasteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLOSkillMasteryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// â”€â”€â”€ Delegates pÃºblicos â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	UPROPERTY(BlueprintAssignable, Category = "Mastery|Delegates")
	FOnMasteryLevelUp OnMasteryLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Mastery|Delegates")
	FOnSkillUnlocked OnSkillUnlocked;

	// â”€â”€â”€ API pÃºblica â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * AÃ±ade XPAmount de XP al tipo de arma indicado.
	 * Si la XP acumulada supera el umbral, sube el nivel (hasta nivel 20)
	 * y llama a TryAutoUnlockSkills tras cada subida.
	 * Solo ejecuta lÃ³gica real en el servidor (HasAuthority).
	 */
	UFUNCTION(BlueprintCallable, Category = "Mastery")
	void AddMasteryXP(EWeaponType WeaponType, float XPAmount);

	/** Devuelve true si la skill con el ID indicado estÃ¡ desbloqueada */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	bool IsSkillUnlocked(FName SkillID) const;

	/**
	 * Comprueba si el jugador cumple todos los requisitos para desbloquear SkillData:
	 *   1. El nivel de maestrÃ­a con el arma requerida es suficiente.
	 *   2. Todos los prerequisitos (RequiredSkillIDs) estÃ¡n desbloqueados.
	 *   3. La skill no estÃ¡ ya desbloqueada.
	 */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	bool CanUnlockSkill(USOLOSwordSkillDataAsset* SkillData) const;

	/** Devuelve el nivel de maestrÃ­a actual con el tipo de arma indicado */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	int32 GetMasteryLevel(EWeaponType WeaponType) const;

	/** Devuelve la XP acumulada actual con el tipo de arma indicado */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	float GetMasteryXP(EWeaponType WeaponType) const;

	/**
	 * Devuelve todos los DataAssets de skills desbloqueadas para el tipo de arma indicado.
	 * Usa el AssetManager para cargar sÃ­ncronamente los assets (ya deben estar en memoria
	 * o en un bundle cargado previamente).
	 */
	UFUNCTION(BlueprintCallable, Category = "Mastery")
	TArray<USOLOSwordSkillDataAsset*> GetUnlockedSkillsForWeapon(EWeaponType WeaponType) const;

	/**
	 * Cambia el tipo de arma activa.
	 * Se llama desde el sistema de equipamiento cuando el jugador equipa un arma nueva.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mastery")
	void SetActiveWeaponType(EWeaponType NewType);

	/** Devuelve el tipo de arma actualmente equipada */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	EWeaponType GetActiveWeaponType() const { return ActiveWeaponType; }

	/** Devuelve la copia completa de MasteryData para un arma dada (puede estar vacÃ­a si nunca se usÃ³) */
	UFUNCTION(BlueprintPure, Category = "Mastery")
	FWeaponMasteryData GetMasteryData(EWeaponType WeaponType) const;

	const TArray<FName>& GetUnlockedSkillIDs() const { return UnlockedSkillIDs; }

	/** Reemplaza la lista de skills desbloqueadas (solo servidor; p. ej. LoadGame). */
	void RestoreUnlockedSkillsFromSave(const TArray<FName>& InSkillIDs);

private:
	// â”€â”€â”€ Datos replicados â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Progreso de maestrÃ­a por tipo de arma (una entrada por WeaponType). */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Mastery|Data")
	TArray<FWeaponMasteryData> MasteryRows;

	/** SkillIDs desbloqueados (sin duplicados; mantener en servidor). */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Mastery|Data")
	TArray<FName> UnlockedSkillIDs;

	/** Tipo de arma actualmente equipada */
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Mastery|Data")
	EWeaponType ActiveWeaponType = EWeaponType::None;

	// â”€â”€â”€ LÃ³gica interna â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Carga todas las SwordSkillDataAssets desde el AssetManager y desbloquea
	 * aquellas cuyo RequiredMasteryLevel <= nivel actual, RequiredWeaponType == WeaponType
	 * y cuyos prerequisitos estÃ¡n cumplidos.
	 * Se llama automÃ¡ticamente despuÃ©s de cada subida de nivel.
	 */
	void TryAutoUnlockSkills(EWeaponType WeaponType, int32 CurrentLevel);

	/** Marca una skill como desbloqueada y hace broadcast de OnSkillUnlocked */
	void UnlockSkillInternal(FName SkillID);

	FWeaponMasteryData& GetOrAddMasteryRow(EWeaponType WeaponType);
	const FWeaponMasteryData* FindMasteryRow(EWeaponType WeaponType) const;
};
