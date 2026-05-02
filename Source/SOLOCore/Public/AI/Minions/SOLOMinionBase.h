#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "SOLOMinionBase.generated.h"

class UAbilitySystemComponent;
class USOLOAttributeSet;

UCLASS(Abstract)
class SOLOCORE_API ASOLOMinionBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASOLOMinionBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, Category = "Minion")
	void InitMinion(AActor* MinionOwner, float AttackScale, float HPScale, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Minion")
	void ApplyStatBuff(float Multiplier);

	UFUNCTION(BlueprintPure, Category = "Minion")
	AActor* GetMinionOwner() const { return MinionOwner.Get(); }

	UFUNCTION(BlueprintPure, Category = "Minion")
	bool IsAlive() const { return !bDead; }

	UFUNCTION(BlueprintCallable, Category = "Minion")
	void Kill();

protected:
	UPROPERTY(VisibleAnywhere) TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<USOLOAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Minion")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	TWeakObjectPtr<AActor> MinionOwner;
	float LifeSpanDuration = 60.f;
	bool bDead = false;

	virtual void BeginPlay() override;
	void OnLifeSpanExpired();
};
