#include "Combat/SOLORespawnComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Inventory/SOLOInventoryComponent.h"
#include "GameFramework/Character.h"

USOLORespawnComponent::USOLORespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLORespawnComponent::EnterAgony(AActor* Killer)
{
	if (bIsInAgony) return;
	bIsInAgony = true;

	if (auto* Char = Cast<ACharacter>(GetOwner()))
	{
		Char->GetCharacterMovement()->DisableMovement();
	}

	GetWorld()->GetTimerManager().SetTimer(AgonyTimerHandle, this, &USOLORespawnComponent::OnAgonyTimerExpired, AgonyDuration, false);
	OnAgonyStarted.Broadcast(Killer);
}

void USOLORespawnComponent::StartRevive(AActor* Reviver)
{
	if (!bIsInAgony || bIsBeingRevived) return;
	bIsBeingRevived = true;
	CurrentReviver = Reviver;
	GetWorld()->GetTimerManager().SetTimer(ReviveTimerHandle, this, &USOLORespawnComponent::CompleteRevive, ReviveChannelTime, false);
}

void USOLORespawnComponent::CompleteRevive()
{
	if (!bIsInAgony) return;

	bIsInAgony = false;
	bIsBeingRevived = false;
	CurrentReviver = nullptr;

	GetWorld()->GetTimerManager().ClearTimer(AgonyTimerHandle);
	RestoreVitals();

	if (auto* Char = Cast<ACharacter>(GetOwner()))
	{
		Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	OnRevived.Broadcast();
}

void USOLORespawnComponent::InterruptRevive()
{
	if (!bIsBeingRevived) return;
	bIsBeingRevived = false;
	CurrentReviver = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(ReviveTimerHandle);
}

void USOLORespawnComponent::RespawnAtLastCampfire()
{
	bIsInAgony = false;
	bIsBeingRevived = false;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (LastCampfire.IsValid())
	{
		if (auto* Char = Cast<ACharacter>(GetOwner()))
		{
			Char->SetActorLocation(LastCampfire->GetActorLocation() + FVector(0, 0, 100.f));
			Char->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}

	RestoreVitals();
	OnRespawned.Broadcast();
}

void USOLORespawnComponent::ApplyDeathPenalty()
{
	auto* InvComp = GetOwner()->FindComponentByClass<USOLOInventoryComponent>();
	if (InvComp)
	{
		int32 Penalty = FMath::FloorToInt(InvComp->Currency * DeathGoldPenalty);
		InvComp->SpendCurrency(Penalty);
	}
}

void USOLORespawnComponent::SetLastCampfire(AActor* Campfire)
{
	LastCampfire = Campfire;
}

void USOLORespawnComponent::OnAgonyTimerExpired()
{
	if (!bIsBeingRevived)
	{
		ApplyDeathPenalty();
		RespawnAtLastCampfire();
		OnAgonyExpired.Broadcast();
	}
}

void USOLORespawnComponent::RestoreVitals()
{
	auto* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (!ASI) return;
	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs) return;

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Override, Attrs->GetMaxHealth() * ReviveHPPercent);
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Override, Attrs->GetMaxMana() * ReviveMPPercent);
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Override, Attrs->GetMaxStamina());
}
