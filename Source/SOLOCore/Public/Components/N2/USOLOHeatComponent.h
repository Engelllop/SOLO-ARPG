#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "USOLOHeatComponent.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeatSegmentChanged, int32, NewSegmentCount);

// Manages the 5-segment heat meter for the player character in N2.
// Segments accumulate via ambient timer (12s/seg outdoors) or heat zones.
// Each segment applies a stacking GE tag (Status.Heat.Segment1-5).
UCLASS(ClassGroup = "SOLO", meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOHeatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLOHeatComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// Called by HeatZone; Multiplier > 1 speeds segment gain
	UFUNCTION(BlueprintCallable) void EnterHeatZone(float AccelerationMultiplier = 1.f);
	UFUNCTION(BlueprintCallable) void ExitHeatZone();

	// Called by HeatShelter overlap
	UFUNCTION(BlueprintCallable) void EnterShelter();
	UFUNCTION(BlueprintCallable) void ExitShelter();

	// Directly add/remove one segment (e.g. from abilities or items)
	UFUNCTION(BlueprintCallable) void AddHeatSegment();
	UFUNCTION(BlueprintCallable) void RemoveHeatSegment();
	UFUNCTION(BlueprintCallable) void ClearAllHeat();

	UFUNCTION(BlueprintPure) int32 GetCurrentSegments() const { return CurrentSegments; }

	UPROPERTY(BlueprintAssignable) FOnHeatSegmentChanged OnHeatSegmentChanged;

protected:
	// GEs to apply at each segment (index 0 = segment 1, etc.)
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	TArray<TSubclassOf<UGameplayEffect>> SegmentGEs;

	// Seconds between ambient segment gain when outdoors and not in shelter
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	float AmbientSegmentInterval = 12.f;

	// Seconds between segment drain when inside a shelter
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	float ShelterCooldownInterval = 8.f;

private:
	int32 CurrentSegments = 0;
	bool  bInShelter      = false;
	int32 HeatZoneCount   = 0;    // stacked zone count
	float ZoneAcceleration = 1.f; // current combined multiplier

	FTimerHandle AmbientTimer;
	FTimerHandle CooldownTimer;

	TArray<FActiveGameplayEffectHandle> ActiveSegmentHandles;

	UAbilitySystemComponent* GetOwnerASC() const;

	void ApplySegmentGE(int32 SegIndex);
	void RemoveSegmentGE(int32 SegIndex);

	void TickAmbientHeat();
	void TickShelterCooldown();
	void RestartAmbientTimer();
};
