#include "AbilitySystem/SOLOMagicDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Data/SOLODamageTypes.h"

struct FMagicDmgStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMultiplier);

	FMagicDmgStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, MagicDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, Intelligence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, MagicResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, CritMultiplier, Source, false);
	}
};

static const FMagicDmgStatics& MagicDmgStatics()
{
	static FMagicDmgStatics Statics;
	return Statics;
}

USOLOMagicDamageExecution::USOLOMagicDamageExecution()
{
	RelevantAttributesToCapture.Add(MagicDmgStatics().MagicDamageDef);
	RelevantAttributesToCapture.Add(MagicDmgStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(MagicDmgStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(MagicDmgStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(MagicDmgStatics().CritMultiplierDef);
}

void USOLOMagicDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (!SourceASC || !TargetASC) return;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float MagPow = 0.f, Int = 0.f, MagRes = 0.f, CritCh = 0.f, CritMult = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDmgStatics().MagicDamageDef, EvalParams, MagPow);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDmgStatics().IntelligenceDef, EvalParams, Int);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDmgStatics().MagicResistanceDef, EvalParams, MagRes);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDmgStatics().CritChanceDef, EvalParams, CritCh);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDmgStatics().CritMultiplierDef, EvalParams, CritMult);

	float SpellMultiplier = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.SpellMultiplier")), false, 1.0f);

	float BaseDamage = MagPow * SpellMultiplier;
	float ResReduction = 1.f - (MagRes / (MagRes + 300.f));
	float Damage = BaseDamage * ResReduction;

	// Elemental interactions: Wet + Electric = x2
	FGameplayTag DmgTypeTag = Spec.GetDynamicAssetTags().GetByIndex(0);
	if (EvalParams.TargetTags)
	{
		bool bTargetWet = EvalParams.TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Wet")));
		if (bTargetWet)
		{
			if (Spec.GetDynamicAssetTags().HasTag(SOLODamageTags::Damage_Electric))
			{
				Damage *= 2.0f;
			}
			else if (Spec.GetDynamicAssetTags().HasTag(SOLODamageTags::Damage_Ice))
			{
				// Instant freeze handled by status effect handler
				Damage *= 1.5f;
			}
		}
	}

	// Crit
	float TotalCritChance = CritCh + 5.f + (Int * 0.05f);
	if (FMath::FRandRange(0.f, 100.f) < TotalCritChance)
	{
		Damage *= FMath::Max(CritMult, 1.5f);
	}

	// Vulnerability/Resistance
	if (EvalParams.TargetTags && EvalParams.TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Vulnerability.Magic"))))
	{
		Damage *= 1.25f;
	}
	if (EvalParams.TargetTags && EvalParams.TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Resistance.Magic"))))
	{
		Damage *= 0.75f;
	}

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USOLOAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage));
	}
}
