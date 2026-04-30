#include "AI/SOLOBossPhaseComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

USOLOBossPhaseComponent::USOLOBossPhaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLOBossPhaseComponent::BeginPlay()
{
	Super::BeginPlay();
	if (PhaseDataTable)
	{
		TArray<FBossPhaseData*> Rows;
		PhaseDataTable->GetAllRows<FBossPhaseData>(TEXT("BossPhase"), Rows);
		for (FBossPhaseData* Row : Rows)
			if (Row) Phases.Add(*Row);
		Phases.Sort([](const FBossPhaseData& A, const FBossPhaseData& B)
			{ return A.HPThreshold > B.HPThreshold; });
	}
}

void USOLOBossPhaseComponent::CheckPhaseTransition(float CurrentHP, float MaxHP)
{
	if (Phases.IsEmpty() || MaxHP <= 0.f) return;
	float HPPct = CurrentHP / MaxHP;

	for (int32 i = CurrentPhaseIndex + 1; i < Phases.Num(); i++)
	{
		if (HPPct <= Phases[i].HPThreshold)
		{
			EnterPhase(i);
			break;
		}
	}
}

void USOLOBossPhaseComponent::EnterPhase(int32 PhaseIndex)
{
	CurrentPhaseIndex = PhaseIndex;
	const FBossPhaseData& Phase = Phases[PhaseIndex];

	// Grant phase abilities to owner
	if (auto* IASI = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			for (const TSubclassOf<UGameplayAbility>& AbilityClass : Phase.PhaseAbilities)
			{
				if (AbilityClass)
				{
					FGameplayAbilitySpec Spec(AbilityClass, 1);
					ASC->GiveAbility(Spec);
				}
			}
		}
	}

	OnPhaseChanged.Broadcast(PhaseIndex);
}

const FBossPhaseData* USOLOBossPhaseComponent::GetCurrentPhaseData() const
{
	return Phases.IsValidIndex(CurrentPhaseIndex) ? &Phases[CurrentPhaseIndex] : nullptr;
}
