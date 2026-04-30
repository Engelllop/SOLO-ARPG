#pragma once
#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON0_GuardianBoss.generated.h"

class ASOLON0_CainDeathEventActor;

// Guardián de la Primera Garganta — first boss of N0.
// Phase 1 (100%–51%): standard attacks.
// Phase 2 (50%–26%): speed increase, adds charge attack.
// Phase 3 (25%–0%): triggers Caín death cutscene, then 5s vulnerability window.
UCLASS()
class SOLOCORE_API ASOLON0_GuardianBoss : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON0_GuardianBoss();

	virtual void OnPhaseChanged_Implementation(int32 NewPhase) override;

protected:
	// Set this in the level to the ASOLON0_CainDeathEventActor placed in the same sublevel
	UPROPERTY(EditInstanceOnly, Category = "N0|Boss")
	TSoftObjectPtr<ASOLON0_CainDeathEventActor> CainDeathEvent;

	// GE applied at Phase 2 transition (attack speed / movement speed increase)
	UPROPERTY(EditDefaultsOnly, Category = "N0|Boss")
	TSubclassOf<UGameplayEffect> Phase2SpeedEffect;

	// Duration of the vulnerability window after Caín's death cutscene
	UPROPERTY(EditDefaultsOnly, Category = "N0|Boss")
	float VulnerabilityWindowDuration = 5.f;

	// GE that marks the Guardian as vulnerable (used by damage execs to bypass defense)
	UPROPERTY(EditDefaultsOnly, Category = "N0|Boss")
	TSubclassOf<UGameplayEffect> VulnerableEffect;

private:
	void BeginVulnerabilityWindow();
};
