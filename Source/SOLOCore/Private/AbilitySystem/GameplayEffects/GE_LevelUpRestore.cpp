#include "AbilitySystem/GameplayEffects/GE_LevelUpRestore.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameplayEffectTypes.h"

USOLO_GE_LevelUpRestore::USOLO_GE_LevelUpRestore()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = USOLOAttributeSet::GetHealthAttribute();
		Mod.ModifierOp = EGameplayModOp::Additive;
		FSetByCallerFloat HPCaller;
		HPCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreHP"));
		Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(HPCaller);
		Modifiers.Add(Mod);
	}
	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = USOLOAttributeSet::GetManaAttribute();
		Mod.ModifierOp = EGameplayModOp::Additive;
		FSetByCallerFloat ManaCaller;
		ManaCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.LevelUpRestoreMana"));
		Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaCaller);
		Modifiers.Add(Mod);
	}
}
