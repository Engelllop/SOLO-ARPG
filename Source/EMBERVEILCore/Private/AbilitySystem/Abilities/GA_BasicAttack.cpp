#include "AbilitySystem/Abilities/GA_BasicAttack.h"
#include "AbilitySystem/GameplayEffects/GE_Poison.h"
#include "AbilitySystem/GameplayEffects/GE_Stagger.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICombatInterface.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/EMBERVEILHUDBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Skills/EMBERVEILSkillMasteryComponent.h"

UGA_BasicAttack::UGA_BasicAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = EEMBERVEILAbilityInputID::BasicAttack;

	AttackingTag    = FGameplayTag::RequestGameplayTag(FName("State.Attacking"));
	WeaponSocketTag = FGameplayTag::RequestGameplayTag(FName("CombatSocket.Weapon"));

	PoisonEffect       = UGE_Poison::StaticClass();
	StaggerEffectClass = UGE_Stagger::StaticClass();
}

void UGA_BasicAttack::ActivateAbility(
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

	FGameplayTagContainer TagsToApply;
	TagsToApply.AddTag(AttackingTag);
	ASC->AddLooseGameplayTags(TagsToApply);

	PlayCurrentComboMontage();
}

void UGA_BasicAttack::PlayCurrentComboMontage()
{
	if (!AttackMontages.IsValidIndex(ComboStep) || !AttackMontages[ComboStep])
	{
		ResetCombo();
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontages[ComboStep],
		1.0f,
		NAME_None,
		true
	);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_BasicAttack::OnMontageCompleted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_BasicAttack::OnMontageBlendOut);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_BasicAttack::OnMontageCancelled);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_BasicAttack::OnMontageCancelled);
	MontageTask->ReadyForActivation();

	HitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FGameplayTag::RequestGameplayTag(FName("Event.Combat.HitReact")),
		nullptr,
		true,
		true
	);
	HitEventTask->EventReceived.AddDynamic(this, &UGA_BasicAttack::OnHitEventReceived);
	HitEventTask->ReadyForActivation();

	bComboWindowOpen = false;
	bComboQueued = false;
	bHitConfirmed = false;

	// Open combo window at ~60% through the animation — designer tunes montage notify instead in BP
	float MontageDuration = AttackMontages[ComboStep]->GetPlayLength();
	GetWorld()->GetTimerManager().SetTimer(
		ComboWindowTimer,
		this,
		&UGA_BasicAttack::OpenComboWindow,
		MontageDuration * 0.55f,
		false
	);
}

void UGA_BasicAttack::OpenComboWindow()
{
	bComboWindowOpen = true;
}

void UGA_BasicAttack::CloseComboWindow()
{
	bComboWindowOpen = false;
	bComboQueued = false;
}

void UGA_BasicAttack::OnHitEventReceived(FGameplayEventData Payload)
{
	PerformHitDetection();
}

void UGA_BasicAttack::PerformHitDetection()
{
	ACharacter* OwnerCharacter = GetPlayerCharacter();
	if (!OwnerCharacter)
	{
		return;
	}

	if (!OwnerCharacter->Implements<UCombatInterface>())
	{
		return;
	}

	const FVector TraceOrigin = ICombatInterface::Execute_GetCombatSocketLocation(OwnerCharacter, WeaponSocketTag);
	FVector TraceEnd = TraceOrigin + OwnerCharacter->GetActorForwardVector() * 150.f;

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerCharacter);

	UKismetSystemLibrary::SphereTraceMulti(
		OwnerCharacter,
		TraceOrigin,
		TraceEnd,
		80.f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	TSet<AActor*> HitActors;
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			continue;
		}

		if (HitActor->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);
		ApplyDamageToTarget(HitActor);
	}
}

void UGA_BasicAttack::ApplyDamageToTarget(AActor* Target)
{
	if (!Target || !DamageEffectClass)
	{
		return;
	}

	// Increment combo on first hit confirm
	if (!bHitConfirmed)
	{
		bHitConfirmed = true;
		if (ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			if (APlayerController* PC = Cast<APlayerController>(Avatar->GetController()))
			{
				if (AEMBERVEILHUDBase* HUD = PC->GetHUD<AEMBERVEILHUDBase>())
				{
					HUD->UpdateComboCounter(ComboStep, 3);
				}
			}
		}
	}

	const UEMBERVEILAttributeSet* AttributeSet = GetAttributeSet();
	if (!AttributeSet)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!TargetASC || !SourceASC)
	{
		return;
	}

	float DamageMultiplier = DamageMultipliers.IsValidIndex(ComboStep) ? DamageMultipliers[ComboStep] : 1.0f;
	float BaseDamage = AttributeSet->GetAttackDamage() * DamageMultiplier;

	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContext);
	if (!SpecHandle.IsValid())
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FGameplayTag::RequestGameplayTag(FName("Damage.Type.Physical")), BaseDamage);

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);

	if (ACharacter* Avatar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (Avatar->HasAuthority())
		{
			if (APlayerState* PS = Avatar->GetPlayerState())
			{
				if (UEMBERVEILSkillMasteryComponent* Mastery = PS->FindComponentByClass<UEMBERVEILSkillMasteryComponent>())
				{
					Mastery->AddMasteryXP(Mastery->GetActiveWeaponType(), 1.f);
				}
			}

			// Tercer golpe: probabilidad de aplicar veneno (solo servidor)
			if (ComboStep == 2 && PoisonEffect && FMath::FRand() < PoisonChanceOnComboFinisher)
			{
				const FGameplayEffectSpecHandle PoisonSpec = SourceASC->MakeOutgoingSpec(PoisonEffect, 1, EffectContext);
				if (PoisonSpec.IsValid())
				{
					SourceASC->ApplyGameplayEffectSpecToTarget(*PoisonSpec.Data.Get(), TargetASC);
				}
			}
		}
	}

	// Third combo hit staggers the enemy
	if (ComboStep == 2 && StaggerEffectClass)
	{
		FGameplayEffectSpecHandle StaggerSpec = SourceASC->MakeOutgoingSpec(StaggerEffectClass, GetAbilityLevel(), EffectContext);
		if (StaggerSpec.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerSpec.Data.Get(), TargetASC);
		}
	}
}

void UGA_BasicAttack::OnMontageCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(ComboWindowTimer);

	if (bComboQueued && ComboStep < 2)
	{
		ComboStep++;
		PlayCurrentComboMontage();
	}
	else
	{
		ResetCombo();
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC)
		{
			FGameplayTagContainer TagsToRemove;
			TagsToRemove.AddTag(AttackingTag);
			ASC->RemoveLooseGameplayTags(TagsToRemove);
		}
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UGA_BasicAttack::OnMontageBlendOut()
{
	// Blend out fires before completed on some montages — let OnMontageCompleted handle logic
}

void UGA_BasicAttack::OnMontageCancelled()
{
	ResetCombo();
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		FGameplayTagContainer TagsToRemove;
		TagsToRemove.AddTag(AttackingTag);
		ASC->RemoveLooseGameplayTags(TagsToRemove);
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UGA_BasicAttack::InputPressed(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	// Buffer the next combo hit if the window is open
	if (bComboWindowOpen && ComboStep < 2)
	{
		bComboQueued = true;
	}
}

void UGA_BasicAttack::ResetCombo()
{
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController<APlayerController>())
		{
			if (AEMBERVEILHUDBase* HUD = PC->GetHUD<AEMBERVEILHUDBase>())
			{
				HUD->ResetComboCounter();
			}
		}
		World->GetTimerManager().ClearTimer(ComboWindowTimer);
	}

	ComboStep = 0;
	bComboWindowOpen = false;
	bComboQueued = false;
}

void UGA_BasicAttack::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	GetWorld()->GetTimerManager().ClearTimer(ComboWindowTimer);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
