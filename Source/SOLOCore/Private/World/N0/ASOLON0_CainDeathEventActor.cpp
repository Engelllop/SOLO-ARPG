#include "World/N0/ASOLON0_CainDeathEventActor.h"
#include "Character/SOLOEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Step timing in seconds (delay after previous step)
static constexpr float StepDelays[] = {
	0.f,   // 1 — Caín sees Guardian stumbling
	2.f,   // 2 — Caín: "¡Ahora!"
	1.5f,  // 3 — Guardian smiles (animation)
	1.0f,  // 4 — Guardian catches Caín
	0.8f,  // 5 — First stab (left arm)
	0.6f,  // 6 — Second stab (right side)
	0.8f,  // 7 — Third stab (through chest)
	1.5f,  // 8 — Caín looks at protagonist
	1.0f,  // 9 — Fourth stab (lethal)
	1.0f,  // 10 — Guardian throws Caín
	1.5f,  // 11 — Caín on ground, smiles
	0.5f,  // 12 — Grant SimultaneousCuts to protagonist
	0.3f,  // 13 — Grant WeaveArrow to Mira
	0.5f,  // 14 — Vassen frozen 3s
	3.0f,  // 15 — Guardian still (5s vulnerability window opened)
	0.0f,  // 16 — (end of active sequence — Guardian can now be killed)
};
static constexpr int32 TotalSteps = 16;

ASOLON0_CainDeathEventActor::ASOLON0_CainDeathEventActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASOLON0_CainDeathEventActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASOLON0_CainDeathEventActor::TriggerEvent(ASOLOEnemyCharacter* InGuardian)
{
	if (bEventActive) return;
	bEventActive = true;
	GuardianRef = InGuardian;
	CurrentStep = 0;

	// Freeze all companion AIs during the event
	FreezeCharacterAI(MiraCharacter, true);
	FreezeCharacterAI(VassenCharacter, true);
	FreezeCharacterAI(CainCharacter, true);

	AdvanceToStep(0);
}

void ASOLON0_CainDeathEventActor::AdvanceToStep(int32 Step)
{
	CurrentStep = Step;
	ExecuteStep(Step);
	OnStepReached.Broadcast(Step);

	if (Step + 1 < TotalSteps)
		ScheduleNextStep(StepDelays[Step + 1]);
	else
		OnEventFinished.Broadcast();
}

void ASOLON0_CainDeathEventActor::ScheduleNextStep(float Delay)
{
	if (Delay <= 0.f)
	{
		AdvanceToStep(CurrentStep + 1);
		return;
	}
	int32 NextStep = CurrentStep + 1;
	GetWorld()->GetTimerManager().SetTimer(StepTimer,
		FTimerDelegate::CreateWeakLambda(this, [this, NextStep]()
		{ AdvanceToStep(NextStep); }), Delay, false);
}

void ASOLON0_CainDeathEventActor::ExecuteStep(int32 Step)
{
	switch (Step)
	{
	case 4: ApplyStabToCharacter(CainCharacter, false); break; // arm wound
	case 5: ApplyStabToCharacter(CainCharacter, false); break; // side wound
	case 6: ApplyStabToCharacter(CainCharacter, false); break; // chest wound
	case 8: ApplyStabToCharacter(CainCharacter, true);  break; // lethal

	case 11: // Grant SimultaneousCuts to protagonist
		GrantAbilityToCharacter(ProtagonistCharacter, SimultaneousCutsClass);
		break;

	case 12: // Grant WeaveArrow to Mira
		GrantAbilityToCharacter(MiraCharacter, WeaveArrowClass);
		FreezeCharacterAI(MiraCharacter, false);
		break;

	case 13: // Vassen unfreezes after 3s of shock
		GetWorld()->GetTimerManager().SetTimer(StepTimer,
			FTimerDelegate::CreateWeakLambda(this, [this]()
			{ FreezeCharacterAI(VassenCharacter, false); }), 3.0f, false);
		break;

	case 14: // Guardian 5s vulnerability window
		TriggerGuardianVulnerability();
		break;

	default: break;
	}
}

void ASOLON0_CainDeathEventActor::GrantAbilityToCharacter(ACharacter* Character, TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!Character || !AbilityClass) return;
	if (auto* IASI = Cast<IAbilitySystemInterface>(Character))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1);
			ASC->GiveAbility(Spec);
		}
	}
}

void ASOLON0_CainDeathEventActor::FreezeCharacterAI(ACharacter* Character, bool bFreeze)
{
	if (!Character) return;
	if (AAIController* AIC = Cast<AAIController>(Character->GetController()))
	{
		if (bFreeze) AIC->StopMovement();
		AIC->SetActorTickEnabled(!bFreeze);
	}
	if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
		Move->SetMovementMode(bFreeze ? MOVE_None : MOVE_Walking);
}

void ASOLON0_CainDeathEventActor::ApplyStabToCharacter(ACharacter* Character, bool bLethal)
{
	if (!Character || !StabDamageGE) return;
	if (auto* IASI = Cast<IAbilitySystemInterface>(Character))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			// Level 10 = lethal, level 1 = wound
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(StabDamageGE, bLethal ? 10 : 1, Ctx);
			if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
}

void ASOLON0_CainDeathEventActor::TriggerGuardianVulnerability()
{
	ASOLOEnemyCharacter* Guardian = GuardianRef.Get();
	if (!Guardian || !GuardianVulnerableGE) return;

	if (auto* IASI = Cast<IAbilitySystemInterface>(Guardian))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GuardianVulnerableGE, 1, Ctx);
			if (Spec.IsValid())
			{
				FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
				FTimerHandle VulnEnd;
				TWeakObjectPtr<UAbilitySystemComponent> WeakASC(ASC);
				GetWorld()->GetTimerManager().SetTimer(VulnEnd,
					FTimerDelegate::CreateWeakLambda(this, [WeakASC, Handle]()
					{
						if (UAbilitySystemComponent* LASC = WeakASC.Get())
							LASC->RemoveActiveGameplayEffect(Handle);
					}), 5.0f, false);
			}
		}
	}
}
