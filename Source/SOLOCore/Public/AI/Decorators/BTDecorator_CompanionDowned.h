#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CompanionDowned.generated.h"

UCLASS()
class SOLOCORE_API UBTDecorator_CompanionDowned : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_CompanionDowned();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector CompanionKey;
};
