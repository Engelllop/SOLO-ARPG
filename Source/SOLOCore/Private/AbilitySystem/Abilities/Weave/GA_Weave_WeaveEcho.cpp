#include "AbilitySystem/Abilities/Weave/GA_Weave_WeaveEcho.h"
#include "AbilitySystemComponent.h"

UGA_Weave_WeaveEcho::UGA_Weave_WeaveEcho()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Weave_WeaveEcho::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{ EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	HitCounter = 0;

	// Stay active and count attack hit events; never OnlyTriggerOnce so it persists
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.Attack.HitConfirm")),
		nullptr,
		true,  // OnlyMatchExact
		true); // OnlyTriggerOnce=false keeps the task alive
	Task->EventReceived.AddDynamic(this, &UGA_Weave_WeaveEcho::OnAttackHit);
	Task->ReadyForActivation();
}

void UGA_Weave_WeaveEcho::OnAttackHit(FGameplayEventData Payload)
{
	if (++HitCounter < HitsPerCycle) return;

	HitCounter = 0;
	if (!UnblockableEffect || !CurrentActorInfo) return;
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	if (!ASC) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(UnblockableEffect, GetAbilityLevel(), Ctx);
	if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}
