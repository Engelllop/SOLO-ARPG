#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skills/SOLOSwordSkillTypes.h"
#include "SOLOSwordSkillDataAsset.generated.h"

class USOLOGameplayAbility;
class UTexture2D;

/**
 * DataAsset que define completamente una Sword Skill.
 *
 * Cada skill en el juego se crea como una instancia de este asset en el editor.
 * El tipo primario es "SwordSkill", lo que permite al AssetManager cargar
 * todas las skills de forma asÃ­ncrona y filtrarlas por tipo.
 *
 * Uso habitual:
 *   - Crear un BP basado en este asset en Content/Skills/
 *   - Asignarle un SkillID Ãºnico (e.g. "Sword_WindSlash")
 *   - Configurar el AbilityClass que implementa la lÃ³gica GAS de la skill
 *   - Asignar RequiredSkillIDs para construir el Ã¡rbol de habilidades
 */
UCLASS(BlueprintType)
class SOLOCORE_API USOLOSwordSkillDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USOLOSwordSkillDataAsset();

	// â”€â”€â”€ Identidad â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Identificador Ãºnico de la skill. Debe ser Ãºnico en todo el proyecto (e.g. "Sword_WindSlash") */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	FName SkillID;

	/** Nombre localizable que se muestra en la UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	FText SkillName;

	/** DescripciÃ³n detallada para tooltips y el Ã¡rbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity", Meta = (MultiLine = true))
	FText SkillDescription;

	/** Icono que aparece en el AbilityBar y en el Ã¡rbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Identity")
	TSoftObjectPtr<UTexture2D> Icon;

	// â”€â”€â”€ ClasificaciÃ³n â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** CategorÃ­a funcional: Active, Passive o Ultimate */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification")
	ESwordSkillCategory Category = ESwordSkillCategory::Active;

	/** Tipo de arma que debe estar equipada para poder usar esta skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification")
	EWeaponType RequiredWeaponType = EWeaponType::Sword;

	/** Nivel mÃ­nimo de maestrÃ­a con el arma requerida para desbloquear esta skill */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Classification", Meta = (ClampMin = 0, ClampMax = 20))
	int32 RequiredMasteryLevel = 1;

	// â”€â”€â”€ Gameplay Ability â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Clase de GameplayAbility que implementa el comportamiento de la skill en GAS */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|GAS")
	TSubclassOf<USOLOGameplayAbility> AbilityClass;

	/** Nivel con el que se otorga la ability al ASC */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|GAS", Meta = (ClampMin = 1))
	int32 AbilityLevel = 1;

	// â”€â”€â”€ Costes y tiempos â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Coste de manÃ¡ al activar la skill (referencial; el coste real lo gestiona el GE de la ability) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float ManaCost = 0.f;

	/** Coste de estamina al activar la skill (referencial) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float StaminaCost = 0.f;

	/** Tiempo de recarga en segundos (referencial; el cooldown real lo gestiona el GE de la ability) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|Costs", Meta = (ClampMin = 0.f))
	float Cooldown = 0.f;

	// â”€â”€â”€ Ãrbol de habilidades â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/** Skills que deben estar desbloqueadas antes de poder desbloquear esta (prerequisitos directos) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	TArray<FName> RequiredSkillIDs;

	/** Icono secundario usado en la vista del Ã¡rbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	TSoftObjectPtr<UTexture2D> SkillTreeIcon;

	/** PosiciÃ³n 2D de este nodo dentro del widget del Ã¡rbol de habilidades */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill|SkillTree")
	FVector2D SkillTreePosition = FVector2D::ZeroVector;

	// â”€â”€â”€ UPrimaryDataAsset interface â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

	/**
	 * Tipo primario "SwordSkill" â€” permite al AssetManager cargar y filtrar
	 * todas las skills del juego con GetPrimaryAssetIdList("SwordSkill").
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
