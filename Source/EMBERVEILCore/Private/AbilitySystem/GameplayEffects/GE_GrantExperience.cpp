#include "AbilitySystem/GameplayEffects/GE_GrantExperience.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameplayEffectTypes.h"

UEMBERVEIL_GE_GrantExperience::UEMBERVEIL_GE_GrantExperience()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UEMBERVEILAttributeSet::GetExperienceAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat XPCaller;
	XPCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.XPReward"));
	Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(XPCaller);
	Modifiers.Add(Mod);
}
