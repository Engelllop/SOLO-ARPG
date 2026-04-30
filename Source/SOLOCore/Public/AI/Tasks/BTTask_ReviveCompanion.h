#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReviveCompanion.generated.h"

UCLASS()
class SOLOCORE_API UBTTask_ReviveCompanion : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ReviveCompanion();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere) FBlackboardKeySelector DownedCompanionKey;
	UPROPERTY(EditAnywhere) float ReviveDuration = 5.f;
private:
	float ElapsedTime = 0.f;
};
