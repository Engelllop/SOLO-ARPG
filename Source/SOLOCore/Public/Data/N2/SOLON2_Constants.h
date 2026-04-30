#pragma once
#include "CoreMinimal.h"

namespace N2
{
	// ─── NPCs ────────────────────────────────────────────────────────────────────
	static const FName NPC_Lian (TEXT("NPC.N2.Lian"));

	// ─── Quests ──────────────────────────────────────────────────────────────────
	static const FName QUEST_DerrotarPastor  (TEXT("Quest.N2.DerrotarPastor"));
	static const FName QUEST_LianSuministros (TEXT("Quest.N2.LianSuministros"));
	static const FName QUEST_HombreAtrapado  (TEXT("Quest.N2.HombreAtrapado"));

	// ─── Items ───────────────────────────────────────────────────────────────────
	static const FName ITEM_TunicaCenizaInerte    (TEXT("Item.N2.TunicaCenizaInerte"));
	static const FName ITEM_NucleoFuegoBlanco     (TEXT("Item.N2.NucleoFuegoBlanco"));
	static const FName ITEM_FragmentoBaculo       (TEXT("Item.N2.FragmentoBaculo"));
	static const FName ITEM_CantaroAguaFria       (TEXT("Item.N2.CantaroAguaFria"));
	static const FName ITEM_ColmilloBrasa         (TEXT("Item.N2.ColmilloBrasa"));
	static const FName ITEM_FragmentoPiedraHumana (TEXT("Item.N2.FragmentoPiedraHumana"));
	static const FName ITEM_LlaveTunel            (TEXT("Item.N2.LlaveTunel"));

	// ─── Campfires ───────────────────────────────────────────────────────────────
	static const FName CAMPFIRE_Lian          (TEXT("Campfire.N2.Lian"));
	static const FName CAMPFIRE_RefugioCentral(TEXT("Campfire.N2.RefugioCentral"));
	static const FName CAMPFIRE_Nido          (TEXT("Campfire.N2.Nido"));

	// ─── Enemy tags ──────────────────────────────────────────────────────────────
	namespace EnemyTag
	{
		static constexpr TCHAR PerroCeniza[] = TEXT("Enemy.N2.PerroCeniza");
		static constexpr TCHAR EspectroCalor[]= TEXT("Enemy.N2.EspectroCalor");
		static constexpr TCHAR GusanoBreasa[] = TEXT("Enemy.N2.GusanoBrasa");
		static constexpr TCHAR PastorCenizas[]= TEXT("Enemy.N2.PastorCenizas");
	}

	// ─── Story flags ─────────────────────────────────────────────────────────────
	static const FName FLAG_PastorKilled         (TEXT("Flag.N2.PastorKilled"));
	static const FName FLAG_HombreRescatado      (TEXT("Flag.N2.HombreAtrapado.Rescued"));
	static const FName FLAG_HombreMuerto         (TEXT("Flag.N2.HombreAtrapado.Killed"));

	// ─── Heat system tags ────────────────────────────────────────────────────────
	// Applied to the player by USOLOHeatComponent at each segment threshold.
	namespace HeatTag
	{
		static constexpr TCHAR Seg1[] = TEXT("Status.Heat.Segment1");
		static constexpr TCHAR Seg2[] = TEXT("Status.Heat.Segment2");
		static constexpr TCHAR Seg3[] = TEXT("Status.Heat.Segment3");
		static constexpr TCHAR Seg4[] = TEXT("Status.Heat.Segment4");
		static constexpr TCHAR Seg5[] = TEXT("Status.Heat.Segment5");
	}

	static const FName BOSS_Pastor (TEXT("Boss.N2.PastorCenizas"));
}
