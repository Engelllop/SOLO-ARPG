#include "Player/SOLOCompanionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

USOLOCompanionComponent::USOLOCompanionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOLOCompanionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USOLOCompanionComponent, ActiveCompanion);
}

void USOLOCompanionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USOLOCompanionComponent::SummonCompanion(TSubclassOf<AActor> CompanionClass)
{
	if (!GetOwner() || !CompanionClass) return;

	if (GetOwner()->HasAuthority())
	{
		ServerSummonCompanion_Implementation(CompanionClass);
	}
	else
	{
		ServerSummonCompanion(CompanionClass);
	}
}

bool USOLOCompanionComponent::ServerSummonCompanion_Validate(TSubclassOf<AActor> CompanionClass)
{
	return true;
}

void USOLOCompanionComponent::ServerSummonCompanion_Implementation(TSubclassOf<AActor> CompanionClass)
{
	// Primero remover la mascota actual si existe
	if (ActiveCompanion)
	{
		ServerDismissCompanion_Implementation();
	}

	UWorld* World = GetWorld();
	if (World && CompanionClass)
	{
		// Spawn offset (ej. a la derecha del jugador)
		FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorRightVector() * 100.f);
		FRotator SpawnRotation = GetOwner()->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* NewCompanion = World->SpawnActor<AActor>(CompanionClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewCompanion)
		{
			ActiveCompanion = NewCompanion;
			OnCompanionSummoned.Broadcast(ActiveCompanion);
		}
	}
}

void USOLOCompanionComponent::DismissCompanion()
{
	if (!GetOwner()) return;

	if (GetOwner()->HasAuthority())
	{
		ServerDismissCompanion_Implementation();
	}
	else
	{
		ServerDismissCompanion();
	}
}

bool USOLOCompanionComponent::ServerDismissCompanion_Validate()
{
	return true;
}

void USOLOCompanionComponent::ServerDismissCompanion_Implementation()
{
	if (ActiveCompanion)
	{
		ActiveCompanion->Destroy();
		ActiveCompanion = nullptr;
		OnCompanionDismissed.Broadcast();
	}
}

void USOLOCompanionComponent::OnRep_ActiveCompanion(AActor* OldCompanion)
{
	if (ActiveCompanion)
	{
		OnCompanionSummoned.Broadcast(ActiveCompanion);
	}
	else if (OldCompanion)
	{
		// Si ahora es nullptr pero antes habÃ­a uno, significa que se despachÃ³
		OnCompanionDismissed.Broadcast();
	}
}
