#include "Items/SOLOLootPickup.h"
#include "Components/SphereComponent.h"
#include "Inventory/SOLOInventoryComponent.h"

ASOLOLootPickup::ASOLOLootPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetSphereRadius(80.f);
	CollectionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollectionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ASOLOLootPickup::BeginPlay()
{
	Super::BeginPlay();
	CollectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASOLOLootPickup::OnSphereBeginOverlap);
	SetLifeSpan(LifeSpan);
}

void ASOLOLootPickup::InitPickup(const FInventorySlot& InSlot)
{
	ItemSlot = InSlot;
}

void ASOLOLootPickup::OnSphereBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor) return;
	if (USOLOInventoryComponent* Inv = OtherActor->FindComponentByClass<USOLOInventoryComponent>())
	{
		Inv->AddItem(ItemSlot.ItemID, ItemSlot.Quantity);
		Destroy();
	}
}
