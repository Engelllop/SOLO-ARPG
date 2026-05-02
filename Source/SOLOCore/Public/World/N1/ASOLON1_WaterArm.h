#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ASOLON1_WaterArm.generated.h"
class USphereComponent;
class UNiagaraComponent;
class UGameplayEffect;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaterArmBroken, ASOLON1_WaterArm*, Arm);

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_WaterArm : public AActor
{
	GENERATED_BODY()
public:
	ASOLON1_WaterArm();

	UPROPERTY(BlueprintAssignable) FOnWaterArmBroken OnBroken;

	UFUNCTION(BlueprintCallable, Category = "N1|WaterArm")
	void BreakArm();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& Event, AController* Instigator, AActor* Causer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|WaterArm")
	TObjectPtr<USphereComponent> GrabCollision;

	UPROPERTY(EditDefaultsOnly, Category = "N1|WaterArm")
	float SlowPercent = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|WaterArm")
	float DamagePerSecond = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|WaterArm")
	float GrabRadius = 120.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|WaterArm|GAS")
	TSubclassOf<UGameplayEffect> SlowGE;

	UPROPERTY(EditDefaultsOnly, Category = "N1|WaterArm|GAS")
	TSubclassOf<UGameplayEffect> DotGE;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnGrab(AActor* Target);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnBreak();

private:
	TWeakObjectPtr<AActor> GrabbedTarget;
	FActiveGameplayEffectHandle SlowHandle;
	bool bBroken = false;

	UFUNCTION() void OnGrabOverlap(UPrimitiveComponent* Comp, AActor* Other,
		UPrimitiveComponent* OtherComp, int32 Idx, bool bSweep, const FHitResult& Hit);

	void ApplyGrabEffects(AActor* Target);
	void RemoveGrabEffects();
};


