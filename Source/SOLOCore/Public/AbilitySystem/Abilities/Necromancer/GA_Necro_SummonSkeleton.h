#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Necro_SummonSkeleton.generated.h"

UCLASS()
class SOLOCORE_API UGA_Necro_SummonSkeleton : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Necro_SummonSkeleton();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ASOLOSkeletonMinion> SkeletonClass;
	UPROPERTY(EditDefaultsOnly) int32 MaxSkeletons = 3;
	UPROPERTY(EditDefaultsOnly) float MinionDuration = 60.f;
	UPROPERTY(EditDefaultsOnly) float AttackScale = 0.8f;
	UPROPERTY(EditDefaultsOnly) float HPScale = 0.5f;

	UPROPERTY() TArray<TWeakObjectPtr<AActor>> ActiveSkeletons;
	void CleanupDeadSkeletons();
};
