#include "Character/N0/ASOLON0_GuardianBoss.h"
#include "World/N0/ASOLON0_CainDeathEventActor.h"
#include "AbilitySystemComponent.h"

ASOLON0_GuardianBoss::ASOLON0_GuardianBoss()
{
	bIsBoss = true;
	// Phase thresholds: enter phase 1 at 50%, phase 2 at 25%
	PhaseThresholds = { 0.5f, 0.25f };
	BossDisplayName = FText::FromString(TEXT("Guardián de la Garganta"));
	XPReward = 100.f;
}

void ASOLON0_GuardianBoss::OnPhaseChanged_Implementation(int32 NewPhase)
{
	Super::OnPhaseChanged_Implementation(NewPhase);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();

	switch (NewPhase)
	{
	case 1: // 50% HP — faster, gains charge attack
		if (Phase2SpeedEffect && ASC)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(Phase2SpeedEffect, GetCharacterLevel_Implementation(), Ctx);
			if (Spec.IsValid()) ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
		break;

	case 2: // 25% HP — trigger Caín death event
		if (ASOLON0_CainDeathEventActor* EventActor = CainDeathEvent.Get())
		{
			EventActor->TriggerEvent(this);
		}
		else
		{
			// No event actor assigned — skip directly to vulnerability window
			BeginVulnerabilityWindow();
		}
		break;

	default: break;
	}
}

void ASOLON0_GuardianBoss::BeginVulnerabilityWindow()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (VulnerableEffect && ASC)
	{
		FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(VulnerableEffect, 1, Ctx);
		if (Spec.IsValid())
		{
			FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			FTimerHandle VulnTimer;
			TWeakObjectPtr<UAbilitySystemComponent> WeakASC(ASC);
			GetWorld()->GetTimerManager().SetTimer(VulnTimer,
				FTimerDelegate::CreateWeakLambda(this, [WeakASC, Handle]()
				{
					if (UAbilitySystemComponent* LASC = WeakASC.Get())
						LASC->RemoveActiveGameplayEffect(Handle);
				}), VulnerabilityWindowDuration, false);
		}
	}
}
