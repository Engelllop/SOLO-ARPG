#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SOLOTimeManagerComponent.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class ESOLOTimeOfDay : uint8
{
	Day UMETA(DisplayName = "DÃ­a"),
	Night UMETA(DisplayName = "Noche")
};

UENUM(BlueprintType)
enum class ESOLOWeather : uint8
{
	Clear UMETA(DisplayName = "Despejado"),
	Rain UMETA(DisplayName = "Lluvia"),
	Storm UMETA(DisplayName = "Tormenta")
};

/**
 * Componente que maneja el paso del tiempo y el clima del mundo.
 * Debe estar en el GameState. Inyecta Global Gameplay Tags al entorno.
 */
UCLASS( ClassGroup=(World), meta=(BlueprintSpawnableComponent) )
class SOLOCORE_API USOLOTimeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USOLOTimeManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Multiplicador para la velocidad del paso del tiempo. Ej: 24.0 = 1 hora de juego pasa en 2.5 minutos reales. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeScale = 24.0f;

	/** Hora actual del dÃ­a [0.0 - 24.0) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", ReplicatedUsing = OnRep_CurrentHour)
	float CurrentHour = 8.0f; // Comienza a las 8 AM

	UFUNCTION(BlueprintCallable, Category = "Time")
	ESOLOTimeOfDay GetTimeOfDay() const;

	UFUNCTION(BlueprintCallable, Category = "Weather")
	ESOLOWeather GetCurrentWeather() const { return CurrentWeather; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Weather")
	void SetWeather(ESOLOWeather NewWeather);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeather)
	ESOLOWeather CurrentWeather = ESOLOWeather::Clear;

	UFUNCTION()
	void OnRep_CurrentHour();

	UFUNCTION()
	void OnRep_CurrentWeather();

	ESOLOTimeOfDay CachedTimeOfDay = ESOLOTimeOfDay::Day;

	void UpdateGlobalGameplayTags();
	void DispatchTagToAllPlayers(FGameplayTag TagToAdd, FGameplayTag TagToRemove);
};
