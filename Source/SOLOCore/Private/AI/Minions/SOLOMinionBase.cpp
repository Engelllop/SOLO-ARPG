#include "AI/Minions/SOLOMinionBase.h"
#include "AbilitySystemComponent.h"
#include "Attributes/SOLOAttributeSet.h"

ASOLOMinionBase::ASOLOMinionBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<USOLOAttributeSet>(TEXT("AttributeSet"));
}

void ASOLOMinionBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (DefaultAttributesEffect)
	{
		FGameplayEffectContextHandle Ctx = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect, 1.f, Ctx);
		if (Spec.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
}

void ASOLOMinionBase::InitMinion(AActor* Owner, float AttackScale, float HPScale, float Duration)
{
	MinionOwner = Owner;
	LifeSpanDuration = Duration;

	if (AttributeSet && Owner)
	{
		if (auto* OwnerASI = Cast<IAbilitySystemInterface>(Owner))
		{
			if (auto* OwnerASC = OwnerASI->GetAbilitySystemComponent())
			{
				const auto* OwnerAttrs = OwnerASC->GetSet<USOLOAttributeSet>();
				if (OwnerAttrs)
				{
					float ScaledAtk = OwnerAttrs->GetMagicDamage() * AttackScale;
					float ScaledHP = OwnerAttrs->GetMaxHealth() * HPScale;
					AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetAttackDamageAttribute(), EGameplayModOp::Override, ScaledAtk);
					AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetMaxHealthAttribute(), EGameplayModOp::Override, ScaledHP);
					AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Override, ScaledHP);
				}
			}
		}
	}

	SetLifeSpan(LifeSpanDuration);
}

void ASOLOMinionBase::ApplyStatBuff(float Multiplier)
{
	if (!AttributeSet) return;
	float CurAtk = AttributeSet->GetAttackDamage();
	float CurHP = AttributeSet->GetMaxHealth();
	AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetAttackDamageAttribute(), EGameplayModOp::Override, CurAtk * Multiplier);
	AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetMaxHealthAttribute(), EGameplayModOp::Override, CurHP * Multiplier);
	AbilitySystemComponent->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Override, CurHP * Multiplier);
}

void ASOLOMinionBase::Kill()
{
	bDead = true;
	SetLifeSpan(0.1f);
}
