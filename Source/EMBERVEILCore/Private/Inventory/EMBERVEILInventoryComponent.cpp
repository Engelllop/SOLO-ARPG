#include "Inventory/EMBERVEILInventoryComponent.h"
#include "Inventory/EMBERVEILItemBase.h"
#include "Inventory/EMBERVEILItemInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerState.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────

UEMBERVEILInventoryComponent::UEMBERVEILInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);

    MaxInventorySlots = 40;
    Currency = 0;
}

// ─── REPLICACIÓN ──────────────────────────────────────────────────────────────

void UEMBERVEILInventoryComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEMBERVEILInventoryComponent, InventorySlots);
    DOREPLIFETIME(UEMBERVEILInventoryComponent, EquippedEntries);
    DOREPLIFETIME(UEMBERVEILInventoryComponent, Currency);
}

// ─── LIFECYCLE ────────────────────────────────────────────────────────────────

void UEMBERVEILInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // Preinicializar el array de slots con entradas vacías para que la UI
    // pueda iterar siempre sobre MaxInventorySlots posiciones fijas.
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        InventorySlots.SetNum(MaxInventorySlots);
    }
}

// ─── OnRep ────────────────────────────────────────────────────────────────────

void UEMBERVEILInventoryComponent::OnRep_InventorySlots()
{
    OnInventoryChanged.Broadcast(InventorySlots);
}

void UEMBERVEILInventoryComponent::OnRep_EquipmentSlots()
{
    for (const FReplicatedEquipmentEntry& Entry : EquippedEntries)
    {
        OnItemEquipped.Broadcast(Entry.Slot, Entry.ItemInstance.Get());
    }
}

void UEMBERVEILInventoryComponent::OnRep_Currency()
{
    OnCurrencyChanged.Broadcast(Currency);
}

// ─── GESTIÓN DEL INVENTARIO ───────────────────────────────────────────────────

bool UEMBERVEILInventoryComponent::AddItem(UEMBERVEILItemBase* ItemData, int32 Quantity)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::AddItem — Solo puede llamarse en el servidor."));
        return false;
    }

    if (!IsValid(ItemData))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::AddItem — ItemData es nulo o inválido."));
        return false;
    }

    if (Quantity <= 0)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::AddItem — Quantity debe ser >= 1."));
        return false;
    }

    int32 RemainingQuantity = Quantity;

    // 1. Intentar completar stacks existentes del mismo item
    while (RemainingQuantity > 0)
    {
        int32 ExistingIndex = FindExistingStackIndex(ItemData->ItemID, ItemData->MaxStackSize);
        if (ExistingIndex == INDEX_NONE)
        {
            break;
        }

        FInventorySlot& ExistingSlot = InventorySlots[ExistingIndex];
        int32 SpaceInStack = ItemData->MaxStackSize - ExistingSlot.Quantity;
        int32 AmountToAdd = FMath::Min(SpaceInStack, RemainingQuantity);

        ExistingSlot.Quantity += AmountToAdd;
        RemainingQuantity -= AmountToAdd;
    }

    // 2. Crear nuevos slots para la cantidad restante
    while (RemainingQuantity > 0)
    {
        int32 EmptyIndex = FindEmptySlotIndex();
        if (EmptyIndex == INDEX_NONE)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UEMBERVEILInventoryComponent::AddItem — Inventario lleno. "
                     "No se pudo añadir %s (restante: %d)."),
                *ItemData->ItemID.ToString(), RemainingQuantity);
            return false;
        }

        // Crear instancia para el nuevo slot
        UEMBERVEILItemInstance* NewInstance = NewObject<UEMBERVEILItemInstance>(GetOwner());
        NewInstance->Initialize(ItemData);

        int32 AmountForNewSlot = FMath::Min(ItemData->MaxStackSize, RemainingQuantity);

        FInventorySlot& NewSlot = InventorySlots[EmptyIndex];
        NewSlot.ItemInstance = NewInstance;
        NewSlot.Quantity = AmountForNewSlot;

        RemainingQuantity -= AmountForNewSlot;
    }

    OnInventoryChanged.Broadcast(InventorySlots);
    return true;
}

bool UEMBERVEILInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::RemoveItem — Solo puede llamarse en el servidor."));
        return false;
    }

    if (ItemID.IsNone() || Quantity <= 0)
    {
        return false;
    }

    // Verificar que hay suficiente cantidad antes de modificar nada
    if (!HasItem(ItemID, Quantity))
    {
        return false;
    }

    int32 RemainingToRemove = Quantity;

    for (FInventorySlot& Slot : InventorySlots)
    {
        if (RemainingToRemove <= 0)
        {
            break;
        }

        if (Slot.IsEmpty())
        {
            continue;
        }

        if (!IsValid(Slot.ItemInstance) || !IsValid(Slot.ItemInstance->ItemData))
        {
            continue;
        }

        if (Slot.ItemInstance->ItemData->ItemID != ItemID)
        {
            continue;
        }

        int32 AmountToRemove = FMath::Min(Slot.Quantity, RemainingToRemove);
        Slot.Quantity -= AmountToRemove;
        RemainingToRemove -= AmountToRemove;

        // Limpiar el slot si quedó vacío
        if (Slot.Quantity <= 0)
        {
            Slot.ItemInstance = nullptr;
            Slot.Quantity = 0;
        }
    }

    OnInventoryChanged.Broadcast(InventorySlots);
    return true;
}

// ─── EQUIPAMIENTO ─────────────────────────────────────────────────────────────

bool UEMBERVEILInventoryComponent::EquipItem(UEMBERVEILItemInstance* ItemInstance)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — Solo puede llamarse en el servidor."));
        return false;
    }

    if (!IsValid(ItemInstance))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — ItemInstance es nulo o inválido."));
        return false;
    }

    if (!IsValid(ItemInstance->ItemData))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — ItemInstance no tiene ItemData."));
        return false;
    }

    if (!ItemInstance->ItemData->bIsEquipable)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — El item '%s' no es equipable."),
            *ItemInstance->ItemData->ItemID.ToString());
        return false;
    }

    if (ItemInstance->IsBroken())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — El item '%s' está roto."),
            *ItemInstance->ItemData->ItemID.ToString());
        return false;
    }

    EEMBERVEILEquipmentSlot TargetSlot = ItemInstance->ItemData->EquipmentSlot;
    if (TargetSlot == EEMBERVEILEquipmentSlot::None)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::EquipItem — El item '%s' tiene EquipmentSlot = None."),
            *ItemInstance->ItemData->ItemID.ToString());
        return false;
    }

    // Si hay un item equipado en el mismo slot, desequiparlo primero
    if (IsValid(FindEquippedInstance(TargetSlot)))
    {
        UnequipItem(TargetSlot);
    }

    SetEquippedEntry(TargetSlot, ItemInstance);
    ItemInstance->bIsEquipped = true;
    ItemInstance->EquippedInSlot = TargetSlot;

    // Aplicar GameplayEffect de equipo al ASC del owner
    if (ItemInstance->ItemData->OnEquipEffect)
    {
        ApplyEquipEffect(TargetSlot, ItemInstance->ItemData->OnEquipEffect);
    }

    OnItemEquipped.Broadcast(TargetSlot, ItemInstance);
    return true;
}

bool UEMBERVEILInventoryComponent::UnequipItem(EEMBERVEILEquipmentSlot Slot)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::UnequipItem — Solo puede llamarse en el servidor."));
        return false;
    }

    if (Slot == EEMBERVEILEquipmentSlot::None)
    {
        return false;
    }

    UEMBERVEILItemInstance* ItemInstance = FindEquippedInstance(Slot);
    if (!IsValid(ItemInstance))
    {
        return false;
    }

    // Eliminar el GameplayEffect de equipo del ASC
    RemoveEquipEffect(Slot);

    // Actualizar estado de la instancia
    ItemInstance->bIsEquipped = false;
    ItemInstance->EquippedInSlot = EEMBERVEILEquipmentSlot::None;

    ClearEquippedEntry(Slot);

    OnItemEquipped.Broadcast(Slot, nullptr);
    return true;
}

// ─── USO DE ITEMS ─────────────────────────────────────────────────────────────

bool UEMBERVEILInventoryComponent::UseItem(UEMBERVEILItemInstance* ItemInstance)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::UseItem — Solo puede llamarse en el servidor."));
        return false;
    }

    if (!IsValid(ItemInstance))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::UseItem — ItemInstance es nulo o inválido."));
        return false;
    }

    if (!IsValid(ItemInstance->ItemData))
    {
        return false;
    }

    if (ItemInstance->IsBroken())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::UseItem — El item '%s' está roto y no puede usarse."),
            *ItemInstance->ItemData->ItemID.ToString());
        return false;
    }

    // Verificar que tenemos al menos 1 en el inventario
    if (!HasItem(ItemInstance->ItemData->ItemID, 1))
    {
        return false;
    }

    // Aplicar GameplayEffect de uso al ASC del owner
    if (ItemInstance->ItemData->OnUseEffect)
    {
        UAbilitySystemComponent* ASC = GetOwnerASC();
        if (IsValid(ASC))
        {
            FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
            EffectContext.AddSourceObject(GetOwner());

            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
                ItemInstance->ItemData->OnUseEffect,
                1.f,
                EffectContext);

            if (SpecHandle.IsValid())
            {
                ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning,
                TEXT("UEMBERVEILInventoryComponent::UseItem — Owner '%s' no tiene ASC. "
                     "OnUseEffect no aplicado."),
                *GetOwner()->GetName());
        }
    }

    // Consumir 1 unidad del item
    RemoveItem(ItemInstance->ItemData->ItemID, 1);

    return true;
}

// ─── CONSULTAS ────────────────────────────────────────────────────────────────

bool UEMBERVEILInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    return GetItemQuantity(ItemID) >= Quantity;
}

int32 UEMBERVEILInventoryComponent::GetItemQuantity(FName ItemID) const
{
    if (ItemID.IsNone())
    {
        return 0;
    }

    int32 TotalQuantity = 0;

    for (const FInventorySlot& Slot : InventorySlots)
    {
        if (Slot.IsEmpty())
        {
            continue;
        }

        if (!IsValid(Slot.ItemInstance) || !IsValid(Slot.ItemInstance->ItemData))
        {
            continue;
        }

        if (Slot.ItemInstance->ItemData->ItemID == ItemID)
        {
            TotalQuantity += Slot.Quantity;
        }
    }

    return TotalQuantity;
}

UEMBERVEILItemInstance* UEMBERVEILInventoryComponent::GetEquippedItem(
    EEMBERVEILEquipmentSlot Slot) const
{
    if (Slot == EEMBERVEILEquipmentSlot::None)
    {
        return nullptr;
    }

    return FindEquippedInstance(Slot);
}

TArray<FInventorySlot> UEMBERVEILInventoryComponent::GetAllItems() const
{
    return InventorySlots;
}

// ─── MONEDA ───────────────────────────────────────────────────────────────────

void UEMBERVEILInventoryComponent::AddCurrency(int32 Amount)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::AddCurrency — Solo puede llamarse en el servidor."));
        return;
    }

    if (Amount <= 0)
    {
        return;
    }

    Currency += Amount;
    OnCurrencyChanged.Broadcast(Currency);
}

bool UEMBERVEILInventoryComponent::SpendCurrency(int32 Amount)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::SpendCurrency — Solo puede llamarse en el servidor."));
        return false;
    }

    if (Amount <= 0)
    {
        return false;
    }

    if (Currency < Amount)
    {
        return false;
    }

    Currency -= Amount;
    OnCurrencyChanged.Broadcast(Currency);
    return true;
}

// ─── HELPERS PRIVADOS ─────────────────────────────────────────────────────────

UAbilitySystemComponent* UEMBERVEILInventoryComponent::GetOwnerASC() const
{
    AActor* Owner = GetOwner();
    if (!IsValid(Owner))
    {
        return nullptr;
    }

    // Intentar obtener el ASC directamente del owner (enemigos, NPCs)
    IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Owner);
    if (ASCInterface)
    {
        return ASCInterface->GetAbilitySystemComponent();
    }

    // Para el jugador, el ASC vive en el PlayerState
    APawn* OwnerPawn = Cast<APawn>(Owner);
    if (IsValid(OwnerPawn))
    {
        APlayerState* PS = OwnerPawn->GetPlayerState();
        if (IsValid(PS))
        {
            IAbilitySystemInterface* PSInterface = Cast<IAbilitySystemInterface>(PS);
            if (PSInterface)
            {
                return PSInterface->GetAbilitySystemComponent();
            }
        }
    }

    return nullptr;
}

void UEMBERVEILInventoryComponent::ApplyEquipEffect(EEMBERVEILEquipmentSlot Slot,
    TSubclassOf<UGameplayEffect> EffectClass)
{
    if (!EffectClass)
    {
        return;
    }

    UAbilitySystemComponent* ASC = GetOwnerASC();
    if (!IsValid(ASC))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("UEMBERVEILInventoryComponent::ApplyEquipEffect — Owner no tiene ASC. "
                 "Efecto no aplicado para slot %d."),
            static_cast<int32>(Slot));
        return;
    }

    FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
    EffectContext.AddSourceObject(GetOwner());

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
    if (!SpecHandle.IsValid())
    {
        return;
    }

    FActiveGameplayEffectHandle ActiveHandle =
        ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

    if (ActiveHandle.IsValid())
    {
        ActiveEquipEffectHandles.Add(Slot, ActiveHandle);
    }
}

void UEMBERVEILInventoryComponent::RemoveEquipEffect(EEMBERVEILEquipmentSlot Slot)
{
    FActiveGameplayEffectHandle* FoundHandle = ActiveEquipEffectHandles.Find(Slot);
    if (!FoundHandle || !FoundHandle->IsValid())
    {
        return;
    }

    UAbilitySystemComponent* ASC = GetOwnerASC();
    if (IsValid(ASC))
    {
        ASC->RemoveActiveGameplayEffect(*FoundHandle);
    }

    ActiveEquipEffectHandles.Remove(Slot);
}

int32 UEMBERVEILInventoryComponent::FindExistingStackIndex(FName ItemID,
    int32 MaxStackSize) const
{
    for (int32 Index = 0; Index < InventorySlots.Num(); ++Index)
    {
        const FInventorySlot& Slot = InventorySlots[Index];

        if (Slot.IsEmpty())
        {
            continue;
        }

        if (!IsValid(Slot.ItemInstance) || !IsValid(Slot.ItemInstance->ItemData))
        {
            continue;
        }

        if (Slot.ItemInstance->ItemData->ItemID != ItemID)
        {
            continue;
        }

        if (Slot.Quantity < MaxStackSize)
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

int32 UEMBERVEILInventoryComponent::FindEmptySlotIndex() const
{
    for (int32 Index = 0; Index < InventorySlots.Num(); ++Index)
    {
        if (InventorySlots[Index].IsEmpty())
        {
            return Index;
        }
    }

    return INDEX_NONE;
}

UEMBERVEILItemInstance* UEMBERVEILInventoryComponent::FindEquippedInstance(EEMBERVEILEquipmentSlot Slot) const
{
    for (const FReplicatedEquipmentEntry& Entry : EquippedEntries)
    {
        if (Entry.Slot == Slot)
        {
            return Entry.ItemInstance.Get();
        }
    }
    return nullptr;
}

void UEMBERVEILInventoryComponent::SetEquippedEntry(EEMBERVEILEquipmentSlot Slot, UEMBERVEILItemInstance* Item)
{
    for (FReplicatedEquipmentEntry& Entry : EquippedEntries)
    {
        if (Entry.Slot == Slot)
        {
            Entry.ItemInstance = Item;
            return;
        }
    }

    FReplicatedEquipmentEntry NewEntry;
    NewEntry.Slot = Slot;
    NewEntry.ItemInstance = Item;
    EquippedEntries.Add(NewEntry);
}

void UEMBERVEILInventoryComponent::ClearEquippedEntry(EEMBERVEILEquipmentSlot Slot)
{
    for (int32 Index = 0; Index < EquippedEntries.Num(); ++Index)
    {
        if (EquippedEntries[Index].Slot == Slot)
        {
            EquippedEntries.RemoveAt(Index);
            return;
        }
    }
}
