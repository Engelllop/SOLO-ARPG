#include "AbilitySystem/SOLOPhysicalDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Combat/SOLOPostureComponent.h"

struct FPhysDmgStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);

	FPhysDmgStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, AttackDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, CritMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USOLOAttributeSet, Dexterity, Source, false);
	}
};

static const FPhysDmgStatics& PhysDmgStatics()
{
	static FPhysDmgStatics Statics;
	return Statics;
}

USOLOPhysicalDamageExecution::USOLOPhysicalDamageExecution()
{
	RelevantAttributesToCapture.Add(PhysDmgStatics().AttackDamageDef);
	RelevantAttributesToCapture.Add(PhysDmgStatics().StrengthDef);
	RelevantAttributesToCapture.Add(PhysDmgStatics().DefenseDef);
	RelevantAttributesToCapture.Add(PhysDmgStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(PhysDmgStatics().CritMultiplierDef);
	RelevantAttributesToCapture.Add(PhysDmgStatics().DexterityDef);
}

void USOLOPhysicalDamageExecution::Execute_Implementation(
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

	float AttackDmg = 0.f, Str = 0.f, Def = 0.f, CritCh = 0.f, CritMult = 0.f, Dex = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().AttackDamageDef, EvalParams, AttackDmg);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().StrengthDef, EvalParams, Str);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().DefenseDef, EvalParams, Def);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().CritChanceDef, EvalParams, CritCh);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().CritMultiplierDef, EvalParams, CritMult);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(PhysDmgStatics().DexterityDef, EvalParams, Dex);

	float HitMultiplier = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.HitMultiplier")), false, 1.0f);

	float BaseDamage = AttackDmg * HitMultiplier;
	float DefReduction = 1.f - (Def / (Def + 300.f));
	float Damage = BaseDamage * DefReduction;

	// Crit check: base 5% + 0.1% per Dexterity point
	float TotalCritChance = CritCh + 5.f + (Dex * 0.1f);
	bool bIsCrit = FMath::FRandRange(0.f, 100.f) < TotalCritChance;

	// Auto-crit if target is stunned or posture broken
	if (!bIsCrit && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned"))))
	{
		bIsCrit = true;
	}

	if (!bIsCrit)
	{
		if (auto* PostureComp = TargetASC->GetOwner()->FindComponentByClass<USOLOPostureComponent>())
		{
			if (PostureComp->IsPostureBroken()) bIsCrit = true;
		}
	}

	if (bIsCrit)
	{
		float FinalCritMult = FMath::Max(CritMult, 1.5f);
		Damage *= FinalCritMult;
	}

	// Posture bonus damage
	if (auto* PostureComp = TargetASC->GetOwner()->FindComponentByClass<USOLOPostureComponent>())
	{
		Damage *= PostureComp->GetBonusDamageMultiplier();
	}

	// Vulnerability/Resistance tags
	if (EvalParams.TargetTags && EvalParams.TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Vulnerability.Physical"))))
	{
		Damage *= 1.25f;
	}
	if (EvalParams.TargetTags && EvalParams.TargetTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Resistance.Physical"))))
	{
		Damage *= 0.75f;
	}

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USOLOAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage));
	}
}
