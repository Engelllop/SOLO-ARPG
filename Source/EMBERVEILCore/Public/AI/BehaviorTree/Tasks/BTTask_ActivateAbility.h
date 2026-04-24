#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "BTTask_ActivateAbility.generated.h"

UCLASS(ClassGroup = (EMBERVEIL), meta = (DisplayName = "Activate Ability"))
class EMBERVEILCORE_API UBTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbility();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	// Tag of the GameplayAbility to activate — set in the BT editor per node instance
	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag;

	// When true the task stays In Progress until the ability ends naturally
	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bWaitForAbilityEnd = true;

private:
	// We hold a weak reference to avoid dangling pointer if the ASC is destroyed mid-ability
	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	FDelegateHandle AbilityEndedHandle;
};
