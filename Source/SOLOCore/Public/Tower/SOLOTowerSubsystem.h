#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tower/SOLOTowerTypes.h"
#include "SOLOTowerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOLOTowerFloorChanged, int32, NewFloorIndex);

/**
 * Estado global de la torre (piso activo, transiciones).
 * La generaciÃ³n de salas y combate por piso se acoplarÃ¡ aquÃ­ o en Actors de mapa.
 */
UCLASS()
class SOLOCORE_API USOLOTowerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Tower")
	int32 GetCurrentFloorIndex() const { return CurrentFloorIndex; }

	UFUNCTION(BlueprintCallable, Category = "Tower")
	void SetCurrentFloorIndex(int32 NewFloorIndex);

	UPROPERTY(BlueprintAssignable, Category = "Tower")
	FSOLOTowerFloorChanged OnFloorChanged;

private:
	UPROPERTY()
	int32 CurrentFloorIndex = 0;
};
