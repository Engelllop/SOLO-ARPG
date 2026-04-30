#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Druid_NatureAvatar.generated.h"

UCLASS()
class SOLOCORE_API UGA_Druid_NatureAvatar : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Druid_NatureAvatar();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) float AvatarDuration = 15.f;
	UPROPERTY(EditDefaultsOnly) float AttackRadius = 500.f;
	UPROPERTY(EditDefaultsOnly) float HealAuraRadius = 800.f;
	UPROPERTY(EditDefaultsOnly) float HealPercentPerSecond = 0.02f;
	UPROPERTY(EditDefaultsOnly) FVector AvatarScale = FVector(2.0f);
private:
	FTimerHandle AuraTickHandle;
	FTimerHandle DurationHandle;
	void OnAuraTick();
	void EndAvatar();
};
