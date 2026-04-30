#pragma once
#include "CoreMinimal.h"

// Compile-time string constants for all N0 (Emberveil) identifiers.
// Use these instead of raw literals to avoid typos across systems.
namespace N0
{
	// ─── NPC IDs ────────────────────────────────────────────────────────────────
	static const FName NPC_Aldric   (TEXT("NPC.N0.Aldric"));
	static const FName NPC_Greta    (TEXT("NPC.N0.Greta"));
	static const FName NPC_Doran    (TEXT("NPC.N0.Doran"));
	static const FName NPC_Kael     (TEXT("NPC.N0.Kael"));
	static const FName NPC_Guard1   (TEXT("NPC.N0.Guard1"));
	static const FName NPC_Guard2   (TEXT("NPC.N0.Guard2"));

	// ─── Quest IDs ──────────────────────────────────────────────────────────────
	// Main
	static const FName QUEST_Jabali          (TEXT("Quest.N0.Jabali"));
	static const FName QUEST_AtaqueDirigido  (TEXT("Quest.N0.AtaqueErrante"));  // post-event
	static const FName QUEST_CaminoTorre     (TEXT("Quest.N0.CaminoTorre"));
	// Tutorial optional
	static const FName QUEST_SopaGreta       (TEXT("Quest.N0.SopaGreta"));
	// Rank E side quests
	static const FName QUEST_Lobos           (TEXT("Quest.N0.Lobos"));
	static const FName QUEST_FloresThalia    (TEXT("Quest.N0.FloresThalia"));
	static const FName QUEST_ColmilloPerdido (TEXT("Quest.N0.ColmilloPerdido"));
	static const FName QUEST_LenaPostada     (TEXT("Quest.N0.LenaPostada"));
	static const FName QUEST_Mensaje         (TEXT("Quest.N0.Mensaje"));
	static const FName QUEST_LimpiezaCamino  (TEXT("Quest.N0.LimpiezaCamino"));
	static const FName QUEST_FantasmaTemplo  (TEXT("Quest.N0.FantasmaTemplo"));

	// ─── Item IDs ───────────────────────────────────────────────────────────────
	static const FName ITEM_ColmilloJabali       (TEXT("Item.N0.ColmilloJabali"));
	static const FName ITEM_Sopa                 (TEXT("Item.N0.Sopa"));
	static const FName ITEM_CartaDoran           (TEXT("Item.N0.CartaDoran"));
	static const FName ITEM_FloreNocturna        (TEXT("Item.N0.FloreNocturna"));
	static const FName ITEM_Madera               (TEXT("Item.N0.Madera"));
	static const FName ITEM_AnilloPrincipiante   (TEXT("Item.N0.AnilloPrincipiante"));

	// ─── Campfire / SavePoint IDs ───────────────────────────────────────────────
	static const FName CAMPFIRE_Plaza    (TEXT("Campfire.N0.Plaza"));
	static const FName CAMPFIRE_Bosque   (TEXT("Campfire.N0.Bosque"));
	static const FName CAMPFIRE_Posada   (TEXT("Campfire.N0.Posada"));
	static const FName CAMPFIRE_Garganta (TEXT("Campfire.N0.Garganta"));
	static const FName CAMPFIRE_Entrada  (TEXT("Campfire.N0.Entrada"));
	// Special — triggers Tejido vision of young Caín
	static const FName CAMPFIRE_Errante  (TEXT("Campfire.N0.Errante"));

	// ─── Enemy tags (used in FQuestObjective.TargetTag) ─────────────────────────
	namespace EnemyTag
	{
		static constexpr TCHAR Jabali[]    = TEXT("Enemy.N0.Jabali");
		static constexpr TCHAR Lobo[]      = TEXT("Enemy.N0.Lobo");
		static constexpr TCHAR Serpiente[] = TEXT("Enemy.N0.Serpiente");
		static constexpr TCHAR Gargola[]   = TEXT("Enemy.N0.Gargola");
		static constexpr TCHAR Esqueleto[] = TEXT("Enemy.N0.Esqueleto");
		static constexpr TCHAR Errante[]   = TEXT("Enemy.N0.Errante");
	}

	// ─── Story flags ────────────────────────────────────────────────────────────
	static const FName FLAG_ErranteKilled   (TEXT("Flag.N0.ErranteKilled"));
	static const FName FLAG_GuardianKilled  (TEXT("Flag.N0.GuardianKilled"));
	static const FName FLAG_CainDied        (TEXT("Flag.N0.CainDied"));
	static const FName FLAG_RankD           (TEXT("Flag.N0.RankD"));
	static const FName FLAG_DayAfterEvent   (TEXT("Flag.N0.DayAfterEvent"));

	// ─── Weave ability tags granted after Caín's death ─────────────────────────
	namespace Weave
	{
		static constexpr TCHAR SimultaneousCuts[] = TEXT("Ability.Weave.SimultaneousCuts");
		static constexpr TCHAR WeaveArrow[]       = TEXT("Ability.Weave.WeaveArrow");
	}

	// ─── Guardian boss tag used in phase data ──────────────────────────────────
	static const FName BOSS_Guardian (TEXT("Boss.N0.Guardian"));
}
