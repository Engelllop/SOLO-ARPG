#include "AbilitySystem/GameplayEffects/GE_StatusEffectBase.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayModMagnitudeCalculation.h"

UGE_StatusEffectBase::UGE_StatusEffectBase()
{
	// Sensible defaults — PostInitProperties finalizes once CDO properties are set
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}

void UGE_StatusEffectBase::PostInitProperties()
{
	Super::PostInitProperties();

	// PostInitProperties runs on CDO and on each new instance.
	// We only want to reconfigure when this is the CDO being initialised from defaults,
	// not every time the GE is applied at runtime.
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// --- Duration ---
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(Duration));

	// --- Status tag granted to the target ---
	if (StatusTag.IsValid())
	{
		FInheritedTagContainer TagContainer;
		TagContainer.Added.AddTag(StatusTag);
		InheritableOwnedTagsContainer = TagContainer;
	}

	// --- Periodic damage modifier (DoT only) ---
	if (DamagePerTick > 0.f)
	{
		Period = TickInterval;
		bExecutePeriodicEffectOnApplication = false;

		FGameplayModifierInfo DamageModifier;
		DamageModifier.Attribute   = UEMBERVEILAttributeSet::GetIncomingDamageAttribute();
		DamageModifier.ModifierOp  = EGameplayModOp::Additive;

		DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(DamagePerTick));

		Modifiers.Add(DamageModifier);
	}
}
