#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/SOLOBossData.h"
#include "SOLOBossPhaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, int32, NewPhaseIndex);

UCLASS(ClassGroup=(SOLO), meta=(BlueprintSpawnableComponent))
class SOLOCORE_API USOLOBossPhaseComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLOBossPhaseComponent();

	void CheckPhaseTransition(float CurrentHP, float MaxHP);
	int32 GetCurrentPhase() const { return CurrentPhaseIndex; }
	const FBossPhaseData* GetCurrentPhaseData() const;

	UPROPERTY(BlueprintAssignable) FOnPhaseChanged OnPhaseChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) UDataTable* PhaseDataTable;

private:
	int32 CurrentPhaseIndex = 0;
	TArray<FBossPhaseData> Phases;

	void EnterPhase(int32 PhaseIndex);
};
