#include "World/N0/ASOLON0_ErrantEventActor.h"
#include "Character/SOLOEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

// Delay (seconds) before each step fires
static constexpr float ErrantStepDelays[] = {
	0.f,   // 0 — bells / camera toward east gate
	2.f,   // 1 — Errante enters, lands in plaza
	2.5f,  // 2 — 2 guards run toward Errante
	2.0f,  // 3 — Errante kills both guards
	1.5f,  // 4 — Aldric exits the Guild
	2.0f,  // 5 — Errante charges Aldric
	1.5f,  // 6 — Aldric dodges, grabs Errante by neck
	2.0f,  // 7 — 2s pause, Aldric crushes neck
	1.5f,  // 8 — Errante falls dead; Aldric walks back
};
static constexpr int32 TotalErrantSteps = 9;

ASOLON0_ErrantEventActor::ASOLON0_ErrantEventActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASOLON0_ErrantEventActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASOLON0_ErrantEventActor::TriggerEvent()
{
	if (bTriggered) return;
	bTriggered = true;
	CurrentStep = 0;
	AdvanceToStep(0);
}

void ASOLON0_ErrantEventActor::AdvanceToStep(int32 Step)
{
	CurrentStep = Step;
	ExecuteStep(Step);
	OnStepReached.Broadcast(Step);

	if (Step + 1 < TotalErrantSteps)
	{
		float Delay = ErrantStepDelays[Step + 1];
		int32 Next = Step + 1;
		if (Delay > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(StepTimer,
				FTimerDelegate::CreateWeakLambda(this, [this, Next]()
				{ AdvanceToStep(Next); }), Delay, false);
		}
		else
		{
			AdvanceToStep(Next);
		}
	}
	else
	{
		OnEventFinished.Broadcast();
	}
}

void ASOLON0_ErrantEventActor::ExecuteStep(int32 Step)
{
	switch (Step)
	{
	case 1: // Spawn Errante at the east gate entry point
		if (ErrantClass)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if (ASOLOEnemyCharacter* E = GetWorld()->SpawnActor<ASOLOEnemyCharacter>(
				ErrantClass, ErrantSpawnLocation, FRotator::ZeroRotator, Params))
			{
				SpawnedErrante = E;
			}
		}
		break;

	case 3: // Kill both guards
		KillActor(Guard1);
		KillActor(Guard2);
		break;

	case 8: // Kill the Errante (Aldric's grip)
		KillErrante();
		break;

	default: break;
	}
}

void ASOLON0_ErrantEventActor::KillActor(ACharacter* Target)
{
	if (!Target || !LethalDamageGE) return;
	if (auto* IASI = Cast<IAbilitySystemInterface>(Target))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			// Level 99 ensures lethal magnitude in the GE's scalable float
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(LethalDamageGE, 99, Ctx);
			if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
}

void ASOLON0_ErrantEventActor::KillErrante()
{
	ASOLOEnemyCharacter* E = SpawnedErrante.Get();
	if (!E) return;

	if (auto* IASI = Cast<IAbilitySystemInterface>(E))
	{
		if (UAbilitySystemComponent* ASC = IASI->GetAbilitySystemComponent())
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(LethalDamageGE, 99, Ctx);
			if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}
}
