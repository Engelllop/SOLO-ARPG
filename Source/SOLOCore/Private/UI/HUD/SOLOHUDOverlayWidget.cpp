#include "UI/HUD/SOLOHUDOverlayWidget.h"
#include "UI/Widgets/WG_VitalBars.h"
#include "UI/Widgets/WG_ExperienceBar.h"
#include "UI/Widgets/WG_Minimap.h"
#include "UI/Widgets/WG_QuestLogWidget.h"
#include "UI/Widgets/WG_BattleLogWidget.h"
#include "UI/Widgets/WG_StatusEffectsWidget.h"
#include "UI/Widgets/WG_Hotbar.h"
#include "UI/Widgets/WG_BossHealthBar.h"
#include "UI/Widgets/WG_ComboCounter.h"
#include "UI/Widgets/WG_PartyFrames.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"

void USOLOHUDOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LayoutSwitcher)
	{
		LayoutSwitcher->SetActiveWidgetIndex(bUseV4Layout ? 0 : 1);
	}

	// Configurar layout V4
	if (bUseV4Layout)
	{
		// Posicionamiento absoluto se maneja desde el Blueprint (WBP_HUD_Overlay)
		// con las anclas adecuadas en el CanvasPanel V4Canvas
	}

	// Inicializar estado por defecto
	if (BossHealthBar)
	{
		BossHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PartyFrames)
	{
		PartyFrames->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ComboCounter)
	{
		ComboCounter->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USOLOHUDOverlayWidget::ShowBossHealthBar(ASOLOEnemyCharacter* Boss)
{
	if (BossHealthBar)
	{
		BossHealthBar->SetVisibility(ESlateVisibility::Visible);

		// El BossHealthBar tiene su propio mÃ©todo BindToBoss en el .h
		// Se llamarÃ¡ desde Blueprint o desde acÃ¡ si agregamos el mÃ©todo
	}
}

void USOLOHUDOverlayWidget::HideBossHealthBar()
{
	if (BossHealthBar)
	{
		BossHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}
