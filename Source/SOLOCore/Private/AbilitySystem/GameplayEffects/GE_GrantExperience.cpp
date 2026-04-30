#include "AbilitySystem/GameplayEffects/GE_GrantExperience.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameplayEffectTypes.h"

USOLO_GE_GrantExperience::USOLO_GE_GrantExperience()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = USOLOAttributeSet::GetExperienceAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat XPCaller;
	XPCaller.DataTag = FGameplayTag::RequestGameplayTag(FName("Data.XPReward"));
	Mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(XPCaller);
	Modifiers.Add(Mod);
}
