#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayEffects/GE_StatusEffectBase.h"
#include "GE_Poison.generated.h"

/**
 * Poison DoT GameplayEffect.
 *
 * Configures itself in the CDO constructor:
 *   - Duration  : 5 seconds
 *   - Period    : 1 second (tick per second)
 *   - Stacking  : AggregateByTarget, limit 3
 *   - GrantedTag: State.Poisoned
 *   - Damage/tick driven by SetByCaller tag Damage.Type.Poison (default 5.0)
 *
 * StatusEffectBase::PostInitProperties is bypassed for the modifier so we can
 * wire SetByCaller instead of a flat ScalableFloat.
 */
UCLASS(Blueprintable)
class EMBERVEILCORE_API UGE_Poison : public UGE_StatusEffectBase
{
	GENERATED_BODY()

public:
	UGE_Poison();

	virtual void PostInitProperties() override;
};
