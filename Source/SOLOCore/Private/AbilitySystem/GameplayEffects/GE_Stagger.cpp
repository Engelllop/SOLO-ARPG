#include "AbilitySystem/GameplayEffects/GE_Stagger.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagsModule.h"

UGE_Stagger::UGE_Stagger()
{
	// ── Duration ──────────────────────────────────────────────────────────────
	DurationPolicy    = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(0.8f));

	// ── Granted tag: State.Stunned ────────────────────────────────────────────
	// InheritableOwnedTagsContainer is the UE5 GAS mechanism for granting tags
	// to the target while the effect is active and removing them on expiry.
	const FGameplayTag StunnedTag =
		FGameplayTag::RequestGameplayTag(FName("State.Stunned"));

	FInheritedTagContainer TagContainer;
	TagContainer.Added.AddTag(StunnedTag);
	InheritableOwnedTagsContainer = TagContainer;

	// No attribute modifiers — stagger is a pure CC effect.
}
