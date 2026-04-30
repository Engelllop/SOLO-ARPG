#pragma once
#include "CoreMinimal.h"
#include "Character/SOLOCharacterBase.h"
#include "Data/SOLOEnums.h"
#include "SOLOCompanionCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompanionDownedChanged, bool, bDowned);

UCLASS()
class SOLOCORE_API ASOLOCompanionCharacter : public ASOLOCharacterBase
{
	GENERATED_BODY()
public:
	ASOLOCompanionCharacter();

	void InitCompanion(ESOLOCompanionArchetype Archetype);

	void SetDowned(bool bIsDowned);
	bool IsDowned() const { return bDowned; }
	float GetReviveChannelTime() const { return ReviveChannelTime; }

	ESOLOCompanionArchetype GetArchetype() const { return CompanionArchetype; }

	UPROPERTY(BlueprintAssignable) FOnCompanionDownedChanged OnDownedChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) ESOLOCompanionArchetype CompanionArchetype = ESOLOCompanionArchetype::Warrior;
	UPROPERTY(EditDefaultsOnly) FText CompanionName;
	UPROPERTY(EditDefaultsOnly) float ReviveChannelTime = 5.f;

	// Per-archetype default ability sets assigned in BP
	UPROPERTY(EditDefaultsOnly) TArray<TSubclassOf<UGameplayAbility>> ArchetypeAbilities;

private:
	bool bDowned = false;
};
