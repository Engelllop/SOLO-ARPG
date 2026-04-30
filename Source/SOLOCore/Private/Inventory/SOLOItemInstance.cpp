#include "Inventory/SOLOItemInstance.h"
#include "Inventory/SOLOItemBase.h"

USOLOItemInstance::USOLOItemInstance()
{
    CurrentDurability = 100;
    MaxDurability = 100;
    bIsEquipped = false;
    EquippedInSlot = ESOLOEquipmentSlot::None;
}

void USOLOItemInstance::Initialize(USOLOItemBase* InItemData)
{
    if (!ensureMsgf(InItemData != nullptr,
        TEXT("USOLOItemInstance::Initialize â€” InItemData es nulo. "
             "AsegÃºrate de pasar un DataAsset vÃ¡lido.")))
    {
        return;
    }

    ItemData = InItemData;

    // La durabilidad mÃ¡xima por defecto es 100 para todos los items.
    // Los items de tipo QuestItem o Currency no se desgastan, pero mantenemos
    // la misma estructura por uniformidad.
    MaxDurability = 100;
    CurrentDurability = MaxDurability;

    bIsEquipped = false;
    EquippedInSlot = ESOLOEquipmentSlot::None;
    DynamicStats.Empty();
}

float USOLOItemInstance::GetDurabilityPercent() const
{
    if (MaxDurability <= 0)
    {
        return 0.f;
    }

    return static_cast<float>(CurrentDurability) / static_cast<float>(MaxDurability);
}

bool USOLOItemInstance::IsBroken() const
{
    return CurrentDurability <= 0;
}

void USOLOItemInstance::ReduceDurability(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    CurrentDurability = FMath::Max(0, CurrentDurability - Amount);
}

void USOLOItemInstance::RepairDurability(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    CurrentDurability = FMath::Min(MaxDurability, CurrentDurability + Amount);
}

void USOLOItemInstance::SetDynamicStat(FName StatName, float Value)
{
    if (StatName.IsNone())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("USOLOItemInstance::SetDynamicStat â€” StatName es None. Ignorado."));
        return;
    }

    DynamicStats.Add(StatName, Value);
}

float USOLOItemInstance::GetDynamicStat(FName StatName) const
{
    const float* FoundValue = DynamicStats.Find(StatName);
    return FoundValue != nullptr ? *FoundValue : 0.f;
}
