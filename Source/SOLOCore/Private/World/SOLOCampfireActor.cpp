#include "World/SOLOCampfireActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Attributes/SOLOAttributeSet.h"
#include "Combat/SOLORespawnComponent.h"
#include "Combat/SOLOManaComponent.h"
#include "GameFramework/Character.h"

ASOLOCampfireActor::ASOLOCampfireActor()
{
	PrimaryActorTick.bCanEverTick = false;

	FireMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireMesh"));
	RootComponent = FireMesh;

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetSphereRadius(200.f);
	InteractionVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	FireVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireVFX"));
	FireVFX->SetupAttachment(RootComponent);
}

void ASOLOCampfireActor::Activate()
{
	bIsActive = true;
	if (FireVFX) FireVFX->Activate();
}

void ASOLOCampfireActor::Rest(ACharacter* Character)
{
	if (!Character) return;

	if (!bIsActive) Activate();

	RestoreAllResources(Character);
	ReviveCompanions(Character);
	SaveCheckpoint(Character);
	ApplyCampfireBonus(Character);

	if (auto* ManaComp = Character->FindComponentByClass<USOLOManaComponent>())
	{
		ManaComp->ResetFatigue();
	}
}

void ASOLOCampfireActor::RestoreAllResources(ACharacter* Character)
{
	auto* ASI = Cast<IAbilitySystemInterface>(Character);
	if (!ASI) return;
	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;
	const auto* Attrs = ASC->GetSet<USOLOAttributeSet>();
	if (!Attrs) return;

	ASC->ApplyModToAttribute(USOLOAttributeSet::GetHealthAttribute(), EGameplayModOp::Override, Attrs->GetMaxHealth());
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetManaAttribute(), EGameplayModOp::Override, Attrs->GetMaxMana());
	ASC->ApplyModToAttribute(USOLOAttributeSet::GetStaminaAttribute(), EGameplayModOp::Override, Attrs->GetMaxStamina());
}

void ASOLOCampfireActor::ReviveCompanions(ACharacter* Character)
{
	// Companion revival delegated to the companion system
}

void ASOLOCampfireActor::SaveCheckpoint(ACharacter* Character)
{
	if (auto* RespawnComp = Character->FindComponentByClass<USOLORespawnComponent>())
	{
		RespawnComp->SetLastCampfire(this);
	}
}

void ASOLOCampfireActor::ApplyCampfireBonus(ACharacter* Character)
{
	if (CampfireType != ESOLOCampfireType::Greater || !GreaterBuff) return;

	auto* ASI = Cast<IAbilitySystemInterface>(Character);
	if (!ASI) return;
	auto* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GreaterBuff, 1.f, Ctx);
	if (Spec.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}
