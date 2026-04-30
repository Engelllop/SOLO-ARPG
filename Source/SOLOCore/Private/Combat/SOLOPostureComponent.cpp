#include "Combat/SOLOPostureComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"

USOLOPostureComponent::USOLOPostureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentPosture = MaxPosture;
	TimeSinceLastDamage = 0.f;
}

void USOLOPostureComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentPosture = MaxPosture;
}

void USOLOPostureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsBlocking)
	{
		ConsumeBlockSP(DeltaTime);
	}

	TimeSinceLastDamage += DeltaTime;

	if (!bPostureBroken && TimeSinceLastDamage >= RecoveryDelay && CurrentPosture < MaxPosture)
	{
		RecoverPosture(DeltaTime);
	}

	if (bPostureBroken && TimeSinceLastDamage >= RecoveryDelay * 2.f)
	{
		bPostureBroken = false;
		CurrentPosture = MaxPosture * 0.3f;
		OnPostureChanged.Broadcast(GetPosturePercent());
	}
}

void USOLOPostureComponent::StartBlock()
{
	if (CanBlock())
	{
		bIsBlocking = true;
	}
}

void USOLOPostureComponent::StopBlock()
{
	bIsBlocking = false;
}

bool USOLOPostureComponent::AttemptParry()
{
	if (!CanParry()) return false;

	bIsParrying = true;
	bParryOnCooldown = true;

	GetWorld()->GetTimerManager().SetTimer(ParryWindowHandle, this, &USOLOPostureComponent::EndParryWindow, ParryWindowSeconds, false);
	GetWorld()->GetTimerManager().SetTimer(ParryCooldownHandle, this, &USOLOPostureComponent::ResetParryCooldown, ParryCooldown, false);

	return true;
}

float USOLOPostureComponent::ApplyBlockedDamage(float IncomingDamage, AActor* Attacker, FGameplayTag DamageTypeTag)
{
	if (!bIsBlocking) return IncomingDamage;

	if (DamageTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Attack.Unblockable"))))
	{
		return IncomingDamage;
	}

	if (bIsParrying)
	{
		if (Attacker)
		{
			if (auto* ASI = Cast<IAbilitySystemInterface>(Attacker))
			{
				if (auto* ASC = ASI->GetAbilitySystemComponent())
				{
					ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));
					FTimerHandle StunHandle;
					FTimerDelegate StunDelegate;
					StunDelegate.BindLambda([ASC]()
					{
						ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stunned")));
					});
					GetWorld()->GetTimerManager().SetTimer(StunHandle, StunDelegate, StunDurationOnParry, false);
				}
			}
		}
		return 0.f;
	}

	bool bIsMagic = DamageTypeTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Damage.Category.Magical")));
	float Reduction = bIsMagic ? BlockMagicalReduction : BlockPhysicalReduction;
	float ReducedDamage = IncomingDamage * (1.f - Reduction);

	ApplyPostureDamage(IncomingDamage * 0.5f);

	return ReducedDamage;
}

void USOLOPostureComponent::ApplyPostureDamage(float Amount)
{
	CurrentPosture = FMath::Max(0.f, CurrentPosture - Amount);
	TimeSinceLastDamage = 0.f;
	OnPostureChanged.Broadcast(GetPosturePercent());

	if (CurrentPosture <= 0.f && !bPostureBroken)
	{
		BreakPosture();
	}
}

void USOLOPostureComponent::BreakPosture()
{
	bPostureBroken = true;
	bIsBlocking = false;
	CurrentPosture = 0.f;
	OnPostureBroken.Broadcast();
}

void USOLOPostureComponent::RecoverPosture(float DeltaTime)
{
	float RecoveryAmount = (MaxPosture * PostureRecoveryRate / 100.f) * DeltaTime;
	CurrentPosture = FMath::Min(MaxPosture, CurrentPosture + RecoveryAmount);
	OnPostureChanged.Broadcast(GetPosturePercent());
}

bool USOLOPostureComponent::CanBlock() const
{
	return !bPostureBroken && !bIsBlocking;
}

bool USOLOPostureComponent::CanParry() const
{
	return !bPostureBroken && !bParryOnCooldown;
}

float USOLOPostureComponent::GetPosturePercent() const
{
	return MaxPosture > 0.f ? CurrentPosture / MaxPosture : 0.f;
}

float USOLOPostureComponent::GetBonusDamageMultiplier() const
{
	if (bPostureBroken) return 1.5f;
	float LostPercent = 1.f - GetPosturePercent();
	int32 Tiers = FMath::FloorToInt(LostPercent * 10.f);
	return 1.f + (Tiers * 0.02f);
}

void USOLOPostureComponent::EndParryWindow()
{
	bIsParrying = false;
}

void USOLOPostureComponent::ResetParryCooldown()
{
	bParryOnCooldown = false;
}

void USOLOPostureComponent::ConsumeBlockSP(float DeltaTime)
{
	auto* Owner = GetOwner();
	if (!Owner) return;

	auto* ASI = Cast<IAbilitySystemInterface>(Owner);
	if (!ASI) return;

	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;

	const USOLOAttributeSet* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs) return;

	float CurrentSP = Attrs->GetStamina();
	float Cost = SPPerSecondBlockCost * DeltaTime;

	if (CurrentSP <= Cost)
	{
		StopBlock();
		BreakPosture();
		return;
	}

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -Cost);
}
