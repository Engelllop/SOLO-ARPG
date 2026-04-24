#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GameplayTagContainer.h"
#include "BTService_CombatUpdate.generated.h"

UCLASS(ClassGroup = (EMBERVEIL), meta = (DisplayName = "Combat Update"))
class EMBERVEILCORE_API UBTService_CombatUpdate : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CombatUpdate();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

private:
	FGameplayTag Tag_Stunned;
	FGameplayTag Tag_Dead;
	FGameplayTag Tag_Attacking;
};
