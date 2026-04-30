#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Mage/SOLOMageSpellBase.h"
#include "GA_Druid_BeastForm.generated.h"

UENUM(BlueprintType)
enum class EBeastFormType : uint8 { Bear, Wolf };

UCLASS()
class SOLOCORE_API UGA_Druid_BeastForm : public USOLOMageSpellBase
{
	GENERATED_BODY()
public:
	UGA_Druid_BeastForm();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> BearFormEffect;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UGameplayEffect> WolfFormEffect;
	UPROPERTY(EditDefaultsOnly) float Duration = 20.f;
	UPROPERTY(EditDefaultsOnly) EBeastFormType DefaultForm = EBeastFormType::Bear;
};
