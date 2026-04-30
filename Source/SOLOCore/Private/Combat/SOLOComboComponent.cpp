#include "Combat/SOLOComboComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/Character.h"

USOLOComboComponent::USOLOComboComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USOLOComboComponent::StartLightCombo()
{
	if (bIsInCombo && CurrentAttackType == ESOLOAttackType::Light && CanContinueCombo())
	{
		return ExecuteNextStep();
	}

	if (bIsInCombo)
	{
		const auto& Steps = GetCurrentSteps();
		if (CurrentComboStep < Steps.Num() && Steps[CurrentComboStep].bIsUncancellable)
			return false;
	}

	ResetCombo();
	CurrentAttackType = ESOLOAttackType::Light;
	bIsInCombo = true;
	return ExecuteNextStep();
}

bool USOLOComboComponent::StartHeavyCombo()
{
	if (bIsInCombo && CurrentAttackType == ESOLOAttackType::Heavy && CanContinueCombo())
	{
		return ExecuteNextStep();
	}

	if (bIsInCombo)
	{
		const auto& Steps = GetCurrentSteps();
		if (CurrentComboStep < Steps.Num() && Steps[CurrentComboStep].bIsUncancellable)
			return false;
	}

	ResetCombo();
	CurrentAttackType = ESOLOAttackType::Heavy;
	bIsInCombo = true;
	return ExecuteNextStep();
}

void USOLOComboComponent::CancelCombo()
{
	const auto& Steps = GetCurrentSteps();
	if (CurrentComboStep > 0 && CurrentComboStep <= Steps.Num())
	{
		if (Steps[CurrentComboStep - 1].bIsUncancellable) return;
	}
	ResetCombo();
}

void USOLOComboComponent::OnComboStepCompleted()
{
	const auto& Steps = GetCurrentSteps();
	if (CurrentComboStep >= Steps.Num())
	{
		if (IsFinaleStep())
		{
			OnComboFinaleEvent.Broadcast();
		}
		ResetCombo();
	}
}

bool USOLOComboComponent::CanContinueCombo() const
{
	if (!bIsInCombo) return false;
	float TimeSince = GetWorld()->GetTimeSeconds() - LastAttackTime;
	return TimeSince <= CurrentComboData.ComboInputWindow;
}

bool USOLOComboComponent::IsFinaleStep() const
{
	const auto& Steps = GetCurrentSteps();
	return CurrentComboStep > 0 && CurrentComboStep == Steps.Num();
}

float USOLOComboComponent::GetCurrentStepDamage() const
{
	const auto& Steps = GetCurrentSteps();
	if (CurrentComboStep <= 0 || CurrentComboStep > Steps.Num()) return 1.0f;
	float Dmg = Steps[CurrentComboStep - 1].DamageMultiplier;
	if (Steps[CurrentComboStep - 1].bIsFinale)
	{
		Dmg *= CurrentComboData.FinaleDamageBonus;
	}
	return Dmg;
}

float USOLOComboComponent::GetCurrentStepPostureDamage() const
{
	const auto& Steps = GetCurrentSteps();
	if (CurrentComboStep <= 0 || CurrentComboStep > Steps.Num()) return 1.0f;
	return Steps[CurrentComboStep - 1].PostureMultiplier;
}

void USOLOComboComponent::SetWeaponComboData(const FWeaponComboData& InData)
{
	CurrentComboData = InData;
	ResetCombo();
}

bool USOLOComboComponent::ExecuteNextStep()
{
	const auto& Steps = GetCurrentSteps();
	if (CurrentComboStep >= Steps.Num()) return false;

	const FComboStep& Step = Steps[CurrentComboStep];
	if (!ConsumeSPForStep(Step)) return false;

	CurrentComboStep++;
	LastAttackTime = GetWorld()->GetTimeSeconds();

	if (auto* Char = Cast<ACharacter>(GetOwner()))
	{
		if (CurrentComboData.ComboMontage && Char->GetMesh()->GetAnimInstance())
		{
			Char->GetMesh()->GetAnimInstance()->Montage_Play(CurrentComboData.ComboMontage);
			Char->GetMesh()->GetAnimInstance()->Montage_JumpToSection(Step.MontageSection, CurrentComboData.ComboMontage);
		}
	}

	OnComboStepEvent.Broadcast(CurrentComboStep, Steps.Num());
	return true;
}

void USOLOComboComponent::ResetCombo()
{
	CurrentComboStep = 0;
	bIsInCombo = false;
	OnComboResetEvent.Broadcast();
}

const TArray<FComboStep>& USOLOComboComponent::GetCurrentSteps() const
{
	return CurrentAttackType == ESOLOAttackType::Light
		? CurrentComboData.LightComboSteps
		: CurrentComboData.HeavyComboSteps;
}

bool USOLOComboComponent::ConsumeSPForStep(const FComboStep& Step)
{
	auto* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASI) return false;
	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return false;
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs || Attrs->GetStamina() < Step.SPCost) return false;
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -Step.SPCost);
	return true;
}
