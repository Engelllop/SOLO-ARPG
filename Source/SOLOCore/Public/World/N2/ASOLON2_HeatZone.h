#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON2_HeatZone.generated.h"

class UBoxComponent;

// Placed in the level to define areas where ambient heat builds faster.
// AccelerationMultiplier > 1 shortens the interval between heat segments.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON2_HeatZone : public AActor
{
	GENERATED_BODY()
public:
	ASOLON2_HeatZone();

protected:
	UPROPERTY(VisibleAnywhere) UBoxComponent* ZoneVolume;

	UPROPERTY(EditAnywhere, Category = "HeatZone")
	float AccelerationMultiplier = 2.f;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
