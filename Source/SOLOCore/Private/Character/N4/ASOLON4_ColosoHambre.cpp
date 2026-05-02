#include "Character/N4/ASOLON4_ColosoHambre.h"
#include "Character/N4/ASOLON4_CorazonHambre.h"
#include "World/N4/ASOLON4_HungerComponent.h"
#include "Kismet/GameplayStatics.h"

ASOLON4_ColosoHambre::ASOLON4_ColosoHambre()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON4_ColosoHambre::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(6.0f, 6.0f, 6.0f));
}

void ASOLON4_ColosoHambre::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON4_ColosoHambre::AdvanceLocalPhase()
{
    LocalPhase = 2;
    SpawnCorazon();
}

void ASOLON4_ColosoHambre::SuckHungerFromPlayer(AActor* PlayerActor)
{
    if (!PlayerActor) return;

    UASOLON4_HungerComponent* HungerComp = PlayerActor->FindComponentByClass<UASOLON4_HungerComponent>();
    if (HungerComp)
    {
        if (HungerComp->GetCurrentHunger() > 0)
        {
            HungerComp->ModifyHunger(-1);
            float HealAmount = SelfHealPerHunger;
            // Heal removed - implement in BP or use modify health
        }
    }
}

void ASOLON4_ColosoHambre::SpawnCorazon()
{
    if (!CorazonClass) return;

    FActorSpawnParameters Params;
    Params.Owner = this;
    Params.Instigator = GetInstigator();

    Corazon = GetWorld()->SpawnActor<ASOLON4_CorazonHambre>(CorazonClass, GetActorLocation(), FRotator::ZeroRotator, Params);
}

// OnDefeated handled in Blueprint
void ASOLON4_ColosoHambre::OnDefeated()
{
    if (Corazon)
    {
        Corazon->Destroy();
        Corazon = nullptr;
    }
    Destroy();
}
