#include "Components/N2/USOLOAshRegenComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USOLOAshRegenComponent::USOLOAshRegenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLOAshRegenComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(GroundCheckTimer,
		this, &USOLOAshRegenComponent::TickGroundCheck,
		GroundCheckInterval, true);
}

void USOLOAshRegenComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	GetWorld()->GetTimerManager().ClearTimer(GroundCheckTimer);
	GetWorld()->GetTimerManager().ClearTimer(CancelTimer);
}

void USOLOAshRegenComponent::SetOnAsh(bool bIsOnAsh)
{
	if (bOnAsh == bIsOnAsh) return;
	bOnAsh = bIsOnAsh;

	if (bOnAsh && !bIceSuppressed)
		ApplyRegen();
	else
		RemoveRegen();
}

void USOLOAshRegenComponent::OnIceDamageReceived()
{
	bIceSuppressed = true;
	RemoveRegen();

	GetWorld()->GetTimerManager().SetTimer(CancelTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			bIceSuppressed = false;
			if (bOnAsh) ApplyRegen();
		}),
		CancelDuration, false);
}

void USOLOAshRegenComponent::TickGroundCheck()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;

	FVector Start = Owner->GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	Params.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	bool bNowOnAsh = false;

	if (bHit && Hit.PhysMaterial.IsValid())
	{
		// Blueprint-side: assign UPhysicalMaterial with a SurfaceType tagged as ash,
		// or check landscape layer name. Here we check the surface type name.
		bNowOnAsh = (Hit.PhysMaterial->SurfaceType == SurfaceType1); // SurfaceType1 = Ash by project convention
	}

	SetOnAsh(bNowOnAsh);
}

UAbilitySystemComponent* USOLOAshRegenComponent::GetOwnerASC() const
{
	if (auto* IASI = Cast<IAbilitySystemInterface>(GetOwner()))
		return IASI->GetAbilitySystemComponent();
	return nullptr;
}

void USOLOAshRegenComponent::ApplyRegen()
{
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC || !RegenGE || RegenHandle.IsValid()) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(RegenGE, 1, Ctx);
	if (Spec.IsValid())
		RegenHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void USOLOAshRegenComponent::RemoveRegen()
{
	UAbilitySystemComponent* ASC = GetOwnerASC();
	if (!ASC || !RegenHandle.IsValid()) return;
	ASC->RemoveActiveGameplayEffect(RegenHandle);
	RegenHandle = FActiveGameplayEffectHandle();
}
