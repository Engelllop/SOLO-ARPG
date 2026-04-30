#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOLootPickup.generated.h"

UCLASS()
class SOLOCORE_API ASOLOLootPickup : public AActor
{
	GENERATED_BODY()
public:
	ASOLOLootPickup();

	void InitPickup(const FInventorySlot& InSlot);

protected:
	virtual void BeginPlay() override;

	UFUNCTION() void OnSphereBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere) class USphereComponent* CollectionSphere;
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly) float LifeSpan = 60.f;

private:
	FInventorySlot ItemSlot;
	bool bCurrency = false;
};
