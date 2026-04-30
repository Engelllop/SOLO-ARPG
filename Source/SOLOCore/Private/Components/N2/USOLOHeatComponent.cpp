#include "Components/N2/USOLOHeatComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

USOLOHeatComponent::USOLOHeatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLOHeatComponent::BeginPlay()
{
	Super::BeginPlay();
	RestartAmbientTimer();
}

void USOLOHeatComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	GetWorld()->GetTimerManager().ClearTimer(AmbientTimer);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
}

// ─── Zone / Shelter interface ────────────────────────────────────────────────

void USOLOHeatComponent::EnterHeatZone(float AccelerationMultiplier)
{
	HeatZoneCount++;
	ZoneAcceleration = FMath::Max(ZoneAcceleration, AccelerationMultiplier);
	if (!bInShelter) RestartAmbientTimer();
}

void USOLOHeatComponent::ExitHeatZone()
{
	HeatZoneCount = FMath::Max(0, HeatZoneCount - 1);
	if (HeatZoneCount == 0) ZoneAcceleration = 1.f;
	if (!bInShelter) RestartAmbientTimer();
}

void USOLOHeatComponent::EnterShelter()
{
	bInShelter = true;
	GetWorld()->GetTimerManager().ClearTimer(AmbientTimer);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer,
		this, &USOLOHeatComponent::TickShelterCooldown,
		ShelterCooldownInterval, true);
}

void USOLOHeatComponent::ExitShelter()
{
	bInShelter = false;
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
	RestartAmbientTimer();
}

// ─── Direct segment manipulation ─────────────────────────────────────────────

void USOLOHeatComponent::AddHeatSegment()
{
	if (CurrentSegments >= 5) return;
	ApplySegmentGE(CurrentSegments);
	CurrentSegments++;
	OnHeatSegmentChanged.Broadcast(CurrentSegments);
}

void USOLOHeatComponent::RemoveHeatSegment()
{
	if (CurrentSegments <= 0) return;
	CurrentSegments--;
	RemoveSegmentGE(CurrentSegments);
	OnHeatSegmentChanged.Broadcast(CurrentSegments);
}

void USOLOHeatComponent::ClearAllHeat()
{
	while (CurrentSegments > 0)
		RemoveHeatSegment();
}

// ─── Timer callbacks ──────────────────────────────────────────────────────────

void USOLOHeatComponent::TickAmbientHeat()
{
	AddHeatSegment();
	if (CurrentSegments >= 5)
		GetWorld()->GetTimerManager().ClearTimer(AmbientTimer);
}

void USOLOHeatComponent::TickShelterCooldown()
{
	RemoveHeatSegment();
	if (CurrentSegments <= 0)
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
}

void USOLOHeatComponent::RestartAmbientTimer()
{
	if (CurrentSegments >= 5) return;
	float Interval = AmbientSegmentInterval / FMath::Max(ZoneAcceleration, 1.f);
	GetWorld()->GetTimerManager().SetTimer(AmbientTimer,
		this, &USOLOHeatComponent::TickAmbientHeat,
		Interval, true);
}

// ─── GE helpers ───────────────────────────────────────────────────────────────

UAbilitySystemComponent* USOLOHeatComponent::GetOwnerASC() const
{
	if (auto* IASI = Cast<IAbilitySystemInterface>(GetOwner()))
		return IASI->GetAbilitySystemComponent();
	return nullptr;
}

void USOLOHeatComponent::ApplySegmentGE(int32 SegIndex)
{
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC || !SegmentGEs.IsValidIndex(SegIndex) || !SegmentGEs[SegIndex]) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(SegmentGEs[SegIndex], 1, Ctx);
	if (Spec.IsValid())
	{
		FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		if (ActiveSegmentHandles.Num() <= SegIndex)
			ActiveSegmentHandles.SetNum(SegIndex + 1);
		ActiveSegmentHandles[SegIndex] = Handle;
	}
}

void USOLOHeatComponent::RemoveSegmentGE(int32 SegIndex)
{
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC || !ActiveSegmentHandles.IsValidIndex(SegIndex)) return;
	if (ActiveSegmentHandles[SegIndex].IsValid())
		ASC->RemoveActiveGameplayEffect(ActiveSegmentHandles[SegIndex]);
}
