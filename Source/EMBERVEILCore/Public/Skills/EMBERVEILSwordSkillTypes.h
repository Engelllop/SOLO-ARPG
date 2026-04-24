#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "EMBERVEILSwordSkillTypes.generated.h"

// Forward declaration — definida en EMBERVEILSwordSkillDataAsset.h
class UEMBERVEILSwordSkillDataAsset;

// ─── ENUMS ────────────────────────────────────────────────────────────────────

/** Tipo de arma para el sistema de maestría */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None		UMETA(DisplayName = "None"),
	Sword		UMETA(DisplayName = "Sword"),
	GreatSword	UMETA(DisplayName = "Great Sword"),
	Dagger		UMETA(DisplayName = "Dagger"),
	Spear		UMETA(DisplayName = "Spear"),
	Axe			UMETA(DisplayName = "Axe"),
	Bow			UMETA(DisplayName = "Bow"),
	Staff		UMETA(DisplayName = "Staff"),
	Shield		UMETA(DisplayName = "Shield")
};

/** Categoría funcional de una Sword Skill */
UENUM(BlueprintType)
enum class ESwordSkillCategory : uint8
{
	Active		UMETA(DisplayName = "Active"),	  // Consume stamina/mana al activarse
	Passive		UMETA(DisplayName = "Passive"),	  // Siempre activa al desbloquear
	Ultimate	UMETA(DisplayName = "Ultimate")   // Cooldown largo, coste alto
};

// ─── STRUCTS ──────────────────────────────────────────────────────────────────

/**
 * Representa una skill asignada a un slot concreto del AbilityBar (índices 0-5).
 * Almacena la referencia al DataAsset y el handle de la ability en el ASC
 * para poder activarla y revocarla eficientemente.
 */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FAbilitySlot
{
	GENERATED_BODY()

	/** Índice del slot en el AbilityBar (0-5). -1 = slot vacío */
	UPROPERTY(BlueprintReadOnly, Category = "AbilitySlot")
	int32 SlotIndex = -1;

	/** DataAsset de la skill asignada a este slot */
	UPROPERTY(BlueprintReadOnly, Category = "AbilitySlot")
	TSoftObjectPtr<UEMBERVEILSwordSkillDataAsset> SkillData;

	/** Handle que devuelve el ASC al otorgar la ability; necesario para activarla y revocarla */
	UPROPERTY(BlueprintReadOnly, Category = "AbilitySlot")
	FGameplayAbilitySpecHandle AbilityHandle;

	/** Devuelve true si el slot tiene un índice válido y una skill asignada */
	bool IsValid() const { return SlotIndex >= 0 && !SkillData.IsNull(); }
};

/**
 * Guarda el progreso de maestría de un tipo de arma concreto.
 * MasteryLevel va de 0 a 20; la XP necesaria escala con Level^1.5 * 100.
 */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FWeaponMasteryData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Mastery")
	EWeaponType WeaponType = EWeaponType::None;

	UPROPERTY(BlueprintReadOnly, Category = "Mastery")
	float CurrentXP = 0.f;

	/** Nivel actual (0–20). A nivel 20 se alcanza la maestría máxima */
	UPROPERTY(BlueprintReadOnly, Category = "Mastery")
	int32 MasteryLevel = 0;

	/**
	 * Cantidad de XP necesaria para subir al siguiente nivel.
	 * Fórmula: 100 * (Level + 1)^1.5  — así el nivel 1 cuesta ~283 XP, el 20 ~9526 XP.
	 */
	float GetXPForNextLevel() const
	{
		if (MasteryLevel >= 20)
		{
			return TNumericLimits<float>::Max();
		}
		const float NextLevel = static_cast<float>(MasteryLevel + 1);
		return 100.f * FMath::Pow(NextLevel, 1.5f);
	}
};
