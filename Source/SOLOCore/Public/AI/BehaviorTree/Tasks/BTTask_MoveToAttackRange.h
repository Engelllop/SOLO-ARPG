#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToAttackRange.generated.h"

USTRUCT()
struct FBTMoveToAttackRangeMemory
{
	GENERATED_BODY()

	FVector LastKnownTargetLocation = FVector::ZeroVector;
	float   ElapsedTime             = 0.f;
	bool    bMovementRequested      = false;
};

UCLASS(ClassGroup = (SOLO), meta = (DisplayName = "Move To Attack Range"))
class SOLOCORE_API UBTTask_MoveToAttackRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToAttackRange();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTMoveToAttackRangeMemory); }
	virtual FString GetStaticDescription() const override;

private:
	EBTNodeResult::Type RequestMove(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, AActor* Target, float AcceptanceRadius);

	// Seconds allowed to reach the target before the task fails
	static constexpr float MoveTimeout = 8.f;
	// Minimum target displacement (cm) that triggers a path recalculation
	static constexpr float PathRebuildThreshold = 150.f;
};
