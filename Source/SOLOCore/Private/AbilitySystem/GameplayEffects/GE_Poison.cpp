#include "AbilitySystem/GameplayEffects/GE_Poison.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagsModule.h"

UGE_Poison::UGE_Poison()
{
	// Set the StatusEffectBase UPROPERTY fields that PostInitProperties reads.
	// These values must be set before PostInitProperties runs so the base class
	// duration/tag logic picks them up correctly.
	StatusTag    = FGameplayTag::RequestGameplayTag(FName("State.Poisoned"));
	Duration     = 5.f;
	TickInterval = 1.f;
	// DamagePerTick is left at 0 intentionally â€” we install the SetByCaller
	// modifier manually in PostInitProperties, bypassing the flat-scalar path
	// in the base class.
	DamagePerTick = 0.f;
}

void UGE_Poison::PostInitProperties()
{
	// Let the base class handle: DurationPolicy, DurationMagnitude, GrantedTags.
	// Because DamagePerTick == 0 the base class will NOT add a modifier, so we
	// are free to add the SetByCaller modifier ourselves below.
	Super::PostInitProperties();

	// Only configure the CDO, not runtime instances.
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	// â”€â”€ Stacking â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	StackingType              = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount           = 3;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	StackPeriodResetPolicy    = EGameplayEffectStackingPeriodPolicy::ResetOnSuccessfulApplication;

	// â”€â”€ Periodic tick â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	Period                          = TickInterval;   // 1 second
	bExecutePeriodicEffectOnApplication = false;

	// â”€â”€ SetByCaller damage modifier â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
	// The caller (GA_BasicAttack) is expected to call
	//   UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, PoisonDamageTag, Value)
	// before applying the spec. The default tag magnitude is 5.0 â€” GAS uses that
	// value when no SetByCaller assignment has been made for this tag.
	const FGameplayTag PoisonDamageTag =
		FGameplayTag::RequestGameplayTag(FName("Damage.Type.Poison"));

	FSetByCallerFloat SetByCallerData;
	SetByCallerData.DataTag  = PoisonDamageTag;
	SetByCallerData.DataName = NAME_None;

	FGameplayModifierInfo DamageModifier;
	DamageModifier.Attribute         = USOLOAttributeSet::GetIncomingDamageAttribute();
	DamageModifier.ModifierOp        = EGameplayModOp::Additive;
	DamageModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(SetByCallerData);

	Modifiers.Add(DamageModifier);
}
