#include "Combat/SOLODodgeComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"

USOLODodgeComponent::USOLODodgeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USOLODodgeComponent::StartDodge(FVector Direction)
{
	if (!CanDodge()) return false;
	if (!ConsumeSP()) return false;

	bIsDodging = true;
	bOnCooldown = true;

	if (Direction.IsNearlyZero())
	{
		if (auto* Char = Cast<ACharacter>(GetOwner()))
		{
			Direction = Char->GetActorForwardVector();
		}
	}
	Direction.Z = 0.f;
	Direction.Normalize();

	EnableIFrames();

	if (auto* Char = Cast<ACharacter>(GetOwner()))
	{
		Char->LaunchCharacter(Direction * DodgeDistance / DodgeDuration, true, true);
	}

	auto& TM = GetWorld()->GetTimerManager();
	TM.SetTimer(IFrameHandle, this, &USOLODodgeComponent::DisableIFrames, IFrameDuration, false);
	TM.SetTimer(DodgeEndHandle, this, &USOLODodgeComponent::OnDodgeEnd, DodgeDuration, false);
	TM.SetTimer(CooldownHandle, this, &USOLODodgeComponent::ResetCooldown, DodgeCooldown + DodgeDuration, false);

	OnDodgeStarted.Broadcast();
	return true;
}

bool USOLODodgeComponent::CanDodge() const
{
	if (bIsDodging || bOnCooldown) return false;

	auto* Owner = GetOwner();
	if (!Owner) return false;

	auto* ASI = Cast<IAbilitySystemInterface>(Owner);
	if (!ASI || !ASI->GetAbilitySystemComponent()) return false;

	const auto* Attrs = ASI->GetAbilitySystemComponent()->GetSet<USOLOAttributeSet>();
	if (!Attrs) return false;

	return Attrs->GetStamina() >= DodgeSPCost;
}

void USOLODodgeComponent::EnableIFrames()
{
	bInIFrames = true;
	if (auto* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (auto* ASC = ASI->GetAbilitySystemComponent())
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Invulnerable")));
		}
	}
}

void USOLODodgeComponent::DisableIFrames()
{
	bInIFrames = false;
	if (auto* ASI = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (auto* ASC = ASI->GetAbilitySystemComponent())
		{
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Invulnerable")));
		}
	}
}

void USOLODodgeComponent::OnDodgeEnd()
{
	bIsDodging = false;
	OnDodgeEnded.Broadcast();
}

void USOLODodgeComponent::ResetCooldown()
{
	bOnCooldown = false;
}

bool USOLODodgeComponent::ConsumeSP()
{
	auto* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASI) return false;

	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return false;

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -DodgeSPCost);
	return true;
}
