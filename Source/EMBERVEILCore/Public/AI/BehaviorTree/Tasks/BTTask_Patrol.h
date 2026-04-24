#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Patrol.generated.h"

USTRUCT()
struct FBTPatrolMemory
{
	GENERATED_BODY()

	FVector  TargetLocation    = FVector::ZeroVector;
	float    WaitElapsed       = 0.f;
	bool     bWaiting          = false;
	bool     bMovementStarted  = false;
	int32    RetryCount        = 0;
};

UCLASS(ClassGroup = (EMBERVEIL), meta = (DisplayName = "Patrol"))
class EMBERVEILCORE_API UBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Patrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTPatrolMemory); }
	virtual FString GetStaticDescription() const override;

protected:
	// Maximum distance from PatrolOrigin for random patrol points
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 800.f;

	// How long (seconds) the enemy idles at the patrol point before picking a new one
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float WaitTimeAtPoint = 2.f;

private:
	static constexpr int32 MaxRetries = 3;

	bool TryPickPatrolPoint(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
