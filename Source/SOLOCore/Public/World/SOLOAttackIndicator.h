#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOLOAttackIndicator.generated.h"

UCLASS()
class SOLOCORE_API ASOLOAttackIndicator : public AActor
{
	GENERATED_BODY()
public:
	ASOLOAttackIndicator();

	void ShowIndicator(FVector Location, float Radius, FLinearColor Color, float Duration);
	void HideIndicator();

protected:
	UPROPERTY(VisibleAnywhere) class UDecalComponent* IndicatorDecal;
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* IndicatorMesh;

private:
	FTimerHandle HideTimer;
};
