#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_PartyFrames.generated.h"

class APlayerState;

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FPartyMemberData
{
	GENERATED_BODY()

	// Truncated to MaxNameDisplayLength in AddPartyMember
	UPROPERTY(BlueprintReadWrite, Category = "PartyMember")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = "PartyMember")
	float HealthPercent = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = "PartyMember")
	bool bIsDead = false;

	// Distance threshold: >50m triggers reduced-opacity treatment
	UPROPERTY(BlueprintReadWrite, Category = "PartyMember")
	bool bIsOutOfRange = false;

	UPROPERTY(BlueprintReadWrite, Category = "PartyMember")
	TObjectPtr<APlayerState> MemberPlayerState = nullptr;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class EMBERVEILCORE_API UWG_PartyFrames : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void AddPartyMember(APlayerState* NewMember);

	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void RemovePartyMember(APlayerState* Member);

	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void UpdateMemberHealth(int32 MemberIndex, float NewHealthPercent);

	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void SetMemberDead(int32 MemberIndex, bool bDead);

	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void SetMemberOutOfRange(int32 MemberIndex, bool bOutOfRange);

	// Hides the entire frame list — called when party size drops to 1
	UFUNCTION(BlueprintCallable, Category = "PartyFrames")
	void SetPartyVisible(bool bVisible);

	UPROPERTY(BlueprintReadOnly, Category = "PartyFrames|State")
	TArray<FPartyMemberData> PartyMembers;

	static constexpr int32 MaxPartySize = 10;
	static constexpr int32 MaxNameDisplayLength = 10;
	// Out-of-range distance in cm (50m)
	static constexpr float OutOfRangeDistanceSq = 5000.f * 5000.f;

protected:
	// Blueprint applies grayscale material switch + skull icon
	UFUNCTION(BlueprintImplementableEvent, Category = "PartyFrames|Events")
	void BP_SetMemberDead(int32 MemberIndex, bool bDead);

	// Blueprint sets RenderOpacity to 0.4 for out-of-range members
	UFUNCTION(BlueprintImplementableEvent, Category = "PartyFrames|Events")
	void BP_SetMemberOutOfRange(int32 MemberIndex, bool bOutOfRange);

	// Blueprint rebuilds the list widget with current PartyMembers data
	UFUNCTION(BlueprintImplementableEvent, Category = "PartyFrames|Events")
	void BP_RefreshPartyList();

private:
	// Light-weight HP polling — runs every 0.5s instead of every tick
	float PollingAccumulator = 0.f;
	static constexpr float PollingInterval = 0.5f;

	void PollMemberHealth();
};
