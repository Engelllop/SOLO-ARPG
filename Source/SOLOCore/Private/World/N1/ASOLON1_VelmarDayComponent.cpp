#include "World/N1/ASOLON1_VelmarDayComponent.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Kismet/GameplayStatics.h"

USOLON1_VelmarDayComponent::USOLON1_VelmarDayComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOLON1_VelmarDayComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyDayEvents(CurrentDay);
}

void USOLON1_VelmarDayComponent::AdvanceDay()
{
	int32 Next = static_cast<int32>(CurrentDay) + 1;
	if (Next > static_cast<int32>(EVelmarDay::Day3_Reckoning)) return;

	CurrentDay = static_cast<EVelmarDay>(Next);
	OnDayChanged.Broadcast(CurrentDay);
	ApplyDayEvents(CurrentDay);
}

void USOLON1_VelmarDayComponent::ApplyDayEvents(EVelmarDay Day)
{
	switch (Day)
	{
	case EVelmarDay::Day1_Arrival:   BP_OnDay1(); break;
	case EVelmarDay::Day2_Search:    BP_OnDay2(); break;
	case EVelmarDay::Day3_Reckoning: BP_OnDay3(); break;
	}
}
