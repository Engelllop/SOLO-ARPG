#include "World/EMBERVEILFactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

UEMBERVEILFactionComponent::UEMBERVEILFactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEMBERVEILFactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEMBERVEILFactionComponent, FactionReputation);
}

void UEMBERVEILFactionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEMBERVEILFactionComponent::AddReputation(FName FactionID, int32 Amount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (FactionID == NAME_None || Amount == 0) return;

	int32 NewRep = Amount;
	bool bFound = false;

	for (FEMBERVEILFactionReputationEntry& Entry : FactionReputation)
	{
		if (Entry.FactionID == FactionID)
		{
			Entry.Reputation += Amount;
			NewRep = Entry.Reputation;
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		FEMBERVEILFactionReputationEntry NewEntry;
		NewEntry.FactionID = FactionID;
		NewEntry.Reputation = Amount;
		FactionReputation.Add(NewEntry);
	}
	
	OnReputationChanged.Broadcast(FactionID, NewRep, Amount);
	
	// Force replication trigger
	GetOwner()->ForceNetUpdate();
}

int32 UEMBERVEILFactionComponent::GetReputation(FName FactionID) const
{
	for (const FEMBERVEILFactionReputationEntry& Entry : FactionReputation)
	{
		if (Entry.FactionID == FactionID)
		{
			return Entry.Reputation;
		}
	}
	return 0; // Neutral by default si no hay registro
}

EEMBERVEILAttitude UEMBERVEILFactionComponent::GetAttitudeTowards(AActor* OtherActor) const
{
	if (!OtherActor) return EEMBERVEILAttitude::Neutral;

	UEMBERVEILFactionComponent* OtherFactionComp = OtherActor->FindComponentByClass<UEMBERVEILFactionComponent>();
	if (!OtherFactionComp) return EEMBERVEILAttitude::Neutral; // Si no tiene facción, neutral

	// Si son de la misma facción base, amistosos por defecto
	if (BaseFactionID != NAME_None && BaseFactionID == OtherFactionComp->BaseFactionID)
	{
		return EEMBERVEILAttitude::Friendly;
	}

	// Consultar cómo me llevo con la facción base del otro
	return GetAttitudeTowardsFaction(OtherFactionComp->BaseFactionID);
}

EEMBERVEILAttitude UEMBERVEILFactionComponent::GetAttitudeTowardsFaction(FName OtherFactionID) const
{
	if (OtherFactionID == NAME_None) return EEMBERVEILAttitude::Neutral;

	int32 Rep = GetReputation(OtherFactionID);

	if (Rep <= HostileThreshold)
	{
		return EEMBERVEILAttitude::Hostile;
	}
	if (Rep >= FriendlyThreshold)
	{
		return EEMBERVEILAttitude::Friendly;
	}

	return EEMBERVEILAttitude::Neutral;
}

void UEMBERVEILFactionComponent::OnRep_FactionReputation()
{
	// Evento para UI cuando cambie la reputación replicada
	// No tenemos el 'Delta' en el cliente fácilmente por TMap, pero se puede actualizar el menú.
}
