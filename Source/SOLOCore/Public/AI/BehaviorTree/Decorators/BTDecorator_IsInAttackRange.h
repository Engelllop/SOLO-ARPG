#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsInAttackRange.generated.h"

UCLASS(ClassGroup = (SOLO), meta = (DisplayName = "Is In Attack Range"))
class SOLOCORE_API UBTDecorator_IsInAttackRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	virtual FName GetNodeIconName() const override;
};
