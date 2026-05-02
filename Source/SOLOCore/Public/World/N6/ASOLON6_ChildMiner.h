#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "ASOLON6_ChildMiner.generated.h"

UCLASS()
class SOLOCORE_API AASOLON6_ChildMiner : public ACharacter
{
    GENERATED_BODY()

public:
    AASOLON6_ChildMiner();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Miner")
    FText DialogueLine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N6|Miner")
    float DialogueRadius = 300.0f;

    UPROPERTY()
    bool bHasSpoken = false;

    UFUNCTION(BlueprintCallable, Category = "N6|Miner")
    void TrySpeakToPlayer(AActor* PlayerActor);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Miner")
    void OnSpokenTo();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N6|Miner")
    void OnPlayerNearby();
};
