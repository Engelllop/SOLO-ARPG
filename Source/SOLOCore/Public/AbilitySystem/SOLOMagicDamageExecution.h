#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SOLOMagicDamageExecution.generated.h"

UCLASS()
class SOLOCORE_API USOLOMagicDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USOLOMagicDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
