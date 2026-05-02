#pragma once
#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON2_PastorBoss.generated.h"

class UNiagaraComponent;

// Pastor de Cenizas — boss of N2 (El Nido).
// Phase 0 (100–60%): melee sweeps with ash-dog summons.
// Phase 1 (59–30%): enters frenzy, summon rate doubles, gains charge attack.
// Phase 2 (29–0%):  exposes white nucleus; takes 3× damage if HombreAtrapado was rescued.
//                   Spawns an exposed-nucleus actor for targeted attacks.
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON2_PastorBoss : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON2_PastorBoss();

	virtual void BeginPlay() override;
	virtual void OnPhaseChanged_Implementation(int32 NewPhase) override;
	virtual void Die_Implementation(AActor* InstigatorActor) override;

protected:
	// ─── Phase 0/1 config ───────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Summon")
	TSubclassOf<ASOLOEnemyCharacter> AshDogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Summon")
	float SummonInterval = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Summon")
	float FrenzySummonInterval = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Summon")
	float SummonRadius = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Phase1")
	TSubclassOf<UGameplayEffect> FrenzyGE;

	// ─── Phase 2 config ──────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Phase2")
	TSubclassOf<AActor> NucleusActorClass;

	// Bonus damage multiplier when HombreAtrapado rescued flag is set
	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Phase2")
	float RescuedBonusDamageMultiplier = 3.f;

	// GE that tags the Pastor as having exposed nucleus (can be filtered in damage GEs)
	UPROPERTY(EditDefaultsOnly, Category = "Pastor|Phase2")
	TSubclassOf<UGameplayEffect> NucleusExposedGE;

	UFUNCTION(BlueprintImplementableEvent) void BP_OnFrenzyEnter();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnNucleusExposed(AActor* NucleusActor);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnPastorDeath();

private:
	FTimerHandle SummonTimer;
	TWeakObjectPtr<AActor> SpawnedNucleus;

	void StartSummonLoop(float Interval);
	void SummonAshDog();
	void EnterPhase1();
	void EnterPhase2();
};
