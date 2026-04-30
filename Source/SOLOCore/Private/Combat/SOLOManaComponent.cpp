#include "Combat/SOLOManaComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"

USOLOManaComponent::USOLOManaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USOLOManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastSpell += DeltaTime;

	if (FatigueLevel > 0 && TimeSinceLastSpell >= FatigueRecoveryTime)
	{
		FatigueLevel = FMath::Max(0, FatigueLevel - 1);
		TimeSinceLastSpell = 0.f;
		if (FatigueLevel == 0) ConsecutiveSpells = 0;
		OnFatigueChanged.Broadcast(FatigueLevel);
	}
}

void USOLOManaComponent::OnSpellCast()
{
	ConsecutiveSpells++;
	TimeSinceLastSpell = 0.f;

	if (ConsecutiveSpells >= SpellsBeforeFatigue)
	{
		IncreaseFatigue();
		ConsecutiveSpells = 0;
	}
}

bool USOLOManaComponent::TryOverload(float SpellManaCost)
{
	auto* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASI) return false;
	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return false;
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs) return false;

	float HPCost = SpellManaCost * OverloadHPMultiplier;
	float BonusDamage = Attrs->GetMaxHealth() * OverloadBonusDamagePercent;
	float TotalCost = HPCost + BonusDamage;

	if (Attrs->GetHealth() < TotalCost + 1.f) return false;

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -TotalCost);
	return true;
}

void USOLOManaComponent::ResetFatigue()
{
	FatigueLevel = 0;
	ConsecutiveSpells = 0;
	TimeSinceLastSpell = 0.f;
	OnFatigueChanged.Broadcast(0);
}

float USOLOManaComponent::GetManaRegenPenalty() const
{
	switch (FatigueLevel)
	{
	case 1: return 0.2f;
	case 2: return 0.35f;
	case 3: return 0.5f;
	default: return 0.f;
	}
}

float USOLOManaComponent::GetSpellDamagePenalty() const
{
	return FatigueLevel >= 2 ? 0.15f : 0.f;
}

float USOLOManaComponent::GetManaCostMultiplier() const
{
	return FatigueLevel >= 3 ? 1.5f : 1.0f;
}

bool USOLOManaComponent::ShouldSpellFail() const
{
	if (FatigueLevel < 3) return false;
	return FMath::FRand() < 0.05f;
}

void USOLOManaComponent::IncreaseFatigue()
{
	FatigueLevel = FMath::Min(3, FatigueLevel + 1);
	OnFatigueChanged.Broadcast(FatigueLevel);
}
