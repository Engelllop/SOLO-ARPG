#include "AbilitySystem/Abilities/GA_Ultimate.h"

#include "AbilitySystem/GameplayEffects/GE_DamageBase.h"
#include "AbilitySystem/GameplayEffects/GE_Stagger.h"
#include "Attributes/EMBERVEILAttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICombatInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

UGA_Ultimate::UGA_Ultimate()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = EEMBERVEILAbilityInputID::Ultimate;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	StaggerEffectClass = UGE_Stagger::StaticClass();

	ManaCostTag = FGameplayTag::RequestGameplayTag(FName("Cost.Mana"), false);
	UltimateImpactEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Ability.Ultimate.Impact"), false);

	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Ultimate"), false);

	CooldownTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Cooldown.Ability.Ultimate"), false));
}

const FGameplayTagContainer* UGA_Ultimate::GetCooldownTags() const
{
	return &CooldownTagContainer;
}

void UGA_Ultimate::ApplyCooldown(
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
	if (!SpecHandle.IsValid())
	{
		return;
	}

	SpecHandle.Data->SetDuration(CooldownDuration, true);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UGA_Ultimate::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const UEMBERVEILAttributeSet* AttributeSet = GetAttributeSet();
	if (!AttributeSet || AttributeSet->GetMana() + KINDA_SMALL_NUMBER < ManaCost)
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
	ACharacter* Avatar = GetPlayerCharacter();
	if (!ASC || !Avatar)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ManaCostEffect && ManaCostTag.IsValid())
	{
		FGameplayEffectContextHandle CostContext = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle ManaSpec = ASC->MakeOutgoingSpec(ManaCostEffect, GetAbilityLevel(), CostContext);
		if (ManaSpec.IsValid())
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(ManaSpec, ManaCostTag, -ManaCost);
			ASC->ApplyGameplayEffectSpecToSelf(*ManaSpec.Data.Get());
		}
	}

	if (UltimateMontage)
	{
		if (UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				UltimateMontage,
				1.f,
				NAME_None,
				true,
				1.f,
				0.f,
				false))
		{
			MontageTask->OnCompleted.AddDynamic(this, &UGA_Ultimate::OnUltimateMontageFinished);
			MontageTask->OnBlendOut.AddDynamic(this, &UGA_Ultimate::OnUltimateMontageFinished);
			MontageTask->OnInterrupted.AddDynamic(this, &UGA_Ultimate::OnUltimateMontageFinished);
			MontageTask->OnCancelled.AddDynamic(this, &UGA_Ultimate::OnUltimateMontageFinished);
			MontageTask->ReadyForActivation();
		}
	}

	if (UltimateImpactEventTag.IsValid())
	{
		if (UAbilityTask_WaitGameplayEvent* ImpactTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this,
				UltimateImpactEventTag,
				nullptr,
				true,
				true))
		{
			ImpactTask->EventReceived.AddDynamic(this, &UGA_Ultimate::OnUltimateImpact);
			ImpactTask->ReadyForActivation();
		}
	}
	else
	{
		OnUltimateImpact(FGameplayEventData());
	}
}

void UGA_Ultimate::OnUltimateImpact(FGameplayEventData Payload)
{
	(void)Payload;

	if (bUltimateImpactHandled)
	{
		return;
	}
	bUltimateImpactHandled = true;

	ACharacter* Avatar = GetPlayerCharacter();
	if (Avatar && Avatar->HasAuthority())
	{
		ApplyUltimateToOverlappingEnemies();
	}

	SpawnUltimateVFX();

	if (!UltimateMontage)
	{
		OnUltimateMontageFinished();
	}
}

void UGA_Ultimate::OnUltimateMontageFinished()
{
	if (!IsActive())
	{
		return;
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UGA_Ultimate::ApplyUltimateToOverlappingEnemies()
{
	ACharacter* Avatar = GetPlayerCharacter();
	if (!Avatar)
	{
		return;
	}

	const UEMBERVEILAttributeSet* AttributeSet = GetAttributeSet();
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!AttributeSet || !SourceASC || !DamageEffectClass)
	{
		return;
	}

	const TSubclassOf<UGameplayEffect> StaggerClass = StaggerEffectClass
		? StaggerEffectClass
		: TSubclassOf<UGameplayEffect>(UGE_Stagger::StaticClass());
	if (!StaggerClass)
	{
		return;
	}

	UWorld* World = Avatar->GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Center = Avatar->GetActorLocation();
	const float BaseDamage = AttributeSet->GetAttackDamage() * UltimateDamageMultiplier;

	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UltimateOverlap), false, Avatar);
	Params.AddIgnoredActor(Avatar);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	World->OverlapMultiByObjectType(
		Overlaps,
		Center,
		FQuat::Identity,
		ObjectParams,
		FCollisionShape::MakeSphere(UltimateRadius),
		Params
	);

	TSet<AActor*> Seen;
	for (const FOverlapResult& O : Overlaps)
	{
		AActor* HitActor = O.GetActor();
		if (!HitActor || Seen.Contains(HitActor))
		{
			continue;
		}

		AEMBERVEILEnemyCharacter* Enemy = Cast<AEMBERVEILEnemyCharacter>(HitActor);
		if (!Enemy)
		{
			continue;
		}

		if (Enemy->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Enemy))
		{
			continue;
		}

		Seen.Add(HitActor);

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Enemy);
		if (!TargetASC)
		{
			continue;
		}

		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		EffectContext.AddSourceObject(Avatar);

		FGameplayEffectSpecHandle DamageSpec = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContext);
		if (DamageSpec.IsValid())
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				DamageSpec,
				FGameplayTag::RequestGameplayTag(FName("Damage.Type.Physical")),
				BaseDamage
			);
			SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpec.Data.Get(), TargetASC);
		}

		FGameplayEffectSpecHandle StaggerSpec = SourceASC->MakeOutgoingSpec(StaggerClass, GetAbilityLevel(), EffectContext);
		if (StaggerSpec.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToTarget(*StaggerSpec.Data.Get(), TargetASC);
		}

		FVector Dir = Enemy->GetActorLocation() - Center;
		Dir.Z = 0.f;
		Dir = Dir.GetSafeNormal();
		if (Dir.IsNearlyZero())
		{
			Dir = Avatar->GetActorForwardVector();
		}

		const FVector LaunchVel = Dir * KnockbackHorizontalForce + FVector(0.f, 0.f, KnockbackUpwardForce);
		Enemy->LaunchCharacter(LaunchVel, true, true);
	}
}

void UGA_Ultimate::SpawnUltimateVFX() const
{
	ACharacter* Avatar = GetPlayerCharacter();
	if (!Avatar || !UltimateAreaVFX)
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		Avatar,
		UltimateAreaVFX,
		Avatar->GetActorLocation(),
		Avatar->GetActorRotation(),
		FVector(UltimateRadius / 250.f)
	);
}

