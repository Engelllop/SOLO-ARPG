#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLOManaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFatigueChanged, int32, NewLevel);

UCLASS(ClassGroup = (SOLO), meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLOManaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Magic|Mana")
	void OnSpellCast();

	UFUNCTION(BlueprintCallable, Category = "Magic|Mana")
	bool TryOverload(float SpellManaCost);

	UFUNCTION(BlueprintCallable, Category = "Magic|Mana")
	void ResetFatigue();

	UFUNCTION(BlueprintPure, Category = "Magic|Mana")
	int32 GetFatigueLevel() const { return FatigueLevel; }

	UFUNCTION(BlueprintPure, Category = "Magic|Mana")
	float GetManaRegenPenalty() const;

	UFUNCTION(BlueprintPure, Category = "Magic|Mana")
	float GetSpellDamagePenalty() const;

	UFUNCTION(BlueprintPure, Category = "Magic|Mana")
	float GetManaCostMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "Magic|Mana")
	bool ShouldSpellFail() const;

	UPROPERTY(BlueprintAssignable)
	FOnFatigueChanged OnFatigueChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fatigue")
	int32 SpellsBeforeFatigue = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Fatigue")
	float FatigueRecoveryTime = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Overload")
	float OverloadHPMultiplier = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Overload")
	float OverloadBonusDamagePercent = 0.1f;

private:
	int32 FatigueLevel = 0;
	int32 ConsecutiveSpells = 0;
	float TimeSinceLastSpell = 0.f;

	void IncreaseFatigue();
};
