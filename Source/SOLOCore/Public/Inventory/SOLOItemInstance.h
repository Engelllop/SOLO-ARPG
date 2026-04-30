#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/SOLOItemTypes.h"
#include "SOLOItemInstance.generated.h"

class USOLOItemBase;

/**
 * Instancia runtime de un item en el inventario.
 * Contiene el estado mutable de un item concreto (durabilidad actual, si estÃ¡ equipado,
 * stats dinÃ¡micos por rolls de drop o encantamientos de runas, etc.).
 *
 * El estado estÃ¡tico (nombre, icono, tipo, etc.) vive en USOLOItemBase (DataAsset).
 *
 * IMPORTANTE: No replicar directamente â€” la replicaciÃ³n se maneja desde
 * USOLOInventoryComponent via FInventorySlot arrays.
 */
UCLASS(BlueprintType)
class SOLOCORE_API USOLOItemInstance : public UObject
{
    GENERATED_BODY()

public:
    USOLOItemInstance();

    // â”€â”€â”€ INICIALIZACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Inicializa la instancia con los datos del DataAsset.
     * Debe llamarse inmediatamente despuÃ©s de NewObject<USOLOItemInstance>.
     * Establece la durabilidad mÃ¡xima a 100 por defecto (override posible desde BP).
     *
     * @param InItemData  DataAsset del item base (no puede ser nulo)
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Instance")
    void Initialize(USOLOItemBase* InItemData);

    // â”€â”€â”€ DATOS BASE â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** DataAsset con los datos estÃ¡ticos del item */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Instance")
    TObjectPtr<USOLOItemBase> ItemData;

    // â”€â”€â”€ DURABILIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Durabilidad actual del item (0 = roto, no puede usarse) */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Durability")
    int32 CurrentDurability = 100;

    /** Durabilidad mÃ¡xima del item */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Durability")
    int32 MaxDurability = 100;

    /**
     * Devuelve la durabilidad como porcentaje normalizado [0.0, 1.0].
     * Retorna 0.0 si MaxDurability es 0 para evitar divisiÃ³n por cero.
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    float GetDurabilityPercent() const;

    /**
     * Devuelve true si la durabilidad es <= 0 (item roto, no puede equiparse ni usarse).
     */
    UFUNCTION(BlueprintCallable, Category = "Item|Durability")
    bool IsBroken() const;

    // â”€â”€â”€ ESTADO DE EQUIPO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Si true, este item estÃ¡ actualmente equipado en algÃºn slot del personaje */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Equipment")
    bool bIsEquipped = false;

    /** Slot en el que estÃ¡ equipado (None si no estÃ¡ equipado) */
    UPROPERTY(BlueprintReadOnly, Category = "Item|Equipment")
    ESOLOEquipmentSlot EquippedInSlot = ESOLOEquipmentSlot::None;

    // â”€â”€â”€ STATS DINÃMICOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Stats adicionales generados proceduralmente al dropearse el item.
     * Clave = nombre del atributo (p.ej. "BonusStrength"), Valor = magnitud.
     * Usado por runas encantadas y drops con rolls aleatorios.
     */
    UPROPERTY(BlueprintReadWrite, Category = "Item|DynamicStats")
    TMap<FName, float> DynamicStats;

    // â”€â”€â”€ HELPERS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Aplica daÃ±o a la durabilidad del item.
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
     * AÃ±ade o sobreescribe un stat dinÃ¡mico.
     * @param StatName  Identificador del stat dinÃ¡mico
     * @param Value     Magnitud del stat
     */
    UFUNCTION(BlueprintCallable, Category = "Item|DynamicStats")
    void SetDynamicStat(FName StatName, float Value);

    /**
     * Obtiene el valor de un stat dinÃ¡mico. Devuelve 0.0 si no existe.
     * @param StatName  Identificador del stat dinÃ¡mico
     */
    UFUNCTION(BlueprintCallable, Category = "Item|DynamicStats")
    float GetDynamicStat(FName StatName) const;
};
