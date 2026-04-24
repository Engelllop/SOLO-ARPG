#include "AI/EMBERVEILThreatComponent.h"
#include "GameFramework/Actor.h"

UEMBERVEILThreatComponent::UEMBERVEILThreatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentHighestThreatTarget = nullptr;
}

void UEMBERVEILThreatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEMBERVEILThreatComponent::AddThreat(AActor* Instigator, float Amount)
{
	if (!Instigator || Amount == 0.f) return;

	float CurrentThreat = 0.f;
	if (const float* FoundThreat = ThreatTable.Find(Instigator))
	{
		CurrentThreat = *FoundThreat;
	}

	// Sumamos el threat asegurándonos de que no baje de cero
	float NewThreat = FMath::Max(0.f, CurrentThreat + Amount);
	ThreatTable.Add(Instigator, NewThreat);

	EvaluateHighestThreatTarget();
}

void UEMBERVEILThreatComponent::RemoveTarget(AActor* TargetToRemove)
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

AActor* UEMBERVEILThreatComponent::GetHighestThreatTarget() const
{
	return CurrentHighestThreatTarget;
}

float UEMBERVEILThreatComponent::GetThreatValue(AActor* Target) const
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

void UEMBERVEILThreatComponent::ClearAllThreat()
{
	ThreatTable.Empty();
	if (CurrentHighestThreatTarget != nullptr)
	{
		CurrentHighestThreatTarget = nullptr;
		OnHighestThreatTargetChanged.Broadcast(nullptr);
	}
}

void UEMBERVEILThreatComponent::EvaluateHighestThreatTarget()
{
	AActor* BestTarget = nullptr;
	float HighestThreat = -1.f;

	for (const auto& Pair : ThreatTable)
	{
		AActor* PotentialTarget = Pair.Key;
		float TargetThreat = Pair.Value;

		// Si el actor es válido y no está marcado para destrucción
		if (IsValid(PotentialTarget))
		{
			if (TargetThreat > HighestThreat)
			{
				HighestThreat = TargetThreat;
				BestTarget = PotentialTarget;
			}
		}
	}

	// Limpieza perezosa: si había actores inválidos, los borramos (opcional)
	// Para mayor robustez, se recomienda vincular al evento OnDestroyed del Instigator

	if (BestTarget != CurrentHighestThreatTarget)
	{
		CurrentHighestThreatTarget = BestTarget;
		OnHighestThreatTargetChanged.Broadcast(CurrentHighestThreatTarget);
	}
}
