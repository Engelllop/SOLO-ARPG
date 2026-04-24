#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "BTDecorator_CanAttack.generated.h"

UCLASS(ClassGroup = (EMBERVEIL), meta = (DisplayName = "Can Attack"))
class EMBERVEILCORE_API UBTDecorator_CanAttack : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;

private:
	// Cached tags to avoid per-tick string lookups
	FGameplayTag Tag_Stunned;
	FGameplayTag Tag_Dead;
	FGameplayTag Tag_Attacking;
};
