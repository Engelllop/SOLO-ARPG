#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SOLODamageExecCalc.generated.h"

UCLASS()
class SOLOCORE_API USOLODamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USOLODamageExecCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
