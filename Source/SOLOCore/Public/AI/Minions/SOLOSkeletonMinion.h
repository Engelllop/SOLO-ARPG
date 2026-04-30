#pragma once

#include "CoreMinimal.h"
#include "AI/Minions/SOLOMinionBase.h"
#include "SOLOSkeletonMinion.generated.h"

UCLASS()
class SOLOCORE_API ASOLOSkeletonMinion : public ASOLOMinionBase
{
	GENERATED_BODY()
public:
	ASOLOSkeletonMinion();

	UPROPERTY(EditDefaultsOnly, Category = "Skeleton")
	bool bIsElite = false;
};
