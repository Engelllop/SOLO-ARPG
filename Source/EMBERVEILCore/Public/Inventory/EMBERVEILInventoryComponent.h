#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/EMBERVEILItemTypes.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "EMBERVEILInventoryComponent.generated.h"

class UEMBERVEILItemBase;
class UEMBERVEILItemInstance;
class UAbilitySystemComponent;

// ─── DELEGATES PARA UI ────────────────────────────────────────────────────────

/** Disparado cuando el contenido del inventario cambia (item añadido, quitado o cantidad modificada) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged,
    const TArray<FInventorySlot>&, UpdatedSlots);

/** Disparado cuando un item es equipado o desequipado en cualquier slot */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped,
    EEMBERVEILEquipmentSlot, Slot,
    UEMBERVEILItemInstance*, Item);

/** Disparado cuando la cantidad de monedas cambia */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, int32, NewAmount);

/**
 * Componente de inventario de EMBERVEIL.
 * Gestiona el inventario en grid, el equipo del personaje y la moneda.
 * Se coloca en el PlayerState (jugadores) o en el Character/NPC según corresponda.
 *
 * Soporta replicación completa — las modificaciones deben hacerse solo en el servidor.
 * Los delegates se usan exclusivamente para actualizar la UI en cliente local.
 */
UCLASS(ClassGroup = (EMBERVEIL), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class EMBERVEILCORE_API UEMBERVEILInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEMBERVEILInventoryComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // ─── DELEGATES ────────────────────────────────────────────────────────────

    /** Notifica a la UI cuando el inventario cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnInventoryChanged OnInventoryChanged;

    /** Notifica a la UI cuando un slot de equipo cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnItemEquipped OnItemEquipped;

    /** Notifica a la UI cuando la moneda cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnCurrencyChanged OnCurrencyChanged;

    // ─── CONFIGURACIÓN ────────────────────────────────────────────────────────

    /** Número máximo de slots en el inventario. Puede expandirse con upgrades. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Config",
        meta = (ClampMin = "1", ClampMax = "200"))
    int32 MaxInventorySlots = 40;

    // ─── DATOS REPLICADOS ─────────────────────────────────────────────────────

    /** Array de slots del inventario (índice = posición en el grid de la UI) */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Data",
        ReplicatedUsing = OnRep_InventorySlots)
    TArray<FInventorySlot> InventorySlots;

    /** Equipo replicado (TMap replicado no está soportado por UHT). */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Data",
        ReplicatedUsing = OnRep_EquipmentSlots)
    TArray<FReplicatedEquipmentEntry> EquippedEntries;

    /** Cantidad de moneda del personaje */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Currency",
        ReplicatedUsing = OnRep_Currency)
    int32 Currency = 0;

    // ─── GESTIÓN DEL INVENTARIO ───────────────────────────────────────────────

    /**
     * Añade un item al inventario. Agrupa en stacks existentes antes de crear un slot nuevo.
     * Si ItemData es nulo o no hay espacio disponible, devuelve false.
     *
     * @param ItemData  DataAsset del item a añadir
     * @param Quantity  Cantidad a añadir (mínimo 1)
     * @return          true si todos los items fueron añadidos correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Management",
        meta = (GameplayTagFilter = ""))
    bool AddItem(UEMBERVEILItemBase* ItemData, int32 Quantity = 1);

    /**
     * Elimina una cantidad del item con el ItemID dado.
     * Vacía slots si la cantidad llega a 0.
     *
     * @param ItemID    ID del item a eliminar
     * @param Quantity  Cantidad a eliminar (mínimo 1)
     * @return          true si había suficiente cantidad y se eliminó correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Management")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

    // ─── EQUIPAMIENTO ─────────────────────────────────────────────────────────

    /**
     * Equipa el item dado en su slot correspondiente.
     * Si ya hay un item en ese slot, lo desequipa primero (swap).
     * Aplica el OnEquipEffect del item al ASC del owner si existe.
     *
     * @param ItemInstance  Instancia del item a equipar
     * @return              true si el item fue equipado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Equipment")
    bool EquipItem(UEMBERVEILItemInstance* ItemInstance);

    /**
     * Desequipa el item del slot dado.
     * Elimina el GameplayEffect de equipo del ASC si corresponde.
     *
     * @param Slot  Slot de equipo a liberar
     * @return      true si había un item equipado y fue desequipado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Equipment")
    bool UnequipItem(EEMBERVEILEquipmentSlot Slot);

    // ─── USO DE ITEMS ─────────────────────────────────────────────────────────

    /**
     * Usa un item consumible. Aplica el OnUseEffect al ASC del owner y reduce la cantidad en 1.
     * Elimina el slot si la cantidad llega a 0.
     *
     * @param ItemInstance  Instancia del consumible a usar
     * @return              true si el item fue usado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Use")
    bool UseItem(UEMBERVEILItemInstance* ItemInstance);

    // ─── CONSULTAS ────────────────────────────────────────────────────────────

    /**
     * Devuelve true si el inventario contiene al menos Quantity unidades del item dado.
     *
     * @param ItemID    ID del item a buscar
     * @param Quantity  Cantidad mínima requerida
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    bool HasItem(FName ItemID, int32 Quantity = 1) const;

    /**
     * Devuelve la cantidad total del item con el ID dado en todos los slots.
     *
     * @param ItemID  ID del item a contar
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    int32 GetItemQuantity(FName ItemID) const;

    /**
     * Devuelve la instancia del item equipado en el slot dado, o nullptr si está vacío.
     *
     * @param Slot  Slot de equipo a consultar
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    UEMBERVEILItemInstance* GetEquippedItem(EEMBERVEILEquipmentSlot Slot) const;

    /**
     * Devuelve una copia del array completo de slots del inventario (incluye slots vacíos).
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    TArray<FInventorySlot> GetAllItems() const;

    // ─── MONEDA ───────────────────────────────────────────────────────────────

    /**
     * Añade moneda al personaje (p.ej. al recoger loot o vender items).
     *
     * @param Amount  Cantidad a añadir (valor positivo)
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
    void AddCurrency(int32 Amount);

    /**
     * Gasta moneda del personaje. Devuelve false si no hay suficiente.
     *
     * @param Amount  Cantidad a gastar (valor positivo)
     * @return        true si había suficiente moneda y fue gastada
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
    bool SpendCurrency(int32 Amount);

protected:
    virtual void BeginPlay() override;

    // ─── OnRep CALLBACKS ──────────────────────────────────────────────────────

    UFUNCTION()
    void OnRep_InventorySlots();

    UFUNCTION()
    void OnRep_EquipmentSlots();

    UFUNCTION()
    void OnRep_Currency();

private:
    // ─── HELPERS INTERNOS ─────────────────────────────────────────────────────

    /**
     * Obtiene el ASC del actor owner (busca en él y en su PlayerState).
     * Devuelve nullptr si el owner no tiene ASC.
     */
    UAbilitySystemComponent* GetOwnerASC() const;

    /**
     * Aplica un GameplayEffect al ASC del owner y almacena el handle activo
     * asociado al slot de equipo para poder eliminarlo al desequipar.
     *
     * @param Slot        Slot de equipo al que corresponde el efecto
     * @param EffectClass Clase del GameplayEffect a aplicar
     */
    void ApplyEquipEffect(EEMBERVEILEquipmentSlot Slot,
        TSubclassOf<UGameplayEffect> EffectClass);

    /**
     * Elimina el GameplayEffect activo asociado al slot de equipo dado.
     *
     * @param Slot  Slot de equipo cuyo efecto se desea eliminar
     */
    void RemoveEquipEffect(EEMBERVEILEquipmentSlot Slot);

    /**
     * Busca el índice del primer slot que contiene el item con el ItemID dado
     * y tiene espacio para más cantidad. Devuelve INDEX_NONE si no encuentra ninguno.
     */
    int32 FindExistingStackIndex(FName ItemID, int32 MaxStackSize) const;

    /**
     * Busca el índice del primer slot vacío del inventario.
     * Devuelve INDEX_NONE si el inventario está lleno.
     */
    int32 FindEmptySlotIndex() const;

    /** Mapa de handles de GE activos por slot de equipo (para eliminarlos al desequipar) */
    TMap<EEMBERVEILEquipmentSlot, FActiveGameplayEffectHandle> ActiveEquipEffectHandles;

    UEMBERVEILItemInstance* FindEquippedInstance(EEMBERVEILEquipmentSlot Slot) const;
    void SetEquippedEntry(EEMBERVEILEquipmentSlot Slot, UEMBERVEILItemInstance* Item);
    void ClearEquippedEntry(EEMBERVEILEquipmentSlot Slot);
};
