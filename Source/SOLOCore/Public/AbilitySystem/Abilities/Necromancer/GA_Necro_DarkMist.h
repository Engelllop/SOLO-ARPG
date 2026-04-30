#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_DarkMist.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_DarkMist : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_DarkMist();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> MistFieldClass;
	UPROPERTY(EditDefaultsOnly) float FieldDuration = 12.f;
	UPROPERTY(EditDefaultsOnly) float FieldRadius = 1000.f;
};
