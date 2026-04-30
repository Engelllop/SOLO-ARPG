#include "Components/N2/USOLOHeatPhaseComponent.h"
#include "Components/N2/USOLOHeatComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

USOLOHeatPhaseComponent::USOLOHeatPhaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLOHeatPhaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// Find the local player pawn and subscribe to their heat changes
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			if (auto* Heat = PlayerPawn->FindComponentByClass<USOLOHeatComponent>())
			{
				TrackedHeatComp = Heat;
				Heat->OnHeatSegmentChanged.AddDynamic(this, &USOLOHeatPhaseComponent::OnPlayerHeatChanged);
				// Evaluate immediately for current state
				OnPlayerHeatChanged(Heat->GetCurrentSegments());
			}
		}
	}
}

void USOLOHeatPhaseComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	if (TrackedHeatComp.IsValid())
		TrackedHeatComp->OnHeatSegmentChanged.RemoveDynamic(this, &USOLOHeatPhaseComponent::OnPlayerHeatChanged);
}

void USOLOHeatPhaseComponent::OnPlayerHeatChanged(int32 NewSegments)
{
	if (NewSegments >= IntangibilityHeatThreshold)
		ApplyIntangible();
	else
		RemoveIntangible();
}

void USOLOHeatPhaseComponent::ApplyIntangible()
{
	if (IntangibleHandle.IsValid()) return;
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC || !IntangibleGE) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(IntangibleGE, 1, Ctx);
	if (Spec.IsValid())
	{
		IntangibleHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		BP_OnBecomeIntangible();
	}
}

void USOLOHeatPhaseComponent::RemoveIntangible()
{
	if (!IntangibleHandle.IsValid()) return;
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (ASC) ASC->RemoveActiveGameplayEffect(IntangibleHandle);
	IntangibleHandle = FActiveGameplayEffectHandle();
	BP_OnBecomeTangible();
}

UAbilitySystemComponent* USOLOHeatPhaseComponent::GetOwnerASC() const
{
	if (auto* IASI = Cast<IAbilitySystemInterface>(GetOwner()))
		return IASI->GetAbilitySystemComponent();
	return nullptr;
}
