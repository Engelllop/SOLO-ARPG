#include "Skills/SOLOTalentTreeComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Attributes/SOLOAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

USOLOTalentTreeComponent::USOLOTalentTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOLOTalentTreeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USOLOTalentTreeComponent, UnlockedTalentNodes);
}

void USOLOTalentTreeComponent::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* USOLOTalentTreeComponent::GetOwnerAbilitySystem() const
{
	if (AActor* Owner = GetOwner())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	}
	return nullptr;
}

const USOLOAttributeSet* USOLOTalentTreeComponent::GetAttributeSet() const
{
	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystem())
	{
		return ASC->GetSet<USOLOAttributeSet>();
	}
	return nullptr;
}

void USOLOTalentTreeComponent::AllocateStatPoint(FGameplayAttribute AttributeToUpgrade)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		// Si ya somos el servidor, lo aplicamos directamente
		ServerAllocateStatPoint_Implementation(AttributeToUpgrade);
	}
	else
	{
		// Si somos cliente, llamamos al RPC
		ServerAllocateStatPoint(AttributeToUpgrade);
	}
}

bool USOLOTalentTreeComponent::ServerAllocateStatPoint_Validate(FGameplayAttribute AttributeToUpgrade)
{
	return true;
}

void USOLOTalentTreeComponent::ServerAllocateStatPoint_Implementation(FGameplayAttribute AttributeToUpgrade)
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystem();
	const USOLOAttributeSet* AS = GetAttributeSet();

	if (!ASC || !AS) return;

	// Verificar si hay puntos disponibles
	if (AS->GetUnspentStatPoints() >= 1.0f)
	{
		// Modificar el Stat Point
		ASC->ApplyModToAttributeUnsafe(USOLOAttributeSet::GetUnspentStatPointsAttribute(), EGameplayModOp::Additive, -1.0f);
		
		// Modificar el Atributo destino (ej. Fuerza, Destreza)
		ASC->ApplyModToAttributeUnsafe(AttributeToUpgrade, EGameplayModOp::Additive, 1.0f);
	}
}

void USOLOTalentTreeComponent::UnlockTalentNode(FName NodeID)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		ServerUnlockTalentNode_Implementation(NodeID);
	}
	else
	{
		ServerUnlockTalentNode(NodeID);
	}
}

bool USOLOTalentTreeComponent::ServerUnlockTalentNode_Validate(FName NodeID)
{
	return true;
}

void USOLOTalentTreeComponent::ServerUnlockTalentNode_Implementation(FName NodeID)
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystem();
	const USOLOAttributeSet* AS = GetAttributeSet();

	if (!ASC || !AS) return;

	// Evitar duplicados
	if (UnlockedTalentNodes.Contains(NodeID))
	{
		return;
	}

	if (AS->GetUnspentTalentPoints() < 1.0f)
	{
		return;
	}

	// Gastar el punto de talento
	ASC->ApplyModToAttributeUnsafe(USOLOAttributeSet::GetUnspentTalentPointsAttribute(), EGameplayModOp::Additive, -1.0f);

	// Registrar el nodo como desbloqueado (replicado al cliente automÃ¡ticamente)
	UnlockedTalentNodes.Add(NodeID);

	// Aplicar el gameplay tag asociado al nodo (Blueprints pueden reaccionar a este tag)
	// El tag esperado sigue el patrÃ³n: Talent.NodeID (debe existir en DefaultGameplayTags.ini)
	const FString TagName = FString::Printf(TEXT("Talent.%s"), *NodeID.ToString());
	const FGameplayTag NodeTag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
	if (NodeTag.IsValid())
	{
		ASC->AddLooseGameplayTag(NodeTag);
	}

	// Notificar a la UI y a los Blueprints para que apliquen el efecto del nodo
	OnTalentNodeUnlocked.Broadcast(NodeID);
}

void USOLOTalentTreeComponent::RestoreUnlockedTalentsFromSave(const TArray<FName>& SavedNodes)
{
	if (!GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetOwnerAbilitySystem();

	for (const FName& NodeID : SavedNodes)
	{
		if (UnlockedTalentNodes.Contains(NodeID))
		{
			continue;
		}

		UnlockedTalentNodes.Add(NodeID);

		// Re-aplicar el tag del nodo al ASC sin gastar puntos (ya se gastaron cuando se guardÃ³)
		const FString TagName = FString::Printf(TEXT("Talent.%s"), *NodeID.ToString());
		const FGameplayTag NodeTag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
		if (NodeTag.IsValid() && ASC)
		{
			ASC->AddLooseGameplayTag(NodeTag);
		}

		OnTalentNodeUnlocked.Broadcast(NodeID);
	}
}
