#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_BattleHymn.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_BattleHymn : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_BattleHymn();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> HymnBuff;
	UPROPERTY(EditDefaultsOnly) float BuffRadius = 1500.f;
};
