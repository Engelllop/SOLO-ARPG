#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLOThreatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighestThreatTargetChangedSignature, AActor*, NewTarget);

/**
 * Componente que gestiona la tabla de amenaza (Aggro) para Jefes y Enemigos.
 * Determina a quÃ© jugador o mascota debe atacar la IA.
 */
UCLASS( ClassGroup=(AI), meta=(BlueprintSpawnableComponent) )
class SOLOCORE_API USOLOThreatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USOLOThreatComponent();

	/** 
	 * AÃ±ade (o reduce) amenaza a un objetivo especÃ­fico. 
	 * @param Instigator El actor que causÃ³ el daÃ±o o curaciÃ³n.
	 * @param Amount La cantidad de amenaza a aÃ±adir (puede ser negativa).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void AddThreat(AActor* Instigator, float Amount);

	/** Remueve un actor completamente de la tabla de amenaza (ej. si muere o se hace invisible). */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void RemoveTarget(AActor* TargetToRemove);

	/** Devuelve el actor que tiene el mayor nivel de amenaza acumulado. */
	UFUNCTION(BlueprintPure, Category = "AI|Threat")
	AActor* GetHighestThreatTarget() const;

	/** Devuelve el valor numÃ©rico de la amenaza de un actor especÃ­fico. */
	UFUNCTION(BlueprintPure, Category = "AI|Threat")
	float GetThreatValue(AActor* Target) const;

	/** Limpia toda la tabla de amenaza. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "AI|Threat")
	void ClearAllThreat();

	/** Evento disparado cuando el objetivo prioritario (Top Threat) cambia. Ãštil para que la IA cambie su Target. */
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

	/** Recalcula internamente quiÃ©n es el nuevo HighestThreatTarget y dispara el evento si cambiÃ³. */
	void EvaluateHighestThreatTarget();
};
