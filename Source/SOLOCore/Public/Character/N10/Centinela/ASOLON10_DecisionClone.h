#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON10_DecisionClone.generated.h"

UCLASS()
class SOLOCORE_API ASOLON10_DecisionClone : public AActor
{
    GENERATED_BODY()

public:
    ASOLON10_DecisionClone();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DecisionClone")
    FText DecisionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DecisionClone")
    bool bIsAbsorbed = false;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DecisionClone")
    void OnAbsorb();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DecisionClone")
    void OnDestroyedByForce();

public:
    UFUNCTION(BlueprintCallable, Category = "DecisionClone")
    void Absorb();

    UFUNCTION(BlueprintCallable, Category = "DecisionClone")
    void SetDecisionTag(const FString& Tag);
};
