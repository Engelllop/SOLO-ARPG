#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLONoiseAggroComponent.generated.h"

// Attach to a sleeping enemy (e.g. Gárgola Durmiente) to trigger aggro on nearby movement.
// Periodically checks for pawns within NoiseRadius moving faster than MinNoiseSpeed.
// One-shot: disables itself after first awakening.
UCLASS(ClassGroup=(SOLO), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API USOLONoiseAggroComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLONoiseAggroComponent();

	// Call from owning enemy's BeginPlay after all components are ready
	UFUNCTION(BlueprintCallable) void StartListening();
	UFUNCTION(BlueprintCallable) void StopListening();

	UPROPERTY(EditDefaultsOnly) float NoiseRadius = 1500.f;     // 15m default
	UPROPERTY(EditDefaultsOnly) float MinNoiseSpeed = 10.f;     // cm/s threshold
	UPROPERTY(EditDefaultsOnly) float CheckInterval = 0.3f;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION() void CheckForNoise();

	FTimerHandle CheckTimer;
	bool bListening = false;
};
