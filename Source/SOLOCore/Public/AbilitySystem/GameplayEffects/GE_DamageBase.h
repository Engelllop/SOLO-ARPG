#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_DamageBase.generated.h"

/**
 * Minimal C++ base for damage GameplayEffects.
 * All modifier/execution/tag configuration is done in Blueprint subclasses
 * (GE_DamagePhysical, GE_DamageFire, etc.) to keep iteration fast for designers.
 */
UCLASS(Blueprintable)
class SOLOCORE_API UGE_DamageBase : public UGameplayEffect
{
	GENERATED_BODY()
};
