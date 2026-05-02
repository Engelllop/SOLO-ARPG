#pragma once

#include "CoreMinimal.h"
#include "AI/SOLOCompanionCharacter.h"
#include "BP_Garrik_Aelthar.generated.h"

class ASOLON3_LeviathanBoss;

UCLASS()
class SOLOCORE_API ABP_Garrik_Aelthar : public ASOLOCompanionCharacter
{
    GENERATED_BODY()

public:
    ABP_Garrik_Aelthar();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // --- Diálogos ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik")
    bool bHasMetPlayer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik")
    bool bHasInvestigatedTemple = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik")
    bool bLeviathanDefeated = false;

    // --- Eventos de diálogo ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik")
    void OnMeetingPlayer();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik")
    void OnTempleInvestigation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik")
    void OnLeviathanDefeated();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik")
    void OnCainReflection();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik")
    void OnChildWhisper();

    // --- Combate ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik|Combate")
    float BaseDamage = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik|Combate")
    float CriticalChance = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik|Combate")
    float CriticalMultiplier = 2.5f;

    UFUNCTION(BlueprintCallable, Category = "N3|Garrik|Combate")
    void TwinBladeAttack(AActor* Target);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "N3|Garrik|Combate")
    void OnAttackLanded(AActor* HitTarget);

    // --- Videncia (ve patrones del Tejido) ---
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N3|Garrik|Tejido")
    void RevealTejidoPattern();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik|Tejido")
    float TejidoRevealRadius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N3|Garrik|Tejido")
    float TejidoRevealDuration = 5.0f;

    // --- Post-N3 flags ---
    UFUNCTION(BlueprintCallable, Category = "N3|Garrik|Flags")
    void SetStoryFlag(const FString& FlagName);

    UFUNCTION(BlueprintCallable, Category = "N3|Garrik|Flags")
    bool HasStoryFlag(const FString& FlagName) const;

    UPROPERTY()
    TSet<FString> StoryFlags;
};
