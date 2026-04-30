#include "AbilitySystem/Abilities/GA_Dodge.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GameplayEffects/GE_DodgeInvulnerability.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

static constexpr float StaminaDodgeCost = 25.f;

UGA_Dodge::UGA_Dodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = ESOLOAbilityInputID::Dodge;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	DodgingTag    = FGameplayTag::RequestGameplayTag(FName("State.Dodging"));
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("Cost.Stamina"));

	CooldownTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Dodge")));

	DodgeInvulnerabilityEffect = USOLO_GE_DodgeInvulnerability::StaticClass();
}

const FGameplayTagContainer* UGA_Dodge::GetCooldownTags() const
{
	return &CooldownTagContainer;
}

void UGA_Dodge::ApplyCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!CooldownEffect)
	{
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CooldownEffect, GetAbilityLevel(), EffectContext);
	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UGA_Dodge::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const USOLOAttributeSet* AttributeSet = GetAttributeSet();
	if (!AttributeSet || AttributeSet->GetStamina() < StaminaDodgeCost)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ACharacter* Character = GetPlayerCharacter();
	if (!ASC || !Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Apply stamina cost via GE with SetByCaller
	if (StaminaCostEffect)
	{
		FGameplayEffectContextHandle CostContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle CostSpec = ASC->MakeOutgoingSpec(StaminaCostEffect, GetAbilityLevel(), CostContext);
		if (CostSpec.IsValid())
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(CostSpec, StaminaCostTag, -StaminaDodgeCost);
			ASC->ApplyGameplayEffectSpecToSelf(*CostSpec.Data.Get());
		}
	}

	// i-frames: GameplayEffect con State.Invulnerable (replicado por el ASC en servidor/clientes)
	{
		const TSubclassOf<UGameplayEffect> InvulnClass = DodgeInvulnerabilityEffect
			? DodgeInvulnerabilityEffect
			: TSubclassOf<UGameplayEffect>(USOLO_GE_DodgeInvulnerability::StaticClass());
		if (InvulnClass)
		{
			FGameplayEffectContextHandle InvulnContext = ASC->MakeEffectContext();
			InvulnContext.AddSourceObject(Character);
			FGameplayEffectSpecHandle InvulnSpec = ASC->MakeOutgoingSpec(InvulnClass, GetAbilityLevel(), InvulnContext);
			if (InvulnSpec.IsValid())
			{
				InvulnSpec.Data->SetDuration(DodgeInvulnerabilityDuration, true);
				InvulnerabilityEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*InvulnSpec.Data.Get());
			}
		}
	}

	// Estado de animaciÃ³n / lÃ³gica de juego (no confundir con invulnerabilidad)
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(DodgingTag);
		ASC->AddLooseGameplayTags(StateTags);
	}

	// Montaje: UGameplayAbility::PlayMontageAndWait â†’ ASC->PlayMontage (FGameplayAbilityRepAnimMontage replicado a todos los clientes).
	if (DodgeMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			DodgeMontage,
			1.f,
			NAME_None,
			true,
			1.f,
			0.f,
			false
		);
		if (MontageTask)
		{
			MontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
			MontageTask->OnBlendOut.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
			MontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCancelled);
			MontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCancelled);
			MontageTask->ReadyForActivation();
		}
		else
		{
			FTimerHandle EndTimer;
			GetWorld()->GetTimerManager().SetTimer(
				EndTimer,
				this,
				&UGA_Dodge::OnMontageCompleted,
				DodgeDuration,
				false
			);
		}
	}
	else
	{
		FTimerHandle EndTimer;
		GetWorld()->GetTimerManager().SetTimer(
			EndTimer,
			this,
			&UGA_Dodge::OnMontageCompleted,
			DodgeDuration,
			false
		);
	}

	// Desplazamiento: Root Motion Source constante (no solo animaciÃ³n: empuje fÃ­sico vÃ­a CMC; el movimiento se replica).
	FVector DodgeDirection = Character->GetLastMovementInputVector();
	if (DodgeDirection.IsNearlyZero())
	{
		DodgeDirection = -Character->GetActorForwardVector();
	}
	DodgeDirection = DodgeDirection.GetSafeNormal();

	const float Force = DodgeDistance / DodgeDuration;

	UAbilityTask_ApplyRootMotionConstantForce* RootMotionTask =
		UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this,
			NAME_None,
			DodgeDirection,
			Force,
			DodgeDuration,
			false,
			nullptr,
			ERootMotionFinishVelocityMode::ClampVelocity,
			FVector::ZeroVector,
			0.f,
			false
		);
	RootMotionTask->ReadyForActivation();
}

void UGA_Dodge::ClearInvulnerabilityEffect()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC && InvulnerabilityEffectHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(InvulnerabilityEffectHandle);
	}
	InvulnerabilityEffectHandle.Invalidate();
}

void UGA_Dodge::OnMontageCompleted()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(DodgingTag);
		ASC->RemoveLooseGameplayTags(StateTags);
	}
	ClearInvulnerabilityEffect();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Dodge::OnMontageCancelled()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(DodgingTag);
		ASC->RemoveLooseGameplayTags(StateTags);
	}
	ClearInvulnerabilityEffect();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGA_Dodge::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	ClearInvulnerabilityEffect();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
