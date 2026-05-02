#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASOLON1_VoiceEventActor.generated.h"

class UBoxComponent;
class USOLOGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVoiceLine, int32, LineIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVoiceEventFinished);

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_VoiceEventActor : public AActor
{
	GENERATED_BODY()
public:
	ASOLON1_VoiceEventActor();

	UPROPERTY(BlueprintAssignable) FOnVoiceLine OnVoiceLine;
	UPROPERTY(BlueprintAssignable) FOnVoiceEventFinished OnEventFinished;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "N1|Voice")
	TObjectPtr<UBoxComponent> TriggerVolume;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voice")
	TArray<FText> VoiceLines;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voice")
	TArray<float> LineDelays;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Voice")
	FName StoryFlagOnComplete;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnLineStarted(int32 LineIndex, const FText& Line);
	UFUNCTION(BlueprintImplementableEvent) void BP_OnEventFinished();

private:
	bool bTriggered = false;
	int32 CurrentLineIndex = 0;
	FTimerHandle LineTimer;

	UFUNCTION() void OnTriggerOverlap(UPrimitiveComponent* Comp, AActor* Other,
		UPrimitiveComponent* OtherComp, int32 Idx, bool bSweep, const FHitResult& Hit);

	void PlayNextLine();
};
