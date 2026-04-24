#include "Player/EMBERVEILCompanionComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UEMBERVEILCompanionComponent::UEMBERVEILCompanionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEMBERVEILCompanionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEMBERVEILCompanionComponent, ActiveCompanion);
}

void UEMBERVEILCompanionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEMBERVEILCompanionComponent::SummonCompanion(TSubclassOf<AActor> CompanionClass)
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

bool UEMBERVEILCompanionComponent::ServerSummonCompanion_Validate(TSubclassOf<AActor> CompanionClass)
{
	return true;
}

void UEMBERVEILCompanionComponent::ServerSummonCompanion_Implementation(TSubclassOf<AActor> CompanionClass)
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

void UEMBERVEILCompanionComponent::DismissCompanion()
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

bool UEMBERVEILCompanionComponent::ServerDismissCompanion_Validate()
{
	return true;
}

void UEMBERVEILCompanionComponent::ServerDismissCompanion_Implementation()
{
	if (ActiveCompanion)
	{
		ActiveCompanion->Destroy();
		ActiveCompanion = nullptr;
		OnCompanionDismissed.Broadcast();
	}
}

void UEMBERVEILCompanionComponent::OnRep_ActiveCompanion(AActor* OldCompanion)
{
	if (ActiveCompanion)
	{
		OnCompanionSummoned.Broadcast(ActiveCompanion);
	}
	else if (OldCompanion)
	{
		// Si ahora es nullptr pero antes había uno, significa que se despachó
		OnCompanionDismissed.Broadcast();
	}
}
