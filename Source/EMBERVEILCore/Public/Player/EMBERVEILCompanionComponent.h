#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMBERVEILCompanionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompanionSummonedSignature, AActor*, SummonedCompanion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompanionDismissedSignature);

/**
 * Componente que gestiona la invocación de mascotas o monturas.
 * Diseñado para ir en el PlayerCharacter.
 */
UCLASS( ClassGroup=(Player), meta=(BlueprintSpawnableComponent) )
class EMBERVEILCORE_API UEMBERVEILCompanionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEMBERVEILCompanionComponent();

	/** Invoca a un compañero o montura en el mundo cerca del jugador. */
	UFUNCTION(BlueprintCallable, Category = "Companions")
	void SummonCompanion(TSubclassOf<AActor> CompanionClass);

	/** Remueve al compañero activo del mundo. */
	UFUNCTION(BlueprintCallable, Category = "Companions")
	void DismissCompanion();

	/** Devuelve el compañero actual, o nullptr si no hay ninguno. */
	UFUNCTION(BlueprintPure, Category = "Companions")
	AActor* GetActiveCompanion() const { return ActiveCompanion; }

	/** Evento que notifica a la UI u otros sistemas que ha aparecido una mascota. */
	UPROPERTY(BlueprintAssignable, Category = "Companions|Events")
	FOnCompanionSummonedSignature OnCompanionSummoned;

	/** Evento que notifica que la mascota se ha ido (útil para ocultar barras de vida). */
	UPROPERTY(BlueprintAssignable, Category = "Companions|Events")
	FOnCompanionDismissedSignature OnCompanionDismissed;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Servidor: Crea el actor. */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSummonCompanion(TSubclassOf<AActor> CompanionClass);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDismissCompanion();

private:
	/** Referencia al compañero activo replicada para todos los clientes. */
	UPROPERTY(ReplicatedUsing = OnRep_ActiveCompanion)
	TObjectPtr<AActor> ActiveCompanion;

	UFUNCTION()
	void OnRep_ActiveCompanion(AActor* OldCompanion);
};
