#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tower/EMBERVEILTowerTypes.h"
#include "EMBERVEILTowerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEMBERVEILTowerFloorChanged, int32, NewFloorIndex);

/**
 * Estado global de la torre (piso activo, transiciones).
 * La generación de salas y combate por piso se acoplará aquí o en Actors de mapa.
 */
UCLASS()
class EMBERVEILCORE_API UEMBERVEILTowerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Tower")
	int32 GetCurrentFloorIndex() const { return CurrentFloorIndex; }

	UFUNCTION(BlueprintCallable, Category = "Tower")
	void SetCurrentFloorIndex(int32 NewFloorIndex);

	UPROPERTY(BlueprintAssignable, Category = "Tower")
	FEMBERVEILTowerFloorChanged OnFloorChanged;

private:
	UPROPERTY()
	int32 CurrentFloorIndex = 0;
};
