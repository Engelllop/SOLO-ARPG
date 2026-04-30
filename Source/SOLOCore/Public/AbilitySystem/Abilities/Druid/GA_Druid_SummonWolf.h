#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Druid_SummonWolf.generated.h"

UCLASS()
class SOLOCORE_API UGA_Druid_SummonWolf : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Druid_SummonWolf();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ASOLOWolfMinion> WolfClass;
	UPROPERTY(EditDefaultsOnly) int32 MaxWolves = 2;
	UPROPERTY(EditDefaultsOnly) float WolfDuration = 45.f;
	UPROPERTY(EditDefaultsOnly) float AttackScale = 0.8f;
	UPROPERTY(EditDefaultsOnly) float HPScale = 0.4f;

	UPROPERTY() TArray<TWeakObjectPtr<AActor>> ActiveWolves;
};
