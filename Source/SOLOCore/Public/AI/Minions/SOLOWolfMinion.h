#pragma once
#include "CoreMinimal.h"
#include "AI/Minions/SOLOMinionBase.h"
#include "SOLOWolfMinion.generated.h"

UCLASS()
class SOLOCORE_API ASOLOWolfMinion : public ASOLOMinionBase
{
	GENERATED_BODY()
public:
	ASOLOWolfMinion();
	UPROPERTY(EditDefaultsOnly, Category = "Wolf") TSubclassOf<UGameplayEffect> BleedEffect;
};
