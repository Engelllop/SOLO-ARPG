#include "UI/Widgets/WG_Minimap.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Math/UnrealMathUtility.h"

void UWG_Minimap::NativeConstruct()
{
	Super::NativeConstruct();

	SetRenderOpacity(MinimapOpacityBase);
}

void UWG_Minimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsVisible)
	{
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn)
	{
		return;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const float PlayerYaw = PlayerPawn->GetActorRotation().Yaw;

	GatherWorldDots(PlayerLocation, PlayerYaw);
	BP_DrawDots(ActiveDots);
	BP_SetPlayerRotation(PlayerYaw);
}

void UWG_Minimap::RegisterQuestMarker(FVector WorldLocation)
{
	QuestMarkerLocations.AddUnique(WorldLocation);
}

void UWG_Minimap::UnregisterQuestMarker(FVector WorldLocation)
{
	QuestMarkerLocations.Remove(WorldLocation);
}

void UWG_Minimap::ToggleMinimapVisibility(float FadeDuration)
{
	bIsVisible = !bIsVisible;
	BP_PlayFadeAnimation(bIsVisible, FadeDuration);

	// Hide from interaction entirely when not visible to avoid hover opacity changes
	SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UWG_Minimap::GatherWorldDots(const FVector& PlayerLocation, float PlayerYaw)
{
	ActiveDots.Reset();

	// Player dot is always at the center
	FMinimapDot PlayerDot;
	PlayerDot.ScreenPosition = FVector2D::ZeroVector;
	PlayerDot.Type = EMinimapDotType::Player;
	ActiveDots.Add(PlayerDot);

	// Enemies
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<AEMBERVEILEnemyCharacter> It(World); It; ++It)
	{
		AEMBERVEILEnemyCharacter* Enemy = *It;
		if (!IsValid(Enemy))
		{
			continue;
		}

		const FVector2D Normalized = WorldToMinimapNormalized(Enemy->GetActorLocation(), PlayerLocation, PlayerYaw);

		// Discard actors outside the circular radar area (unit circle in normalized space)
		if (Normalized.SizeSquared() > 1.f)
		{
			continue;
		}

		FMinimapDot Dot;
		Dot.ScreenPosition = Normalized;
		Dot.Owner = Enemy;
		// Distinguish patrolling enemies from those that have noticed the player
		Dot.Type = Enemy->IsAggro() ? EMinimapDotType::EnemyAggro : EMinimapDotType::EnemyPatrol;
		ActiveDots.Add(Dot);
	}

	// Static quest markers
	for (const FVector& MarkerLoc : QuestMarkerLocations)
	{
		const FVector2D Normalized = WorldToMinimapNormalized(MarkerLoc, PlayerLocation, PlayerYaw);
		if (Normalized.SizeSquared() > 1.f)
		{
			continue;
		}

		FMinimapDot Dot;
		Dot.ScreenPosition = Normalized;
		Dot.Type = EMinimapDotType::QuestMarker;
		ActiveDots.Add(Dot);
	}
}

FVector2D UWG_Minimap::WorldToMinimapNormalized(const FVector& WorldPos, const FVector& PlayerPos, float PlayerYaw) const
{
	const FVector Delta = WorldPos - PlayerPos;

	// Rotate the world-space delta by negative player yaw so north is always up
	const float YawRad = FMath::DegreesToRadians(-PlayerYaw);
	const float Cos = FMath::Cos(YawRad);
	const float Sin = FMath::Sin(YawRad);

	const float LocalX = Delta.X * Cos - Delta.Y * Sin;
	const float LocalY = Delta.X * Sin + Delta.Y * Cos;

	// Divide by MapRange to get [-1,1]; actors beyond MapRange are outside the circle
	return FVector2D(LocalY / MapRange, -LocalX / MapRange);
}
