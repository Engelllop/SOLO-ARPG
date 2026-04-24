#include "AbilitySystem/EMBERVEILDamageExecCalc.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Player/EMBERVEILPlayerController.h"
#include "UI/HUD/EMBERVEILHUDBase.h"

struct FEMBERVEILDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritMultiplier);

	FEMBERVEILDamageStatics()
	{
		// Capture AttackPower, Crit from source
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEMBERVEILAttributeSet, AttackDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEMBERVEILAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEMBERVEILAttributeSet, CritMultiplier, Source, false);
		
		// Capture Defenses from target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEMBERVEILAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEMBERVEILAttributeSet, MagicResistance, Target, false);
	}
};

static const FEMBERVEILDamageStatics& DamageStatics()
{
	static FEMBERVEILDamageStatics Statics;
	return Statics;
}

UEMBERVEILDamageExecCalc::UEMBERVEILDamageExecCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritMultiplierDef);
}

void UEMBERVEILDamageExecCalc::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDamageDef, EvalParams, AttackPower);
	AttackPower = FMath::Max<float>(AttackPower, 0.0f);

	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvalParams, Defense);
	Defense = FMath::Max<float>(Defense, 0.0f);

	float MagicResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MagicResistanceDef, EvalParams, MagicResistance);
	MagicResistance = FMath::Max<float>(MagicResistance, 0.0f);

	float CritChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvalParams, CritChance);
	CritChance = FMath::Max<float>(CritChance, 0.0f);

	float CritMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritMultiplierDef, EvalParams, CritMultiplier);
	CritMultiplier = FMath::Max<float>(CritMultiplier, 1.0f);

	// Capture magnitudes for different damage types via SetByCaller
	const float PhysicalDmg  = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Physical")), false, 0.f);
	const float FireDmg      = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Fire")),     false, 0.f);
	const float IceDmg       = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Ice")),      false, 0.f);
	const float LightningDmg = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Lightning")),false, 0.f);
	const float PoisonDmg    = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Poison")),   false, 0.f);
	const float DarkDmg      = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Dark")),     false, 0.f);
	const float HolyDmg      = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Holy")),     false, 0.f);

	// Calculate base physical damage (Mitigation formula: Dmg * 100 / (100 + Defense))
	float MitigatedPhysicalDamage = (PhysicalDmg + AttackPower) * (100.f / (100.f + Defense));
	if (PhysicalDmg <= 0.f)
	{
		// Only apply attack power if there's actually a physical attack
		MitigatedPhysicalDamage = 0.f;
	}

	// Calculate base magical damage
	float TotalMagicDmg = FireDmg + IceDmg + LightningDmg + PoisonDmg + DarkDmg + HolyDmg;
	float MitigatedMagicDamage = TotalMagicDmg * (100.f / (100.f + MagicResistance));

	float FinalDamage = MitigatedPhysicalDamage + MitigatedMagicDamage;

	// Apply Critical Hit
	bool bIsCriticalHit = false;
	if (CritChance > 0.f && FMath::FRand() < CritChance)
	{
		bIsCriticalHit = true;
		FinalDamage *= CritMultiplier;
	}

	// Apply minimum damage if any damage was intended
	if (PhysicalDmg > 0.f || TotalMagicDmg > 0.f)
	{
		FinalDamage = FMath::Max<float>(FinalDamage, 1.0f);
	}

	// Output to IncomingDamage attribute (Not Health directly)
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			UEMBERVEILAttributeSet::GetIncomingDamageAttribute(),
			EGameplayModOp::Additive,
			FinalDamage
		)
	);
}
