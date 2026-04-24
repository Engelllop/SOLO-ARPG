#include "Inventory/EMBERVEILItemInstance.h"
#include "Inventory/EMBERVEILItemBase.h"

UEMBERVEILItemInstance::UEMBERVEILItemInstance()
{
    CurrentDurability = 100;
    MaxDurability = 100;
    bIsEquipped = false;
    EquippedInSlot = EEMBERVEILEquipmentSlot::None;
}

void UEMBERVEILItemInstance::Initialize(UEMBERVEILItemBase* InItemData)
{
    if (!ensureMsgf(InItemData != nullptr,
        TEXT("UEMBERVEILItemInstance::Initialize — InItemData es nulo. "
             "Asegúrate de pasar un DataAsset válido.")))
    {
        return;
    }

    ItemData = InItemData;

    // La durabilidad máxima por defecto es 100 para todos los items.
    // Los items de tipo QuestItem o Currency no se desgastan, pero mantenemos
    // la misma estructura por uniformidad.
    MaxDurability = 100;
    CurrentDurability = MaxDurability;

    bIsEquipped = false;
    EquippedInSlot = EEMBERVEILEquipmentSlot::None;
    DynamicStats.Empty();
}

float UEMBERVEILItemInstance::GetDurabilityPercent() const
{
    if (MaxDurability <= 0)
    {
        return 0.f;
    }

    return static_cast<float>(CurrentDurability) / static_cast<float>(MaxDurability);
}

bool UEMBERVEILItemInstance::IsBroken() const
{
    return CurrentDurability <= 0;
}

void UEMBERVEILItemInstance::ReduceDurability(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    CurrentDurability = FMath::Max(0, CurrentDurability - Amount);
}

void UEMBERVEILItemInstance::RepairDurability(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    CurrentDurability = FMath::Min(MaxDurability, CurrentDurability + Amount);
}

void UEMBERVEILItemInstance::SetDynamicStat(FName StatName, float Value)
{
    if (StatName.IsNone())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILItemInstance::SetDynamicStat — StatName es None. Ignorado."));
        return;
    }

    DynamicStats.Add(StatName, Value);
}

float UEMBERVEILItemInstance::GetDynamicStat(FName StatName) const
{
    const float* FoundValue = DynamicStats.Find(StatName);
    return FoundValue != nullptr ? *FoundValue : 0.f;
}
