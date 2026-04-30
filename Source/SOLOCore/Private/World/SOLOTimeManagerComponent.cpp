#include "World/SOLOTimeManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/World.h"

USOLOTimeManagerComponent::USOLOTimeManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void USOLOTimeManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USOLOTimeManagerComponent, CurrentHour);
	DOREPLIFETIME(USOLOTimeManagerComponent, CurrentWeather);
}

void USOLOTimeManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		CachedTimeOfDay = GetTimeOfDay();
		UpdateGlobalGameplayTags();
	}
}

void USOLOTimeManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		// Advance time
		float HoursPassed = (DeltaTime * TimeScale) / 3600.0f; // TimeScale is real seconds to game hours?
		// Si TimeScale = 24.0, entonces 1 hora real = 24 horas del juego.
		// DeltaTime estÃ¡ en segundos. DeltaTime * TimeScale es el avance en "segundos de juego".
		HoursPassed = (DeltaTime * TimeScale) / 3600.f; // Ajustar escala para que TimeScale sea multiplicador directo
		// Mejor: Si TimeScale = 60, 1 min real = 1 hora juego. (DeltaTime / 60) * 1 hr
		HoursPassed = DeltaTime * (TimeScale / 3600.0f);
		
		CurrentHour += HoursPassed;
		if (CurrentHour >= 24.0f)
		{
			CurrentHour -= 24.0f;
		}

		// Check for day/night transition
		ESOLOTimeOfDay NewTimeOfDay = GetTimeOfDay();
		if (NewTimeOfDay != CachedTimeOfDay)
		{
			CachedTimeOfDay = NewTimeOfDay;
			UpdateGlobalGameplayTags();
		}
	}
}

ESOLOTimeOfDay USOLOTimeManagerComponent::GetTimeOfDay() const
{
	// DÃ­a de 6 AM a 6 PM (18:00)
	if (CurrentHour >= 6.0f && CurrentHour < 18.0f)
	{
		return ESOLOTimeOfDay::Day;
	}
	return ESOLOTimeOfDay::Night;
}

void USOLOTimeManagerComponent::SetWeather(ESOLOWeather NewWeather)
{
	if (GetOwner() && GetOwner()->HasAuthority() && CurrentWeather != NewWeather)
	{
		CurrentWeather = NewWeather;
		UpdateGlobalGameplayTags();
	}
}

void USOLOTimeManagerComponent::OnRep_CurrentHour()
{
	// Opcional: disparar evento a Blueprint para actualizar UI del reloj
}

void USOLOTimeManagerComponent::OnRep_CurrentWeather()
{
	// Opcional: disparar evento a Blueprint para cambiar efectos visuales (lluvia, sol)
}

void USOLOTimeManagerComponent::UpdateGlobalGameplayTags()
{
	// Solo el servidor despacha Tags
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	FGameplayTag TagDay = FGameplayTag::RequestGameplayTag(FName("Time.Day"));
	FGameplayTag TagNight = FGameplayTag::RequestGameplayTag(FName("Time.Night"));

	if (CachedTimeOfDay == ESOLOTimeOfDay::Day)
	{
		DispatchTagToAllPlayers(TagDay, TagNight);
	}
	else
	{
		DispatchTagToAllPlayers(TagNight, TagDay);
	}

	// Manejo similar para Weather: Weather.Clear, Weather.Rain, Weather.Storm
	// TODO: Expandir lÃ³gica para Clima
}

void USOLOTimeManagerComponent::DispatchTagToAllPlayers(FGameplayTag TagToAdd, FGameplayTag TagToRemove)
{
	UWorld* World = GetWorld();
	if (!World) return;

	AGameStateBase* GS = World->GetGameState();
	if (!GS) return;

	for (APlayerState* PS : GS->PlayerArray)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PS))
		{
			if (TagToRemove.IsValid())
			{
				ASC->RemoveLooseGameplayTag(TagToRemove);
			}
			if (TagToAdd.IsValid())
			{
				ASC->AddLooseGameplayTag(TagToAdd);
			}
		}
	}
}
