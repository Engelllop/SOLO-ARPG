#pragma once
#include "CoreMinimal.h"

// Compile-time string constants for N1 (Velmar) identifiers.
namespace N1
{
	// ─── NPC IDs ────────────────────────────────────────────────────────────────
	static const FName NPC_Anselmo  (TEXT("NPC.N1.Anselmo"));
	static const FName NPC_Lira     (TEXT("NPC.N1.Lira"));        // missing girl (non-interactive)
	static const FName NPC_Taberna  (TEXT("NPC.N1.TavernaKeep")); // El Abismo bartender

	// ─── Quest IDs ──────────────────────────────────────────────────────────────
	static const FName QUEST_NinaVelmar   (TEXT("Quest.N1.NinaVelmar"));
	static const FName QUEST_LuzDelSantuario (TEXT("Quest.N1.LuzSantuario"));

	// ─── Item IDs ───────────────────────────────────────────────────────────────
	static const FName ITEM_CartaLira         (TEXT("Item.N1.CartaLira"));
	static const FName ITEM_ArmaduraVelmar    (TEXT("Item.N1.ArmaduraLigera"));
	static const FName ITEM_AmuletoDeLuz      (TEXT("Item.N1.AmuletoDeLuz"));
	static const FName ITEM_LlaveN2           (TEXT("Item.N1.LlaveN2"));

	// ─── Campfire IDs ───────────────────────────────────────────────────────────
	static const FName CAMPFIRE_Mercado   (TEXT("Campfire.N1.Mercado"));
	static const FName CAMPFIRE_Santuario (TEXT("Campfire.N1.Santuario"));

	// ─── Enemy tags ─────────────────────────────────────────────────────────────
	namespace EnemyTag
	{
		static constexpr TCHAR AlmaErrante[] = TEXT("Enemy.N1.AlmaErrante");
		static constexpr TCHAR LaVoz[]       = TEXT("Enemy.N1.LaVoz");
	}

	// ─── Story flags ────────────────────────────────────────────────────────────
	static const FName FLAG_LaVozKilled     (TEXT("Flag.N1.LaVozKilled"));
	static const FName FLAG_LiraLetterFound (TEXT("Flag.N1.LiraLetterFound"));
	static const FName FLAG_N2Unlocked      (TEXT("Flag.N1.N2Unlocked"));

	// ─── Boss tag ───────────────────────────────────────────────────────────────
	static const FName BOSS_LaVoz (TEXT("Boss.N1.LaVoz"));
}
