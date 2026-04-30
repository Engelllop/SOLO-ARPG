#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON2_TrappedManEvent.generated.h"

class UBoxComponent;
class UGameplayEffect;
class ASOLOEnemyCharacter;

UENUM(BlueprintType)
enum class ETrappedManChoice : uint8
{
	None    UMETA(DisplayName = "None / Not yet chosen"),
	Rescue  UMETA(DisplayName = "Rescue (30s with 3 dog waves)"),
	Ignore  UMETA(DisplayName = "Ignore (walk away)"),
	Kill    UMETA(DisplayName = "Kill for item"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrappedManChoiceMade, ETrappedManChoice, Choice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRescueCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRescueFailed);

// Moral event: player encounters a man trapped under a fallen beam in N2.
// Three choices with narrative/reputation consequences:
//   Rescue  — 30s timed extraction defended by 3 Ash Dog waves (+reputation, FLAG set)
//   Ignore  — walk away (no flag, neutral)
//   Kill    — player kills him for ITEM_FragmentoPiedraHumana (-reputation, Mira horror reaction)
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON2_TrappedManEvent : public AActor
{
	GENERATED_BODY()
public:
	ASOLON2_TrappedManEvent();
	virtual void BeginPlay() override;

	// Called by player interaction / choice widget
	UFUNCTION(BlueprintCallable) void MakeChoice(ETrappedManChoice Choice);

	// Returns true while rescue timer is active
	UFUNCTION(BlueprintPure) bool IsRescueInProgress() const { return bRescueActive; }
	UFUNCTION(BlueprintPure) float GetRescueTimeRemaining() const;
	UFUNCTION(BlueprintPure) ETrappedManChoice GetMadeChoice() const { return MadeChoice; }

	UPROPERTY(BlueprintAssignable) FOnTrappedManChoiceMade OnChoiceMade;
	UPROPERTY(BlueprintAssignable) FOnRescueCompleted    OnRescueCompleted;
	UPROPERTY(BlueprintAssignable) FOnRescueFailed       OnRescueFailed;

protected:
	UPROPERTY(VisibleAnywhere) UBoxComponent* InteractionVolume;

	// ─── Rescue config ────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Rescue")
	TSubclassOf<ASOLOEnemyCharacter> AshDogClass;

	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Rescue")
	int32 DogsPerWave = 2;

	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Rescue")
	float RescueDuration = 30.f;

	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Rescue")
	float DogSpawnRadius = 600.f;

	// ─── Kill config ──────────────────────────────────────────────────────────
	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Kill")
	TSubclassOf<UGameplayEffect> LethalGE;   // applied to trapped man to kill him

	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Kill")
	FName RewardItemID = TEXT("Item.N2.FragmentoPiedraHumana");

	// Reputation delta (negative = loss)
	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Kill")
	int32 KillReputationDelta = -20;

	UPROPERTY(EditDefaultsOnly, Category = "TrappedMan|Rescue")
	int32 RescueReputationDelta = 15;

	// ─── Blueprint hooks ──────────────────────────────────────────────────────
	UFUNCTION(BlueprintImplementableEvent) void BP_OnRescueStarted();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnWaveSpawned(int32 WaveIndex);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnRescueCompleted();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnRescueFailed();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnIgnore();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnKill();
	// Mira's horror reaction — BP plays anim/dialogue
	UFUNCTION(BlueprintImplementableEvent) void BP_OnMiraHorrorReaction();

private:
	ETrappedManChoice MadeChoice = ETrappedManChoice::None;

	// ─── Rescue state ─────────────────────────────────────────────────────────
	bool  bRescueActive  = false;
	int32 CurrentWave    = 0;
	int32 AliveDogsCount = 0;
	float RescueStartTime = 0.f;

	FTimerHandle RescueTimer;
	FTimerHandle WaveSpawnTimer;

	void StartRescue();
	void SpawnDogWave();
	void OnDogKilled(AActor* Dog);
	void OnRescueTimerExpired();
	void FinalizeRescue();

	void ExecuteIgnore();
	void ExecuteKill();

	void ApplyReputationDelta(int32 Delta);
	void GrantItemToPlayer(FName ItemID);
};
