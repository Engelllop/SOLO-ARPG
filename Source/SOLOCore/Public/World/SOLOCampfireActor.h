#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/SOLOEnums.h"
#include "Interaction/ISOLOInteractable.h"
#include "SOLOCampfireActor.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class SOLOCORE_API ASOLOCampfireActor : public AActor, public ISOLOInteractable
{
	GENERATED_BODY()

public:
	ASOLOCampfireActor();

	UFUNCTION(BlueprintCallable, Category = "World|Campfire")
	void Rest(ACharacter* Character);

	UFUNCTION(BlueprintPure, Category = "World|Campfire")
	bool IsActive() const { return bIsActive; }

	UFUNCTION(BlueprintCallable, Category = "World|Campfire")
	void Activate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campfire")
	ESOLOCampfireType CampfireType = ESOLOCampfireType::Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Campfire")
	TSubclassOf<UGameplayEffect> GreaterBuff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> InteractionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> FireMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> FireVFX;

private:
	bool bIsActive = false;

	void RestoreAllResources(ACharacter* Character);
	void ReviveCompanions(ACharacter* Character);
	void SaveCheckpoint(ACharacter* Character);
	void ApplyCampfireBonus(ACharacter* Character);
};
