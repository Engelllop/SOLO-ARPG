#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_GravityField.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_GravityField : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Mage_GravityField();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> GravityFieldClass;
	UPROPERTY(EditDefaultsOnly) float FieldDuration = 5.f;
	UPROPERTY(EditDefaultsOnly) float FieldRadius = 800.f;
	UPROPERTY(EditDefaultsOnly) float SpawnRange = 2000.f;
};
