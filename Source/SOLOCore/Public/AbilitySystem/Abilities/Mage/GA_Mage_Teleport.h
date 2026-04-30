#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_Teleport.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_Teleport : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Mage_Teleport();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float TeleportDistance = 1000.f;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> TeleportVFXClass;
};
