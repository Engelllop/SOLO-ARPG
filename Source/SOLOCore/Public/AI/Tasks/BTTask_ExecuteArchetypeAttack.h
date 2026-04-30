#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteArchetypeAttack.generated.h"

UCLASS()
class SOLOCORE_API UBTTask_ExecuteArchetypeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ExecuteArchetypeAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere) FGameplayTag AttackAbilityTag;
};
