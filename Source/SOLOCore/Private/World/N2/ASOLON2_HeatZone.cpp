#include "World/N2/ASOLON2_HeatZone.h"
#include "Components/BoxComponent.h"
#include "Components/N2/USOLOHeatComponent.h"

ASOLON2_HeatZone::ASOLON2_HeatZone()
{
	PrimaryActorTick.bCanEverTick = false;

	ZoneVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneVolume"));
	ZoneVolume->SetBoxExtent(FVector(500.f, 500.f, 200.f));
	ZoneVolume->SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(ZoneVolume);

	ZoneVolume->OnComponentBeginOverlap.AddDynamic(this, &ASOLON2_HeatZone::OnBeginOverlap);
	ZoneVolume->OnComponentEndOverlap.AddDynamic(this, &ASOLON2_HeatZone::OnEndOverlap);
}

void ASOLON2_HeatZone::OnBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (auto* Heat = OtherActor->FindComponentByClass<USOLOHeatComponent>())
		Heat->EnterHeatZone(AccelerationMultiplier);
}

void ASOLON2_HeatZone::OnEndOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32)
{
	if (auto* Heat = OtherActor->FindComponentByClass<USOLOHeatComponent>())
		Heat->ExitHeatZone();
}
