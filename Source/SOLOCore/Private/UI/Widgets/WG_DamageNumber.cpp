#include "UI/Widgets/WG_DamageNumber.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UWG_DamageNumber::Activate(float DamageAmount, EDamageNumberType Type, bool bIsCritical, FVector WorldLocation, float XScreenOffset, float InLifetime)
{
	ElapsedTime = 0.f;
	VerticalRise = 0.f;
	Lifetime = FMath::Max(0.1f, InLifetime);
	SetRenderOpacity(1.f);
	bIsActive = true;
	CurrentWorldLocation = WorldLocation;
	XOffset = XScreenOffset;

	const FLinearColor Color = GetColorForType(Type);
	const float Scale = GetScaleForCritical(bIsCritical);

	FString NumberStr = FString::Printf(TEXT("%.0f"), FMath::Abs(DamageAmount));

	// Healing gets a "+" prefix and critical hits get an "!" suffix
	if (Type == EDamageNumberType::Healing)
	{
		NumberStr = TEXT("+") + NumberStr;
	}
	else if (bIsCritical)
	{
		NumberStr += TEXT("!");
	}

	BP_SetDamageText(FText::FromString(NumberStr), Color, Scale);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	BP_PlayAppearAnimation();
}

void UWG_DamageNumber::Deactivate()
{
	bIsActive = false;
	SetVisibility(ESlateVisibility::Collapsed);
	ElapsedTime = 0.f;
	VerticalRise = 0.f;
}

void UWG_DamageNumber::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsActive)
	{
		return;
	}

	ElapsedTime += InDeltaTime;

	if (ElapsedTime >= Lifetime)
	{
		Deactivate();
		return;
	}

	// Rise in screen space rather than world space — avoids projection issues with rapid camera movement
	VerticalRise += RiseSpeed * InDeltaTime;

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	FVector2D ScreenPos;
	if (PC->ProjectWorldLocationToScreen(CurrentWorldLocation, ScreenPos, true))
	{
		ScreenPos.X += XOffset;
		ScreenPos.Y -= VerticalRise;

		SetPositionInViewport(ScreenPos, false);

		// Fade out over the final FadeStartRatio of lifetime
		const float FadeProgress = FMath::Max(0.f, (ElapsedTime / Lifetime - FadeStartRatio) / (1.f - FadeStartRatio));
		SetRenderOpacity(1.f - FadeProgress);
	}
}

FLinearColor UWG_DamageNumber::GetColorForType(EDamageNumberType Type)
{
	switch (Type)
	{
	case EDamageNumberType::Physical:   return FLinearColor::White;
	case EDamageNumberType::Fire:       return FLinearColor(1.f, 0.4f, 0.f, 1.f);
	case EDamageNumberType::Ice:        return FLinearColor(0.4f, 0.8f, 1.f, 1.f);
	case EDamageNumberType::Lightning:  return FLinearColor(1.f, 1.f, 0.2f, 1.f);
	case EDamageNumberType::Poison:     return FLinearColor(0.4f, 1.f, 0.4f, 1.f);
	case EDamageNumberType::Dark:       return FLinearColor(0.6f, 0.f, 0.8f, 1.f);
	case EDamageNumberType::Holy:       return FLinearColor(1.f, 0.9f, 0.5f, 1.f);
	case EDamageNumberType::Healing:    return FLinearColor(0.2f, 1.f, 0.4f, 1.f);
	default:                            return FLinearColor::White;
	}
}

float UWG_DamageNumber::GetScaleForCritical(bool bIsCritical)
{
	// Crits render 30% larger per spec
	return bIsCritical ? 1.3f : 1.f;
}
