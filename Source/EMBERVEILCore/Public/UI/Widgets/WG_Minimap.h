#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_Minimap.generated.h"

UENUM(BlueprintType)
enum class EMinimapDotType : uint8
{
	Player,
	PartyMember,
	EnemyAggro,
	EnemyPatrol,
	QuestMarker,
	Landmark
};

USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FMinimapDot
{
	GENERATED_BODY()

	// Normalized [-1,1] space relative to player — Blueprint converts to widget coords
	UPROPERTY(BlueprintReadWrite, Category = "MinimapDot")
	FVector2D ScreenPosition = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "MinimapDot")
	EMinimapDotType Type = EMinimapDotType::EnemyPatrol;

	// Weak ref — actor may be destroyed between ticks
	UPROPERTY(BlueprintReadWrite, Category = "MinimapDot")
	TObjectPtr<AActor> Owner = nullptr;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisableNativeTick = "false"))
class EMBERVEILCORE_API UWG_Minimap : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void RegisterQuestMarker(FVector WorldLocation);

	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void UnregisterQuestMarker(FVector WorldLocation);

	// Fades visibility in/out; default 0.3s matches design spec
	UFUNCTION(BlueprintCallable, Category = "Minimap")
	void ToggleMinimapVisibility(float FadeDuration = 0.3f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|Config")
	float MapRange = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|Config")
	float MinimapOpacityBase = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap|Config")
	float MinimapOpacityHover = 0.9f;

	UPROPERTY(BlueprintReadOnly, Category = "Minimap|State")
	TArray<FMinimapDot> ActiveDots;

protected:
	// Blueprint renders colored dots over the circular canvas each tick
	UFUNCTION(BlueprintImplementableEvent, Category = "Minimap|Events")
	void BP_DrawDots(const TArray<FMinimapDot>& Dots);

	// Blueprint rotates the player direction arrow
	UFUNCTION(BlueprintImplementableEvent, Category = "Minimap|Events")
	void BP_SetPlayerRotation(float YawDegrees);

	// Blueprint drives the UMG fade animation
	UFUNCTION(BlueprintImplementableEvent, Category = "Minimap|Events")
	void BP_PlayFadeAnimation(bool bFadeIn, float Duration);

private:
	bool bIsVisible = true;

	// World-space positions for quest markers (static, not tied to actors)
	TArray<FVector> QuestMarkerLocations;

	// Builds ActiveDots from the world — called every tick
	void GatherWorldDots(const FVector& PlayerLocation, float PlayerYaw);

	// Returns the normalized [-1,1] offset for a world position relative to the player
	FVector2D WorldToMinimapNormalized(const FVector& WorldPos, const FVector& PlayerPos, float PlayerYaw) const;
};
