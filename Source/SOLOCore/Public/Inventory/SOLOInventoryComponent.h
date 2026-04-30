#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/SOLOItemTypes.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "SOLOInventoryComponent.generated.h"

class USOLOItemBase;
class USOLOItemInstance;
class UAbilitySystemComponent;

// â”€â”€â”€ DELEGATES PARA UI â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Disparado cuando el contenido del inventario cambia (item aÃ±adido, quitado o cantidad modificada) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged,
    const TArray<FInventorySlot>&, UpdatedSlots);

/** Disparado cuando un item es equipado o desequipado en cualquier slot */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped,
    ESOLOEquipmentSlot, Slot,
    USOLOItemInstance*, Item);

/** Disparado cuando la cantidad de monedas cambia */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, int32, NewAmount);

/**
 * Componente de inventario de SOLO.
 * Gestiona el inventario en grid, el equipo del personaje y la moneda.
 * Se coloca en el PlayerState (jugadores) o en el Character/NPC segÃºn corresponda.
 *
 * Soporta replicaciÃ³n completa â€” las modificaciones deben hacerse solo en el servidor.
 * Los delegates se usan exclusivamente para actualizar la UI en cliente local.
 */
UCLASS(ClassGroup = (SOLO), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USOLOInventoryComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Notifica a la UI cuando el inventario cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnInventoryChanged OnInventoryChanged;

    /** Notifica a la UI cuando un slot de equipo cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnItemEquipped OnItemEquipped;

    /** Notifica a la UI cuando la moneda cambia */
    UPROPERTY(BlueprintAssignable, Category = "Inventory|Delegates")
    FOnCurrencyChanged OnCurrencyChanged;

    // â”€â”€â”€ CONFIGURACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** NÃºmero mÃ¡ximo de slots en el inventario. Puede expandirse con upgrades. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Config",
        meta = (ClampMin = "1", ClampMax = "200"))
    int32 MaxInventorySlots = 40;

    // â”€â”€â”€ DATOS REPLICADOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Array de slots del inventario (Ã­ndice = posiciÃ³n en el grid de la UI) */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Data",
        ReplicatedUsing = OnRep_InventorySlots)
    TArray<FInventorySlot> InventorySlots;

    /** Equipo replicado (TMap replicado no estÃ¡ soportado por UHT). */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Data",
        ReplicatedUsing = OnRep_EquipmentSlots)
    TArray<FReplicatedEquipmentEntry> EquippedEntries;

    /** Cantidad de moneda del personaje */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory|Currency",
        ReplicatedUsing = OnRep_Currency)
    int32 Currency = 0;

    // â”€â”€â”€ GESTIÃ“N DEL INVENTARIO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * AÃ±ade un item al inventario. Agrupa en stacks existentes antes de crear un slot nuevo.
     * Si ItemData es nulo o no hay espacio disponible, devuelve false.
     *
     * @param ItemData  DataAsset del item a aÃ±adir
     * @param Quantity  Cantidad a aÃ±adir (mÃ­nimo 1)
     * @return          true si todos los items fueron aÃ±adidos correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Management",
        meta = (GameplayTagFilter = ""))
    bool AddItem(USOLOItemBase* ItemData, int32 Quantity = 1);

    /**
     * Elimina una cantidad del item con el ItemID dado.
     * VacÃ­a slots si la cantidad llega a 0.
     *
     * @param ItemID    ID del item a eliminar
     * @param Quantity  Cantidad a eliminar (mÃ­nimo 1)
     * @return          true si habÃ­a suficiente cantidad y se eliminÃ³ correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Management")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

    // â”€â”€â”€ EQUIPAMIENTO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Equipa el item dado en su slot correspondiente.
     * Si ya hay un item en ese slot, lo desequipa primero (swap).
     * Aplica el OnEquipEffect del item al ASC del owner si existe.
     *
     * @param ItemInstance  Instancia del item a equipar
     * @return              true si el item fue equipado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Equipment")
    bool EquipItem(USOLOItemInstance* ItemInstance);

    /**
     * Desequipa el item del slot dado.
     * Elimina el GameplayEffect de equipo del ASC si corresponde.
     *
     * @param Slot  Slot de equipo a liberar
     * @return      true si habÃ­a un item equipado y fue desequipado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Equipment")
    bool UnequipItem(ESOLOEquipmentSlot Slot);

    // â”€â”€â”€ USO DE ITEMS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Usa un item consumible. Aplica el OnUseEffect al ASC del owner y reduce la cantidad en 1.
     * Elimina el slot si la cantidad llega a 0.
     *
     * @param ItemInstance  Instancia del consumible a usar
     * @return              true si el item fue usado correctamente
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Use")
    bool UseItem(USOLOItemInstance* ItemInstance);

    // â”€â”€â”€ CONSULTAS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Devuelve true si el inventario contiene al menos Quantity unidades del item dado.
     *
     * @param ItemID    ID del item a buscar
     * @param Quantity  Cantidad mÃ­nima requerida
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
     * Devuelve la instancia del item equipado en el slot dado, o nullptr si estÃ¡ vacÃ­o.
     *
     * @param Slot  Slot de equipo a consultar
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    USOLOItemInstance* GetEquippedItem(ESOLOEquipmentSlot Slot) const;

    /**
     * Devuelve una copia del array completo de slots del inventario (incluye slots vacÃ­os).
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Query")
    TArray<FInventorySlot> GetAllItems() const;

    // â”€â”€â”€ MONEDA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * AÃ±ade moneda al personaje (p.ej. al recoger loot o vender items).
     *
     * @param Amount  Cantidad a aÃ±adir (valor positivo)
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
    void AddCurrency(int32 Amount);

    /**
     * Gasta moneda del personaje. Devuelve false si no hay suficiente.
     *
     * @param Amount  Cantidad a gastar (valor positivo)
     * @return        true si habÃ­a suficiente moneda y fue gastada
     */
    UFUNCTION(BlueprintCallable, Category = "Inventory|Currency")
    bool SpendCurrency(int32 Amount);

protected:
    virtual void BeginPlay() override;

    // â”€â”€â”€ OnRep CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    UFUNCTION()
    void OnRep_InventorySlots();

    UFUNCTION()
    void OnRep_EquipmentSlots();

    UFUNCTION()
    void OnRep_Currency();

private:
    // â”€â”€â”€ HELPERS INTERNOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Obtiene el ASC del actor owner (busca en Ã©l y en su PlayerState).
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
    void ApplyEquipEffect(ESOLOEquipmentSlot Slot,
        TSubclassOf<UGameplayEffect> EffectClass);

    /**
     * Elimina el GameplayEffect activo asociado al slot de equipo dado.
     *
     * @param Slot  Slot de equipo cuyo efecto se desea eliminar
     */
    void RemoveEquipEffect(ESOLOEquipmentSlot Slot);

    /**
     * Busca el Ã­ndice del primer slot que contiene el item con el ItemID dado
     * y tiene espacio para mÃ¡s cantidad. Devuelve INDEX_NONE si no encuentra ninguno.
     */
    int32 FindExistingStackIndex(FName ItemID, int32 MaxStackSize) const;

    /**
     * Busca el Ã­ndice del primer slot vacÃ­o del inventario.
     * Devuelve INDEX_NONE si el inventario estÃ¡ lleno.
     */
    int32 FindEmptySlotIndex() const;

    /** Mapa de handles de GE activos por slot de equipo (para eliminarlos al desequipar) */
    TMap<ESOLOEquipmentSlot, FActiveGameplayEffectHandle> ActiveEquipEffectHandles;

    USOLOItemInstance* FindEquippedInstance(ESOLOEquipmentSlot Slot) const;
    void SetEquippedEntry(ESOLOEquipmentSlot Slot, USOLOItemInstance* Item);
    void ClearEquippedEntry(ESOLOEquipmentSlot Slot);
};
