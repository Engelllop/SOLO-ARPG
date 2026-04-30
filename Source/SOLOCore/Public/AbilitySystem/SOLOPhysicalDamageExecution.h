#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SOLOPhysicalDamageExecution.generated.h"

UCLASS()
class SOLOCORE_API USOLOPhysicalDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USOLOPhysicalDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
