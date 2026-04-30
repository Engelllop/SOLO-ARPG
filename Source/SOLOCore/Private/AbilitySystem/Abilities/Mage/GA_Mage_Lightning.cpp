#include "AbilitySystem/Abilities/Mage/GA_Mage_Lightning.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

UGA_Mage_Lightning::UGA_Mage_Lightning()
{
	ManaCost = 30.f;
	SpellMultiplier = 2.0f;
}

void UGA_Mage_Lightning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || !ConsumeManaCost(ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	NotifyManaComponent(ActorInfo);

	ACharacter* Caster = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Caster) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	FVector Start = Caster->GetActorLocation() + FVector(0, 0, 50.f);
	FVector End = Start + Caster->GetControlRotation().Vector() * TraceRange;

	FHitResult Hit;
	TArray<AActor*> Ignore; Ignore.Add(Caster);

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Pawn), false, Ignore, EDrawDebugTrace::None, Hit, true))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (auto* TargetASI = Cast<IAbilitySystemInterface>(HitActor))
			{
				if (auto* TargetASC = TargetASI->GetAbilitySystemComponent())
				{
					float Mult = SpellMultiplier;
					if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Wet"))))
					{
						Mult = WetBonusMultiplier;
					}

					FGameplayEffectSpecHandle Spec = MakeDamageSpec(ActorInfo, Mult);
					if (Spec.IsValid())
					{
						ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
					}
				}
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
