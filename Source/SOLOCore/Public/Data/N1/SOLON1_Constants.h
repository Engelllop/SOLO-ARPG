#pragma once
#include "CoreMinimal.h"

namespace N1
{
	// ─── NPC IDs ────────────────────────────────────────────────────────────────
	static const FName NPC_Tova        (TEXT("NPC.N1.Tova"));         // Posada del Viento Hundido
	static const FName NPC_Natan       (TEXT("NPC.N1.Natan"));        // Herrero / Forja
	static const FName NPC_Shae        (TEXT("NPC.N1.Shae"));         // Herborista, Mercado
	static const FName NPC_Lian        (TEXT("NPC.N1.Lian"));         // Taberna El Columpio (condicional)
	static const FName NPC_Yuri        (TEXT("NPC.N1.Yuri"));         // Niño, Día 2
	static const FName NPC_GuardiaVelmar(TEXT("NPC.N1.Guardia"));     // Guardia del Borde

	// ─── Companion IDs ─────────────────────────────────────────────────────────
	static const FName COMPANION_Vassen (TEXT("Companion.N1.Vassen"));
	static const FName COMPANION_Mira   (TEXT("Companion.N1.Mira"));

	// ─── Quest IDs ──────────────────────────────────────────────────────────────
	static const FName QUEST_Descender         (TEXT("Quest.N1.Descender"));          // Principal
	static const FName QUEST_NinaDesaparecida  (TEXT("Quest.N1.NinaDesaparecida"));   // Opcional

	// ─── Item IDs ───────────────────────────────────────────────────────────────
	static const FName ITEM_FragmentoVoz       (TEXT("Item.N1.FragmentoVoz"));
	static const FName ITEM_ColganteCristal    (TEXT("Item.N1.ColganteCristal"));
	static const FName ITEM_VendasShae         (TEXT("Item.N1.VendasShae"));
	static const FName ITEM_EstofadoTova       (TEXT("Item.N1.EstofadoTova"));
	static const FName ITEM_BufandaNina        (TEXT("Item.N1.BufandaNina"));         // Quest evidence

	// ─── Campfire / Save IDs ────────────────────────────────────────────────────
	static const FName CAMPFIRE_Posada   (TEXT("Campfire.N1.Posada"));
	static const FName CAMPFIRE_Borde    (TEXT("Campfire.N1.Borde"));

	// ─── Enemy tags ─────────────────────────────────────────────────────────────
	namespace EnemyTag
	{
		static constexpr TCHAR EchoShadow[]      = TEXT("Enemy.N1.EchoShadow");
		static constexpr TCHAR WaterArm[]         = TEXT("Enemy.N1.WaterArm");
		static constexpr TCHAR VozManifestacion[] = TEXT("Enemy.N1.VozManifestacion");
		static constexpr TCHAR LaVoz[]            = TEXT("Enemy.N1.LaVoz");
	}

	// ─── Story flags (used with USOLOGameInstance::SetStoryFlag / HasStoryFlag) ─
	static const FName FLAG_MetVoice           (TEXT("Flag.N1.MetVoice"));
	static const FName FLAG_CainStonePlaced    (TEXT("Flag.N1.CainStonePlaced"));
	static const FName FLAG_NinaFound          (TEXT("Flag.N1.NinaFound"));
	static const FName FLAG_NinaDead           (TEXT("Flag.N1.NinaDead"));
	static const FName FLAG_ShaeExpanded       (TEXT("Flag.N1.ShaeExpandedDialogue"));
	static const FName FLAG_LianMet            (TEXT("Flag.N1.LianMet"));
	static const FName FLAG_NatanPurchased     (TEXT("Flag.N1.NatanPurchased"));
	static const FName FLAG_LaVozKilled        (TEXT("Flag.N1.LaVozKilled"));
	static const FName FLAG_N2Unlocked         (TEXT("Flag.N1.N2Unlocked"));

	// ─── Velmar Day IDs ─────────────────────────────────────────────────────────
	static const FName DAY_Arrival   (TEXT("Day.N1.Arrival"));    // Day 1
	static const FName DAY_Search    (TEXT("Day.N1.Search"));     // Day 2
	static const FName DAY_Reckoning (TEXT("Day.N1.Reckoning"));  // Day 3

	// ─── Boss tag ───────────────────────────────────────────────────────────────
	static const FName BOSS_LaVoz (TEXT("Boss.N1.LaVoz"));

	// ─── Crystal Pillar counts ──────────────────────────────────────────────────
	static constexpr int32 TOTAL_PILLARS       = 6;
	static constexpr int32 PILLARS_FOR_PHASE2  = 4;
	static constexpr int32 PILLAR_HIT_POINTS   = 3;
}
