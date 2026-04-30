#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "USOLOAshRegenComponent.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

// Perro de Ceniza: regenerates HP while standing on ash-tagged terrain.
// Ice/water damage tag (Damage.Element.Ice) cancels regen for CancelDuration seconds.
UCLASS(ClassGroup = "SOLO|N2", meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOAshRegenComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	USOLOAshRegenComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// Called by line-trace ground tag checker (set up in Blueprint or BeginPlay)
	UFUNCTION(BlueprintCallable) void SetOnAsh(bool bIsOnAsh);

	// Called when the owner takes ice/water damage
	UFUNCTION(BlueprintCallable) void OnIceDamageReceived();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AshRegen")
	TSubclassOf<UGameplayEffect> RegenGE;

	// How long ice damage suppresses regen
	UPROPERTY(EditDefaultsOnly, Category = "AshRegen")
	float CancelDuration = 5.f;

	// Ground check interval
	UPROPERTY(EditDefaultsOnly, Category = "AshRegen")
	float GroundCheckInterval = 0.5f;

	// Tag to look for on the physical material / landscape layer below the pawn
	UPROPERTY(EditDefaultsOnly, Category = "AshRegen")
	FName AshGroundTag = TEXT("Ground.Ash");

private:
	bool bOnAsh       = false;
	bool bIceSuppressed = false;

	FActiveGameplayEffectHandle RegenHandle;
	FTimerHandle CancelTimer;
	FTimerHandle GroundCheckTimer;

	UAbilitySystemComponent* GetOwnerASC() const;
	void ApplyRegen();
	void RemoveRegen();
	void TickGroundCheck();
};
