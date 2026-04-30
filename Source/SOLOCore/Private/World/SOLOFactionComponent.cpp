#include "World/SOLOFactionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

USOLOFactionComponent::USOLOFactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOLOFactionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USOLOFactionComponent, FactionReputation);
}

void USOLOFactionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USOLOFactionComponent::AddReputation(FName FactionID, int32 Amount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	if (FactionID == NAME_None || Amount == 0) return;

	int32 NewRep = Amount;
	bool bFound = false;

	for (FSOLOFactionReputationEntry& Entry : FactionReputation)
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
		FSOLOFactionReputationEntry NewEntry;
		NewEntry.FactionID = FactionID;
		NewEntry.Reputation = Amount;
		FactionReputation.Add(NewEntry);
	}
	
	OnReputationChanged.Broadcast(FactionID, NewRep, Amount);
	
	// Force replication trigger
	GetOwner()->ForceNetUpdate();
}

int32 USOLOFactionComponent::GetReputation(FName FactionID) const
{
	for (const FSOLOFactionReputationEntry& Entry : FactionReputation)
	{
		if (Entry.FactionID == FactionID)
		{
			return Entry.Reputation;
		}
	}
	return 0; // Neutral by default si no hay registro
}

ESOLOAttitude USOLOFactionComponent::GetAttitudeTowards(AActor* OtherActor) const
{
	if (!OtherActor) return ESOLOAttitude::Neutral;

	USOLOFactionComponent* OtherFactionComp = OtherActor->FindComponentByClass<USOLOFactionComponent>();
	if (!OtherFactionComp) return ESOLOAttitude::Neutral; // Si no tiene facciÃ³n, neutral

	// Si son de la misma facciÃ³n base, amistosos por defecto
	if (BaseFactionID != NAME_None && BaseFactionID == OtherFactionComp->BaseFactionID)
	{
		return ESOLOAttitude::Friendly;
	}

	// Consultar cÃ³mo me llevo con la facciÃ³n base del otro
	return GetAttitudeTowardsFaction(OtherFactionComp->BaseFactionID);
}

ESOLOAttitude USOLOFactionComponent::GetAttitudeTowardsFaction(FName OtherFactionID) const
{
	if (OtherFactionID == NAME_None) return ESOLOAttitude::Neutral;

	int32 Rep = GetReputation(OtherFactionID);

	if (Rep <= HostileThreshold)
	{
		return ESOLOAttitude::Hostile;
	}
	if (Rep >= FriendlyThreshold)
	{
		return ESOLOAttitude::Friendly;
	}

	return ESOLOAttitude::Neutral;
}

void USOLOFactionComponent::OnRep_FactionReputation()
{
	// Evento para UI cuando cambie la reputaciÃ³n replicada
	// No tenemos el 'Delta' en el cliente fÃ¡cilmente por TMap, pero se puede actualizar el menÃº.
}
