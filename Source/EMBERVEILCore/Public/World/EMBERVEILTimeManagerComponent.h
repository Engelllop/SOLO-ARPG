#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILTimeManagerComponent.generated.h"

class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEMBERVEILTimeOfDay : uint8
{
	Day UMETA(DisplayName = "Día"),
	Night UMETA(DisplayName = "Noche")
};

UENUM(BlueprintType)
enum class EEMBERVEILWeather : uint8
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
class EMBERVEILCORE_API UEMBERVEILTimeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEMBERVEILTimeManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Multiplicador para la velocidad del paso del tiempo. Ej: 24.0 = 1 hora de juego pasa en 2.5 minutos reales. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeScale = 24.0f;

	/** Hora actual del día [0.0 - 24.0) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", ReplicatedUsing = OnRep_CurrentHour)
	float CurrentHour = 8.0f; // Comienza a las 8 AM

	UFUNCTION(BlueprintCallable, Category = "Time")
	EEMBERVEILTimeOfDay GetTimeOfDay() const;

	UFUNCTION(BlueprintCallable, Category = "Weather")
	EEMBERVEILWeather GetCurrentWeather() const { return CurrentWeather; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Weather")
	void SetWeather(EEMBERVEILWeather NewWeather);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeather)
	EEMBERVEILWeather CurrentWeather = EEMBERVEILWeather::Clear;

	UFUNCTION()
	void OnRep_CurrentHour();

	UFUNCTION()
	void OnRep_CurrentWeather();

	EEMBERVEILTimeOfDay CachedTimeOfDay = EEMBERVEILTimeOfDay::Day;

	void UpdateGlobalGameplayTags();
	void DispatchTagToAllPlayers(FGameplayTag TagToAdd, FGameplayTag TagToRemove);
};
