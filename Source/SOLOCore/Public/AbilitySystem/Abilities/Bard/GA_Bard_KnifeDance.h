#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_KnifeDance.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_KnifeDance : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_KnifeDance();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> EvasionEffect;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> KnifeDamageEffect;
	UPROPERTY(EditDefaultsOnly) float HitRadius = 250.f;
	UPROPERTY(EditDefaultsOnly) float StaminaCost = 25.f;
private:
	void PerformHit(UAbilitySystemComponent* ASC, AActor* AvatarActor);
};
