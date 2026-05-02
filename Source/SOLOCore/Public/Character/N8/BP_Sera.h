#pragma once

#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"

// Not a real enemy — just a reference for the companion
// The real Sera is in BP with Blueprint logic
#include "BP_Sera.generated.h"

UCLASS()
class SOLOCORE_API ABP_Sera : public ASOLOEnemyCharacter
{
    GENERATED_BODY()

public:
    ABP_Sera();

protected:
    virtual void BeginPlay() override;

    // --- State tracking for N7+N8 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera")
    bool bHasJoined = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera")
    int32 DialogueProgress = 0;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Sera")
    void OnFirstMeeting();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Sera")
    void OnSecondMeeting();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Sera")
    void OnThirdMeeting();

    // --- Tejido abilities ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Sera|Tejido")
    void RevealTejidoTraces();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera|Tejido")
    float TejidoRevealRadius = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera|Tejido")
    float TejidoDuration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera|Tejido")
    float TejidoCooldown = 20.0f;

    // --- Combat ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera|Combate")
    float BaseDamage = 12.0f;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8|Sera|Combate")
    void KatanaAttack(AActor* Target);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8|Sera|Combate")
    float RespawnTime = 10.0f;
};
