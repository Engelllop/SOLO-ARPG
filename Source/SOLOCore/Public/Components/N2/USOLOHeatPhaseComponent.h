#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USOLOHeatPhaseComponent.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class USOLOHeatComponent;

// Espectro de Calor: becomes intangible when the target player has >= IntangibilityHeatThreshold segments.
// Listens to the local player's USOLOHeatComponent delegate.
UCLASS(ClassGroup = "SOLO|N2", meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOHeatPhaseComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLOHeatPhaseComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

protected:
	// Player heat segments at or above this count make the Espectro intangible
	UPROPERTY(EditDefaultsOnly, Category = "HeatPhase")
	int32 IntangibilityHeatThreshold = 3;

	// GE that makes this enemy intangible (collision disabled, damage immune tag, visual)
	UPROPERTY(EditDefaultsOnly, Category = "HeatPhase")
	TSubclassOf<UGameplayEffect> IntangibleGE;

	UFUNCTION(BlueprintImplementableEvent) void BP_OnBecomeIntangible();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnBecomeTangible();

private:
	FActiveGameplayEffectHandle IntangibleHandle;
	TWeakObjectPtr<USOLOHeatComponent> TrackedHeatComp;

	UFUNCTION() void OnPlayerHeatChanged(int32 NewSegments);

	void ApplyIntangible();
	void RemoveIntangible();
	UAbilitySystemComponent* GetOwnerASC() const;
};
