#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_DiscordantNote.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_DiscordantNote : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_DiscordantNote();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<AActor> NoteProjectileClass;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> ConfuseEffect;
};
