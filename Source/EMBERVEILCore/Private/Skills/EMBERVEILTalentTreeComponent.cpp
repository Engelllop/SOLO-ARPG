#include "Skills/EMBERVEILTalentTreeComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

UEMBERVEILTalentTreeComponent::UEMBERVEILTalentTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEMBERVEILTalentTreeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEMBERVEILTalentTreeComponent, UnlockedTalentNodes);
}

void UEMBERVEILTalentTreeComponent::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* UEMBERVEILTalentTreeComponent::GetOwnerAbilitySystem() const
{
	if (AActor* Owner = GetOwner())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);
	}
	return nullptr;
}

const UEMBERVEILAttributeSet* UEMBERVEILTalentTreeComponent::GetAttributeSet() const
{
	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystem())
	{
		return ASC->GetSet<UEMBERVEILAttributeSet>();
	}
	return nullptr;
}

void UEMBERVEILTalentTreeComponent::AllocateStatPoint(FGameplayAttribute AttributeToUpgrade)
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

bool UEMBERVEILTalentTreeComponent::ServerAllocateStatPoint_Validate(FGameplayAttribute AttributeToUpgrade)
{
	return true;
}

void UEMBERVEILTalentTreeComponent::ServerAllocateStatPoint_Implementation(FGameplayAttribute AttributeToUpgrade)
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystem();
	const UEMBERVEILAttributeSet* AS = GetAttributeSet();

	if (!ASC || !AS) return;

	// Verificar si hay puntos disponibles
	if (AS->GetUnspentStatPoints() >= 1.0f)
	{
		// Modificar el Stat Point
		ASC->ApplyModToAttributeUnsafe(UEMBERVEILAttributeSet::GetUnspentStatPointsAttribute(), EGameplayModOp::Additive, -1.0f);
		
		// Modificar el Atributo destino (ej. Fuerza, Destreza)
		ASC->ApplyModToAttributeUnsafe(AttributeToUpgrade, EGameplayModOp::Additive, 1.0f);
	}
}

void UEMBERVEILTalentTreeComponent::UnlockTalentNode(FName NodeID)
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

bool UEMBERVEILTalentTreeComponent::ServerUnlockTalentNode_Validate(FName NodeID)
{
	return true;
}

void UEMBERVEILTalentTreeComponent::ServerUnlockTalentNode_Implementation(FName NodeID)
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystem();
	const UEMBERVEILAttributeSet* AS = GetAttributeSet();

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
	ASC->ApplyModToAttributeUnsafe(UEMBERVEILAttributeSet::GetUnspentTalentPointsAttribute(), EGameplayModOp::Additive, -1.0f);

	// Registrar el nodo como desbloqueado (replicado al cliente automáticamente)
	UnlockedTalentNodes.Add(NodeID);

	// Aplicar el gameplay tag asociado al nodo (Blueprints pueden reaccionar a este tag)
	// El tag esperado sigue el patrón: Talent.NodeID (debe existir en DefaultGameplayTags.ini)
	const FString TagName = FString::Printf(TEXT("Talent.%s"), *NodeID.ToString());
	const FGameplayTag NodeTag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
	if (NodeTag.IsValid())
	{
		ASC->AddLooseGameplayTag(NodeTag);
	}

	// Notificar a la UI y a los Blueprints para que apliquen el efecto del nodo
	OnTalentNodeUnlocked.Broadcast(NodeID);
}

void UEMBERVEILTalentTreeComponent::RestoreUnlockedTalentsFromSave(const TArray<FName>& SavedNodes)
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

		// Re-aplicar el tag del nodo al ASC sin gastar puntos (ya se gastaron cuando se guardó)
		const FString TagName = FString::Printf(TEXT("Talent.%s"), *NodeID.ToString());
		const FGameplayTag NodeTag = FGameplayTag::RequestGameplayTag(FName(*TagName), false);
		if (NodeTag.IsValid() && ASC)
		{
			ASC->AddLooseGameplayTag(NodeTag);
		}

		OnTalentNodeUnlocked.Broadcast(NodeID);
	}
}
