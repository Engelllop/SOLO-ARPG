#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_StatusEffectBase.generated.h"

/**
 * Abstract C++ base for status effects (DoTs, stuns, roots, etc.).
 * PostInitProperties configures the GE programmatically so Blueprint subclasses
 * only need to set the exposed properties without touching Modifiers/Duration by hand.
 */
UCLASS(Blueprintable, Abstract)
class SOLOCORE_API UGE_StatusEffectBase : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_StatusEffectBase();

	virtual void PostInitProperties() override;

	/** Tag applied to the target while this effect is active (e.g. State.Burning). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	FGameplayTag StatusTag;

	/** How long the status lasts in seconds. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	float Duration = 3.f;

	/** Tick interval for damage-over-time effects. Ignored when DamagePerTick is 0. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	float TickInterval = 0.5f;

	/** Damage applied each tick. Set to 0 for pure CC effects (stun, root, freeze). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StatusEffect")
	float DamagePerTick = 0.f;
};
