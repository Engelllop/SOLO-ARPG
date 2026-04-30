#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/SOLOSetData.h"
#include "SOLOSetManager.generated.h"

UCLASS()
class SOLOCORE_API USOLOSetManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// Call when a player's equipped items change; applies/removes set bonuses via GE
	UFUNCTION(BlueprintCallable)
	void RefreshSetBonuses(class UAbilitySystemComponent* ASC, const TArray<FName>& EquippedItemIDs);

protected:
	UPROPERTY(EditDefaultsOnly) UDataTable* SetDataTable;

private:
	// ASC -> active set bonus handles
	TMap<UAbilitySystemComponent*, TArray<FActiveGameplayEffectHandle>> ActiveBonuses;

	int32 CountEquippedPieces(const FSOLOSetData& SetData, const TArray<FName>& EquippedIDs) const;
};
