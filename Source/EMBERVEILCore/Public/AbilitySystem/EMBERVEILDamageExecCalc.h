#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EMBERVEILDamageExecCalc.generated.h"

UCLASS()
class EMBERVEILCORE_API UEMBERVEILDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEMBERVEILDamageExecCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
