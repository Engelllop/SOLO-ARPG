#include "Character/N10/Centinela/ASOLON10_DecisionClone.h"

ASOLON10_DecisionClone::ASOLON10_DecisionClone()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON10_DecisionClone::BeginPlay()
{
    Super::BeginPlay();
    bIsAbsorbed = false;
}

void ASOLON10_DecisionClone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON10_DecisionClone::Absorb()
{
    if (bIsAbsorbed) return;
    bIsAbsorbed = true;
    OnAbsorb();
    SetLifeSpan(0.5f); // Destroy after absorption animation
}

void ASOLON10_DecisionClone::SetDecisionTag(const FString& Tag)
{
    DecisionName = FText::FromString(Tag);
}
