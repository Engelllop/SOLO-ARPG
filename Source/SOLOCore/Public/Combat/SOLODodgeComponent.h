#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOLODodgeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeEnded);

UCLASS(ClassGroup = (SOLO), meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLODodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOLODodgeComponent();

	UFUNCTION(BlueprintCallable, Category = "Combat|Dodge")
	bool StartDodge(FVector Direction);

	UFUNCTION(BlueprintPure, Category = "Combat|Dodge")
	bool CanDodge() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Dodge")
	bool IsDodging() const { return bIsDodging; }

	UFUNCTION(BlueprintPure, Category = "Combat|Dodge")
	bool IsInIFrames() const { return bInIFrames; }

	UPROPERTY(BlueprintAssignable)
	FOnDodgeStarted OnDodgeStarted;

	UPROPERTY(BlueprintAssignable)
	FOnDodgeEnded OnDodgeEnded;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeSPCost = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float IFrameDuration = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeCooldown = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeDuration = 0.5f;

private:
	bool bIsDodging = false;
	bool bInIFrames = false;
	bool bOnCooldown = false;
	FTimerHandle IFrameHandle;
	FTimerHandle DodgeEndHandle;
	FTimerHandle CooldownHandle;

	void EnableIFrames();
	void DisableIFrames();
	void OnDodgeEnd();
	void ResetCooldown();
	bool ConsumeSP();
};
