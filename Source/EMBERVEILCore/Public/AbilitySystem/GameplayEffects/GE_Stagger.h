#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Stagger.generated.h"

/**
 * Stagger GameplayEffect.
 *
 * Pure crowd-control effect — no attribute modifiers, only grants State.Stunned
 * for 0.8 seconds. Configured entirely in the CDO constructor.
 */
UCLASS(Blueprintable)
class EMBERVEILCORE_API UGE_Stagger : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Stagger();
};
