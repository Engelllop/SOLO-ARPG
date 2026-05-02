#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON85_Subfloor.generated.h"

UCLASS()
class SOLOCORE_API ASOLON85_Subfloor : public AActor
{
    GENERATED_BODY()

public:
    ASOLON85_Subfloor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8.5")
    bool bHasBeenEntered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8.5")
    bool bHasReadNote = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "N8.5")
    FText DeskNoteText;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8.5")
    void OnPlayerEntersSubfloor();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "N8.5")
    void OnReadDeskNote();

    UFUNCTION(BlueprintCallable, Category = "N8.5")
    void ReadDeskNote();

protected:
    virtual void BeginPlay() override;
};
