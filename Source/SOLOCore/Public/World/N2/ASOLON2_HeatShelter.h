#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON2_HeatShelter.generated.h"

class UBoxComponent;
class UNiagaraComponent;

// Thermal refuge: drains heat segments from any overlapping character.
// Blueprint subclass should add the steam Niagara particle system.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON2_HeatShelter : public AActor
{
	GENERATED_BODY()
public:
	ASOLON2_HeatShelter();

	UFUNCTION(BlueprintImplementableEvent) void BP_OnPlayerEnter();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnPlayerExit();

protected:
	UPROPERTY(VisibleAnywhere) UBoxComponent* ShelterVolume;
	UPROPERTY(VisibleAnywhere) UNiagaraComponent* SteamParticles;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
