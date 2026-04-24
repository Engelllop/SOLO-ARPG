#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "EMBERVEILTalentTreeComponent.generated.h"

class UAbilitySystemComponent;
class UEMBERVEILAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTalentNodeUnlockedSignature, FName, NodeID);

/**
 * Componente responsable de la asignación manual de Puntos de Stat y Talentos.
 * Debe agregarse al PlayerState para aprovechar la replicación y la autoridad del servidor.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMBERVEILCORE_API UEMBERVEILTalentTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEMBERVEILTalentTreeComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Intenta gastar 1 UnspentStatPoint para incrementar el atributo especificado (ej. Fuerza, Destreza).
	 * Se puede llamar desde el UI del cliente; se enruta automáticamente al servidor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Talents|Stats")
	void AllocateStatPoint(FGameplayAttribute AttributeToUpgrade);

	/** RPC al servidor para aplicar el stat. */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAllocateStatPoint(FGameplayAttribute AttributeToUpgrade);

	/** Intenta desbloquear un nodo del árbol de talentos gastando UnspentTalentPoints. */
	UFUNCTION(BlueprintCallable, Category = "Talents|Tree")
	void UnlockTalentNode(FName NodeID);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnlockTalentNode(FName NodeID);

	/** Devuelve true si el nodo ya fue desbloqueado. */
	UFUNCTION(BlueprintPure, Category = "Talents|Tree")
	bool IsNodeUnlocked(FName NodeID) const { return UnlockedTalentNodes.Contains(NodeID); }

	/** Restaura nodos desbloqueados al cargar una partida guardada (solo servidor). */
	void RestoreUnlockedTalentsFromSave(const TArray<FName>& SavedNodes);

	/** Lista de IDs de nodos de talento desbloqueados (replicada al cliente para la UI). */
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Talents")
	TArray<FName> UnlockedTalentNodes;

	/** Broadcast cuando se desbloquea un nodo; la UI y los Blueprints se vinculan aquí. */
	UPROPERTY(BlueprintAssignable, Category = "Talents|Events")
	FOnTalentNodeUnlockedSignature OnTalentNodeUnlocked;

protected:
	virtual void BeginPlay() override;

	/** Helper para obtener el ASC del owner (PlayerState) */
	UAbilitySystemComponent* GetOwnerAbilitySystem() const;

	/** Helper para obtener el AttributeSet de EMBERVEIL */
	const UEMBERVEILAttributeSet* GetAttributeSet() const;
};
