#include "AI/SOLOThreatComponent.h"
#include "GameFramework/Actor.h"

USOLOThreatComponent::USOLOThreatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHighestThreatTarget = nullptr;
}

void USOLOThreatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USOLOThreatComponent::AddThreat(AActor* Instigator, float Amount)
{
	if (!Instigator || Amount == 0.f) return;

	float CurrentThreat = 0.f;
	if (const float* FoundThreat = ThreatTable.Find(Instigator))
	{
		CurrentThreat = *FoundThreat;
	}

	// Sumamos el threat asegurÃ¡ndonos de que no baje de cero
	float NewThreat = FMath::Max(0.f, CurrentThreat + Amount);
	ThreatTable.Add(Instigator, NewThreat);

	EvaluateHighestThreatTarget();
}

void USOLOThreatComponent::RemoveTarget(AActor* TargetToRemove)
{
	if (!TargetToRemove) return;

	if (ThreatTable.Remove(TargetToRemove) > 0)
	{
		if (CurrentHighestThreatTarget == TargetToRemove)
		{
			EvaluateHighestThreatTarget();
		}
	}
}

AActor* USOLOThreatComponent::GetHighestThreatTarget() const
{
	return CurrentHighestThreatTarget;
}

float USOLOThreatComponent::GetThreatValue(AActor* Target) const
{
	if (Target)
	{
		if (const float* FoundThreat = ThreatTable.Find(Target))
		{
			return *FoundThreat;
		}
	}
	return 0.f;
}

void USOLOThreatComponent::ClearAllThreat()
{
	ThreatTable.Empty();
	if (CurrentHighestThreatTarget != nullptr)
	{
		CurrentHighestThreatTarget = nullptr;
		OnHighestThreatTargetChanged.Broadcast(nullptr);
	}
}

void USOLOThreatComponent::EvaluateHighestThreatTarget()
{
	AActor* BestTarget = nullptr;
	float HighestThreat = -1.f;

	for (const auto& Pair : ThreatTable)
	{
		AActor* PotentialTarget = Pair.Key;
		float TargetThreat = Pair.Value;

		// Si el actor es vÃ¡lido y no estÃ¡ marcado para destrucciÃ³n
		if (IsValid(PotentialTarget))
		{
			if (TargetThreat > HighestThreat)
			{
				HighestThreat = TargetThreat;
				BestTarget = PotentialTarget;
			}
		}
	}

	// Limpieza perezosa: si habÃ­a actores invÃ¡lidos, los borramos (opcional)
	// Para mayor robustez, se recomienda vincular al evento OnDestroyed del Instigator

	if (BestTarget != CurrentHighestThreatTarget)
	{
		CurrentHighestThreatTarget = BestTarget;
		OnHighestThreatTargetChanged.Broadcast(CurrentHighestThreatTarget);
	}
}
