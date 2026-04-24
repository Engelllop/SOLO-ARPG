#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skills/EMBERVEILSwordSkillTypes.h"
#include "EMBERVEILSwordSkillDataAsset.generated.h"

class UEMBERVEILGameplayAbility;
class UTexture2D;

/**
 * DataAsset que define completamente una Sword Skill.
 *
 * Cada skill en el juego se crea como una instancia de este asset en el editor.
 * El tipo primario es "SwordSkill", lo que permite al AssetManager cargar
 * todas las skills de forma asíncrona y filtrarlas por tipo.
 *
 * Uso habitual:
 *   - Crear un BP basado en este asset en Content/Skills/
 *   - Asignarle un SkillID único (e.g. "Sword_WindSlash")
 *   - Configurar el AbilityClass que implementa la lógica GAS de la skill
 *   - Asignar RequiredSkillIDs para construir el árbol de habilidades
 */
UCLASS(BlueprintType)
class EMBERVEILCORE_API UEMBERVEILSwordSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEMBERVEILSwordSkillDataAsset();

	// ─── Identidad ────────────────────────────────────────────────────────────

	/** Identificador único de la skill. Debe ser único en todo el proyecto (e.g. "Sword_WindSlash") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	FName SkillID;

	/** Nombre localizable que se muestra en la UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	FText SkillName;

	/** Descripción detallada para tooltips y el árbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity", Meta = (MultiLine = true))
	FText SkillDescription;

	/** Icono que aparece en el AbilityBar y en el árbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	TSoftObjectPtr<UTexture2D> Icon;

	// ─── Clasificación ────────────────────────────────────────────────────────

	/** Categoría funcional: Active, Passive o Ultimate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification")
	ESwordSkillCategory Category = ESwordSkillCategory::Active;

	/** Tipo de arma que debe estar equipada para poder usar esta skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification")
	EWeaponType RequiredWeaponType = EWeaponType::Sword;

	/** Nivel mínimo de maestría con el arma requerida para desbloquear esta skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification", Meta = (ClampMin = 0, ClampMax = 20))
	int32 RequiredMasteryLevel = 1;

	// ─── Gameplay Ability ─────────────────────────────────────────────────────

	/** Clase de GameplayAbility que implementa el comportamiento de la skill en GAS */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|GAS")
	TSubclassOf<UEMBERVEILGameplayAbility> AbilityClass;

	/** Nivel con el que se otorga la ability al ASC */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|GAS", Meta = (ClampMin = 1))
	int32 AbilityLevel = 1;

	// ─── Costes y tiempos ─────────────────────────────────────────────────────

	/** Coste de maná al activar la skill (referencial; el coste real lo gestiona el GE de la ability) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float ManaCost = 0.f;

	/** Coste de estamina al activar la skill (referencial) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float StaminaCost = 0.f;

	/** Tiempo de recarga en segundos (referencial; el cooldown real lo gestiona el GE de la ability) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float Cooldown = 0.f;

	// ─── Árbol de habilidades ─────────────────────────────────────────────────

	/** Skills que deben estar desbloqueadas antes de poder desbloquear esta (prerequisitos directos) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	TArray<FName> RequiredSkillIDs;

	/** Icono secundario usado en la vista del árbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	TSoftObjectPtr<UTexture2D> SkillTreeIcon;

	/** Posición 2D de este nodo dentro del widget del árbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	FVector2D SkillTreePosition = FVector2D::ZeroVector;

	// ─── UPrimaryDataAsset interface ──────────────────────────────────────────

	/**
	 * Tipo primario "SwordSkill" — permite al AssetManager cargar y filtrar
	 * todas las skills del juego con GetPrimaryAssetIdList("SwordSkill").
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
