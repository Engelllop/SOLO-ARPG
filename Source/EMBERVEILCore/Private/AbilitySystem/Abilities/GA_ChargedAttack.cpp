#include "AbilitySystem/Abilities/GA_ChargedAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GameplayEffects/GE_Stagger.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICombatInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

UGA_ChargedAttack::UGA_ChargedAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID   = EEMBERVEILAbilityInputID::ChargedAttack;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	AttackingTag   = FGameplayTag::RequestGameplayTag(FName("State.Attacking"));
	StaminaCostTag = FGameplayTag::RequestGameplayTag(FName("Cost.Stamina"));

	StaggerEffect = UGE_Stagger::StaticClass();
}

// ─── ActivateAbility ────────────────────────────────────────────────────────

void UGA_ChargedAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Mark character as attacking for animation state machines
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(AttackingTag);
		ASC->AddLooseGameplayTags(StateTags);
	}

	// Reset charge state
	bIsCharging  = true;
	ChargeRatio  = 0.f;
	ActiveChargeVFX = nullptr;

	// Spawn charge VFX attached to the character (async-safe: load then spawn)
	ACharacter* OwnerCharacter = GetPlayerCharacter();
	if (OwnerCharacter && !ChargeVFX.IsNull())
	{
		UNiagaraSystem* ChargeSystem = ChargeVFX.LoadSynchronous();
		if (ChargeSystem)
		{
			ActiveChargeVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ChargeSystem,
				OwnerCharacter->GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true  // auto-destroy on deactivation
			);
		}
	}

	// Start charge buildup timer — fires every 0.05 s to increment ChargeRatio
	const float TickInterval = 0.05f;
	GetWorld()->GetTimerManager().SetTimer(
		ChargeTimerHandle,
		[this]()
		{
			ChargeRatio = FMath::Clamp(ChargeRatio + (TickInterval / ChargeTime), 0.f, 1.f);
		},
		TickInterval,
		true   // looping
	);

	// Play the charge-start montage (does not stop on ability end — release handles that)
	if (ChargeStartMontage)
	{
		UAbilityTask_PlayMontageAndWait* ChargeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("ChargeStart"),
			ChargeStartMontage,
			1.0f,
			NAME_None,
			false
		);
		ChargeTask->ReadyForActivation();
	}

	// Listen for input release via GAS task so prediction works correctly
	UAbilityTask_WaitInputRelease* WaitRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitRelease->OnRelease.AddDynamic(this, &UGA_ChargedAttack::OnInputReleased);
	WaitRelease->ReadyForActivation();
}

// ─── OnInputReleased ────────────────────────────────────────────────────────

void UGA_ChargedAttack::OnInputReleased(float TimeHeld)
{
	if (!bIsCharging)
	{
		return;
	}

	ReleaseCharge();
}

// ─── ReleaseCharge ──────────────────────────────────────────────────────────

void UGA_ChargedAttack::ReleaseCharge()
{
	// 1. Stop charge timer
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);

	// 2. Deactivate / destroy charge VFX
	if (ActiveChargeVFX)
	{
		ActiveChargeVFX->Deactivate();
		ActiveChargeVFX = nullptr;
	}

	bIsCharging = false;

	// 3. Calculate final values from ChargeRatio
	const float FinalDamage  = BaseDamage      * (0.5f + ChargeRatio * 1.5f);
	const float FinalRadius  = ChargeRadius    * (0.5f + ChargeRatio * 0.5f);
	const float FinalStamina = BaseStaminaCost * (0.5f + ChargeRatio * 0.5f);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ACharacter*              OwnerCharacter = GetPlayerCharacter();

	if (!ASC || !OwnerCharacter)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
		return;
	}

	// 4. Apply stamina cost via GE SetByCaller
	if (StaminaCostEffect)
	{
		FGameplayEffectContextHandle CostContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle CostSpec = ASC->MakeOutgoingSpec(StaminaCostEffect, GetAbilityLevel(), CostContext);
		if (CostSpec.IsValid())
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(CostSpec, StaminaCostTag, -FinalStamina);
			ASC->ApplyGameplayEffectSpecToSelf(*CostSpec.Data.Get());
		}
	}

	// 5. AOE overlap — centre on character
	const FVector Origin = OwnerCharacter->GetActorLocation();

	TArray<AActor*> OverlappedActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	UKismetSystemLibrary::SphereOverlapActors(
		OwnerCharacter,
		Origin,
		FinalRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>{ UEngineTypes::ConvertToObjectType(ECC_Pawn) },
		nullptr,
		ActorsToIgnore,
		OverlappedActors
	);

	// 6. Apply damage (and optional stagger) to each valid target
	for (AActor* HitActor : OverlappedActors)
	{
		if (!HitActor)
		{
			continue;
		}

		// Skip dead targets
		if (HitActor->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(HitActor))
		{
			continue;
		}

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)
		{
			continue;
		}

		// a. Apply damage via GE_DamageBase SetByCaller
		if (DamageEffectClass)
		{
			FGameplayEffectContextHandle DmgContext = ASC->MakeEffectContext();
			DmgContext.AddSourceObject(GetAvatarActorFromActorInfo());

			FGameplayEffectSpecHandle DmgSpec = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), DmgContext);
			if (DmgSpec.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
					DmgSpec,
					FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
					FinalDamage
				);
				ASC->ApplyGameplayEffectSpecToTarget(*DmgSpec.Data.Get(), TargetASC);
			}
		}

		// b. Apply stagger on full charge
		if (ChargeRatio >= FullChargeThreshold && StaggerEffect)
		{
			FGameplayEffectContextHandle StaggerContext = ASC->MakeEffectContext();
			StaggerContext.AddSourceObject(GetAvatarActorFromActorInfo());

			FGameplayEffectSpecHandle StaggerSpec = ASC->MakeOutgoingSpec(StaggerEffect, GetAbilityLevel(), StaggerContext);
			if (StaggerSpec.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToTarget(*StaggerSpec.Data.Get(), TargetASC);
			}
		}
	}

	// 7. Spawn release VFX at character location
	if (!ReleaseVFX.IsNull())
	{
		UNiagaraSystem* ReleaseSystem = ReleaseVFX.LoadSynchronous();
		if (ReleaseSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ReleaseSystem,
				Origin,
				OwnerCharacter->GetActorRotation()
			);
		}
	}

	// 8. Play release montage and end ability on completion, or end immediately if none
	if (ChargeReleaseMontage)
	{
		UAbilityTask_PlayMontageAndWait* ReleaseTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("ChargeRelease"),
			ChargeReleaseMontage,
			1.0f,
			NAME_None,
			true
		);
		ReleaseTask->OnCompleted.AddDynamic(this, &UGA_ChargedAttack::OnReleaseMontageCompleted);
		ReleaseTask->OnBlendOut.AddDynamic(this, &UGA_ChargedAttack::OnReleaseMontageCompleted);
		ReleaseTask->OnInterrupted.AddDynamic(this, &UGA_ChargedAttack::OnReleaseMontageInterrupted);
		ReleaseTask->OnCancelled.AddDynamic(this, &UGA_ChargedAttack::OnReleaseMontageInterrupted);
		ReleaseTask->ReadyForActivation();
	}
	else
	{
		// No release montage — clean up tags and end immediately
		UAbilitySystemComponent* CleanupASC = GetAbilitySystemComponentFromActorInfo();
		if (CleanupASC)
		{
			FGameplayTagContainer StateTags;
			StateTags.AddTag(AttackingTag);
			CleanupASC->RemoveLooseGameplayTags(StateTags);
		}
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

// ─── Montage callbacks ───────────────────────────────────────────────────────

void UGA_ChargedAttack::OnReleaseMontageCompleted()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(AttackingTag);
		ASC->RemoveLooseGameplayTags(StateTags);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_ChargedAttack::OnReleaseMontageInterrupted()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayTagContainer StateTags;
		StateTags.AddTag(AttackingTag);
		ASC->RemoveLooseGameplayTags(StateTags);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

// ─── EndAbility ─────────────────────────────────────────────────────────────

void UGA_ChargedAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	// Always clean up charge timer and VFX even if cancelled mid-charge
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);

	if (ActiveChargeVFX)
	{
		ActiveChargeVFX->Deactivate();
		ActiveChargeVFX = nullptr;
	}

	bIsCharging = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
