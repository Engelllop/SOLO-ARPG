#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASOLON1_VelmarDayComponent.generated.h"

class USOLOGameInstance;

UENUM(BlueprintType)
enum class EVelmarDay : uint8
{
	Day1_Arrival   UMETA(DisplayName = "Day 1 — Arrival"),
	Day2_Search    UMETA(DisplayName = "Day 2 — Search"),
	Day3_Reckoning UMETA(DisplayName = "Day 3 — Reckoning")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVelmarDayChanged, EVelmarDay, NewDay);

UCLASS(ClassGroup=(SOLO), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API USOLON1_VelmarDayComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLON1_VelmarDayComponent();

	UFUNCTION(BlueprintCallable, Category = "N1|Day")
	void AdvanceDay();

	UFUNCTION(BlueprintPure, Category = "N1|Day")
	EVelmarDay GetCurrentDay() const { return CurrentDay; }

	UFUNCTION(BlueprintPure, Category = "N1|Day")
	int32 GetCurrentDayIndex() const { return static_cast<int32>(CurrentDay); }

	UPROPERTY(BlueprintAssignable) FOnVelmarDayChanged OnDayChanged;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnDay1();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnDay2();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnDay3();

private:
	EVelmarDay CurrentDay = EVelmarDay::Day1_Arrival;

	void ApplyDayEvents(EVelmarDay Day);
};
