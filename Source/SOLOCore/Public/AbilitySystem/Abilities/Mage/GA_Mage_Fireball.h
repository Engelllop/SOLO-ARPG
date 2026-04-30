#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Mage_Fireball.generated.h"

UCLASS()
class SOLOCORE_API UGA_Mage_Fireball : public USOLOMageSpellBase
{
	GENERATED_BODY()

public:
	UGA_Mage_Fireball();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	TSubclassOf<UGameplayEffect> BurnDOTEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
	float ProjectileSpeed = 3000.f;
};
