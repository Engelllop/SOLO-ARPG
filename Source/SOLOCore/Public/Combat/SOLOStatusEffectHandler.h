#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOLOStatusEffectHandler.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct SOLOCORE_API FStatusEffectInteraction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StatusA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StatusB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ResultEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConsumeBoth = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractionName;
};

UCLASS()
class SOLOCORE_API USOLOStatusEffectHandler : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Combat|Status")
	static void CheckStatusInteractions(UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "Combat|Status")
	static void ApplyStatusImmunity(UAbilitySystemComponent* ASC, FGameplayTag StatusTag, float Duration);

	UFUNCTION(BlueprintPure, Category = "Combat|Status")
	static bool IsImmuneToStatus(UAbilitySystemComponent* ASC, FGameplayTag StatusTag);

	UFUNCTION(BlueprintPure, Category = "Combat|Status")
	static float GetImmunityDuration(FGameplayTag StatusTag, bool bIsBoss);

	static const TArray<FStatusEffectInteraction>& GetInteractions();
};
