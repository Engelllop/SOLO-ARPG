#include "World/N2/ASOLON2_HeatShelter.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Components/N2/USOLOHeatComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ASOLON2_HeatShelter::ASOLON2_HeatShelter()
{
	PrimaryActorTick.bCanEverTick = false;

	ShelterVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ShelterVolume"));
	ShelterVolume->SetBoxExtent(FVector(300.f, 300.f, 150.f));
	ShelterVolume->SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(ShelterVolume);

	SteamParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SteamParticles"));
	SteamParticles->SetupAttachment(ShelterVolume);

	ShelterVolume->OnComponentBeginOverlap.AddDynamic(this, &ASOLON2_HeatShelter::OnBeginOverlap);
	ShelterVolume->OnComponentEndOverlap.AddDynamic(this, &ASOLON2_HeatShelter::OnEndOverlap);
}

void ASOLON2_HeatShelter::OnBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (auto* Heat = OtherActor->FindComponentByClass<USOLOHeatComponent>())
	{
		Heat->EnterShelter();
		if (Cast<APawn>(OtherActor) && Cast<APawn>(OtherActor)->IsLocallyControlled())
			BP_OnPlayerEnter();
	}
}

void ASOLON2_HeatShelter::OnEndOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32)
{
	if (auto* Heat = OtherActor->FindComponentByClass<USOLOHeatComponent>())
	{
		Heat->ExitShelter();
		if (Cast<APawn>(OtherActor) && Cast<APawn>(OtherActor)->IsLocallyControlled())
			BP_OnPlayerExit();
	}
}
