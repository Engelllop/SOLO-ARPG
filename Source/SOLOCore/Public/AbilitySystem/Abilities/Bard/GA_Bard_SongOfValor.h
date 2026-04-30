#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Bard_SongOfValor.generated.h"

UCLASS()
class SOLOCORE_API UGA_Bard_SongOfValor : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Bard_SongOfValor();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> ValorBuff;
	UPROPERTY(EditDefaultsOnly) float AuraRadius = 1200.f;
	UPROPERTY(EditDefaultsOnly) float ManaCostPerSecond = 10.f;
	UPROPERTY(EditDefaultsOnly) float MaxDuration = 20.f;
private:
	FTimerHandle SongHandle;
	float Elapsed = 0.f;
	void OnSongTick();
	void StopSong();
};
