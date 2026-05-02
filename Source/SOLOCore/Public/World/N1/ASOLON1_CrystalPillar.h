#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "ASOLON1_CrystalPillar.generated.h"
class UStaticMeshComponent;
class UNiagaraSystem;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPillarDestroyed, ASOLON1_CrystalPillar*, Pillar, int32, PillarIndex);

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_CrystalPillar : public AActor
{
	GENERATED_BODY()
public:
	ASOLON1_CrystalPillar();

	UPROPERTY(BlueprintAssignable) FOnPillarDestroyed OnPillarDestroyed;

	UFUNCTION(BlueprintPure, Category = "N1|Pillar")
	bool IsDestroyed() const { return bDestroyed; }

	UFUNCTION(BlueprintPure, Category = "N1|Pillar")
	int32 GetPillarIndex() const { return PillarIndex; }

	UFUNCTION(BlueprintPure, Category = "N1|Pillar")
	int32 GetRemainingHits() const { return HitsRemaining; }

	UFUNCTION(BlueprintCallable, Category = "N1|Pillar")
	void SetAOEDamageActive(bool bActive);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& Event, AController* Instigator, AActor* Causer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Pillar")
	TObjectPtr<UStaticMeshComponent> PillarMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "N1|Pillar")
	int32 PillarIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Pillar")
	int32 MaxHits = 3;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Pillar")
	float AOEDamageRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Pillar")
	float AOEDamagePerSecond = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Pillar|GAS")
	TSubclassOf<UGameplayEffect> AOEDamageGE;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Pillar|VFX")
	TObjectPtr<UNiagaraSystem> DestructionVFX;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnHit(int32 Remaining);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnDestroyed();

private:
	int32 HitsRemaining;
	bool bDestroyed = false;
	bool bAOEActive = false;
	float AOETickAccumulator = 0.f;

	void DestroyPillar(AController* InstigatorActor);
	void TickAOEDamage(float DeltaSeconds);
};


