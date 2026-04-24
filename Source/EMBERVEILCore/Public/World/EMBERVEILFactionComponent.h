#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EMBERVEILFactionComponent.generated.h"

UENUM(BlueprintType)
enum class EEMBERVEILAttitude : uint8
{
	Friendly UMETA(DisplayName = "Amistoso"),
	Neutral  UMETA(DisplayName = "Neutral"),
	Hostile  UMETA(DisplayName = "Hostil")
};

USTRUCT(BlueprintType)
struct FEMBERVEILFactionReputationEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	FName FactionID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	int32 Reputation = 0;
};

/**
 * Componente que gestiona la afiliación a una facción base y las reputaciones con otras facciones.
 * Puede acoplarse al PlayerState (para el jugador) o a NPCs (para definir su facción).
 */
UCLASS( ClassGroup=(World), meta=(BlueprintSpawnableComponent) )
class EMBERVEILCORE_API UEMBERVEILFactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEMBERVEILFactionComponent();

	/** Facción principal a la que pertenece este actor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction")
	FName BaseFactionID = NAME_None;

	/** Reputación actual con diferentes facciones (Solo relevante para jugadores o NPCs dinámicos). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction", ReplicatedUsing = OnRep_FactionReputation)
	TArray<FEMBERVEILFactionReputationEntry> FactionReputation;

	UFUNCTION(BlueprintCallable, Category = "Faction")
	void AddReputation(FName FactionID, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Faction")
	int32 GetReputation(FName FactionID) const;

	/** Calcula la actitud hacia otro actor que también tenga un FactionComponent. */
	UFUNCTION(BlueprintCallable, Category = "Faction")
	EEMBERVEILAttitude GetAttitudeTowards(AActor* OtherActor) const;

	/** Calcula la actitud hacia una facción específica basándose en el puntaje. */
	UFUNCTION(BlueprintCallable, Category = "Faction")
	EEMBERVEILAttitude GetAttitudeTowardsFaction(FName OtherFactionID) const;

	// Umbrales para determinar actitud basados en puntos de reputación
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction|Thresholds")
	int32 HostileThreshold = -100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction|Thresholds")
	int32 FriendlyThreshold = 100;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnReputationChangedSignature, FName, FactionID, int32, NewValue, int32, Delta);
	
	UPROPERTY(BlueprintAssignable, Category = "Faction|Events")
	FOnReputationChangedSignature OnReputationChanged;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_FactionReputation();
};
