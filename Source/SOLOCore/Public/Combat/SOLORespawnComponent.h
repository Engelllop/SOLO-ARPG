#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLORespawnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAgonyStarted, AActor*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAgonyExpired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRevived);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawned);

UCLASS(ClassGroup = (SOLO), meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLORespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLORespawnComponent();

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void EnterAgony(AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void StartRevive(AActor* Reviver);

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void CompleteRevive();

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void InterruptRevive();

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void RespawnAtLastCampfire();

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void ApplyDeathPenalty();

	UFUNCTION(BlueprintCallable, Category = "Combat|Respawn")
	void SetLastCampfire(AActor* Campfire);

	UFUNCTION(BlueprintPure, Category = "Combat|Respawn")
	bool IsInAgony() const { return bIsInAgony; }

	UFUNCTION(BlueprintPure, Category = "Combat|Respawn")
	bool IsBeingRevived() const { return bIsBeingRevived; }

	UPROPERTY(BlueprintAssignable) FOnAgonyStarted OnAgonyStarted;
	UPROPERTY(BlueprintAssignable) FOnAgonyExpired OnAgonyExpired;
	UPROPERTY(BlueprintAssignable) FOnRevived OnRevived;
	UPROPERTY(BlueprintAssignable) FOnRespawned OnRespawned;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float AgonyDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float ReviveChannelTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float ReviveHPPercent = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float ReviveMPPercent = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn")
	float DeathGoldPenalty = 0.2f;

private:
	bool bIsInAgony = false;
	bool bIsBeingRevived = false;
	FTimerHandle AgonyTimerHandle;
	FTimerHandle ReviveTimerHandle;

	UPROPERTY()
	TWeakObjectPtr<AActor> LastCampfire;

	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentReviver;

	void OnAgonyTimerExpired();
	void RestoreVitals();
};
