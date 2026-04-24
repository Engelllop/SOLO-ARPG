#include "World/AEMBERVEILSavePoint.h"
#include "Internationalization/Text.h"
#include "GameFramework/EMBERVEILGameMode.h"
#include "Character/EMBERVEILPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "EMBERVEILSavePoint"

AEMBERVEILSavePoint::AEMBERVEILSavePoint()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollision"));
	InteractionCollision->InitSphereRadius(150.f);
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionCollision->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionCollision->SetGenerateOverlapEvents(true);
	SetRootComponent(InteractionCollision);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	IdleVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("IdleVFX"));
	IdleVFX->SetupAttachment(MeshComponent);
	IdleVFX->SetAutoActivate(false);
}

void AEMBERVEILSavePoint::BeginPlay()
{
	Super::BeginPlay();

	if (!IdleVFXSoft.ToSoftObjectPath().IsNull())
	{
		if (UNiagaraSystem* Sys = IdleVFXSoft.LoadSynchronous())
		{
			IdleVFX->SetAsset(Sys);
			IdleVFX->Activate(true);
		}
	}
}

void AEMBERVEILSavePoint::Interact_Implementation(AEMBERVEILPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	if (HasAuthority())
	{
		PerformSaveInteraction(Interactor);
	}
	else
	{
		ServerPerformSave(Interactor);
	}
}

void AEMBERVEILSavePoint::ServerPerformSave_Implementation(AEMBERVEILPlayerCharacter* Interactor)
{
	PerformSaveInteraction(Interactor);
}

void AEMBERVEILSavePoint::PerformSaveInteraction(AEMBERVEILPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	if (bSingleUse && bHasBeenUsed)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		if (AEMBERVEILGameMode* GM = World->GetAuthGameMode<AEMBERVEILGameMode>())
		{
			GM->OnPlayerActivatedSavePoint(Interactor);
		}
	}

	bHasBeenUsed = true;
	MulticastPlaySaveEffects();
}

void AEMBERVEILSavePoint::MulticastPlaySaveEffects_Implementation()
{
	if (IdleVFX && IdleVFX->GetAsset())
	{
		IdleVFX->Activate(true);
	}

	if (InteractSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, InteractSound, GetActorLocation());
	}
}

FText AEMBERVEILSavePoint::GetInteractPrompt_Implementation() const
{
	return LOCTEXT("SavePrompt", "Guardar partida");
}

bool AEMBERVEILSavePoint::CanInteract_Implementation(AEMBERVEILPlayerCharacter* Interactor) const
{
	if (bSingleUse && bHasBeenUsed)
	{
		return false;
	}
	return true;
}

#undef LOCTEXT_NAMESPACE
