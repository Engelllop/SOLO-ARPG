#include "AbilitySystem/GameplayEffects/GE_LevelUpRestore.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameplayEffectTypes.h"

UEMBERVEIL_GE_LevelUpRestore::UEMBERVEIL_GE_LevelUpRestore()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = UEMBERVEILAttributeSet::GetHealthAttribute();
		Mod.ModifierOp = EGameplayModOp::Additive;
		FSetByCallerFloat HPCaller;
		HPCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreHP"));
		Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(HPCaller);
		Modifiers.Add(Mod);
	}
	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = UEMBERVEILAttributeSet::GetManaAttribute();
		Mod.ModifierOp = EGameplayModOp::Additive;
		FSetByCallerFloat ManaCaller;
		ManaCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreMana"));
		Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaCaller);
		Modifiers.Add(Mod);
	}
}
