#include "World/N1/ASOLON1_WaterArm.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

ASOLON1_WaterArm::ASOLON1_WaterArm()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GrabCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GrabCollision"));
	RootComponent = GrabCollision;
	GrabCollision->SetSphereRadius(GrabRadius);
	GrabCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GrabCollision->OnComponentBeginOverlap.AddDynamic(this, &ASOLON1_WaterArm::OnGrabOverlap);
}

void ASOLON1_WaterArm::BeginPlay()
{
	Super::BeginPlay();
	GrabCollision->SetSphereRadius(GrabRadius);
}

void ASOLON1_WaterArm::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bBroken || !GrabbedTarget.IsValid()) return;

	if (DotGE)
	{
		if (auto* IASI = Cast<IAbilitySystemInterface>(GrabbedTarget.Get()))
		{
			if (auto* TargetASC = IASI->GetAbilitySystemComponent())
			{
				// DoT is handled by the GE duration; tick is backup for non-GE approach
			}
		}
	}
}

float ASOLON1_WaterArm::TakeDamage(float Damage, const FDamageEvent& Event,
	AController* InstigatorActor, AActor* Causer)
{
	if (bBroken) return 0.f;
	BreakArm();
	return 1.f;
}

void ASOLON1_WaterArm::OnGrabOverlap(UPrimitiveComponent* Comp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 Idx, bool bSweep, const FHitResult& Hit)
{
	if (bBroken || GrabbedTarget.IsValid()) return;
	if (!Other || Other == this) return;

	auto* IASI = Cast<IAbilitySystemInterface>(Other);
	if (!IASI) return;

	GrabbedTarget = Other;
	ApplyGrabEffects(Other);
	SetActorTickEnabled(true);
	BP_OnGrab(Other);
}

void ASOLON1_WaterArm::ApplyGrabEffects(AActor* Target)
{
	auto* IASI = Cast<IAbilitySystemInterface>(Target);
	if (!IASI) return;
	UAbilitySystemComponent* TargetASC = IASI->GetAbilitySystemComponent();
	if (!TargetASC) return;

	if (SlowGE)
	{
		FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(SlowGE, 1, Ctx);
		if (Spec.IsValid())
			SlowHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	if (DotGE)
	{
		FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
		FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(DotGE, 1, Ctx);
		if (Spec.IsValid())
			TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void ASOLON1_WaterArm::RemoveGrabEffects()
{
	if (!GrabbedTarget.IsValid()) return;
	auto* IASI = Cast<IAbilitySystemInterface>(GrabbedTarget.Get());
	if (!IASI) return;
	UAbilitySystemComponent* TargetASC = IASI->GetAbilitySystemComponent();
	if (TargetASC && SlowHandle.IsValid())
		TargetASC->RemoveActiveGameplayEffect(SlowHandle);
}

void ASOLON1_WaterArm::BreakArm()
{
	if (bBroken) return;
	bBroken = true;
	RemoveGrabEffects();
	BP_OnBreak();
	OnBroken.Broadcast(this);
	SetLifeSpan(1.f);
}

