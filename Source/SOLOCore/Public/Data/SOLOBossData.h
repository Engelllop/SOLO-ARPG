#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "SOLOBossData.generated.h"

class UAnimMontage;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct SOLOCORE_API FBossPhaseData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PhaseName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HPThreshold = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InvulnerabilityDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PostTransitionGrace = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PostureRegenRate = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> PhaseAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> TransitionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText PhaseDialog;
};

USTRUCT(BlueprintType)
struct SOLOCORE_API FBossAttackPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag IndicatorColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TelegraphTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUnblockable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUnparriable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bMagic = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundIndicatorRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> OnHitEffects;
};
