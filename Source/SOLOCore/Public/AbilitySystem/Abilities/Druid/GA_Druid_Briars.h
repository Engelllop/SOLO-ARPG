#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Druid_Briars.generated.h"

UCLASS()
class SOLOCORE_API UGA_Druid_Briars : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Druid_Briars();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> BriarsFieldClass;
	UPROPERTY(EditDefaultsOnly) float FieldDuration = 8.f;
	UPROPERTY(EditDefaultsOnly) float SpawnRange = 1500.f;
};
