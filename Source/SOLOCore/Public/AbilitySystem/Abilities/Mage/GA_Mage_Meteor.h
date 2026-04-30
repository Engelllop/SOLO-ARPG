#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_Meteor.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_Meteor : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Mage_Meteor();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> MeteorClass;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> StunEffect;
	UPROPERTY(EditDefaultsOnly) float ImpactDelay = 2.0f;
	UPROPERTY(EditDefaultsOnly) float ImpactRadius = 1200.f;
	UPROPERTY(EditDefaultsOnly) float SpawnHeight = 3000.f;
	UPROPERTY(EditDefaultsOnly) float TargetRange = 3000.f;
private:
	void OnMeteorImpact(FVector ImpactLocation);
};
