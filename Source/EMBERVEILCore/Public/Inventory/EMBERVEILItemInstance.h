#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/EMBERVEILItemTypes.h"
#include "EMBERVEILItemInstance.generated.h"

class UEMBERVEILItemBase;

/**
 * Instancia runtime de un item en el inventario.
 * Contiene el estado mutable de un item concreto (durabilidad actual, si está equipado,
 * stats dinámicos por rolls de drop o encantamientos de runas, etc.).
 *
 * El estado estático (nombre, icono, tipo, etc.) vive en UEMBERVEILItemBase (DataAsset).
 *
 * IMPORTANTE: No replicar directamente — la replicación se maneja desde
 * UEMBERVEILInventoryComponent via FInventorySlot arrays.
 */
UCLASS(BlueprintType)
class EMBERVEILCORE_API UEMBERVEILItemInstance : public UObject
{
    GENERATED_BODY()

public:
    UEMBERVEILItemInstance();

    // ─── INICIALIZACIÓN ───────────────────────────────────────────────────────

    /**
     * Inicializa la instancia con los datos del DataAsset.
     * Debe llamarse inmediatamente después de NewObject<UEMBERVEILItemInstance>.
     * Establece la durabilidad máxima a 100 por defecto (override posible desde BP).
     *
     * @param InItemData  DataAsset del item base (no puede ser nulo)
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Instance")
    void Initialize(UEMBERVEILItemBase* InItemData);

    // ─── DATOS BASE ───────────────────────────────────────────────────────────

    /** DataAsset con los datos estáticos del item */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Instance")
    TObjectPtr<UEMBERVEILItemBase> ItemData;

    // ─── DURABILIDAD ──────────────────────────────────────────────────────────

    /** Durabilidad actual del item (0 = roto, no puede usarse) */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Durability")
    int32 CurrentDurability = 100;

    /** Durabilidad máxima del item */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Durability")
    int32 MaxDurability = 100;

    /**
     * Devuelve la durabilidad como porcentaje normalizado [0.0, 1.0].
     * Retorna 0.0 si MaxDurability es 0 para evitar división por cero.
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    float GetDurabilityPercent() const;

    /**
     * Devuelve true si la durabilidad es <= 0 (item roto, no puede equiparse ni usarse).
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    bool IsBroken() const;

    // ─── ESTADO DE EQUIPO ─────────────────────────────────────────────────────

    /** Si true, este item está actualmente equipado en algún slot del personaje */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Equipment")
    bool bIsEquipped = false;

    /** Slot en el que está equipado (None si no está equipado) */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Equipment")
    EEMBERVEILEquipmentSlot EquippedInSlot = EEMBERVEILEquipmentSlot::None;

    // ─── STATS DINÁMICOS ──────────────────────────────────────────────────────

    /**
     * Stats adicionales generados proceduralmente al dropearse el item.
     * Clave = nombre del atributo (p.ej. "BonusStrength"), Valor = magnitud.
     * Usado por runas encantadas y drops con rolls aleatorios.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Item|DynamicStats")
    TMap<FName, float> DynamicStats;

    // ─── HELPERS ──────────────────────────────────────────────────────────────

    /**
     * Aplica daño a la durabilidad del item.
     * @param Amount  Puntos de durabilidad a reducir (valor positivo)
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    void ReduceDurability(int32 Amount);

    /**
     * Repara el item restaurando durabilidad.
     * @param Amount  Puntos de durabilidad a restaurar (value positivo)
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    void RepairDurability(int32 Amount);

    /**
     * Añade o sobreescribe un stat dinámico.
     * @param StatName  Identificador del stat dinámico
     * @param Value     Magnitud del stat
     */
    UFUNCTION(BlueprintCallable, Category = "Item|DynamicStats")
    void SetDynamicStat(FName StatName, float Value);

    /**
     * Obtiene el valor de un stat dinámico. Devuelve 0.0 si no existe.
     * @param StatName  Identificador del stat dinámico
     */
    UFUNCTION(BlueprintCallable, Category = "Item|DynamicStats")
    float GetDynamicStat(FName StatName) const;
};
