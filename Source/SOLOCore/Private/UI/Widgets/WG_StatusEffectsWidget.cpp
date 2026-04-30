#include "UI/Widgets/WG_StatusEffectsWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Texture2D.h"

void UWG_StatusEffectsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWG_StatusEffectsWidget::SetEffects(const TArray<FStatusEffectData>& Effects)
{
	ActiveEffects = Effects;

	if (ActiveEffects.Num() > MaxVisibleIcons)
	{
		ActiveEffects.SetNum(MaxVisibleIcons);
	}

	RebuildIcons();
}

void UWG_StatusEffectsWidget::AddOrUpdateEffect(const FStatusEffectData& Effect)
{
	// Buscar si ya existe por nombre
	for (int32 i = 0; i < ActiveEffects.Num(); ++i)
	{
		if (ActiveEffects[i].EffectName.EqualTo(Effect.EffectName))
		{
			ActiveEffects[i] = Effect;
			RebuildIcons();
			return;
		}
	}

	// No existe, añadir
	if (ActiveEffects.Num() < MaxVisibleIcons)
	{
		ActiveEffects.Add(Effect);
		RebuildIcons();
	}
}

void UWG_StatusEffectsWidget::RemoveEffect(const FString& EffectName)
{
	for (int32 i = 0; i < ActiveEffects.Num(); ++i)
	{
		if (ActiveEffects[i].EffectName.ToString() == EffectName)
		{
			ActiveEffects.RemoveAt(i);
			RebuildIcons();
			return;
		}
	}
}

void UWG_StatusEffectsWidget::ClearEffects()
{
	ActiveEffects.Empty();
	if (EffectsContainer)
	{
		EffectsContainer->ClearChildren();
	}
}

void UWG_StatusEffectsWidget::RebuildIcons()
{
	if (!EffectsContainer) return;

	EffectsContainer->ClearChildren();

	for (const FStatusEffectData& Effect : ActiveEffects)
	{
		UImage* IconImage = NewObject<UImage>(this);

		// Usar textura si está disponible, si no crear un placeholder de color
		if (Effect.Icon)
		{
			IconImage->SetBrushFromTexture(Effect.Icon);
		}
		else
		{
			// Placeholder: rectángulo de color
			IconImage->SetColorAndOpacity(Effect.TintColor);
		}

		// Tamaño fijo
		IconImage->SetDesiredSizeOverride(FVector2D(IconSize, IconSize));

		// Tooltip con nombre y duración
		FString TooltipText = Effect.EffectName.ToString();
		if (Effect.MaxDuration > 0.f)
		{
			float Remaining = Effect.RemainingDuration > 0.f ? Effect.RemainingDuration : Effect.MaxDuration;
			TooltipText += FString::Printf(TEXT(" (%.1fs)"), Remaining);
		}
		IconImage->SetToolTipText(FText::FromString(TooltipText));

		// Set tint and opacity
		IconImage->SetOpacity(1.f);

		// Borde de color que indica buff vs debuff
		IconImage->SetBrushTintColor(GetBorderColorForEffect(Effect));

		// Margen entre iconos
		EffectsContainer->AddChildToHorizontalBox(IconImage);
	}
}

FLinearColor UWG_StatusEffectsWidget::GetBorderColorForEffect(const FStatusEffectData& Effect) const
{
	if (Effect.bIsBuff)
	{
		// Buff: borde azulado o verde
		return FLinearColor(0.42f, 0.66f, 0.49f, 0.7f); // #6aa87c
	}
	else
	{
		// Debuff: borde rojizo
		return FLinearColor(0.91f, 0.49f, 0.42f, 0.7f); // #e87c6a
	}
}
