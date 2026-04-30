#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SOLOGameplayAbility.generated.h"

class USOLOAttributeSet;

UENUM(BlueprintType)
enum class ESOLOAbilityInputID : uint8
{
	None			UMETA(DisplayName = "None"),
	BasicAttack		UMETA(DisplayName = "Basic Attack"),
	Dodge			UMETA(DisplayName = "Dodge"),
	ChargedAttack	UMETA(DisplayName = "Charged Attack"),
	Skill1			UMETA(DisplayName = "Skill 1"),
	Skill2			UMETA(DisplayName = "Skill 2"),
	Skill3			UMETA(DisplayName = "Skill 3"),
	Skill4			UMETA(DisplayName = "Skill 4"),
	Ultimate		UMETA(DisplayName = "Ultimate"),
	Block			UMETA(DisplayName = "Block")
};

UCLASS(Abstract, Blueprintable)
class SOLOCORE_API USOLOGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USOLOGameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	ESOLOAbilityInputID AbilityInputID = ESOLOAbilityInputID::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTagContainer CancelAbilitiesWithTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTagContainer BlockAbilitiesWithTags;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	const USOLOAttributeSet* GetAttributeSet() const;
	ACharacter* GetPlayerCharacter() const;
};
