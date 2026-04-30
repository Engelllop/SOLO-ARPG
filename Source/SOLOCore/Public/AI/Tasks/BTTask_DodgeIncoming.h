#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DodgeIncoming.generated.h"

UCLASS()
class SOLOCORE_API UBTTask_DodgeIncoming : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_DodgeIncoming();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere) FGameplayTag DodgeAbilityTag;
};
