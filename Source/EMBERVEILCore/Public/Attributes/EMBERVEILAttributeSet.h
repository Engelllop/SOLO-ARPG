#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EMBERVEILAttributeSet.generated.h"

// Macro estándar de GAS — genera getters, setters e inicializadores para cada atributo
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * AttributeSet principal de EMBERVEIL.
 * Contiene todos los stats del personaje (jugador y enemigos).
 * Vive en el PlayerState (jugadores) o en el Character (enemigos/NPCs).
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEMBERVEILAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/** XP necesaria para pasar de CurrentLevel a CurrentLevel+1: redondeo(100 * CurrentLevel^1.8). */
	UFUNCTION(BlueprintPure, Category = "Attributes|Progression")
	static float GetExperienceRequiredToAdvanceFromLevel(int32 CurrentLevel);

	/**
	 * Recalcula subidas de nivel a partir del Experience actual (servidor).
	 * Llamar tras cambiar Experience fuera de un GameplayEffect (p. ej. misiones, carga).
	 */
	UFUNCTION(BlueprintCallable, Category = "Attributes|Progression")
	void FlushExperienceLevelUps();

	// ─── VIDA ────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MaxHealth)

	// ─── MANÁ ────────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MaxMana)

	// ─── ESTAMINA ────────────────────────────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vital", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MaxStamina)

	// ─── STATS PRIMARIOS ─────────────────────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Vitality)
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Vitality)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Agility)
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Agility)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Spirit)
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Spirit)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Primary", ReplicatedUsing = OnRep_Luck)
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Luck)

	// ─── STATS SECUNDARIOS (derivados, modificados por GameplayEffects) ───────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_AttackDamage)
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, AttackDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_MagicDamage)
	FGameplayAttributeData MagicDamage;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MagicDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Defense)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_MagicResistance)
	FGameplayAttributeData MagicResistance;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MagicResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_CritChance)
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, CritChance)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_CritMultiplier)
	FGameplayAttributeData CritMultiplier;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, CritMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Secondary", ReplicatedUsing = OnRep_CooldownReduction)
	FGameplayAttributeData CooldownReduction;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, CooldownReduction)

	// ─── PROGRESIÓN ───────────────────────────────────────────────────────────
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Progression", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Level)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Progression", ReplicatedUsing = OnRep_Experience)
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, Experience)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Progression", ReplicatedUsing = OnRep_ExperienceToNextLevel)
	FGameplayAttributeData ExperienceToNextLevel;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, ExperienceToNextLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Progression", ReplicatedUsing = OnRep_UnspentStatPoints)
	FGameplayAttributeData UnspentStatPoints;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, UnspentStatPoints)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Progression", ReplicatedUsing = OnRep_UnspentTalentPoints)
	FGameplayAttributeData UnspentTalentPoints;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, UnspentTalentPoints)

	// ─── META-ATRIBUTOS (no se replican, solo sirven para cálculos internos de GE) ─
	// El daño entrante se acumula aquí antes de aplicarse a Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, IncomingDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, IncomingHealing)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingExperience;
	ATTRIBUTE_ACCESSORS(UEMBERVEILAttributeSet, IncomingExperience)

protected:
	// Clampea el valor dentro de [0, MaxValue] antes de que GAS lo aplique
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	/** Procesa una o varias subidas de nivel según Experience y la curva de XP (solo autoridad). */
	void ProcessExperienceLevelUps();

	UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldMana);
	UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION() void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION() void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	UFUNCTION() void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION() void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);
	UFUNCTION() void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION() void OnRep_Vitality(const FGameplayAttributeData& OldVitality);
	UFUNCTION() void OnRep_Agility(const FGameplayAttributeData& OldAgility);
	UFUNCTION() void OnRep_Spirit(const FGameplayAttributeData& OldSpirit);
	UFUNCTION() void OnRep_Luck(const FGameplayAttributeData& OldLuck);
	UFUNCTION() void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);
	UFUNCTION() void OnRep_MagicDamage(const FGameplayAttributeData& OldMagicDamage);
	UFUNCTION() void OnRep_Defense(const FGameplayAttributeData& OldDefense);
	UFUNCTION() void OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance);
	UFUNCTION() void OnRep_CritChance(const FGameplayAttributeData& OldCritChance);
	UFUNCTION() void OnRep_CritMultiplier(const FGameplayAttributeData& OldCritMultiplier);
	UFUNCTION() void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	UFUNCTION() void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	UFUNCTION() void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction);
	UFUNCTION() void OnRep_Level(const FGameplayAttributeData& OldLevel);
	UFUNCTION() void OnRep_Experience(const FGameplayAttributeData& OldExperience);
	UFUNCTION() void OnRep_ExperienceToNextLevel(const FGameplayAttributeData& OldExperienceToNextLevel);
	UFUNCTION() void OnRep_UnspentStatPoints(const FGameplayAttributeData& OldUnspentStatPoints);
	UFUNCTION() void OnRep_UnspentTalentPoints(const FGameplayAttributeData& OldUnspentTalentPoints);
};
