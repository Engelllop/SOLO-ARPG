#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RotateToTarget.generated.h"

UCLASS(ClassGroup = (SOLO), meta = (DisplayName = "Rotate To Target"))
class SOLOCORE_API UBTService_RotateToTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_RotateToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

	// RInterpTo speed â€” higher values = snappier rotation
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSpeed = 5.f;
};
