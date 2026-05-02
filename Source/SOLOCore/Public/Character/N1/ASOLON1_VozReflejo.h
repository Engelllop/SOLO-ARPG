#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON1_VozReflejo.generated.h"

class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReflejoHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReflejoDestroyed);

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_VozReflejo : public AActor
{
	GENERATED_BODY()
public:
	ASOLON1_VozReflejo();

	UPROPERTY(BlueprintAssignable) FOnReflejoHit OnReflejoHit;
	UPROPERTY(BlueprintAssignable) FOnReflejoDestroyed OnReflejoDestroyed;

	UFUNCTION(BlueprintPure, Category = "N1|Reflejo")
	bool IsDestroyed() const { return bDestroyed; }

	UFUNCTION(BlueprintPure, Category = "N1|Reflejo")
	int32 GetRemainingHits() const { return HitsRemaining; }

protected:
	virtual float TakeDamage(float Damage, const FDamageEvent& Event,
		AController* Instigator, AActor* Causer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Reflejo")
	TObjectPtr<UStaticMeshComponent> ReflejoMesh;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Reflejo")
	int32 MaxHits = 3;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnReflejoHit(int32 Remaining);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnReflejoDestroyed();

private:
	int32 HitsRemaining;
	bool bDestroyed = false;
};
