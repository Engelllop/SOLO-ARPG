#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Data/SOLOEnums.h"
#include "BTDecorator_HasCommand.generated.h"

UCLASS()
class SOLOCORE_API UBTDecorator_HasCommand : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_HasCommand();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere) ESOLOCompanionCommand RequiredCommand = ESOLOCompanionCommand::Attack;
	UPROPERTY(EditAnywhere) FBlackboardKeySelector CommandKey;
};
