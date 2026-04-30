#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_EnemyInRange.generated.h"

UCLASS()
class SOLOCORE_API UBTDecorator_EnemyInRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_EnemyInRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector EnemyKey;
	UPROPERTY(EditAnywhere) float Range = 300.f;
};
