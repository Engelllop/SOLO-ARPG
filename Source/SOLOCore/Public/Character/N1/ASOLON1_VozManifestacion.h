#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON1_VozManifestacion.generated.h"
class ASOLON1_VozReflejo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVozManifestacionDefeated);

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_VozManifestacion : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON1_VozManifestacion();

	void LinkReflejo(ASOLON1_VozReflejo* InReflejo);

	UFUNCTION(BlueprintCallable, Category = "N1|Voz")
	void BeginVulnerableWindow();

	UFUNCTION(BlueprintPure, Category = "N1|Voz")
	bool IsVulnerable() const { return bVulnerable; }

	UPROPERTY(BlueprintAssignable) FOnVozManifestacionDefeated OnDefeated;

protected:
	virtual void BeginPlay() override;
	virtual void Die_Implementation(AActor* InstigatorActor) override;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz")
	float VulnerableWindowDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|Attacks")
	float WaterSwipeDamage = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|Attacks")
	float WaterSwipeSlowPercent = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|Attacks")
	float ScreamAOERadius = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|Attacks")
	float ScreamStunDuration = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|GAS")
	TSubclassOf<UGameplayEffect> WaterSwipeGE;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|GAS")
	TSubclassOf<UGameplayEffect> ScreamStunGE;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voz|GAS")
	TSubclassOf<UGameplayEffect> InvulnerableGE;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnVulnerableStart();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnVulnerableEnd();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnDefeated();

private:
	bool bVulnerable = false;
	FTimerHandle VulnerableTimer;
	FActiveGameplayEffectHandle InvulnerableHandle;

	TWeakObjectPtr<ASOLON1_VozReflejo> ReflejoRef;

	void EndVulnerableWindow();
	void ApplyInvulnerability();
	void RemoveInvulnerability();

	UFUNCTION() void OnReflejoHit();
	UFUNCTION() void OnReflejoDestroyed();
};


