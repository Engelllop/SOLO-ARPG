#include "Loot/EMBERVEILLootTableDataAsset.h"

#include "Inventory/EMBERVEILItemBase.h"
#include "Math/UnrealMathUtility.h"

// ─── FPrimaryAssetId ──────────────────────────────────────────────────────────

FPrimaryAssetId UEMBERVEILLootTableDataAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(TEXT("LootTable"), GetFName());
}

// ─── RollLoot ─────────────────────────────────────────────────────────────────

TArray<FLootResult> UEMBERVEILLootTableDataAsset::RollLoot(float LuckBonus) const
{
    TArray<FLootResult> Results;

    if (LootEntries.IsEmpty())
        return Results;

    // Separamos garantizados de probabilísticos para procesarlos en orden.
    TArray<const FLootEntry*> GuaranteedEntries;
    TArray<const FLootEntry*> ProbabilisticEntries;

    for (const FLootEntry& Entry : LootEntries)
    {
        if (Entry.Item.IsNull())
            continue;

        if (Entry.bIsGuaranteed)
            GuaranteedEntries.Add(&Entry);
        else
            ProbabilisticEntries.Add(&Entry);
    }

    // ── 1. Procesar drops garantizados ────────────────────────────────────────
    for (const FLootEntry* Entry : GuaranteedEntries)
    {
        if (Results.Num() >= MaxDropsPerKill)
            break;

        // Los soft pointers deben estar cargados en memoria para usarlos aquí.
        // En producción, el LootComponent llama a esto tras un StreamableManager.RequestSyncLoad.
        UEMBERVEILItemBase* ResolvedItem = Entry->Item.Get();
        if (!ResolvedItem)
            continue;

        const int32 Qty = FMath::RandRange(
            FMath::Max(1, Entry->MinQuantity),
            FMath::Max(1, Entry->MaxQuantity)
        );

        FLootResult& NewResult = Results.AddDefaulted_GetRef();
        NewResult.Item     = ResolvedItem;
        NewResult.Quantity = Qty;
    }

    // ── 2. Procesar drops probabilísticos ─────────────────────────────────────
    for (const FLootEntry* Entry : ProbabilisticEntries)
    {
        if (Results.Num() >= MaxDropsPerKill)
            break;

        UEMBERVEILItemBase* ResolvedItem = Entry->Item.Get();
        if (!ResolvedItem)
            continue;

        // Chance efectiva = DropChance * LuckMultiplier * (1 + LuckBonus), clampeada a [0,1]
        const float EffectiveChance = FMath::Clamp(
            Entry->DropChance * LuckMultiplier * (1.f + LuckBonus),
            0.f,
            1.f
        );

        if (FMath::FRand() > EffectiveChance)
            continue;

        const int32 Qty = FMath::RandRange(
            FMath::Max(1, Entry->MinQuantity),
            FMath::Max(1, Entry->MaxQuantity)
        );

        FLootResult& NewResult = Results.AddDefaulted_GetRef();
        NewResult.Item     = ResolvedItem;
        NewResult.Quantity = Qty;
    }

    return Results;
}

// ─── RollCurrency ─────────────────────────────────────────────────────────────

int32 UEMBERVEILLootTableDataAsset::RollCurrency() const
{
    if (MaxCurrencyDrop <= 0)
        return 0;

    return FMath::RandRange(
        FMath::Max(0, MinCurrencyDrop),
        FMath::Max(0, MaxCurrencyDrop)
    );
}
