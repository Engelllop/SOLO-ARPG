#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMBERVEILThreatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighestThreatTargetChangedSignature, AActor*, NewTarget);

/**
 * Componente que gestiona la tabla de amenaza (Aggro) para Jefes y Enemigos.
 * Determina a qué jugador o mascota debe atacar la IA.
 */
UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class EMBERVEILCORE_API UEMBERVEILThreatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEMBERVEILThreatComponent();

	/** 
	 * Añade (o reduce) amenaza a un objetivo específico. 
	 * @param Instigator El actor que causó el daño o curación.
	 * @param Amount La cantidad de amenaza a añadir (puede ser negativa).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void AddThreat(AActor* Instigator, float Amount);

	/** Remueve un actor completamente de la tabla de amenaza (ej. si muere o se hace invisible). */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void RemoveTarget(AActor* TargetToRemove);

	/** Devuelve el actor que tiene el mayor nivel de amenaza acumulado. */
	UFUNCTION(BlueprintPure, Category = "AI|Threat")
	AActor* GetHighestThreatTarget() const;

	/** Devuelve el valor numérico de la amenaza de un actor específico. */
	UFUNCTION(BlueprintPure, Category = "AI|Threat")
	float GetThreatValue(AActor* Target) const;

	/** Limpia toda la tabla de amenaza. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void ClearAllThreat();

	/** Evento disparado cuando el objetivo prioritario (Top Threat) cambia. Útil para que la IA cambie su Target. */
	UPROPERTY(BlueprintAssignable, Category = "AI|Threat|Events")
	FOnHighestThreatTargetChangedSignature OnHighestThreatTargetChanged;

protected:
	virtual void BeginPlay() override;

private:
	/** Tabla de amenaza: mapea cada actor a su nivel de amenaza actual. */
	UPROPERTY(Transient)
	TMap<AActor*, float> ThreatTable;

	/** Cache del objetivo con mayor amenaza actual para detectar cambios. */
	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentHighestThreatTarget;

	/** Recalcula internamente quién es el nuevo HighestThreatTarget y dispara el evento si cambió. */
	void EvaluateHighestThreatTarget();
};
