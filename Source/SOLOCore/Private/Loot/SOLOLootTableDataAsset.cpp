#include "Loot/SOLOLootTableDataAsset.h"

#include "Inventory/SOLOItemBase.h"
#include "Math/UnrealMathUtility.h"

// â”€â”€â”€ FPrimaryAssetId â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

FPrimaryAssetId USOLOLootTableDataAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(TEXT("LootTable"), GetFName());
}

// â”€â”€â”€ RollLoot â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

TArray<FLootResult> USOLOLootTableDataAsset::RollLoot(float LuckBonus) const
{
    TArray<FLootResult> Results;

    if (LootEntries.IsEmpty())
        return Results;

    // Separamos garantizados de probabilÃ­sticos para procesarlos en orden.
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

    // â”€â”€ 1. Procesar drops garantizados â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    for (const FLootEntry* Entry : GuaranteedEntries)
    {
        if (Results.Num() >= MaxDropsPerKill)
            break;

        // Los soft pointers deben estar cargados en memoria para usarlos aquÃ­.
        // En producciÃ³n, el LootComponent llama a esto tras un StreamableManager.RequestSyncLoad.
        USOLOItemBase* ResolvedItem = Entry->Item.Get();
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

    // â”€â”€ 2. Procesar drops probabilÃ­sticos â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    for (const FLootEntry* Entry : ProbabilisticEntries)
    {
        if (Results.Num() >= MaxDropsPerKill)
            break;

        USOLOItemBase* ResolvedItem = Entry->Item.Get();
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

// â”€â”€â”€ RollCurrency â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

int32 USOLOLootTableDataAsset::RollCurrency() const
{
    if (MaxCurrencyDrop <= 0)
        return 0;

    return FMath::RandRange(
        FMath::Max(0, MinCurrencyDrop),
        FMath::Max(0, MaxCurrencyDrop)
    );
}
