#include "World/SOLOAttackIndicator.h"
#include "Components/DecalComponent.h"

ASOLOAttackIndicator::ASOLOAttackIndicator()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	IndicatorDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	IndicatorDecal->SetupAttachment(RootComponent);
	IndicatorDecal->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
}

void ASOLOAttackIndicator::ShowIndicator(FVector Location, float Radius, FLinearColor Color, float Duration)
{
	SetActorLocation(Location);
	SetActorHiddenInGame(false);

	if (IndicatorDecal)
	{
		IndicatorDecal->DecalSize = FVector(Radius, Radius, Radius);
		if (UMaterialInstanceDynamic* DynMat = IndicatorDecal->CreateDynamicMaterialInstance())
			DynMat->SetVectorParameterValue(TEXT("Color"), Color);
	}

	if (Duration > 0.f)
	{
		GetWorldTimerManager().SetTimer(HideTimer, this, &ASOLOAttackIndicator::HideIndicator, Duration, false);
	}
}

void ASOLOAttackIndicator::HideIndicator()
{
	SetActorHiddenInGame(true);
}
