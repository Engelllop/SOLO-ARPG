#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CombatMove.generated.h"

UENUM()
enum class ECombatMoveType : uint8 { Advance, Retreat, Strafe };

UCLASS()
class SOLOCORE_API UBTTask_CombatMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_CombatMove();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere) FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere) ECombatMoveType MoveType = ECombatMoveType::Advance;
	UPROPERTY(EditAnywhere) float MoveDistance = 300.f;
	UPROPERTY(EditAnywhere) float IdealCombatRange = 200.f;
};
